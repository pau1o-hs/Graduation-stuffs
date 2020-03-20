#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "DataLog.h"
#include "Sorts.h"

const int minRSize = 34;
const ll nullItem = -1;

struct _header {

        char status;                    // 1/0 | consistent/corrupted
        ll topList;
        char tag[5];
        char desc[5][40];
};

struct _registry {

        ll pointer;                     // position in file
        char status;                   // */- | yes/no
        int rSize;                      // registrySize

        ll listAddress;
        int id, nSize, cSize;
        double wage;
        char phone[15];
        char name[110];
        char role[110];
};

int Registry_SwitchStatus(Registry* r, char status) {
        r->status = status;
}

int Registry_isRemoved(Registry* r) {
        return r->status == '*' ? 1 : 0;
}

int Registry_Size(Registry* r) {
        return r->rSize;
}

int Registry_ID(Registry* r) {
        return r->id;
}

int File_Size(FILE* f) {
        fseek(f, 0, SEEK_END);
        return ftell(f);
}

int DataLog_isConsistent(FILE* f) {

        if (f == NULL) return 0;

        char tC;
        fseek(f, 0, SEEK_SET);

        fread(&tC, sizeof(char), 1, f);
        if (tC == '0') return 0;

        return 1;
}

char** Header_GetDescriptions(Header* h) {

        char** t = malloc(5 * sizeof(char*));
        For(i, 0, 5) t[i] = malloc(50 * sizeof(char));

        strcpy(t[0], h->desc[0]);
        strcpy(t[1], h->desc[1]);
        strcpy(t[2], h->desc[2]);
        strcpy(t[3], h->desc[3]);
        strcpy(t[4], h->desc[4]);

        return t;
}

void Header_SwitchStatus(FILE* f, char status) {
        fseek(f, 0, SEEK_SET);
        fwrite(&status, sizeof(char), 1, f);
}

Registry** DataLog_GetList(FILE* f) {

        // HEADER
        Header* h = DataLog_GetHeader(f);

        // TEMP VARIABLES
        int tD, rAmount = 0;
        ll tLL;

        Registry** r = malloc((File_Size(f) / PAGE) * 820 * sizeof(Registry*));
        tLL = h->topList;

        // ADD REMOVED REGISTRIES TO ARRAY
        while (tLL != -1) {

                r[rAmount] = DataLog_RecoverAt(f, tLL, 1);
                tLL = r[rAmount++]->listAddress;
        }

        // END REGISTRY FLAG
        r[rAmount] = malloc(sizeof(Registry));
        r[rAmount++]->id = -1;
        r = realloc(r, rAmount * sizeof(Registry*));

        free(h);
        return r;
}

Header* DataLog_GetHeader(FILE* f) {

        Header* h =  malloc(sizeof(Header));

        fseek(f, 1, SEEK_SET);
        fread(&h->topList, sizeof(ll), 1, f);

        fseek(f, 1, SEEK_CUR);
        fread(h->desc[0], sizeof(char), 40, f);
        fseek(f, 1, SEEK_CUR);
        fread(h->desc[1], sizeof(char), 40, f);
        fseek(f, 1, SEEK_CUR);
        fread(h->desc[2], sizeof(char), 40, f);
        fseek(f, 1, SEEK_CUR);
        fread(h->desc[3], sizeof(char), 40, f);
        fseek(f, 1, SEEK_CUR);
        fread(h->desc[4], sizeof(char), 40, f);

        return h;
}

Registry* Registry_New(char** values) {
        
        Registry* r = malloc(sizeof(Registry));

        r->rSize = 34, r->nSize = 0, r->cSize = 0;
        r->phone[0] = '\0', r->listAddress = -1;

        r->id = atoi(values[0]);
        if (strlen(values[1])) r->wage = atof(values[1]); else r->wage = -1;
        if (strlen(values[2])) strcpy(r->phone, values[2]); else For(i, 1, 14) r->phone[i] = '@';
        if (strlen(values[3])) strcpy(r->name, values[3]), r->nSize = strlen(values[3]) + 2, r->rSize += r->nSize + 4;
        if (strlen(values[4])) strcpy(r->role, values[4]), r->cSize = strlen(values[4]) + 2, r->rSize += r->cSize + 4;
        r->phone[14] = '\0';

        return r;
}

Registry* Registry_EndFlag() {
        Registry* r = malloc(sizeof(Registry));
        r->id = -1;
        return r;
}

void DataLog_AddHeader(FILE* f, char** desc) {
        
        // HEADER
        fwrite("0", sizeof(char), 1, f);
        fwrite(&nullItem, sizeof(ll), 1, f);

        fwrite("i", sizeof(char), 1, f);
        fwrite(desc[0], sizeof(char), strlen(desc[0]) + 1, f);
        For(i, 0, 39 - strlen(desc[0])) fwrite("@", sizeof(char), 1, f);

        fwrite("s", sizeof(char), 1, f);
        fwrite(desc[1], sizeof(char), strlen(desc[1]) + 1, f);
        For(i, 0, 39 - strlen(desc[1])) fwrite("@", sizeof(char), 1, f);


        fwrite("t", sizeof(char), 1, f);
        fwrite(desc[2], sizeof(char), strlen(desc[2]) + 1, f);
        For(i, 0, 39 - strlen(desc[2])) fwrite("@", sizeof(char), 1, f);

        fwrite("n", sizeof(char), 1, f);
        fwrite(desc[3], sizeof(char), strlen(desc[3]) + 1, f);
        For(i, 0, 39 - strlen(desc[3])) fwrite("@", sizeof(char), 1, f);


        fwrite("c", sizeof(char), 1, f);
        fwrite(desc[4], sizeof(char), strlen(desc[4]) + 1, f);
        For(i, 0, 39 - strlen(desc[4])) fwrite("@", sizeof(char), 1, f);

        // FILL REMAINING
        For(i, 214, PAGE) fwrite("@", sizeof(char), 1, f);
}

void DataLog_AddRegistry(FILE* f, Registry* r) {

        // HEADER
        Header* h = DataLog_GetHeader(f);

        // TEMP VARIABLES
        int tD = 0, prevPos, newReg = 1;
        ll tLL;

        // FIND BEST-FIT
        if (h->topList == -1) {
                fseek(f, 0, SEEK_END);
        }
        else {
                // LOWEST SIZE REGISTRY
                fseek(f, h->topList + 1, SEEK_SET);
                fread(&tD, sizeof(int), 1, f);
                fread(&tLL, sizeof(ll), 1, f);

                if (r->rSize <= tD) {
                        tD = ftell(f) - 13;
                        fseek(f, 1, SEEK_SET);
                        fwrite(&tLL, sizeof(ll), 1, f);
                }
                else {
                        while (r->rSize > tD && tLL != -1) {

                                prevPos = ftell(f) - 13;
                                fseek(f, tLL + 1, SEEK_SET);
                                fread(&tD, sizeof(int), 1, f);
                                fread(&tLL, sizeof(ll), 1, f);
                        }

                        if (r->rSize <= tD) {

                                tD = ftell(f) - 13;
                                fseek(f, prevPos + 5, SEEK_SET);
                                fwrite(&tLL, sizeof(ll), 1, f);
                        }
                        else tD = 0;
                }

                if (tD) { fseek(f, tD, SEEK_SET); newReg = 0; }
                else fseek(f, 0, SEEK_END);
        }

        int pagePos = ftell(f) % (long int) PAGE;

        // FITS ON DISK PAGE?
        if (!tD && pagePos + (r->rSize + 5) > PAGE) {

                int lastRPos = 32000, prevRSize = 0;

                // FILE SIZE
                int fSize = File_Size(f);

                fseek(f, 32000, SEEK_SET);
                while(ftell(f) != fSize) {

                        lastRPos = ftell(f);
                        fseek(f, 1, SEEK_CUR);
                        fread(&prevRSize, sizeof(int), 1, f);
                        fseek(f, prevRSize, SEEK_CUR);
                }

                fseek(f, lastRPos + 1, SEEK_SET);

                int newSize = prevRSize + (PAGE - pagePos);
                fwrite(&newSize, sizeof(int), 1, f);

                fseek(f, prevRSize, SEEK_CUR);
                For(i, 0, newSize - prevRSize) fwrite("@", sizeof(char), 1, f);

                prevRSize = newSize;
                pagePos = 0;
        }

        // STORE REGISTRY
        fwrite("-", sizeof(char), 1, f);

        if (newReg) fwrite(&r->rSize, sizeof(int), 1, f);
        else fseek(f, 4, SEEK_CUR);

        fwrite(&r->listAddress, sizeof(ll), 1, f);
        fwrite(&r->id, sizeof(int), 1, f);
        fwrite(&r->wage, sizeof(double), 1, f);
        fwrite(r->phone, sizeof(char), 14, f);

        if (r->nSize) {
                fwrite(&r->nSize, sizeof(int), 1, f);
                fwrite("n", sizeof(char), 1, f);
                fwrite(r->name, sizeof(char), r->nSize - 1, f);
        }

        if (r->cSize) {
                fwrite(&r->cSize, sizeof(int), 1, f);
                fwrite("c", sizeof(char), 1, f);
                fwrite(r->role, sizeof(char), r->cSize - 1, f);
        }

        while (ftell(f) < r->rSize) fwrite("@", sizeof(char), 1, f);
        free(h);
}

Registry* DataLog_RecoverAt(FILE* f, ll rPos, int getTotalSize) {

        Registry* r = malloc(sizeof(Registry));
        
        int tD;
        char t[5], tC;

        fseek(f, rPos, SEEK_SET);

        // DEFAULT DATA STRUCTURE
        r->pointer = rPos;
        fread(&r->status, sizeof(char), 1, f);
        fread(&r->rSize, sizeof(int), 1, f);

        fread(&r->listAddress, sizeof(ll), 1, f);
        fread(&r->id, sizeof(int), 1, f);
        fread(&r->wage, sizeof(double), 1, f);
        fread(r->phone, sizeof(char), 14, f);
        r->phone[14] = '\0', r->name[0] = '\0', r->role[0] = '\0'; 
        r->nSize = 0, r->cSize = 0;

        // HAS NAME OR ROLE?
        if (r->rSize > 34) {

                For(i, 0, 2) {

                        fread(&tC, sizeof(char), 1, f);

                        // NO FIELD NAME | ROLE
                        if (r->rSize == 34 + i * 6 + strlen(r->name) || (tC == '@')) break;
                        fseek(f, -1, SEEK_CUR);
                        
                        // HAS FIELD NAME | ROLE
                        fread(&tD, sizeof(int), 1, f);
                        fread(&tC, sizeof(char), 1, f);

                        // READ NAME | ROLE
                        if (tC == 'n') fread(r->name, sizeof(char), tD - 1, f), r->nSize = tD;
                        else fread(r->role, sizeof(char), tD - 1, f), r->cSize = tD;

                        if (tC == 'c') i = 1;
                }
        }

        fseek(f, rPos + r->rSize + 5, SEEK_SET);

        if (!getTotalSize) {
                r->rSize = 34;
                if (r->nSize) r->rSize += r->nSize + 4;
                if (r->cSize) r->rSize += r->cSize + 4;
        }

        return r;
}

Registry** DataLog_Recover(FILE* f, char* field, char* value, int* diskAcess) {

        // FILE SIZE
        int fSize = File_Size(f);

        // TEMP VARIABLES
        int tD, tDValue, tR, fPos = 32000;
        double tLf;
        char t[300], tC;

        Registry** r = malloc((fSize / PAGE) * 820 * sizeof(Registry*));
        int rAmount = 0;

        // JUMP TO CHOOSED FIELD AND COMPARE THE VALUES
        int j, found = 0, seekRole = 0;

        // ANALYSE AND ADJUST GIVEN SEARCH INFO
        if (!strcmp(field, "idServidor"))                   j = 8;
        else if (!strcmp(field, "salarioServidor"))         j = 12;
        else if (!strcmp(field, "telefoneServidor"))        j = 20;
        else if (!strcmp(field, "nomeServidor"))            j = 34;
        else if (!strcmp(field, "cargoServidor"))           j = 34, seekRole = 1;

        // INITIAL POSITION
        fseek(f, 32000, SEEK_SET);

        // SEARCH
        while (ftell(f) != fSize) {

                // JUMP
                fread(&tC, sizeof(char), 1, f);
                fread(&tR, sizeof(int), 1, f);
                fseek(f, j, SEEK_CUR);

                fPos += tR + 5;

                if (tC == '*') goto NEXT;

                // SEEK ID
                if (j == 8) {
                        fread(&tD, sizeof(int), 1, f);
                        if (tD != atoi(value)) goto NEXT;
                }
                // SEEK WAGE
                else if (j == 12) {
                        fread(&tLf, sizeof(double), 1, f);
                        if (tLf == -1 && !strlen(value)) goto FOUND;
                        if (tLf != atof(value) || !strlen(value)) goto NEXT;
                }
                // SEEK PHONE
                else if (j == 20) {
                        fread(t, sizeof(char), 14, f), t[14] = '\0';
                        if (strcmp(t, value)) goto NEXT;
                }
                // SEEK NAME | ROLE
                else if (j == 34) {

                        char name[110] = "\0", role[110] = "\0";

                        For(i, 0, 2) {

                                fread(&tC, sizeof(char), 1, f);

                                // NO FIELD NAME | ROLE
                                if (tR == 34 + i * 6 + strlen(name) || tC == '@') break;
                                fseek(f, -1, SEEK_CUR);

                                // HAS FIELD NAME | ROLE
                                fread(&tD, sizeof(int), 1, f);
                                fread(&tC, sizeof(char), 1, f);

                                // READ NAME | ROLE
                                if (tC == 'n') fread(name, sizeof(char), tD - 1, f);
                                else fread(role, sizeof(char), tD - 1, f);

                                if (tC == 'c') i = 1;
                        }

                        // MATCH
                        if (seekRole && !strcmp(role, value)) goto FOUND;
                        if (!seekRole && !strcmp(name, value)) goto FOUND;

                        goto NEXT;
                }

        FOUND:
                r[rAmount] = DataLog_RecoverAt(f, fPos - (tR + 5), 1);
                found = 1, rAmount++;

                // SEEKING ID?
                if (j == 8) break;

        NEXT:
                fseek(f, fPos, SEEK_SET);
        }
        
        if (DEBUG) {
                Header* h = DataLog_GetHeader(f);
                int tD;
                ll tLL;

                fseek(f, h->topList + 1, SEEK_SET);
                fread(&tD, sizeof(int), 1, f);

                fread(&tLL, sizeof(ll), 1, f);

                printf("%d %lld %lld\n", tD, h->topList, tLL);

                int i = 1;
                while (tLL != -1) {

                        fseek(f, tLL + 1, SEEK_SET);
                        fread(&tD, sizeof(int), 1, f);
                        fread(&tLL, sizeof(ll), 1, f);

                        printf("%d %ld %lld\n", tD, ftell(f) - 13, tLL);
                        i++;
                }

                printf("%d\n", i);
                free(h);
        }

        // END REGISTRY FLAG
        r[rAmount++] = Registry_EndFlag();
        r = realloc(r, rAmount * sizeof(Registry*));

        DataLog_PrintForOP3(f, r);
        if (diskAcess && found) *diskAcess = ceil(ftell(f) / PAGE);

        return r;
}

void DataLog_Remove(FILE* f, Registry*** r1) {

        if ((*r1)[0]->id == -1) return;

        // VARIABLES
        int i, j, rAmount = 0;

        // HEADER AND LIST
        Header* h = DataLog_GetHeader(f);
        Registry** r2 = DataLog_GetList(f);

        // MERGE R1 AND R2 IN R
        Registry** r = malloc((File_Size(f) / PAGE) * 820 * sizeof(Registry*));     
        for (i = 0; (*r1)[i]->id != -1; i++); 
        for (int k = i - 1; k >= 0; ) r[rAmount++] = (*r1)[k--];
        for (j = 0; r2[j]->id != -1;) r[rAmount++] = r2[j++];
        
        // END REGISTRY FLAG
        r[rAmount++] = (*r1)[i];
        r = realloc(r, rAmount * sizeof(Registry*));
        
        // FREE 
        free(r2[j]), free(*r1), free(r2);

        // ORDER
        MergeSort(r, 0, rAmount - 2, 0, sortBySize);
        
        if (0) {
                printf("%d\n", rAmount);
                For(i, 0, rAmount - 1)
                        printf("|%d\n", r[i]->rSize);
        }

        // TOP LIST
        fseek(f, 1, SEEK_SET);
        fwrite(&r[0]->pointer, sizeof(ll), 1, f);

        // REMOVE
        for (i = 0; i < rAmount - 1; i++) {
                
                // ADJUST POINTER
                fseek(f, r[i]->pointer, SEEK_SET);

                // REMOVE AND LINK
                fwrite("*", sizeof(char), 1, f);
                fseek(f, 4, SEEK_CUR);
                
                // NEXT LINK
                if (i + 2 < rAmount) fwrite(&r[i + 1]->pointer, sizeof(ll), 1, f);
                else fseek(f, 8, SEEK_CUR);
                
                // FILL
                For(j, 0, r[i]->rSize - 8) fwrite("@", sizeof(char), 1, f);
        }

        *r1 = r;
        Free_Registries(r2), free(h);
}

void DataLog_Update(FILE* f, Registry** r, char* field, char* value) {

        // TEMP VARIABLES
        int tD;
        double tLf;
        
        char** t = malloc(5 * sizeof(char*)); 
        For(i, 0, 5) t[i] = malloc(110 * sizeof(char));

        // JUMP TO CHOOSED FIELD AND COMPARE THE VALUES
        int j, seekRole = 0;

        // ANALYSE AND ADJUST GIVEN SEARCH INFO
        if (strcmp(field, "idServidor") == 0)                   j = 8,  tD = atoi(value);
        else if (strcmp(field, "salarioServidor") == 0)         j = 12, tLf = atof(value);
        else if (strcmp(field, "telefoneServidor") == 0)        j = 20;
        else if (strcmp(field, "nomeServidor") == 0)            j = 34;
        else if (strcmp(field, "cargoServidor") == 0)           j = 34, seekRole = 1;

        Registry** toRemove = malloc(File_Size(f) / PAGE * 820 * sizeof(Registry));
        int i, rAmount = 0;

        // UPDATE
        for (i = 0; r[i]->id != -1; i++) {
                
                fseek(f, r[i]->pointer + j, SEEK_SET);

                if (j == 8) fwrite(&tD, sizeof(int), 1, f);
                else if (j == 12) fwrite(&tLf, sizeof(double), 1, f);
                else if (j == 20) {
                        if(strlen(value)) fwrite(value, sizeof(char), 14, f);
                        else {
                                fwrite("\0", sizeof(char), 1, f);
                                For(i, 0, 13) fwrite("@", sizeof(char), 1, f);
                        }
                }

                else if (j == 34) {

                        // MISCELLANEOUS
                        For(i, 0, 5) t[i][0] = '\0';

                        //
                        int realSize = 34;
                        int field4 = 0, field5 = 0, targetFSize;
                        
                        if (r[i]->nSize) realSize += field4 = 4 + r[i]->nSize;
                        if (r[i]->cSize) realSize += field5 = 4 + r[i]->cSize;

                        //
                        int upField = strlen(value);
                        if (upField) upField += 6;

                        //
                        int newNSize = 0, newCSize = 0;
                        char newName[110] = "\0", newRole[110] = "\0";

                        if (seekRole) {
                                strcpy(newName, r[i]->name);
                                strcpy(newRole, value);
                                targetFSize = field5;
                        }
                        else {
                                strcpy(newName, value);
                                strcpy(newRole, r[i]->role);
                                targetFSize = field4;
                        }

                        if (strlen(newName)) newNSize += strlen(newName) + 2;
                        if (strlen(newRole)) newCSize += strlen(newRole) + 2;

                        int rest = (r[i]->rSize - realSize) + (targetFSize - upField);

                        // DONT FIT 
                        if (rest < 0) {

                                sprintf(t[0], "%d", r[i]->id);
                                sprintf(t[1], "%lf", r[i]->wage);
                                strcpy(t[2], r[i]->phone);
                                strcpy(t[3], newName);
                                strcpy(t[4], newRole);

                                Registry* newR = Registry_New(t);
                                DataLog_AddRegistry(f, newR), free(newR);

                                toRemove[rAmount++] = r[i];
                        }
                        // FIT
                        else if (upField) {
                                // REPLACE NAME AND ROLE
                                fseek(f, r[i]->pointer + 5 + j, SEEK_SET);
                                if (newNSize) {
                                        fwrite(&newNSize, sizeof(int), 1, f);
                                        fwrite("n", sizeof(char), 1, f);
                                        fwrite(newName, sizeof(char), newNSize - 1, f);
                                }
                                if (newCSize) {
                                        fwrite(&newCSize, sizeof(int), 1, f);
                                        fwrite("c", sizeof(char), 1, f);
                                        fwrite(newRole, sizeof(char), newCSize - 1, f);
                                }
                                // FILL
                                For(i, 0, rest) fwrite("@", sizeof(char), 1, f);
                        }
                }
        }

        // END REGISTRY FLAG
        toRemove[rAmount++] = r[i];
        toRemove = realloc(toRemove, rAmount * sizeof(Registry*));

        DataLog_Remove(f, &toRemove);
        free(toRemove);

        For(i, 0, 5) free(t[i]);
        free(t);
}

void Free_Registries(Registry** r) {

        int i;
        for (i = 0; r[i]->id != -1; i++) free(r[i]);
        free(r[i]), free(r);
}

void DataLog_PrintForOP2(Registry* r){

        if (r->status == '*') return;

        printf("%d ", r->id);
        r->wage == -1 ? (printf("         ")) : printf("%.2lf ", r->wage);
        !strlen(r->phone) ? printf("              ") : printf("%.14s", r->phone);

        if (r->nSize) {
                printf(" %d", r->nSize - 2);
                printf(" %s", r->name);
        }

        if (r->cSize) {
                printf(" %d", r->cSize - 2);
                printf(" %s", r->role);
        }

        printf("\n");
}

void DataLog_PrintForOP3(FILE* f, Registry** r){

        Header* h = DataLog_GetHeader(f);

        int i = 0;
        for (; r[i]->id != -1; i++) {

                printf("%s: ", h->desc[0]);
                printf("%d\n", r[i]->id);

                printf("%s: ", h->desc[1]);
                r[i]->wage == -1           ? printf("valor nao declarado\n") : printf("%.2lf\n", r[i]->wage);

                printf("%s: ", h->desc[2]);
                !strlen(r[i]->phone)       ? printf("valor nao declarado\n") : printf("%.14s\n", r[i]->phone);

                printf("%s: ", h->desc[3]);
                !r[i]->nSize                ? printf("valor nao declarado\n") : printf("%s\n", r[i]->name);

                printf("%s: ", h->desc[4]);
                !r[i]->cSize                ? printf("valor nao declarado\n") : printf("%s\n", r[i]->role);

                printf("\n");
        }

        free(h);
}

#include "DataLog.h"

int Index_GetAmount(Index** index) {

        if (index == NULL) return 0;

        int amount;
        for (amount = 0; index[amount]->pointer != -1; amount++);

        return amount;
}

Index* Index_New(char* key, long long pointer) {

        Index* index = malloc(sizeof(Index));
        
        strcpy(index->key, key);
        index->pointer = pointer;

        return index;
}

Index* Index_EndFlag() {

        Index* index = malloc(sizeof(Index));
        index->pointer = -1;

        return index;
}

void Free_Indexes(Index** index) {

        int i;
        for (i = 0; index[i]->pointer != -1; i++) free(index[i]);
        free(index[i]), free(index);
}

void DataIndex_AddHeader(FILE* f, int rAmount) {

        rewind(f);

        fwrite("0", sizeof(char), 1, f);
        fwrite(&rAmount, sizeof(int), 1, f);

        For(i, 0, 31995) fwrite("@", sizeof(char), 1, f);
}

void DataIndex_AddIndex(FILE* f, Index* index) {

        fseek(f, 0, SEEK_END);

        fwrite(index->key, sizeof(char), strlen(index->key) + 1, f);
        For(i, 0, 119 - strlen(index->key)) fwrite("@", sizeof(char), 1, f);

        fwrite(&index->pointer, sizeof(ll), 1, f);
}

Registry** DataIndex_Recover(FILE* indexF, FILE* dataF, char* key, int* iAcess, int* dAcess) {

        // LOAD INDEX LIST
        Index** index = DataIndex_LoadAll(indexF);

        // ALLOCATE MAX POSSIBLE ELEMENTS
        Registry** r = malloc((File_GetSize(dataF) / PAGE) * 820 * sizeof(Registry*));
        int rAmount = 0;

        // GET POSITIONS OF SOUGHT KEY
        int* pos = Binary_Search(index, key, compIndexKey);

        // INITIALIZE ACESS COUNT VARIABLES
        if (iAcess != NULL) {
                
                if (pos[0] == -1) *dAcess = 1;
                else *dAcess = 2;

                *iAcess = ceil(File_GetSize(indexF) / PAGE);
        }

        // FOUND
        for (int i = 0; pos[i] != -1; i++) {
                
                r[rAmount++] = DataLog_RecoverAt(dataF, index[pos[i]]->pointer, 0);
                
                if (i) {
                        if (dAcess != NULL && 
                        index[pos[i - 1]]->pointer % (int)PAGE > 
                        index[pos[i]]->pointer % (int)PAGE) (*dAcess)++;
                }
        }
        free(pos);
        
        // FREE
        Free_Indexes(index);

        // END REGISTRY FLAG
        r[rAmount++] = Registry_EndFlag(r);
        r = realloc(r, rAmount * sizeof(Registry*));

        return r;
}

Index** DataIndex_LoadAll(FILE* f) {

        long int fSize = File_GetSize(f);
        fseek(f, 32000, SEEK_SET);

        Index** index = malloc((fSize / PAGE) * 266 * sizeof(Index*));
        int iAmount;

        char* key = malloc(120);
        ll pointer;

        for (iAmount = 0; ftell(f) != fSize; iAmount++) {

                fread(key, sizeof(char), 120, f);
                fread(&pointer, sizeof(ll), 1, f);

                index[iAmount] = Index_New(key, pointer);
        }
        free(key);
        
        index[iAmount++] = Index_EndFlag();
        index = realloc(index, iAmount * sizeof(Index*));

        return index;
}
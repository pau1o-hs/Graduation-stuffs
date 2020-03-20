// PAULO HENRIQUE DA SILVA
// 10734515

#include "lib/DataLog.h"
#include <time.h>

// TEMP VARIABLES
int tD;
double tLf;
char **t, tC;

// RESET VARIABLES
void init() {

        tD = -1;
        tLf = -1;
        For(i, 0, 5) t[i][0] = '\0';
}

int main(int argc, char const *argv[]) {

        // BENCHMARK
        clock_t start, end;
        double cpu_time_used;

        start = clock();

        // START
        t = malloc(5 * sizeof(char*));
        For(i, 0, 5) t[i] = malloc(110 * sizeof(char));

        FILE* dataBase;
        FILE* dataBin;

        char fName[30];
        int op, fSize;

        scanf("%d %s", &op, fName);

        // OPEN AND CHECK IF FILE IS CONSISTENT
        if (op != 1) {

                // OPEN FILE
                dataBin = fopen(fName, "rb+");

                // CHECK ERRORS
                if (!File_isConsistent(dataBin)) {
                        printf("Falha no processamento do arquivo.\n");
                        goto END;
                }

                // FILE SIZE
                fSize = File_GetSize(dataBin);

                // NO REGISTRIES
                if (fSize <= 32000) {
                        printf("Registro inexistente.\n");
                        goto END;
                }

                // STATUS EQUALS TO 0 WHILE FILE IS BEING UPDATED
                // Header_SwitchStatus(dataBin, '0');
        }

        // STORE .CSV DATA
        if (op == 1) {

                dataBase = fopen(fName, "r");
                if (dataBase == NULL) return printf("Falha no carregamento do arquivo.\n"), 0;

                // STORE HEADER
                fscanf(dataBase, "%[^,],%[^,],%[^,],%[^,],%[^\r\n]\r\n",
                t[0], t[1], t[2], t[3], t[4]);

                dataBin = fopen("arquivoTrab1.bin", "wb+");
                DataLog_AddHeader(dataBin, t);
                Registry* r;

                init();

                // STORE REGISTRIES
                while (fscanf(dataBase, "%[^,]", t[0]) != EOF) {

                        fscanf(dataBase, ",%[^,]", t[1]);
                        fscanf(dataBase, ",%[^,]", t[2]);
                        fscanf(dataBase, ",%[^,]", t[3]);
                        fscanf(dataBase, ",%[^\r\n]\r\n", t[4]);

                        r = Registry_New(t);
                        DataLog_AddRegistry(dataBin, NULL, r);
                        init(), free(r);
                }

                printf("arquivoTrab1.bin");
                fclose(dataBase);
        }

        // RECOVER ALL
        else if (op == 2) {

                fseek(dataBin, 32000, SEEK_SET);

                // RECOVER REGISTRIES IN THE FILE POINTER POSITION, THEN PRINT THEM UNTIL EOF
                while (ftell(dataBin) != fSize) {

                        Registry* r = DataLog_RecoverAt(dataBin, ftell(dataBin), 1);
                        if (Registry_isActive(r)) DataLog_PrintForOP2(r);
                        free(r);
                }

                printf("Número de páginas de disco acessadas: %d\n", (int) ceil(fSize / PAGE));
        }

        // RECOVER SPECIFIED
        else if (op == 3) {

                int diskAcess = 0;
                scan_quote_string(t[0]);
                scan_quote_string(t[1]);

                // FIND REGISTRIES WITH GIVEN INFORMATIONS, THEN PRINT
                Registry** r = DataLog_Recover(dataBin, t[0], t[1], &diskAcess);

                DataLog_PrintForOP3(dataBin, r);
                if (r[0]->pointer != -1) printf("Número de páginas de disco acessadas: %d\n", diskAcess);
                else printf("Registro inexistente.\n");

                Free_Registries(r);
        }

        // REMOVE SPECIFIED
        else if (op == 4 || op == 12) {

                // OPEN INDEX LIST FILE
                if (op == 12) scanf("%s", fName);

                int m;
                scanf("%d", &m);

                Registry** r = malloc((fSize / PAGE) * 820 * sizeof(Registry*));
                int rAmount = 0;

                
                For(i, 0, m) {

                        init();

                        // GET INPUT
                        scan_quote_string(t[0]);
                        scan_quote_string(t[1]);

                        // FIND REGISTRIES WITH GIVEN INFORMATIONS, THEN DELETE
                        Registry** _r = DataLog_Recover(dataBin, t[0], t[1], NULL);

                        int j;
                        for (j = 0; _r[j]->pointer != -1; j++) r[rAmount++] = _r[j];
                        free(_r[j]), free(_r);
                }

                r[rAmount++] = Registry_EndFlag();
                r = realloc(r, rAmount * sizeof(Registry*));

                if (op == 12) DataLog_Remove(dataBin, fName, r);
                else DataLog_Remove(dataBin, NULL, r);

                Free_Registries(r);
        }

        // INSERT REGISTRY
        else if (op == 5 || op == 13) {

                FILE* dataIndex;
                Index** index;
                int iAmount;

                if (op == 13) {
                        scanf("%s", fName);
                        dataIndex = fopen(fName, "rb+");

                        // CHECK ERRORS
                        if (!File_isConsistent(dataIndex)) {
                                printf("Falha no processamento do arquivo.\n");
                                goto END;
                        } 

                        index = DataIndex_LoadAll(dataIndex);
                        iAmount = Index_GetAmount(index);

                        index = realloc(index, (fSize / PAGE) * 820 * sizeof(Index*));                   
                }

                int m;
                scanf("%d", &m);

                For(i, 0, m) {

                        // GET INPUT
                        scan_quote_string(t[0]);
                        scan_quote_string(t[1]);
                        scan_quote_string(t[2]);
                        scan_quote_string(t[3]);
                        scan_quote_string(t[4]);

                        if (DEBUG) printf("%s %s %s t[1]: %s t[2]: %s\n", t[0], t[1], t[2], t[3], t[4]);

                        Registry* r = Registry_New(t);

                        if (op == 13 && strlen(t[3])) {
                                index[iAmount + 1] = index[iAmount];
                                DataLog_AddRegistry(dataBin, &index[iAmount++], r);
                        }
                        else DataLog_AddRegistry(dataBin, NULL, r);

                        free(r);
                }

                // REWRITE INDEX LIST FILE
                if (op == 13) {

                        fclose(dataIndex);
                        dataIndex = fopen(fName, "wb+");

                        index = realloc(index, (iAmount + 1) * sizeof(Index*));
                        Merge_Sort((void**) index, 0, iAmount - 1, 3, compIndexKey);

                        DataIndex_AddHeader(dataIndex, iAmount);
                        
                        int i;
                        for (i = 0; i < iAmount; i++) {
                                
                                // printf("%s %lld\n", index[i]->key, index[i]->pointer);
                                DataIndex_AddIndex(dataIndex, index[i]);
                                free(index[i]);
                        }
                        free(index[i]), free(index);

                        Header_SwitchStatus(dataIndex, '1');

                        binarioNaTela1(dataIndex);
                        fclose(dataIndex);
                }
        }

        // UPDATE SPECIFIED
        else if (op == 6) {

                int m;
                scanf("%d", &m);

                For(i, 0, m) {

                        // GET INPUT
                        scan_quote_string(t[0]);
                        scan_quote_string(t[1]);
                        scan_quote_string(t[2]);
                        scan_quote_string(t[3]);

                        // FIND REGISTRIES WITH GIVEN INFORMATIONS, THEN UPDATE
                        Registry** r = DataLog_Recover(dataBin, t[0], t[1], NULL);
                        DataLog_Update(dataBin, r, t[2], t[3]);

                        Free_Registries(r);
                }
        }

        // SORT | MERGE FILES
        else if (op >= 7 && op <= 9) {

                FILE *dataBin2, *sortedData;

                // OPEN OUTPUT FILE
                scanf("%s", fName);
                if (op != 7) {

                        // OPEN .BIN 2
                        dataBin2 = fopen(fName, "rb+");
                        scanf("%s", fName);

                        // CHECK ERRORS
                        if (!File_isConsistent(dataBin2)) {
                                printf("Falha no processamento do arquivo.\n");
                                goto END;
                        }
                }

                sortedData = fopen(fName, "wb+");

                // STORE HEADER
                Header* h = DataLog_GetHeader(dataBin);
                For(i, 0, 5) strcpy(t[i], h->desc[i]);
                DataLog_AddHeader(sortedData, t);
                
                Registry** r = malloc((fSize / PAGE) * 820 * sizeof(Registry*));
                int rAmount = 0;

                // RECOVER REGISTRIES IN THE FILE POINTER POSITION
                fseek(dataBin, 32000, SEEK_SET);
                while (ftell(dataBin) != fSize) {

                        r[rAmount] = DataLog_RecoverAt(dataBin, ftell(dataBin), 0);

                        if (Registry_isActive(r[rAmount])) rAmount++;
                        else free(r[rAmount]);
                }

                // GET SECOND FILE DATA
                if (op != 7) {

                        int fSize2 = File_GetSize(dataBin2);

                        fseek(dataBin2, 32000, SEEK_SET);
                        while (ftell(dataBin2) != fSize2) {

                                r[rAmount] = DataLog_RecoverAt(dataBin2, ftell(dataBin2), 0);

                                if (Registry_isActive(r[rAmount])) rAmount++;
                                else free(r[rAmount]);
                        }
                }
                r = realloc(r, rAmount * sizeof(Registry*));

                // SORT AND STORE REGISTRIES
                if (op == 9) For(i, 0, rAmount) r[i]->status = '*';
                Merge_Sort((void**) r, 0, rAmount - 1, op - 7, compRegID);
                For (i, 0, rAmount) {
                        if (Registry_isActive(r[i])) 
                                DataLog_AddRegistry(sortedData, NULL, r[i]);
                        free(r[i]);
                }
                free(r);
                
                // PRINT AND CLOSE
                Header_SwitchStatus(sortedData, '1');
                binarioNaTela1(sortedData);

                if (op != 7) fclose(dataBin2);
                fclose(sortedData);
        }

        // INDEX LIST FILE
        else if (op == 10) {
                
                // OPEN OUTPUT FILE
                scanf("%s", fName);
                FILE* dataIndex = fopen(fName, "wb+");

                Index** index = malloc((fSize / PAGE) * 820 * sizeof(Index*));
                int iAmount = 0;

                // RECOVER REGISTRIES IN THE FILE POINTER POSITION
                fseek(dataBin, 32000, SEEK_SET);
                while (ftell(dataBin) != fSize) {

                        Registry* _r = DataLog_RecoverAt(dataBin, ftell(dataBin), 0);

                        if (Registry_isActive(_r) && _r->nSize) {
                                index[iAmount++] = Index_New(_r->name, _r->pointer);
                        }

                        free(_r);
                }
                index[iAmount] = Index_EndFlag();

                // STORE HEADER
                DataIndex_AddHeader(dataIndex, iAmount);

                // SORT AND STORE INDEXES
                Merge_Sort((void**)index, 0, iAmount - 1, 3, compIndexKey);
                For(i, 0, iAmount) DataIndex_AddIndex(dataIndex, index[i]);

                // FREE
                Header_SwitchStatus(dataIndex, '1');
                binarioNaTela1(dataIndex);

                Free_Indexes(index), fclose(dataIndex);
        }

        // RECOVER SPECIFIED (USING INDEX LIST)
        else if (op == 11 || op == 14) {

                // OPEN INDEX LIST FILE
                scanf("%s", fName);
                FILE* dataIndex = fopen(fName, "rb");

                // CHECK ERRORS
                if (!File_isConsistent(dataIndex)) {
                        printf("Falha no processamento do arquivo.\n");
                        goto END;
                }

                Header_SwitchStatus(dataIndex, '0');

                scan_quote_string(t[0]);
                scanf(" %[^\r\n]\r\n", t[1]);

                // INDEX ACESS / DISK ACESS
                int iAcess, dAcess = 0, dAcess2;

                // RECOVER AND PRINT
                Registry** r;
                if (op == 11) r = DataIndex_Recover(dataIndex, dataBin, t[1], &iAcess, &dAcess);
                else r = DataLog_Recover(dataBin, t[0], t[1], &dAcess2);
            
                if (op == 14) printf("*** Realizando a busca sem o auxílio de índice\n");
                
                if (r[0]->pointer == -1) {
                        printf("Registro inexistente.\n");
                        if (op == 14) printf("Número de páginas de disco acessadas: %d\n", (int) ceil(fSize / PAGE));
                }
                else {
                        DataLog_PrintForOP3(dataBin, r);
                        if (op == 14) printf("Número de páginas de disco acessadas: %d\n", dAcess2);
                }
                
                if (op == 14) {
                        Free_Registries(r);
                        r = DataIndex_Recover(dataIndex, dataBin, t[1], &iAcess, &dAcess);

                        printf("*** Realizando a busca com o auxílio de um índice secundário fortemente ligado\n");
                        if (r[0]->pointer == -1) printf("Registro inexistente.\n");
                        else DataLog_PrintForOP3(dataBin, r);
                }

                if (op == 14 || r[0]->pointer != -1) {
                        printf("Número de páginas de disco para carregar o arquivo de índice: %d\n", iAcess);
                        printf("Número de páginas de disco para acessar o arquivo de dados: %d\n", dAcess);
                }

                if (op == 14) printf("\nA diferença no número de páginas de disco acessadas: %d\n", abs(dAcess2 - dAcess));

                // FREE
                Header_SwitchStatus(dataIndex, '1');
                Free_Registries(r), fclose(dataIndex);
        }

        // FILE CHANGED SUCCESSFULLY
        Header_SwitchStatus(dataBin, '1');

        // if (op >= 4 && op <= 6) binarioNaTela1(dataBin);
        
        END:

        For(i, 0, 5) free(t[i]); free(t);
        if (dataBin != NULL) fclose(dataBin);

        // BENCHMARK
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        // printf("\nTime: %lf\n", cpu_time_used);
        
        return 0;
}

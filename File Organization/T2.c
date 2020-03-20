// PAULO HENRIQUE DA SILVA
// 10734515

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "DataLog.h"
#include "Sorts.h"
#include "EscreverTela.h"

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
                if (!DataLog_isConsistent(dataBin)) {
                        printf("Falha no processamento do arquivo.\n");
                        goto END;
                }

                // FILE SIZE
                fSize = File_Size(dataBin);

                // NO REGISTRIES
                if (fSize <= 32000) {
                        printf("Registro inexistente.\n");
                        goto END;
                }

                // STATUS EQUALS TO 0 WHILE FILE IS BEING UPDATED
                Header_SwitchStatus(dataBin, '0');
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
                        DataLog_AddRegistry(dataBin, r);
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
                        if (!Registry_isRemoved(r)) DataLog_PrintForOP2(r);
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
                if (diskAcess) printf("Número de páginas de disco acessadas: %d\n", diskAcess);
                else printf("Registro inexistente.\n");

                Free_Registries(r);
        }

        // REMOVE SPECIFIED
        else if (op == 4) {

                int m;
                scanf("%d", &m);

                For(i, 0, m) {

                        init();

                        // GET INPUT
                        scan_quote_string(t[0]);
                        scan_quote_string(t[1]);

                        // FIND REGISTRIES WITH GIVEN INFORMATIONS, THEN DELETE
                        Registry** r = DataLog_Recover(dataBin, t[0], t[1], NULL);
                        DataLog_Remove(dataBin, &r);

                        Free_Registries(r);
                }
        }

        // INSERT REGISTRY
        else if (op == 5) {

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
                        DataLog_AddRegistry(dataBin, r), free(r);
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
                        if (!DataLog_isConsistent(dataBin2)) {
                                printf("Falha no processamento do arquivo.\n");
                                goto END;
                        }
                }

                sortedData = fopen(fName, "wb+");

                // STORE HEADER
                Header* h = DataLog_GetHeader(dataBin);
                char** desc = Header_GetDescriptions(h);
                DataLog_AddHeader(sortedData, desc);

                // FREE
                For(i, 0, 5) free(desc[i]); 
                free(desc), free(h);
                
                Registry** r = malloc((fSize / PAGE) * 820 * sizeof(Registry*));
                int rAmount = 0;

                // RECOVER REGISTRIES IN THE FILE POINTER POSITION
                fseek(dataBin, 32000, SEEK_SET);
                while (ftell(dataBin) != fSize) {

                        r[rAmount] = DataLog_RecoverAt(dataBin, ftell(dataBin), 0);

                        if (!Registry_isRemoved(r[rAmount])) rAmount++;
                        else free(r[rAmount]);
                }

                if (op != 7) {

                        int fSize2 = File_Size(dataBin2);

                        fseek(dataBin2, 32000, SEEK_SET);
                        while (ftell(dataBin2) != fSize2) {

                                r[rAmount] = DataLog_RecoverAt(dataBin2, ftell(dataBin2), 0);

                                if (!Registry_isRemoved(r[rAmount])) rAmount++;
                                else free(r[rAmount]);
                        }
                }
                r = realloc(r, rAmount * sizeof(Registry*));

                // SORT AND STORE REGISTRIES
                if (op == 9) For(i, 0, rAmount) Registry_SwitchStatus(r[i], '*');
                MergeSort(r, 0, rAmount - 1, op - 7, sortByID);
                For (i, 0, rAmount) {
                        if (!Registry_isRemoved(r[i])) 
                                DataLog_AddRegistry(sortedData, r[i]);
                        free(r[i]);
                }
                free(r);
                
                // // PRINT AND CLOSE
                Header_SwitchStatus(sortedData, '1');
                binarioNaTela1(sortedData);

                if (op != 7) fclose(dataBin2);
                fclose(sortedData);
        }

        // FILE CHANGED SUCCESSFULLY
        Header_SwitchStatus(dataBin, '1');

        if (op >= 4 && op <= 6) binarioNaTela1(dataBin);
        
        END:

        For(i, 0, 5) free(t[i]); free(t);
        if (dataBin != NULL) fclose(dataBin);

        // BENCHMARK
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        // printf("\nTime: %lf\n", cpu_time_used);

        return 0;
}

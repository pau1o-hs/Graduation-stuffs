#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define DEBUG 0

typedef struct data {

     char specie[100];
     double dimension[4];
}DATA;

DATA dataBase[1000], dataTest[1000];
int nTestData, nBaseData;

void GetData(DATA* _data, FILE* file, int* nData) {

     //READ FIRST ROW
     fscanf(file, "%*[^\n]");

     while (!feof(file)/*/fscanf(file, "\n%lf,%lf,%lf,%lf,\"%[^\"]\"", &_data[*nData].dimension[0], &_data[*nData].dimension[1], &_data[*nData].dimension[2], &_data[*nData].dimension[3], _data[*nData].specie) == 1*/) {

          fscanf(file, "%lf,%lf,%lf,%lf,\"%[^\"]\"", &_data[*nData].dimension[0], &_data[*nData].dimension[1], &_data[*nData].dimension[2], &_data[*nData].dimension[3], _data[*nData].specie);
          *nData += 1;
     }

     *nData -= 1;

     if(DEBUG) {
          for (int i = 0; i < *nData; i++)
               printf("%.1lf %.1lf %.1lf %.1lf %s\n", _data[i].dimension[0], _data[i].dimension[1], _data[i].dimension[2], _data[i].dimension[3], _data[i].specie);
          printf("\n");
     }
}

void GetEDists(double dist[nTestData][nBaseData], DATA* dataTest, DATA* dataBase) {

     for (int i = 0; i < nTestData; i++) {

          for (int j = 0; j < nBaseData; j++) {

               if (DEBUG) printf("V (%.1lf - %.1lf)² + (%.1lf - %.1lf)² + (%.1lf - %.1lf)² + (%.1lf - %.1lf)² | = ", dataBase[j].dimension[0], dataTest[i].dimension[0], dataBase[j].dimension[1], dataTest[i].dimension[1], dataBase[j].dimension[2], dataTest[i].dimension[2], dataBase[j].dimension[3], dataTest[i].dimension[3]);
               dist[i][j] = sqrt(
                              pow(dataBase[j].dimension[0] - dataTest[i].dimension[0], 2) +
                              pow(dataBase[j].dimension[1] - dataTest[i].dimension[1], 2) +
                              pow(dataBase[j].dimension[2] - dataTest[i].dimension[2], 2) +
                              pow(dataBase[j].dimension[3] - dataTest[i].dimension[3], 2));

               if (DEBUG) printf("%.5lf\n", dist[i][j]);
          }

          if (DEBUG) printf("\n");
     }
}

char* _kNN(int i, double dist[nTestData][nBaseData], int k) {

     char* ans = malloc(100 * sizeof(char));
     char tempAns[k][100];
     double minDist[k];

     for (int j = 0; j < k; j++) {

          int t = 0;
          strcpy(tempAns[j], "");
          minDist[j] = 1000000;
          for (int l = 0; l < nBaseData; l++) {

               if (minDist[j] > dist[i][l]) {

                    strcpy(tempAns[j], dataBase[l].specie);
                    minDist[j] = dist[i][l];
                    t = l;
               }

               if (l + 1 == nBaseData) dist[i][t] = 1000000;
          }
     }

     if (DEBUG) {
          for (int j = 0; j < k; j++)
          printf("%s %.1lf\n", tempAns[j], minDist[j]);
     }

     bool onlyOneSpecie = true;
     int currentMax = 0;
     int equalsIndex[k];
     for (int j = 0; j < k; j++) {

          equalsIndex[j] = 0;
          int freq = 0;
          for (int l = 0; l < k; l++) {

               if (j != l && strcmp(tempAns[j], tempAns[l]) == 0) freq++;
          }

          if (freq > currentMax) {
               strcpy(ans, tempAns[j]);
               currentMax = freq;
               onlyOneSpecie = true;
               for (int l = 0; l < k; l++) {

                    if (strcmp(tempAns[j], tempAns[l]) == 0) equalsIndex[l] = 1;
                    else equalsIndex[l] = 0;
               }
          }
          else if (freq == currentMax) {

               onlyOneSpecie = false;
               for (int l = 0; l < k; l++) {

                    if (strcmp(tempAns[j], tempAns[l]) == 0) equalsIndex[l] = 1;
               }
          }
     }

     if (!onlyOneSpecie) {

          int count = 0;
          for (int j = 0; j < k; j++) {

               double minOfMins = 1000000;
               for (int l = 0; l < k; l++) {

                    if (equalsIndex[l] && minDist[l] < minOfMins) {

                         strcpy(ans, tempAns[l]);
                         minOfMins = minDist[l];
                    }
               }
          }
     }

     return ans;
}

int main(int argc, char const *argv[]) {

     FILE *trainFile, *testFile;

     char trainFilename[100], testFilename[100];
     int k;

     scanf("%s%s%d", trainFilename, testFilename, &k);

     trainFile = fopen(trainFilename, "r");
     testFile = fopen(testFilename, "r");

     //GET TRAINFILE DATA
     GetData(dataBase, trainFile, &nBaseData);

     //GET TESTFILE DATA
     GetData(dataTest, testFile, &nTestData);

     if (k < 1 || k > nBaseData) {
          printf("k is invalid\n");
          return 0;
     }

     // //MAKE EUCLIDIAN DISTANCES
     double dist[nTestData][nBaseData];
     GetEDists(dist, dataTest, dataBase);

     int sucessRate = 0;
     for (int i = 0; i < nTestData; i++) {

          char* strA = _kNN(i, dist, k);
          if (strcmp(dataTest[i].specie, strA) == 0) sucessRate++;

          printf("%s %s\n", strA, dataTest[i].specie);
          free(strA);
     }

     printf("%.4lf\n", (double) sucessRate / nTestData);

     fclose(trainFile);
     fclose(testFile);
     return 0;
}

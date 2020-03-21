#include <stdio.h>
#include <math.h>
#include <stdbool.h>

     bool calculated = false;
     int n = 0, i = 0;
     float t = 0;
     int matriz[16][16];

float calcAverageLine(int line){

     float soma = 0;
     float numElements = 0;
     float average = 0;

     for (int j = 0; j < i; j++) {

          soma += matriz[line][j];
          if(matriz[line][j] != 0) numElements++;
     }

     average = soma / numElements;

     return average;
}

float calculateVote(int line, int column){

     float calcCos = 0, calcCosNum = 0, calcCosDen1 = 0, calcCosDen2 = 0;
     float calcVote = 0, calcVoteNum = 0, calcVoteDen = 0;

     for (int j = 0; j != line; j++) {

          calcCosNum = 0;
          calcCosDen1 = 0;
          calcCosDen2 = 0;

          for (int k = 0; k < i; k++) {

               if(matriz[j][column] != 0){

                    calcCosNum += matriz[j][k] * matriz[line][k];
                    calcCosDen1 += pow(matriz[j][k], 2);
                    calcCosDen2 += pow(matriz[line][k], 2);
               }
               else{
                    //if(j + 1 != line) j++;
               }
          }

          calcCos = calcCosNum / (sqrt(calcCosDen1) * sqrt(calcCosDen2));

          if(calcCos > t){

               calcVoteNum += calcCos * (matriz[j][column] - calcAverageLine(j));
               calcVoteDen += fabs(calcCos);

               calcVote = calcAverageLine(line) + (calcVoteNum / calcVoteDen);
          }
     }

     for (int j = line + 1; j < n; j++) {

          calcCosNum = 0;
          calcCosDen1 = 0;
          calcCosDen2 = 0;

          for (int k = 0; k < i; k++) {

               if(matriz[j][column] != 0){

                    calcCosNum += matriz[j][k] * matriz[line][k];
                    calcCosDen1 += pow(matriz[j][k], 2);
                    calcCosDen2 += pow(matriz[line][k], 2);
               }
               else{
                    //if(j + 1 < n) j++;
               }
          }

          calcCos = calcCosNum / (sqrt(calcCosDen1) * sqrt(calcCosDen2));

          if(calcCos > t){

               calcVoteNum += calcCos * (matriz[j][column] - calcAverageLine(j));
               calcVoteDen += fabs(calcCos);

               calcVote = calcAverageLine(line) + (calcVoteNum / calcVoteDen);
          }
     }

     return calcVote;
}

int main(){

     scanf("%d%d%f", &n, &i, &t);

     for (int j = 0; j < n; j++) {

          for (int k = 0; k < i; k++) {

               scanf("%d", &matriz[j][k]);
          }
     }

     for (int l = 0; l < n; l++) {

          for(int m = 0; m < i; m++){

               if(matriz[l][m] == 0){

                    if(calculateVote(l, m) <= 0) printf("DI ");
                    if(calculateVote(l, m) > 0) printf("%.2f ", calculateVote(l, m));

                    calculated = true;
               }
          }

           if(calculated) printf("\n");
           calculated = false;
     }

     return 0;
}

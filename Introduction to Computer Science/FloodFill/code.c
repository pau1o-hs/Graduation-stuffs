#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

     const double errorMargin = 0.001;

     //For Input
     double map[9][9];
     double base[2][2];
     double nRec;

     //In heap memory
     int (*recPos)[2];

     //For Equation
     double E[2];
     double p;
     bool resourceFounded;

void Input(){

     //Set Matrix
     for (int i = 0; i < 9; i++) {

          for (int j = 0; j < 9; j++) {

               scanf("%lf", &map[i][j]);

               if (map[i][j] == 0)
               p++;
          }
     }

     //Set bases position
     for (int i = 0; i < 2; i++) {

          for (int j = 0; j < 2; j++) scanf("%lf", &base[i][j]);
     }

     //Set resources
     scanf("%lf", &nRec);

     //Setting resources position matrix in heap memory;
     recPos = (int (*)[2]) malloc(sizeof(int) * nRec);
     for (int i = 0; i < nRec; i++) {

          for (int j = 0; j < 2; j++) scanf("%d", &recPos[i][j]);
     }
}

void ResetMatrix() {

     resourceFounded = false;

     for (int i = 0; i < 9; i++) {

          for (int j = 0; j < 9; j++) {

               if(map[i][j] == 2)
               map[i][j] = 0;
          }
     }
}

void CheckResourcesPosition(int line, int column, int targetLine, int targetColumn){

     if(line == targetLine && column == targetColumn)
     resourceFounded = true;
}

double FloadFill(int line, int column, int targetLine, int targetColumn){

     double distToRec = 1;
     map[line][column] = 2;

     //Try anothers directions if end the current
     for (int i = 0; i < 4; i++) {

          //Found some resource?
          CheckResourcesPosition(line, column, targetLine, targetColumn);

          if(!resourceFounded) {

               if (map[line + 1][column] != 0 || line + 1 >= 9) { //Try North

                    if (map[line - 1][column] != 0 || line - 1 < 0) { //Try West

                         if (map[line][column - 1] != 0 || column - 1 < 0) { // Try East

                              if (map[line][column + 1] == 0 && column + 1 < 9) // True?
                                   distToRec += FloadFill(line, column + 1, targetLine, targetColumn);//East
                         }
                         else distToRec += FloadFill(line, column - 1, targetLine, targetColumn);//West
                    }
                    else distToRec += FloadFill(line - 1, column, targetLine, targetColumn);//North
               }
               else distToRec += FloadFill(line + 1, column, targetLine, targetColumn);//South
          }
     }

     return distToRec;
}

void Calculate(){

     double distSum[2];

     //Calculate sum of distances
     for (int i = 0; i < 2; i++) {

          distSum[i] = 0;
          for (int j = 0; j < nRec; j++) {

               distSum[i] += FloadFill(base[i][0], base[i][1], recPos[j][0], recPos[j][1]);
               ResetMatrix();
          }
     }

     E[0] = (1 / (nRec)) * (distSum[0] / p);
     E[1] = (1 / (nRec)) * (distSum[1] / p);

     printf("%lf\n", E[0]);
     printf("%lf\n", E[1]);
}

int main(){

     Input();
     Calculate();

     if(fabs(E[0] - E[1]) < errorMargin) {

          printf("O mapa eh balanceado\n");
     }
     else {

          if (E[0] < E[1]) printf("O jogador 1 possui vantagem\n");
          if (E[1] < E[0]) printf("O jogador 2 possui vantagem\n");
     }

     free(recPos);
     return 0;
}

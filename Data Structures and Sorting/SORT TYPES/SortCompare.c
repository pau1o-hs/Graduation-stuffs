#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct sort{

       char name[50];
       int c;
       int m;
}SORT;

typedef struct comp{

       int largerC;
       char nameLargerC[50];

       int lesserC;
       char nameLesserC[50];

       int largerM;
       char nameLargerM[50];

       int lesserM;
       char nameLesserM[50];
}COMPARE;

void UpdateInfo(SORT* sortConst, COMPARE* compare) {

       //LESSER C
       if (sortConst->c < compare->lesserC) {

              compare->lesserC = sortConst->c;
              strcpy(compare->nameLesserC, sortConst->name);
       }

       //LARGER C
       if (sortConst->c > compare->largerC) {

              compare->largerC = sortConst->c;
              strcpy(compare->nameLargerC, sortConst->name);
       }

       //LESSER M
       if (sortConst->m < compare->lesserM) {

              compare->lesserM = sortConst->m;
              strcpy(compare->nameLesserM, sortConst->name);
       }

       //LARGER M
       if (sortConst->m > compare->largerM) {

              compare->largerM = sortConst->m;
              strcpy(compare->nameLargerM, sortConst->name);
       }

       return;
}

void BubbleSort(SORT* sortConst, int* v, int n) {

       for (int i = 0; i < n - 1; i++) {

              for (int j = i + 1; j < n; j++, sortConst->c++) {

                     if (v[j] < v[i]) {

                            int tmp = v[i];
                            v[i] = v[j];
                            v[j] = tmp;

                            sortConst->m++;
                     }
              }
       }

       return;
}

void InsertionSort(SORT* sortConst, int* v, int n) {

       for (int i = 1; i < n; i++) {

              int j, current = v[i];
              for (j = i - 1; j >= 0 && current < v[j]; j--, sortConst->c++) {

                     v[j + 1] = v[j];
                     sortConst->m++;
              }
              v[j + 1] = current;
              sortConst->m++;
       }

       return;
}

void MergeSort(SORT* sortConst, int* v, int l, int r) {

       if (l >= r) return;

       int mid = (l + r) / 2;

       MergeSort(sortConst, v, l, mid);
       MergeSort(sortConst, v, mid + 1, r);

       int i = l, j = mid + 1, k = 0;
       int tmp[r - l + 1];

       while (i <= mid && j <= r) {

              if (v[i] < v[j])
                     tmp[k++] = v[i++];
              else tmp[k++] = v[j++];

              sortConst->c++;
              sortConst->m++;
       }

       while (i <= mid) { tmp[k++] = v[i++]; sortConst->m++; }

       while (j <= r) { tmp[k++] = v[j++]; sortConst->m++; }

       for (i = l, k = 0; i <= r; i++) {

              v[i] = tmp[k++];
              sortConst->m++;
       }

       return;
}

void HeapSort(SORT* sortConst, int* v, int n) {

       int i = n / 2, parent, child, t;

       while (1) {

              if (i > 0) {

                     i--;
                     t = v[i];

                     sortConst->m++;
              }
              else {

                     n--;
                     if (!n) return;
                     t = v[n];
                     v[n] = v[0];

                     sortConst->m++;
              }

              parent = i;
              child = i * 2 + 1;
              sortConst->m++;

              while (child < n) {

                     sortConst->c += 3;
                     if ((child + 1 < n) && (v[child + 1] > v[child])) child++;
                     if (v[child] > t) {

                            v[parent] = v[child];
                            parent = child;
                            child = parent * 2 + 1;

                            sortConst->m++;
                     }
                     else break;
              }

              v[parent] = t;
              sortConst->m++;
       }

       return;
}

void QuickSort(SORT* sortConst, int* v, int l, int r) {

       int i = l, j = r, tmp;
       int pivot = v[(l + r) / 2];

       while (i <= j) {

              sortConst->c += 2;
              while (v[i] < pivot && i < r) { i++; sortConst->c++; }
              while (v[j] > pivot && j > l) { j--; sortConst->c++; }

              if (i <= j) {

                     tmp = v[i];
                     v[i] = v[j];
                     v[j] = tmp;

                     i++; j--;
                     sortConst->m++;
              }

              sortConst->c++;
       }

       if (j > l) QuickSort(sortConst, v, l, j + 1);
       if (i < r) QuickSort(sortConst, v, i, r);

       return;
}

int main(int argc, char const *argv[]) {

       int o, n;
       scanf("%d %d\n", &o, &n);

       COMPARE compare;
       compare.lesserC = 1000000000;
       compare.largerC = 0;
       compare.lesserM = 1000000000;
       compare.largerM = 0;

       //READ SORT TYPES
       SORT sortConst[o];
       for (int i = 0; i < o; i++) {
              scanf("%s", sortConst[i].name);
              sortConst[i].c = 0;
              sortConst[i].m = 0;
       }

       //READ ARRAY ELEMENTS
       int v[n];
       for (int i = 0; i < n; i++) scanf("%d\n", &v[i]);

       //CALCULATE CONSTANTS
       for (int i = 0; i < o; i++) {

              // int _v[n];
              // for (int j = 0; j < n; j++) _v[j] = v[j];

              if (strcmp(sortConst[i].name, "BUBBLESORT") == 0) BubbleSort(&sortConst[i], v, n);
              else if (strcmp(sortConst[i].name, "INSERTIONSORT") == 0) InsertionSort(&sortConst[i], v, n);
              else if (strcmp(sortConst[i].name, "MERGESORT") == 0) MergeSort(&sortConst[i], v, 0, n - 1);
              else if (strcmp(sortConst[i].name, "HEAPSORT") == 0) HeapSort(&sortConst[i], v, n);
              else QuickSort(&sortConst[i], v, 0, n);

              //PRINT RESULT
              UpdateInfo(&sortConst[i], &compare);
       }

       //PRINT RESULT
       printf("Menor C: %s\n", compare.nameLesserC);
       printf("Maior C: %s\n", compare.nameLargerC);
       printf("Menor M: %s\n", compare.nameLesserM);
       printf("Maior M: %s\n", compare.nameLargerM);

       return 0;
}

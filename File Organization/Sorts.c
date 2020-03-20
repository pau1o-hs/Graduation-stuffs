#include <stdio.h>
#include <stdlib.h>
#include "DataLog.h"

int sortBySize(const void* a, const void* b) {
       return Registry_Size((Registry*) a) - Registry_Size((Registry*) b);
}

int sortByID(const void* a, const void* b) {
       return Registry_ID((Registry*) a) - Registry_ID((Registry*) b);
}

void BubbleSort(int* v, int n) {

       for (int i = 0; i < n - 1; i++) {

              for (int j = i + 1; j < n; j++) {

                     if (v[j] < v[i]) {

                            int tmp = v[i];
                            v[i] = v[j];
                            v[j] = tmp;  
                     }
              }
       }

       return;
}

void InsertionSort(int* v, int n) {

       for (int i = 1; i < n; i++) {

              int j, current = v[i];
              for (j = i - 1; j >= 0 && current < v[j]; j--) {
                     v[j + 1] = v[j];
              }
              v[j + 1] = current;    
       }

       return;
}

void HeapSort(int* v, int n) {

       int i = n / 2, parent, child, t;

       while (1) {

              if (i > 0) {

                     i--;
                     t = v[i]; 
              }
              else {

                     n--;
                     if (!n) return;
                     t = v[n];
                     v[n] = v[0];       
              }

              parent = i;
              child = i * 2 + 1;
             
              while (child < n) {

                     if ((child + 1 < n) && (v[child + 1] > v[child])) child++;
                     if (v[child] > t) {

                            v[parent] = v[child];
                            parent = child;
                            child = parent * 2 + 1;     
                     }
                     else break;
              }

              v[parent] = t;
       }

       return;
}

// doublesOP: [0] KEEP DOUBLES; [1] REMOVE DOUBLES; [2] ONLY DOUBLES
void MergeSort(Registry** v, int l, int r, int doublesOP, int (*comparator)(const void *, const void *)) {

       if (l >= r) return;

       int mid = (l + r) / 2;

       MergeSort(v, l, mid, doublesOP, comparator);
       MergeSort(v, mid + 1, r, doublesOP, comparator);

       int i = l, j = mid + 1, k = 0;
       Registry** tmp = malloc((r - l + 1) * sizeof(Registry*));

       while (i <= mid && j <= r) {

              int comp = comparator(v[i], v[j]);
              if (comp <= 0) {

                     tmp[k++] = v[i++];
                     if (doublesOP == 1 && !comp) Registry_SwitchStatus(v[j], '*');
                     if (doublesOP == 2 && !comp) Registry_SwitchStatus(v[i - 1], '-');
              }
              else tmp[k++] = v[j++];
       }

       while (i <= mid) tmp[k++] = v[i++];

       while (j <= r) tmp[k++] = v[j++];

       for (i = l, k = 0; i <= r; i++) v[i] = tmp[k++];
       
       free(tmp);
       return;
}

void QuickSort(int* v, int l, int r) {

       int i = l, j = r, tmp;
       int pivot = v[(l + r) / 2];

       while (i <= j) {

              while (v[i] < pivot && i < r) i++;
              while (v[j] > pivot && j > l) j--;

              if (i <= j) {

                     tmp = v[i];
                     v[i] = v[j];
                     v[j] = tmp;                    
              }
       }

       if (j > l) QuickSort(v, l, j + 1);
       if (i < r) QuickSort(v, i, r);

       return;
}
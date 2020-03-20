#include "DataLog.h"


int compRegPointer(void* a, void* b) {
       return ((Registry*) a)->pointer - ((Registry*) b)->pointer;
}

int compRegSize(void* a, void* b) {
       return ((Registry*) a)->rSize - ((Registry*) b)->rSize;
}

int compRegID(void* a, void* b) {
       return ((Registry*) a)->id - ((Registry*) b)->id;
}

int compRegName(void* a, void* b) {
       return strcmp(((Registry*) a)->name, ((Registry*) b)->name);
}

int compIndexKey(void* a, void* b) {
       return strcmp(((Index*) a)->key, ((Index*) b)->key);
}

int compIndexPointer(void* a, void* b) {
       return ((Index*) a)->pointer - ((Index*) b)->pointer;
}

int compIPointerToInt(void* a, void* b) {
       return ((Index*) a)->pointer - (*(ll*) b);
}

int compInt(void* a, void* b) {
       return *(int*) a -  *(int*) b;
}

int compStr(void* a, void* b) {
       return strcmp((char*) a, (char*) b);
}

int* Binary_Search(Index** index, void* key, int (*comparator) (void *, void *)) {

       int iAmount = Index_GetAmount(index);
       // For (i, 0, iAmount) if ()) printf("Found");

       int l = 0, r = iAmount - 2;
       int mid, comp;
       
       while (l <= r) {

              mid = (l + r) / 2;
              comp = comparator(index[mid], key);

              if (!comp) break;
              
              if (comp < 0) l = mid + 1;
              else r = mid - 1;
       }

       int* results = malloc(iAmount * sizeof(int));
       int rAmount = 0;

       while (mid && !comparator(index[mid - 1], key)) mid--;
       while (!comparator(index[mid], key)) results[rAmount++] = mid++;
       Insertion_Sort(results, rAmount - 1);

       results[rAmount++] = -1;
       results = realloc(results, rAmount * sizeof(int));
       
       return results;
}

void Bubble_Sort(int* v, int n) {

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

void Insertion_Sort(int* v, int n) {

       for (int i = 1; i < n; i++) {

              int j, current = v[i];
              for (j = i - 1; j >= 0 && current < v[j]; j--) {
                     v[j + 1] = v[j];
              }
              v[j + 1] = current;    
       }

       return;
}

void Heap_Sort(int* v, int n) {

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

void Merge_Sort(void** v, int l, int r, int doublesOP, int (*comparator) (void *, void *)) {

       if (l >= r) return;

       int mid = (l + r) / 2;

       Merge_Sort(v, l, mid, doublesOP, comparator);
       Merge_Sort(v, mid + 1, r, doublesOP, comparator);

       int i = l, j = mid + 1, k = 0;
       void** tmp = malloc((r - l + 1) * 8);

       while (i <= mid && j <= r) {

              int comp = comparator(v[i], v[j]);
              if (comp <= 0) {

                     if (doublesOP == 3 && !comp) {
                            if (((Index*) v[i])->pointer < ((Index*) v[j])->pointer) tmp[k++] = v[i++];
                            else tmp[k++] = v[j++];
                     }
                     else tmp[k++] = v[i++];
                     if (doublesOP == 1 && !comp) ((Registry*) v[j])->status = '*';
                     if (doublesOP == 2 && !comp) ((Registry*) v[i - 1])->status = '-';
              }
              else tmp[k++] = v[j++];
       }

       while (i <= mid) tmp[k++] = v[i++];

       while (j <= r) tmp[k++] = v[j++];

       for (i = l, k = 0; i <= r; i++) v[i] = tmp[k++];

       free(tmp);
       return;
}

void Quick_Sort(int* v, int l, int r) {

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

       if (j > l) Quick_Sort(v, l, j + 1);
       if (i < r) Quick_Sort(v, i, r);

       return;
}
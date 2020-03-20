#ifndef SORTS_H
#define SORTS_H

int compRegPointer(void* a, void* b);
int compRegSize(void* a, void* b);
int compRegID(void* a, void* b);
int compRegName(void* a, void* b);

int compIndexKey(void* a, void* b);
int compIndexPointer(void* a, void* b);
int compIPointerToInt(void* a, void* b);

int compInt(void* a, void* b);
int compStr(void* a, void* b);

int* Binary_Search(Index** index, void* key, int (*comparator)(void *, void *));

void Bubble_Sort(int* v, int n);
void Insertion_Sort(int* v, int n);
void Heap_Sort(int* v, int n);

/* doublesOP:
        Ignored registries will be set '*'
        
       [0] Keep double; 
       [1] Remove double; 
       [2] Only double (Needed to receive registries with '*' status); 
       [3] Sort double by byteoffset;
*/
void Merge_Sort(void** v, int l, int r, int doublesOP, int (*comparator)(void *, void *));
void Quick_Sort(int* v, int l, int r);

#endif
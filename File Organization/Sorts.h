#ifndef SORTS_H
#define SORTS_H

int sortBySize(const void* a, const void* b);
int sortByID(const void* a, const void* b);

void BubbleSort(int* v, int n);
void InsertionSort(int* v, int n);
void HeapSort(int* v, int n);
void MergeSort(Registry** v, int l, int r, int doublesOP, int (*comparator)(const void *, const void *));
void QuickSort(int* v, int l, int r);

#endif
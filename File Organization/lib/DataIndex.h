#ifndef H_DATAINDEX
#define H_DATAINDEX

#include "DataLog.h"

typedef struct _index Index;

struct _index {

        char key[120];
        long long pointer;
};

Index* Index_New(char* key, long long pointer);

void DataIndex_AddHeader(FILE* f, int rAmount);
void DataIndex_AddIndex(FILE* f, Index* index);

int Index_GetAmount(Index** index);

Registry** DataIndex_Recover(FILE* indexF, FILE* dataF, char* key, int* iAcess, int* dAcess);
Index** DataIndex_LoadAll(FILE* f);
Index* Index_EndFlag();
void Free_Indexes(Index** index);

#endif
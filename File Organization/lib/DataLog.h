#ifndef H_DATALOG
#define H_DATALOG

#define DEBUG 0

#define PAGE 32000.0
#define For(i, s, n) for(int i = s; i < n; i++)
#define ll long long

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Registry.h"
#include "Header.h"
#include "DataIndex.h"
#include "Sorts.h"
#include "EscreverTela.h"

long int File_GetSize(FILE* f);
int File_isConsistent(FILE* f);

Registry** DataLog_GetList(FILE* f);
Registry** DataLog_Recover(FILE* f, char* field, char* value, int* diskAcess);

Registry* DataLog_RecoverAt(FILE* f, ll rPos, int getTotalSize);

Header* DataLog_GetHeader(FILE* f);

void DataLog_AddHeader(FILE* f, char** fieldDesc);
void DataLog_AddRegistry(FILE* f, Index** index, Registry* r);
void DataLog_Remove(FILE* dataF, char* indexFName, Registry** r1);
void DataLog_Update(FILE* f, Registry** r, char* field, char* value);

void DataLog_PrintForOP2(Registry* r);
void DataLog_PrintForOP3(FILE* f, Registry** r);

#endif

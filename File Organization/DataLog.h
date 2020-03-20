#ifndef H_DATALOG
#define H_DATALOG

#define DEBUG 0

#define PAGE 32000.0
#define For(i, s, n) for(int i = s; i < n; i++)
#define ll long long

typedef struct _header Header;
typedef struct _registry Registry;

int Registry_SwitchStatus(Registry* r, char status);
int Registry_isRemoved(Registry* r);
int Registry_Size(Registry* r);
int Registry_ID(Registry* r);
int File_Size(FILE* f);
int DataLog_isConsistent(FILE* f);

char** Header_GetDescriptions(Header* h);

Registry** DataLog_GetList(FILE* f);
Registry** DataLog_Recover(FILE* f, char* field, char* value, int* diskAcess);

Registry* Registry_New(char** values);
Registry* Registry_EndFlag();
Registry* DataLog_RecoverAt(FILE* f, ll rPos, int getTotalSize);

Header* DataLog_GetHeader(FILE* f);

void Header_SwitchStatus(FILE* f, char status);

void DataLog_AddHeader(FILE* f, char** fieldDesc);
void DataLog_AddRegistry(FILE* f, Registry* r);
void DataLog_Remove(FILE* f, Registry*** r);
void DataLog_Update(FILE* f, Registry** r, char* field, char* value);

void Free_Registries(Registry** r);

void DataLog_PrintForOP2(Registry* r);
void DataLog_PrintForOP3(FILE* f, Registry** r);

#endif

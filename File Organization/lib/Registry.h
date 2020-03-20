#ifndef H_REGISTRY
#define H_REGISTRY

typedef struct _registry Registry;

struct _registry {

        long long pointer;                     // position in file
        char status;                    // */- | yes/no
        int rSize;                      // registrySize

        long long listAddress;
        int id, nSize, cSize;
        double wage;
        char phone[15];
        char name[110];
        char role[110];
};

int Registry_SwitchStatus(Registry* r, char status);
int Registry_isActive(Registry* r);
int Registry_GetPointer(Registry* r);
int Registry_GetSize(Registry* r);
int Registry_GetID(Registry* r);

Registry* Registry_New(char** values);
Registry* Registry_EndFlag();

void Free_Registries(Registry** r);

#endif
#include "DataLog.h"

int Registry_isActive(Registry* r) {
        return r->status == '*' ? 0 : 1;
}

int Registry_GetAmount(Registry** r) {
        
        if (r == NULL) return 0;
        
        int amount;
        for (amount = 0; r[amount]->pointer != -1; amount++);
        return amount;
}
Registry* Registry_New(char** values) {
        
        Registry* r = malloc(sizeof(Registry));

        r->rSize = 34, r->nSize = 0, r->cSize = 0;
        r->phone[0] = '\0', r->listAddress = -1;

        r->id = atoi(values[0]);
        if (strlen(values[1])) r->wage = atof(values[1]); else r->wage = -1;
        if (strlen(values[2])) strcpy(r->phone, values[2]); else For(i, 1, 14) r->phone[i] = '@';
        if (strlen(values[3])) strcpy(r->name, values[3]), r->nSize = strlen(values[3]) + 2, r->rSize += r->nSize + 4;
        if (strlen(values[4])) strcpy(r->role, values[4]), r->cSize = strlen(values[4]) + 2, r->rSize += r->cSize + 4;
        r->phone[14] = '\0';

        return r;
}

Registry* Registry_EndFlag() {
        Registry* r = malloc(sizeof(Registry));
        r->pointer = -1;
        return r;
}

void Free_Registries(Registry** r) {

        int i;
        for (i = 0; r[i]->pointer != -1; i++) free(r[i]);
        free(r[i]), free(r);
}
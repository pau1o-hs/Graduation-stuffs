#include "DataLog.h"

void Header_SwitchStatus(FILE* f, char status) {
        fseek(f, 0, SEEK_SET);
        fwrite(&status, sizeof(char), 1, f);
}

#ifndef H_HEADER
#define H_HEADER

typedef struct _header Header;

struct _header {

        char status;
        char desc[5][40];

        long long topList;
};

void Header_SwitchStatus(FILE* f, char status);

#endif
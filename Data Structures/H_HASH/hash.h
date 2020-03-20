#ifndef HASH_H
#define HASH_H

typedef struct _hash HASH;

HASH* New_Hash(int size);

int Hash_Code(int value);
void Hash_Insert(HASH* h, int value);
void Hash_Remove(HASH* h, int value);
int Hash_Find(HASH* h, int value);

void Hash_Print(HASH* h);
void Destroy_Hash(HASH* h);
#endif

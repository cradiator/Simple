#pragma once

//// utility for storage memory.
struct MM_Storage;

// Create an new storage.
struct MM_Storage* MM_NewStorage(unsigned int init_size = 0);

// Allocate space from storage.
// If the reserved space is full, the storage would grow automatically.
void* MM_AllocateStorage(struct MM_Storage* storage, unsigned int size);

// Release the storage.
void MM_FreeStorage(struct MM_Storage* storage);



//// utility for gc memroy
struct MM_GCStorage;

struct MM_GCStorage* MM_NewGCStorage();

void* MM_AllocateGCMemory(struct MM_GCStorage* storage, int size, const char* filename, int lineno);

void MM_MarkGCMemory(struct MM_GCStorage* storage, void* p);

void MM_UnmarkGCStorage(struct MM_GCStorage* storage);

void MM_SweepGCMemory(struct MM_GCStorage* storage);


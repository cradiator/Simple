#pragma once

//// utility for storage memory.
struct MM_Storage;

// Create an new storage.
struct MM_Storage* MM_NewStorage(unsigned int init_size /*= 0*/);

// Allocate space from storage.
// If the reserved space is full, the storage would grow automatically.
void* MM_AllocateStorage(struct MM_Storage* storage, unsigned int size);

// Release the storage.
void MM_FreeStorage(struct MM_Storage* storage);


//// utility for gc memroy
struct MM_GCStorage;

// New a gc storage
struct MM_GCStorage* MM_NewGCStorage();

// Allocate space form gc storage.
void* MM_AllocateGCMemory(struct MM_GCStorage* storage, int size, const char* filename, int lineno); 
char* MM_CopyStringInGCMemory(struct MM_GCStorage* storage, const char* string, const char* filename, int lineno);

// Only GC-Collectable memory would be released by mark-sweep.
// The memory allocated by MM_AllocateGCMemory or MM_Malloc is not collectable by default.
void MM_MarkGCMemoryCollectable(struct MM_GCStorage* storage, void* p);

// For Mark-Sweep gc
void MM_MarkGCMemory(struct MM_GCStorage* storage, void* p);
void MM_UnmarkGCStorage(struct MM_GCStorage* storage);
void MM_SweepGCMemory(struct MM_GCStorage* storage);

// Dump allocated space in gc storage.
void MM_DumpGCMemory(struct MM_GCStorage* storage);

// Used this macro to allocate space from gc storage.
#define MM_Malloc(storage, size) MM_AllocateGCMemory((storage), (size), __FILE__, __LINE__)

#define MM_CopyString(storage, string) MM_CopyStringInGCMemory((storage), (string), __FILE__, __LINE__)


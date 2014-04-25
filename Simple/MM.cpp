#include "Common.h"
#include "DBG.h"
#include "MM.h"

// the implementation for memory storage.

#define SIG_LEN           (0x10)
#define SIG_START_CONTENT (0xaa)
#define SIG_END_CONTENT   (0xbb)

#define GET_BUFFER_PTR(BL) ((char*)(&((BL)->used_size)) + sizeof(unsigned int) + sizeof(char) * SIG_LEN)

struct MM_Storage 
{
    struct BufferList* start;
    struct BufferList* current;
};

struct BufferList 
{
    struct BufferList *next;
    unsigned int total_size;
    unsigned int used_size;
    // char sig_start[SIG_LEN];
    // char buffer;
    // char sig_end[SIG_LEN];
};

struct BufferList* NewBufferList(unsigned int size)
{
    DCHECK(size > 0);

    unsigned int realsize = size +
        sizeof(struct BufferList) +
        sizeof(char)* SIG_LEN * 2;
    realsize = ALIGN_UP(realsize, PAGE_SIZE);
    
    unsigned int buffersize = realsize - sizeof(struct BufferList) - sizeof(char)* SIG_LEN * 2;

    BufferList* b = (struct BufferList*)malloc(realsize);
    memset(b, 0, realsize);

    b->next       = NULL;
    b->total_size = buffersize;
    b->used_size  = 0;
    char* pos = ((char*)&b->used_size) + sizeof(b->used_size);
    memset(pos, sizeof(char)* SIG_LEN, SIG_START_CONTENT);
    pos = pos + sizeof(char)* SIG_LEN + buffersize;
    memset(pos, sizeof(char)* SIG_LEN, SIG_END_CONTENT);

    return b;
}

struct MM_Storage* MM_NewStorage(unsigned int init_size /*= 0*/)
{
    if (init_size == 0)
    {
        init_size = PAGE_SIZE * 0xFF;
    }

    struct MM_Storage* s = (struct MM_Storage*)malloc(sizeof(struct MM_Storage));
    memset(s, 0, sizeof(struct MM_Storage));
    s->current = s->start = NewBufferList(init_size);

    return s;
}

void* MM_AllocateStorage(struct MM_Storage* storage, unsigned int size)
{
    DCHECK(storage && size);

    if (storage == 0 || size == 0)
        return 0;

    // enough space.
    struct BufferList* bl = storage->current;
    char* p = GET_BUFFER_PTR(bl);
    if (bl->total_size - bl->used_size >= size)
    {
        p = &p[bl->used_size];
        bl->used_size += size;
        return p;
    }

    // not enough, create new buffer list
    unsigned int ssize = bl->total_size * 2 >= size ? bl->total_size * 2 : ALIGN_UP(size, PAGE_SIZE);
    if (ssize >= 0x400000)
    {
        ssize = 0x400000;
    }

    if (ssize < size)
    {
        DPANIC_FORMAT("size(%u) too large.", size);
    }

    struct BufferList* new_buf_list = NewBufferList(ssize);
    DCHECK(new_buf_list != 0);

    bl->next = new_buf_list;
    storage->current = new_buf_list;

    // Allocate again.
    p = GET_BUFFER_PTR(new_buf_list);
    p = p + new_buf_list->used_size;
    new_buf_list->used_size += size;
    DCHECK(new_buf_list->used_size <= new_buf_list->total_size);

    return p;
}

void MM_FreeStorage(struct MM_Storage* storage)
{
    DCHECK(storage);

    if (storage == 0)
    {
        return;
    }

    struct BufferList* current = storage->start;
    while(current != 0)
    {
        struct BufferList* next = current->next;
        free(current);
        current = next;
    }

    free(storage);
}

// end of implementation for memory storage

// the implementation for gc memory
#define GC_SIG_LEN           (0x10)
#define GC_SIG_START_CONTENT (0xcc)
#define GC_SIG_END_CONTENT   (0xdd)

enum 
{
    GC_FLAG_MARKED = 1,
};

struct GCMemory
{
    struct GCMemory* next;
    unsigned int flag;
    const char* filename;
    int     lineno;
    int     size;
    char    sig_start[GC_SIG_LEN];
    // char buffer[size];
    // char sig_end[GC_SIG_LEN];
};

struct MM_GCStorage
{
    struct GCMemory* start;
};

void CheckGCMemoryValid(struct MM_GCMemory* mem)
{
    char* sig1 = mem->sig_start;
    for(int i = 0; i < GC_SIG_LEN; ++i)
    {
        if (sig1[i] != GC_SIG_START_CONTENT)
        {
            DPANIC_FORMAT("mem corrupt, allocate in %s:%d", mem->filename, mem->lineno);
        }
    }

    char* sig2 = mem->sig_start + GC_SIG_LEN + mem->size;
    for(int i = 0; i < GC_SIG_LEN; ++i)
    {
        if (sig2[i] != GC_SIG_END_CONTENT)
        {
            DPANIC_FORMAT("mem corrupt, allocate in %s:%d", mem->filename, mem->lineno);
        }
    }
}

struct MM_GCStorage* MM_NewGCStorage()
{
    struct MM_GCStorage* s = (struct MM_GCStorage*)mallc(sizeof(struct MM_GCStorage));
    s->start = 0;
    return s;
}

void* MM_AllocateGCMemory(struct MM_GCStorage* storage, int size, const char* filename, int lineno)
{
    DCHECK(storage);
    DCHECK(size);
    
    if (size == 0)
    {
        return 0;
    }

    int realsize = sizeof(struct GCMemory) + sizeof(char) * GC_SIG_LEN + size;
    struct MM_GCMemory* mem = (struct MM_GCMemory*)malloc(realsize);
    memset(mem, 0, realsize);

    mem->next     = storage->start;
    mem->filename = filename;
    mem->lineno   = lineno;
    mem->size     = size;
    memcpy(mem->sig_start, GC_SIG_START_CONTENT, GC_SIG_LEN);
    memcpy(mem->sig_start + GC_SIG_LEN + size, GC_SIG_END_CONTENT, GC_SIG_LEN);

    storage->start = mem;
    return mem->sig_start + GC_SIG_LEN;
}

void MM_MarkGCMemory(struct MM_GCStorage*, void* p)
{
    struct MM_GCMemory* mem = (char*)p - sizeof(struct MM_GCMemory);
    CheckGCMemoryValid(mem);

    mem->flag |= GC_FLAG_MARKED;
}

void MM_UnmarkGCStorage(struct MM_GCStorage* storage)
{
    struct MM_GCMemory* mem = storage->start;
    while (mem != 0)
    {
        mem->flag &= ~GC_FLAG_MARKED;
        mem = mem->next;
    }
}

void MM_SweepGCMemory(struct MM_GCStorage* storage)
{
    struct MM_GCMemory*  mem = storage->mem;
    struct MM_GCMemory** prev = &storage->mem;
    while (mem != 0)
    {
        if (!(mem->flag & GC_FLAG_MARKED))
        {
            struct MM_GCMemory* next = mem->next;
            free(mem);
            *prev = next;
            mem   = next;
            continue;
        }

        prev = &mem->next;
        mem  = mem->next;
    }
}


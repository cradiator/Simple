#include "Common.h"
#include "DBG.h"

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

struct MM_Storage* MMNewStorage(unsigned int init_size /*= 0*/)
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

void* MMAllocateStorage(struct MM_Storage* storage, unsigned int size)
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


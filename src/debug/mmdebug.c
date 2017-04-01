#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MMDEBUG_FROM_IMPL
#include "mmdebug.h"

#ifdef MMDEBUG
typedef struct _mmdebug_memblock_s
{
    struct _mmdebug_memblock_s *prev, *next;
    const char *lineinfo;
    size_t size;
    unsigned int unique;
} mmdebug_memblock_t;

static int unfreed_memblock_num, unique_counter;
static mmdebug_memblock_t first_memblock_fake, last_memblock_fake, *first_memblock, *last_memblock;

# define CDMT_OFFSET sizeof(mmdebug_memblock_t)

void mmdebug_init()
{
    unfreed_memblock_num = 0;
    first_memblock = &first_memblock_fake;
    last_memblock = &last_memblock_fake;
    first_memblock->prev = NULL;
    first_memblock->next = last_memblock;
    last_memblock->prev = first_memblock;
    last_memblock->next = NULL;
}

void mmdebug_exit()
{
    if (unfreed_memblock_num)
    {
        printf("DEBUG WARNING: %i unfreed memory blocks:\n", unfreed_memblock_num);
        mmdebug_unfreed();
    }
}

void *mmdebug_malloc(const char *lineinfo, size_t size)
{
# ifdef MMDEBUG_PRINTF_MALLOC
    printf("%s: attempting to malloc %li bytes of memory ... ", lineinfo, size);
# endif

    void *result = mmdebug_pmalloc(size + CDMT_OFFSET);
    assert(result);
    
    mmdebug_memblock_t *memblock = result;
    memblock->lineinfo = lineinfo;
    memblock->size = size;
    memblock->prev = last_memblock->prev;
    memblock->next = last_memblock;
    memblock->prev->next = memblock;
    memblock->next->prev = memblock;
    memblock->unique = unique_counter++;
    
# ifdef MMDEBUG_PRINTF_MALLOC
    printf("ok (%i).\n", memblock->unique);
# endif

    ++unfreed_memblock_num;
    
    return result + CDMT_OFFSET;
}

void *mmdebug_realloc(const char *lineinfo, void *ptr, size_t size)
{
# ifdef MMDEBUG_PRINTF_REALLOC
    printf("%s: attempting to realloc %li bytes of memory ... ", lineinfo, size);
# endif

    mmdebug_memblock_t *memblock = ptr - CDMT_OFFSET;
    void *result = mmdebug_prealloc(memblock, size + CDMT_OFFSET);
    assert(result);
    
    memblock = result;
    memblock->lineinfo = lineinfo;
    memblock->size = size;
    memblock->unique = unique_counter++;
    
# ifdef MMDEBUG_PRINTF_REALLOC
    printf("ok (%i).\n", memblock->unique);
# endif
    
    return result + CDMT_OFFSET;
}

void mmdebug_free(const char *lineinfo, void *ptr)
{
    mmdebug_memblock_t *memblock = ptr - CDMT_OFFSET;
    unsigned int unique = memblock->unique;
    memblock->prev->next = memblock->next;
    memblock->next->prev = memblock->prev;
    
# ifdef MMDEBUG_PRINTF_FREE
    printf("%s: freeing %li bytes of memory allocated at %s ... ", lineinfo, memblock->size, memblock->lineinfo);
# endif

    mmdebug_pfree(memblock);
    
    --unfreed_memblock_num;
    
# ifdef MMDEBUG_PRINTF_FREE
    printf("ok (%i).\n", unique);
# endif
}

void mmdebug_unfreed()
{
    for (mmdebug_memblock_t *memblock = first_memblock->next; memblock->next; memblock = memblock->next)
    {
        printf("%s: %li bytes of memory (%i)\n", memblock->lineinfo, memblock->size, memblock->unique);
    }
}
#endif


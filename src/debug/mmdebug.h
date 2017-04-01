#ifndef DEBUG__MMDEBUG_H
#define DEBUG__MMDEBUG_H

//#define MMDEBUG
//#define MMDEBUG_PRINTF_MALLOC
//#define MMDEBUG_PRINTF_REALLOC
//#define MMDEBUG_PRINTF_FREE

#ifdef MMDEBUG
# ifdef MMDEBUG_FROM_IMPL
typedef void *(*mmdebug_malloc_t)(size_t);
typedef void *(*mmdebug_realloc_t)(void *, size_t);
typedef void (*mmdebug_free_t)(void *);

static mmdebug_malloc_t mmdebug_pmalloc = malloc;
static mmdebug_realloc_t mmdebug_prealloc = realloc;
static mmdebug_free_t mmdebug_pfree = free;
# endif

# define MMDEBUG_INIT mmdebug_init()
# define MMDEBUG_EXIT mmdebug_exit()

void mmdebug_init();
void mmdebug_exit();
void *mmdebug_malloc(const char *lineinfo, size_t size);
void *mmdebug_realloc(const char *lineinfo, void *ptr, size_t size);
void mmdebug_free(const char *lineinfo, void *ptr);
void mmdebug_unfreed();

# define PLS_STRINGIFY(stuff) #stuff
# define STRINGIFY(stuff) PLS_STRINGIFY(stuff)
# define MMDEBUG_HERE __FILE__ ":" STRINGIFY(__LINE__)
# define malloc(size) mmdebug_malloc(MMDEBUG_HERE, (size))
# define realloc(ptr, size) mmdebug_realloc(MMDEBUG_HERE, (ptr), (size))
# define free(ptr) mmdebug_free(MMDEBUG_HERE, (ptr))
#else
# define MMDEBUG_INIT
# define MMDEBUG_EXIT
#endif

#endif // DEBUG__MMDEBUG_H


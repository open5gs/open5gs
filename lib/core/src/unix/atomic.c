#define TRACE_MODULE _atomic

#include "core_atomic.h"
#include "core_mutex.h"
#include "core_debug.h"

#if HAVE_ATOMIC_BUILTINS == 1

status_t atomic_init()
{
    return CORE_OK;
}

status_t atomic_final()
{
    return CORE_OK;
}

c_uint32_t atomic_read32(volatile c_uint32_t *mem)
{
    return *mem;
}

void atomic_set32(volatile c_uint32_t *mem, c_uint32_t val)
{
    *mem = val;
}

c_uint32_t atomic_add32(volatile c_uint32_t *mem, c_uint32_t val)
{
    return __sync_fetch_and_add(mem, val);
}

void atomic_sub32(volatile c_uint32_t *mem, c_uint32_t val)
{
    __sync_fetch_and_sub(mem, val);
}

c_uint32_t atomic_inc32(volatile c_uint32_t *mem)
{
    return __sync_fetch_and_add(mem, 1);
}

int atomic_dec32(volatile c_uint32_t *mem)
{
    return __sync_sub_and_fetch(mem, 1);
}

c_uint32_t atomic_cas32(volatile c_uint32_t *mem, c_uint32_t with,
                                           c_uint32_t cmp)
{
    return __sync_val_compare_and_swap(mem, cmp, with);
}

c_uint32_t atomic_xchg32(volatile c_uint32_t *mem, c_uint32_t val)
{
    __sync_synchronize();

    return __sync_lock_test_and_set(mem, val);
}

void* atomic_casptr(void *volatile *mem, void *with, const void *cmp)
{
    return (void*) __sync_val_compare_and_swap(mem, (void *)cmp, with);
}

void* atomic_xchgptr(void *volatile *mem, void *with)
{
    __sync_synchronize();

    return (void*) __sync_lock_test_and_set(mem, with);
}

#else

#define DECLARE_MUTEX_LOCKED(name, mem) \
        mutex_id name = mutex_hash(mem)
#define MUTEX_UNLOCK(name) \
        d_assert(mutex_unlock(name) == CORE_OK,,);  

#define NUM_ATOMIC_HASH 7
static mutex_id hash_mutex[NUM_ATOMIC_HASH];

/* shift by 2 to get rid of alignment issues */
#define ATOMIC_HASH(x) (unsigned int)(((unsigned long)(x)>>2)%(unsigned int)NUM_ATOMIC_HASH)

status_t atomic_init()
{
    int i;
    status_t rv;

    for (i = 0; i < NUM_ATOMIC_HASH; i++) 
    {
        rv = mutex_create(&hash_mutex[i], MUTEX_DEFAULT);
        if (rv != CORE_OK) 
        {
           return rv;
        }
    }

    return CORE_OK;
}

status_t atomic_final()
{
    status_t rv;
    int i;

    for (i = 0; i < NUM_ATOMIC_HASH; i++) 
    {
        rv = mutex_delete(hash_mutex[i]);
        if (rv != CORE_OK) 
        {
           return rv;
        }
    }

    return CORE_OK;
}

static CORE_INLINE mutex_id mutex_hash(volatile c_uint32_t *mem)
{
    mutex_id id = hash_mutex[ATOMIC_HASH(mem)];

    d_assert(mutex_lock(id) == CORE_OK, return (mutex_id)NULL,);

    return id;
}

c_uint32_t atomic_read32(volatile c_uint32_t *mem)
{
    return *mem;
}

void atomic_set32(volatile c_uint32_t *mem, c_uint32_t val)
{
    DECLARE_MUTEX_LOCKED(mutex, mem);

    *mem = val;

    MUTEX_UNLOCK(mutex);
}

c_uint32_t atomic_add32(volatile c_uint32_t *mem, c_uint32_t val)
{
    c_uint32_t old_value;
    DECLARE_MUTEX_LOCKED(mutex, mem);

    old_value = *mem;
    *mem += val;

    MUTEX_UNLOCK(mutex);

    return old_value;
}

void atomic_sub32(volatile c_uint32_t *mem, c_uint32_t val)
{
    DECLARE_MUTEX_LOCKED(mutex, mem);
    *mem -= val;
    MUTEX_UNLOCK(mutex);
}

c_uint32_t atomic_inc32(volatile c_uint32_t *mem)
{
    return atomic_add32(mem, 1);
}

int atomic_dec32(volatile c_uint32_t *mem)
{
    c_uint32_t new;
    DECLARE_MUTEX_LOCKED(mutex, mem);

    (*mem)--;
    new = *mem;

    MUTEX_UNLOCK(mutex);

    return new;
}

c_uint32_t atomic_cas32(volatile c_uint32_t *mem, c_uint32_t with,
                              c_uint32_t cmp)
{
    c_uint32_t prev;
    DECLARE_MUTEX_LOCKED(mutex, mem);

    prev = *mem;
    if (prev == cmp) {
        *mem = with;
    }

    MUTEX_UNLOCK(mutex);

    return prev;
}

c_uint32_t atomic_xchg32(volatile c_uint32_t *mem, c_uint32_t val)
{
    c_uint32_t prev;
    DECLARE_MUTEX_LOCKED(mutex, mem);

    prev = *mem;
    *mem = val;

    MUTEX_UNLOCK(mutex);

    return prev;
}

void* atomic_casptr(void *volatile *mem, void *with, const void *cmp)
{
    void *prev;
    DECLARE_MUTEX_LOCKED(mutex, *mem);

    prev = *(void **)mem;
    if (prev == cmp) {
        *mem = with;
    }

    MUTEX_UNLOCK(mutex);

    return prev;
}

void* atomic_xchgptr(void *volatile *mem, void *with)
{
    void *prev;
    DECLARE_MUTEX_LOCKED(mutex, *mem);

    prev = *(void **)mem;
    *mem = with;

    MUTEX_UNLOCK(mutex);

    return prev;
}
#endif

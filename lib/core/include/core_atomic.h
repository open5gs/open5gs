#ifndef __CORE_ATOMIC_H__
#define __CORE_ATOMIC_H__

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * this function is required on some platforms to initialize the
 * atomic operation's internal structures
 * @param p pool
 * @return APR_SUCCESS on successful completion
 * @remark Programs do NOT need to call this directly. APR will call this
 *         automatically from apr_initialize.
 * @internal
 */
CORE_DECLARE(status_t) atomic_init();

CORE_DECLARE(status_t) atomic_final();

/*
 * Atomic operations on 32-bit values
 * Note: Each of these functions internally implements a memory barrier
 * on platforms that require it
 */

/**
 * atomically read an c_uint32_t from memory
 * @param mem the pointer
 */
CORE_DECLARE(c_uint32_t) atomic_read32(volatile c_uint32_t *mem);

/**
 * atomically set an c_uint32_t in memory
 * @param mem pointer to the object
 * @param val value that the object will assume
 */
CORE_DECLARE(void) atomic_set32(volatile c_uint32_t *mem, c_uint32_t val);

/**
 * atomically add 'val' to an c_uint32_t
 * @param mem pointer to the object
 * @param val amount to add
 * @return old value pointed to by mem
 */
CORE_DECLARE(c_uint32_t) atomic_add32(volatile c_uint32_t *mem, c_uint32_t val);

/**
 * atomically subtract 'val' from an c_uint32_t
 * @param mem pointer to the object
 * @param val amount to subtract
 */
CORE_DECLARE(void) atomic_sub32(volatile c_uint32_t *mem, c_uint32_t val);

/**
 * atomically increment an c_uint32_t by 1
 * @param mem pointer to the object
 * @return old value pointed to by mem
 */
CORE_DECLARE(c_uint32_t) atomic_inc32(volatile c_uint32_t *mem);

/**
 * atomically decrement an c_uint32_t by 1
 * @param mem pointer to the atomic value
 * @return zero if the value becomes zero on decrement, otherwise non-zero
 */
CORE_DECLARE(int) atomic_dec32(volatile c_uint32_t *mem);

/**
 * compare an c_uint32_t's value with 'cmp'.
 * If they are the same swap the value with 'with'
 * @param mem pointer to the value
 * @param with what to swap it with
 * @param cmp the value to compare it to
 * @return the old value of *mem
 */
CORE_DECLARE(c_uint32_t) atomic_cas32(volatile c_uint32_t *mem, c_uint32_t with,
                                        c_uint32_t cmp);

/**
 * exchange an c_uint32_t's value with 'val'.
 * @param mem pointer to the value
 * @param val what to swap it with
 * @return the old value of *mem
 */
CORE_DECLARE(c_uint32_t) atomic_xchg32(volatile c_uint32_t *mem, 
                                        c_uint32_t val);

/**
 * compare the pointer's value with cmp.
 * If they are the same swap the value with 'with'
 * @param mem pointer to the pointer
 * @param with what to swap it with
 * @param cmp the value to compare it to
 * @return the old value of the pointer
 */
CORE_DECLARE(void*) atomic_casptr(void *volatile *mem, void *with, 
                                        const void *cmp);

/**
 * exchange a pair of pointer values
 * @param mem pointer to the pointer
 * @param with what to swap it with
 * @return the old value of the pointer
 */
CORE_DECLARE(void*) atomic_xchgptr(void *volatile *mem, void *with);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__CORE_ATOMIC_H__ */

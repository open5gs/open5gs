#ifndef __HASH_H__
#define __HASH_H__

#include "core.h"

/**
 * @file hash.h
 * @brief CORE Hash Tables
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup hash Hash Tables
 * @ingroup CORE 
 * @{
 */

/**
 * When passing a key to hash_set or hash_get, this value can be
 * passed to indicate a string-valued key, and have hash compute the
 * length automatically.
 *
 * @remark hash will use strlen(key) for the length. The NUL terminator
 *         is not included in the hash value (why throw a constant in?).
 *         Since the hash table merely references the provided key (rather
 *         than copying it), hash_this() will return the NUL-term'd key.
 */
#define HASH_KEY_STRING     (-1)

/**
 * Abstract type for hash tables.
 */
typedef struct hash_t hash_t;

/**
 * Abstract type for scanning hash tables.
 */
typedef struct hash_index_t hash_index_t;

/**
 * Callback functions for calculating hash values.
 * @param key The key.
 * @param klen The length of the key, or HASH_KEY_STRING to use the string 
 *             length. If HASH_KEY_STRING then returns the actual key length.
 */
typedef unsigned int (*hashfunc_t)(const char *key, int *klen);

/**
 * The default hash function.
 */
CORE_DECLARE_NONSTD(unsigned int) 
    core_hashfunc_default(const char *key, int *klen);

/**
 * Create a hash table.
 * @return The hash table just created
  */
CORE_DECLARE(hash_t *) hash_make();

/**
 * Create a hash table with a custom hash function
 * @param hash_func A custom hash function.
 * @return The hash table just created
  */
CORE_DECLARE(hash_t *) hash_make_custom(hashfunc_t hash_func);

/* Destroy hash memory */
CORE_DECLARE(void) hash_destroy(hash_t *ht);

/**
 * Associate a value with a key in a hash table.
 * @param ht The hash table
 * @param key Pointer to the key
 * @param klen Length of the key. Can be HASH_KEY_STRING to use the string length.
 * @param val Value to associate with the key
 * @remark If the value is NULL the hash entry is deleted. The key is stored as is,
 *         and so must have a lifetime at least as long as the hash table's pool.
 */
CORE_DECLARE(void) hash_set(hash_t *ht, 
        const void *key, int klen, const void *val);

/**
 * Look up the value associated with a key in a hash table.
 * @param ht The hash table
 * @param key Pointer to the key
 * @param klen Length of the key. Can be HASH_KEY_STRING to use the string length.
 * @return Returns NULL if the key is not present.
 */
CORE_DECLARE(void *) hash_get(hash_t *ht, const void *key, int klen);

/**
 * Look up the value associated with a key in a hash table, or if none exists
 * associate a value.
 * @param ht The hash table
 * @param key Pointer to the key
 * @param klen Length of the key. Can be HASH_KEY_STRING to use the string
 *        length.
 * @param val Value to associate with the key (if none exists).
 * @return Returns the existing value if any, the given value otherwise.
 * @remark If the given value is NULL and a hash entry exists, nothing is done.
 */
CORE_DECLARE(void *) hash_get_or_set(hash_t *ht, 
        const void *key, int klen, const void *val);

/**
 * Start iterating over the entries in a hash table.
 * @param ht The hash table
 * @return The iteration state
 * @remark  There is no restriction on adding or deleting hash entries during
 * an iteration (although the results may be unpredictable unless all you do
 * is delete the current entry) and multiple iterations can be in
 * progress at the same time.
 *
 * @par Example:
 *
 * @code
 * int sum_values(hash_t *ht)
 * {
 *     hash_index_t *hi;
 *     void *val;
 *     int sum = 0;
 *     for (hi = hash_first(ht); hi; hi = hash_next(hi)) {
 *         hash_this(hi, NULL, NULL, &val);
 *         sum += *(int *)val;
 *     }
 *     return sum;
 * }
 * @endcode
 */
CORE_DECLARE(hash_index_t *) hash_first(hash_t *ht);

/**
 * Continue iterating over the entries in a hash table.
 * @param hi The iteration state
 * @return a pointer to the updated iteration state.  NULL if there are no more  
 *         entries.
 */
CORE_DECLARE(hash_index_t *) hash_next(hash_index_t *hi);

/**
 * Get the current entry's details from the iteration state.
 * @param hi The iteration state
 * @param key Return pointer for the pointer to the key.
 * @param klen Return pointer for the key length.
 * @param val Return pointer for the associated value.
 * @remark The return pointers should point to a variable that will be set to the
 *         corresponding data, or they may be NULL if the data isn't interesting.
 */
CORE_DECLARE(void) hash_this(hash_index_t *hi, 
        const void **key, int *klen, void **val);

/**
 * Get the current entry's key from the iteration state.
 * @param hi The iteration state
 * @return The pointer to the key
 */
CORE_DECLARE(const void*) hash_this_key(hash_index_t *hi);

/**
 * Get the current entry's key length from the iteration state.
 * @param hi The iteration state
 * @return The key length
 */
CORE_DECLARE(int) hash_this_key_len(hash_index_t *hi);

/**
 * Get the current entry's value from the iteration state.
 * @param hi The iteration state
 * @return The pointer to the value
 */
CORE_DECLARE(void*) hash_this_val(hash_index_t *hi);

/**
 * Get the number of key/value pairs in the hash table.
 * @param ht The hash table
 * @return The number of key/value pairs in the hash table.
 */
CORE_DECLARE(unsigned int) hash_count(hash_t *ht);

/**
 * Clear any key/value pairs in the hash table.
 * @param ht The hash table
 */
CORE_DECLARE(void) hash_clear(hash_t *ht);

/**
 * Declaration prototype for the iterator callback function of hash_do().
 *
 * @param rec The data passed as the first argument to hash_[v]do()
 * @param key The key from this iteration of the hash table
 * @param klen The key length from this iteration of the hash table
 * @param value The value from this iteration of the hash table
 * @remark Iteration continues while this callback function returns non-zero.
 * To export the callback function for hash_do() it must be declared 
 * in the _NONSTD convention.
 */
typedef int (hash_do_callback_fn_t)(
        void *rec, const void *key, int klen, const void *value);

/** 
 * Iterate over a hash table running the provided function once for every
 * element in the hash table. The @param comp function will be invoked for
 * every element in the hash table.
 *
 * @param comp The function to run
 * @param rec The data to pass as the first argument to the function
 * @param ht The hash table to iterate over
 * @return FALSE if one of the comp() iterations returned zero; TRUE if all
 *            iterations returned non-zero
 * @see hash_do_callback_fn_t
 */
CORE_DECLARE(int) hash_do(hash_do_callback_fn_t *comp,
                             void *rec, const hash_t *ht);

/** @} */

#ifdef __cplusplus
}
#endif

#endif	/* __HASH_H__ */

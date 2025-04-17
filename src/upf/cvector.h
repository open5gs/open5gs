/*
 * Copyright (c) 2015 Evan Teran
 *
 * License: The MIT License (MIT)
 */

#ifndef CVECTOR_H_
#define CVECTOR_H_

/* cvector heap implemented using C library malloc() */

/* in case C library malloc() needs extra protection,
 * allow these defines to be overridden.
 */
#ifndef cvector_clib_free
#include <stdlib.h> /* for free */
#define cvector_clib_free free
#endif
#ifndef cvector_clib_malloc
#include <stdlib.h> /* for malloc */
#define cvector_clib_malloc malloc
#endif
#ifndef cvector_clib_calloc
#include <stdlib.h> /* for calloc */
#define cvector_clib_calloc calloc
#endif
#ifndef cvector_clib_realloc
#include <stdlib.h> /* for realloc */
#define cvector_clib_realloc realloc
#endif
#ifndef cvector_clib_assert
#include <assert.h> /* for assert */
#define cvector_clib_assert assert
#endif
#ifndef cvector_clib_memcpy
#include <string.h> /* for memcpy */
#define cvector_clib_memcpy memcpy
#endif
#ifndef cvector_clib_memmove
#include <string.h> /* for memmove */
#define cvector_clib_memmove memmove
#endif
/* NOTE: Similar to C's qsort and bsearch, you will receive a T*
 * for a vector of Ts. This means that you cannot use `free` directly
 * as a destructor. Instead if you have for example a cvector_vector_type(int *)
 * you will need to supply a function which casts `elem_ptr` to an `int**`
 * and then does a free on what that pointer points to:
 *
 * ex:
 *
 * void free_int(void *p) { free(*(int **)p); }
 */
typedef void (*cvector_elem_destructor_t)(void *elem_ptr);

typedef struct cvector_metadata_t {
    size_t size;
    size_t capacity;
    cvector_elem_destructor_t elem_destructor;
} cvector_metadata_t;

/**
 * @brief cvector_vector_type - The vector type used in this library
 */
#define cvector_vector_type(type) type *

/**
 * @brief cvector - Syntactic sugar to retrieve a vector type
 *
 * @param type The type of vector to act on.
 */
#define cvector(type) cvector_vector_type(type)

/*
 * @brief cvector_iterator - The iterator type used for cvector
 */
#define cvector_iterator(type) cvector_vector_type(type)

/**
 * @brief cvector_vec_to_base - For internal use, converts a vector pointer to a metadata pointer
 * @param vec - the vector
 * @return the metadata pointer of the vector
 * @internal
 */
#define cvector_vec_to_base(vec) \
    (&((cvector_metadata_t *)(vec))[-1])

/**
 * @brief cvector_base_to_vec - For internal use, converts a metadata pointer to a vector pointer
 * @param ptr - pointer to the metadata
 * @return the vector
 * @internal
 */
#define cvector_base_to_vec(ptr) \
    ((void *)&((cvector_metadata_t *)(ptr))[1])

/**
 * @brief cvector_capacity - gets the current capacity of the vector
 * @param vec - the vector
 * @return the capacity as a size_t
 */
#define cvector_capacity(vec) \
    ((vec) ? cvector_vec_to_base(vec)->capacity : (size_t)0)

/**
 * @brief cvector_size - gets the current size of the vector
 * @param vec - the vector
 * @return the size as a size_t
 */
#define cvector_size(vec) \
    ((vec) ? cvector_vec_to_base(vec)->size : (size_t)0)

/**
 * @brief cvector_elem_destructor - get the element destructor function used
 * to clean up elements
 * @param vec - the vector
 * @return the function pointer as cvector_elem_destructor_t
 */
#define cvector_elem_destructor(vec) \
    ((vec) ? cvector_vec_to_base(vec)->elem_destructor : NULL)

/**
 * @brief cvector_empty - returns non-zero if the vector is empty
 * @param vec - the vector
 * @return non-zero if empty, zero if non-empty
 */
#define cvector_empty(vec) \
    (cvector_size(vec) == 0)

/**
 * @brief cvector_reserve - Requests that the vector capacity be at least enough
 * to contain n elements. If n is greater than the current vector capacity, the
 * function causes the container to reallocate its storage increasing its
 * capacity to n (or greater).
 * @param vec - the vector
 * @param n - Minimum capacity for the vector.
 * @return void
 */
#define cvector_reserve(vec, n)                  \
    do {                                         \
        size_t cv_cap__ = cvector_capacity(vec); \
        if (cv_cap__ < (n)) {                    \
            cvector_grow((vec), (n));            \
        }                                        \
    } while (0)

/*
 * @brief cvector_init - Initialize a vector.  The vector must be NULL for this to do anything.
 * @param vec - the vector
 * @param capacity - vector capacity to reserve
 * @param elem_destructor_fn - element destructor function
 * @return void
 */
#define cvector_init(vec, capacity, elem_destructor_fn)               \
    do {                                                              \
        if (!(vec)) {                                                 \
            cvector_reserve((vec), capacity);                         \
            cvector_set_elem_destructor((vec), (elem_destructor_fn)); \
        }                                                             \
    } while (0)

/**
 * @brief cvector_erase - removes the element at index i from the vector
 * @param vec - the vector
 * @param i - index of element to remove
 * @return void
 */
#define cvector_erase(vec, i)                                                               \
    do {                                                                                    \
        if (vec) {                                                                          \
            const size_t cv_sz__ = cvector_size(vec);                                       \
            if ((i) < cv_sz__) {                                                            \
                cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
                if (elem_destructor__) {                                                    \
                    elem_destructor__(&(vec)[i]);                                           \
                }                                                                           \
                cvector_set_size((vec), cv_sz__ - 1);                                       \
                cvector_clib_memmove(                                                       \
                    (vec) + (i),                                                            \
                    (vec) + (i) + 1,                                                        \
                    sizeof(*(vec)) * (cv_sz__ - 1 - (i)));                                  \
            }                                                                               \
        }                                                                                   \
    } while (0)

/**
 * @brief cvector_clear - erase all of the elements in the vector
 * @param vec - the vector
 * @return void
 */
#define cvector_clear(vec)                                                              \
    do {                                                                                \
        if (vec) {                                                                      \
            cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
            if (elem_destructor__) {                                                    \
                size_t i__;                                                             \
                for (i__ = 0; i__ < cvector_size(vec); ++i__) {                         \
                    elem_destructor__(&(vec)[i__]);                                     \
                }                                                                       \
            }                                                                           \
            cvector_set_size(vec, 0);                                                   \
        }                                                                               \
    } while (0)

/**
 * @brief cvector_free - frees all memory associated with the vector
 * @param vec - the vector
 * @return void
 */
#define cvector_free(vec)                                                               \
    do {                                                                                \
        if (vec) {                                                                      \
            void *p1__                                  = cvector_vec_to_base(vec);     \
            cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
            if (elem_destructor__) {                                                    \
                size_t i__;                                                             \
                for (i__ = 0; i__ < cvector_size(vec); ++i__) {                         \
                    elem_destructor__(&(vec)[i__]);                                     \
                }                                                                       \
            }                                                                           \
            cvector_clib_free(p1__);                                                    \
        }                                                                               \
    } while (0)

/**
 * @brief cvector_begin - returns an iterator to first element of the vector
 * @param vec - the vector
 * @return a pointer to the first element (or NULL)
 */
#define cvector_begin(vec) \
    (vec)

/**
 * @brief cvector_end - returns an iterator to one past the last element of the vector
 * @param vec - the vector
 * @return a pointer to one past the last element (or NULL)
 */
#define cvector_end(vec) \
    ((vec) ? &((vec)[cvector_size(vec)]) : NULL)

/* user request to use logarithmic growth algorithm */
#ifdef CVECTOR_LOGARITHMIC_GROWTH

/**
 * @brief cvector_compute_next_grow - returns an the computed size in next vector grow
 * size is increased by multiplication of 2
 * @param size - current size
 * @return size after next vector grow
 */
#define cvector_compute_next_grow(size) \
    ((size) ? ((size) << 1) : 1)

#else

/**
 * @brief cvector_compute_next_grow - returns an the computed size in next vector grow
 * size is increased by 1
 * @param size - current size
 * @return size after next vector grow
 */
#define cvector_compute_next_grow(size) \
    ((size) + 1)

#endif /* CVECTOR_LOGARITHMIC_GROWTH */

/**
 * @brief cvector_push_back - adds an element to the end of the vector
 * @param vec - the vector
 * @param value - the value to add
 * @return void
 */
#define cvector_push_back(vec, value)                                 \
    do {                                                              \
        size_t cv_cap__ = cvector_capacity(vec);                      \
        if (cv_cap__ <= cvector_size(vec)) {                          \
            cvector_grow((vec), cvector_compute_next_grow(cv_cap__)); \
        }                                                             \
        (vec)[cvector_size(vec)] = (value);                           \
        cvector_set_size((vec), cvector_size(vec) + 1);               \
    } while (0)

/**
 * @brief cvector_insert - insert element at position pos to the vector
 * @param vec - the vector
 * @param pos - position in the vector where the new elements are inserted.
 * @param val - value to be copied (or moved) to the inserted elements.
 * @return void
 */
#define cvector_insert(vec, pos, val)                                 \
    do {                                                              \
        size_t cv_cap__ = cvector_capacity(vec);                      \
        if (cv_cap__ <= cvector_size(vec)) {                          \
            cvector_grow((vec), cvector_compute_next_grow(cv_cap__)); \
        }                                                             \
        if ((pos) < cvector_size(vec)) {                              \
            cvector_clib_memmove(                                     \
                (vec) + (pos) + 1,                                    \
                (vec) + (pos),                                        \
                sizeof(*(vec)) * ((cvector_size(vec)) - (pos)));      \
        }                                                             \
        (vec)[(pos)] = (val);                                         \
        cvector_set_size((vec), cvector_size(vec) + 1);               \
    } while (0)

/**
 * @brief cvector_pop_back - removes the last element from the vector
 * @param vec - the vector
 * @return void
 */
#define cvector_pop_back(vec)                                                       \
    do {                                                                            \
        cvector_elem_destructor_t elem_destructor__ = cvector_elem_destructor(vec); \
        if (elem_destructor__) {                                                    \
            elem_destructor__(&(vec)[cvector_size(vec) - 1]);                       \
        }                                                                           \
        cvector_set_size((vec), cvector_size(vec) - 1);                             \
    } while (0)

/**
 * @brief cvector_copy - copy a vector
 * @param from - the original vector
 * @param to - destination to which the function copy to
 * @return void
 */
#define cvector_copy(from, to)                                                       \
    do {                                                                             \
        if ((from)) {                                                                \
            cvector_grow(to, cvector_size(from));                                    \
            cvector_set_size(to, cvector_size(from));                                \
            cvector_clib_memcpy((to), (from), cvector_size(from) * sizeof(*(from))); \
        }                                                                            \
    } while (0)

/**
 * @brief cvector_swap - exchanges the content of the vector by the content of another vector of the same type
 * @param vec - the original vector
 * @param other - the other vector to swap content with
 * @param type - the type of both vectors
 * @return void
 */
#define cvector_swap(vec, other, type)                       \
    do {                                                     \
        if (vec && other) {                                  \
            cvector_vector_type(type) cv_swap__ = vec;       \
            vec                                 = other;     \
            other                               = cv_swap__; \
        }                                                    \
    } while (0)

/**
 * @brief cvector_set_capacity - For internal use, sets the capacity variable of the vector
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 * @internal
 */
#define cvector_set_capacity(vec, size)                  \
    do {                                                 \
        if (vec) {                                       \
            cvector_vec_to_base(vec)->capacity = (size); \
        }                                                \
    } while (0)

/**
 * @brief cvector_set_size - For internal use, sets the size variable of the vector
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 * @internal
 */
#define cvector_set_size(vec, _size)                  \
    do {                                              \
        if (vec) {                                    \
            cvector_vec_to_base(vec)->size = (_size); \
        }                                             \
    } while (0)

/**
 * @brief cvector_set_elem_destructor - set the element destructor function
 * used to clean up removed elements. The vector must NOT be NULL for this to do anything.
 * @param vec - the vector
 * @param elem_destructor_fn - function pointer of type cvector_elem_destructor_t used to destroy elements
 * @return void
 */
#define cvector_set_elem_destructor(vec, elem_destructor_fn)                  \
    do {                                                                      \
        if (vec) {                                                            \
            cvector_vec_to_base(vec)->elem_destructor = (elem_destructor_fn); \
        }                                                                     \
    } while (0)

/**
 * @brief cvector_grow - For internal use, ensures that the vector is at least <count> elements big
 * @param vec - the vector
 * @param count - the new capacity to set
 * @return void
 * @internal
 */
#define cvector_grow(vec, count)                                                      \
    do {                                                                              \
        const size_t cv_sz__ = (count) * sizeof(*(vec)) + sizeof(cvector_metadata_t); \
        if (vec) {                                                                    \
            void *cv_p1__ = cvector_vec_to_base(vec);                                 \
            void *cv_p2__ = cvector_clib_realloc(cv_p1__, cv_sz__);                   \
            cvector_clib_assert(cv_p2__);                                             \
            (vec) = cvector_base_to_vec(cv_p2__);                                     \
        } else {                                                                      \
            void *cv_p__ = cvector_clib_malloc(cv_sz__);                              \
            cvector_clib_assert(cv_p__);                                              \
            (vec) = cvector_base_to_vec(cv_p__);                                      \
            cvector_set_size((vec), 0);                                               \
            cvector_set_elem_destructor((vec), NULL);                                 \
        }                                                                             \
        cvector_set_capacity((vec), (count));                                         \
    } while (0)

/**
 * @brief cvector_shrink_to_fit - requests the container to reduce its capacity to fit its size
 * @param vec - the vector
 * @return void
 */
#define cvector_shrink_to_fit(vec)                     \
    do {                                               \
        if (vec) {                                     \
            const size_t cv_sz___ = cvector_size(vec); \
            cvector_grow(vec, cv_sz___);               \
        }                                              \
    } while (0)

/**
 * @brief cvector_at - returns a reference to the element at position n in the vector.
 * @param vec - the vector
 * @param n - position of an element in the vector.
 * @return the element at the specified position in the vector.
 */
#define cvector_at(vec, n) \
    ((vec) ? (((int)(n) < 0 || (size_t)(n) >= cvector_size(vec)) ? NULL : &(vec)[n]) : NULL)

/**
 * @brief cvector_front - returns a reference to the first element in the vector. Unlike member cvector_begin, which returns an iterator to this same element, this function returns a direct reference.
 * @return a reference to the first element in the vector container.
 */
#define cvector_front(vec) \
    ((vec) ? ((cvector_size(vec) > 0) ? cvector_at(vec, 0) : NULL) : NULL)

/**
 * @brief cvector_back - returns a reference to the last element in the vector.Unlike member cvector_end, which returns an iterator just past this element, this function returns a direct reference.
 * @return a reference to the last element in the vector.
 */
#define cvector_back(vec) \
    ((vec) ? ((cvector_size(vec) > 0) ? cvector_at(vec, cvector_size(vec) - 1) : NULL) : NULL)

/**
 * @brief cvector_resize - resizes the container to contain count elements.
 * @param vec - the vector
 * @param count - new size of the vector
 * @param value - the value to initialize new elements with
 * @return void
 */
#define cvector_resize(vec, count, value)                          \
    do {                                                           \
        if (vec) {                                                 \
            size_t cv_sz_count__ = (size_t)(count);                \
            size_t cv_sz__       = cvector_vec_to_base(vec)->size; \
            if (cv_sz_count__ > cv_sz__) {                         \
                cvector_reserve((vec), cv_sz_count__);             \
                cvector_set_size((vec), cv_sz_count__);            \
                do {                                               \
                    (vec)[cv_sz__++] = (value);                    \
                } while (cv_sz__ < cv_sz_count__);                 \
            } else {                                               \
                while (cv_sz_count__ < cv_sz__--) {                \
                    cvector_pop_back(vec);                         \
                }                                                  \
            }                                                      \
        }                                                          \
    } while (0)

#endif /* CVECTOR_H_ */

#ifndef __CORE_RINGBUF_H__
#define __CORE_RINGBUF_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct _rbuf_header_t {
    int head, tail, size;
    char *pool;
};

#define rbuf_declare(__name, __size) \
    struct { \
        struct _rbuf_header_t h; \
        char int_pool[(__size) + 1]; \
    } __name

#define rbuf_declare_ext(__name) \
    struct { \
        struct _rbuf_header_t h; \
    } __name

#define rbuf_init(__pname, __size) \
    ((__pname)->h.head = (__pname)->h.tail = 0, \
     (__pname)->h.size = __size, \
     (__pname)->h.pool = (__pname)->int_pool)

#define rbuf_init_ext(__pname, __size, __ext_pool) \
    ((__pname)->h.head = (__pname)->h.tail = 0, \
     (__pname)->h.size = __size, \
     (__pname)->h.pool = (char *)(__ext_pool))

#define rbuf_size(__pname) ((__pname)->h.size)

#define rbuf_is_empty(__pname) ((__pname)->h.head == (__pname)->h.tail)
#define rbuf_is_full(__pname) (rbuf_free_bytes(__pname) == 0)

CORE_DECLARE(int) rbuf_bytes(void *__pname);

CORE_DECLARE(int) rbuf_free_bytes(void *__pname);

CORE_DECLARE(int) rbuf_skip_write_pos(void *__pname, int __len);

CORE_DECLARE(int) rbuf_write(void *__pname, const char *__buf, int __buf_len);

CORE_DECLARE(int) rbuf_skip_read_pos(void *__pname, int __len);

CORE_DECLARE(int) rbuf_read(void *__pname, char *__buf, int __buf_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __CORE_RINGBUF_H__ */

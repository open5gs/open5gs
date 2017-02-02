#define TRACE_MODULE _ringbuf
#include "core_debug.h"
#include "core_ringbuf.h"

#define _rbuf_bytes(__h, __t, __s) \
    (((__h) < (__t)) ? (__h) + (__s) - (__t) + 1 : (__h) - (__t))

#define _rbuf_free_bytes(__h, __t, __s) \
    (((__h) < (__t)) ? (__t) - (__h) - 1 : (__t) + (__s) - (__h))

int rbuf_bytes(void *__pname)
{
    int h, t;

    struct _rbuf_header_t *ptr_h = (struct _rbuf_header_t*)__pname;

    /* Store for thread safety */
    h = ptr_h->head; t = ptr_h->tail;

    return _rbuf_bytes(h, t, ptr_h->size);
}

int rbuf_free_bytes(void *__pname)
{
    int h, t;

    struct _rbuf_header_t *ptr_h = (struct _rbuf_header_t*)__pname;

    /* Store for thread safety */
    h = ptr_h->head; t = ptr_h->tail;

    return _rbuf_free_bytes(h, t, ptr_h->size);
}

#define rbuf_is_empty(__pname) ((__pname)->h.head == (__pname)->h.tail)

#define rbuf_is_full(__pname) (rbuf_free_bytes(__pname) == 0)

int rbuf_skip_write_pos(void *__pname, int __len)
{
    /* Write operation must handle only head pointer */

    int n, h, t;

    struct _rbuf_header_t *ptr_h = (struct _rbuf_header_t*)__pname;

    if (ptr_h == NULL)
        return -1;

    t = ptr_h->tail; /* Store for thread safety with read thread */
    h = ptr_h->head;

    /* Check available buffer size */
    n = _rbuf_free_bytes(h, t, ptr_h->size);
    /* If no space, return */
    if (n == 0)
        return -1;
    /* Determin the number of bytes to be skipped */
    n = n < __len ? n : __len;

    /* In this function, only ptr_h->head should be updated */
    ptr_h->head = (h + n) % (ptr_h->size + 1);

    return n;
}

int rbuf_write(void *__pname, const char *__buf, int __buf_len)
{
    /* Write operation must handle only head pointer */

    int n, h, t;

    struct _rbuf_header_t *ptr_h = (struct _rbuf_header_t*)__pname;

    if (ptr_h == NULL)
        return -1;

    t = ptr_h->tail; /* Store for thread safety with read thread */
    h = ptr_h->head;

    /* Check available buffer size */
    n = _rbuf_free_bytes(h, t, ptr_h->size);
    /* If no space, return */
    if (n == 0)
        return -1;
    /* Determin the number of bytes to be written */
    n = n < __buf_len ? n : __buf_len;

    if ((t > h) || (n < ptr_h->size - h + 1))
    {
        memcpy(ptr_h->pool + h, __buf, n);
    }
    else
    {
        memcpy(ptr_h->pool + h, __buf, ptr_h->size - h + 1);
        memcpy(ptr_h->pool, __buf + (ptr_h->size - h + 1),
                n - (ptr_h->size - h + 1));
    }

    /* In this function, only ptr_h->head should be updated */
    ptr_h->head = (h + n) % (ptr_h->size + 1);

    return n;
}

int rbuf_skip_read_pos(void *__pname, int __len)
{
    /* Read operation must handle only tail pointer */
    int n, h, t;

    struct _rbuf_header_t *ptr_h = (struct _rbuf_header_t*)__pname;

    if (ptr_h == NULL)
        return -1;

    h = ptr_h->head;  /* Store for thread safety with write thread */
    t = ptr_h->tail;

    /* Check filled buffer size */
    n = _rbuf_bytes(h, t, ptr_h->size);
    /* If empty, return */
    if (n == 0)
        return -1;
    /* Determine the number of bytes to be skipped */
    n = n < __len ? n : __len;

    /* In this function, only ptr_h->tail should be updated */
    ptr_h->tail = (t + n) % (ptr_h->size + 1);

    return n;
}

int rbuf_read(void *__pname, char *__buf, int __buf_len)
{
    /* Read operation must handle only tail pointer */

    int n, h, t;

    struct _rbuf_header_t *ptr_h = (struct _rbuf_header_t*)__pname;

    if (ptr_h == NULL)
        return -1;

    h = ptr_h->head;  /* Store for thread safety with write thread */
    t = ptr_h->tail;

    /* Check filled buffer size */
    n = _rbuf_bytes(h, t, ptr_h->size);
    /* If empty, return */
    if (n == 0)
        return -1;
    /* Determine the number of bytes to be read */
    n = n < __buf_len ? n : __buf_len;

    if (t < h || (n < ptr_h->size - t + 1))
    {
        memcpy(__buf, ptr_h->pool + t, n);
    }
    else
    {
        memcpy(__buf, ptr_h->pool + t, ptr_h->size - t + 1);
        memcpy(__buf + (ptr_h->size - t + 1), ptr_h->pool,
                n - (ptr_h->size - t + 1));
    }

    /* In this function, only ptr_h->tail should be updated */
    ptr_h->tail = (t + n) % (ptr_h->size + 1);

    return n;
}

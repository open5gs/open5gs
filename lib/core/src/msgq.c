#define TRACE_MODULE _msgq
#include "core_debug.h"
#include "core_pool.h"
#include "core_ringbuf.h"
#include "core_cond.h"
#include "core_mutex.h"
#include "core_msgq.h"
#include "core_pkbuf.h"
#include "core_list.h"

typedef struct _msq_desc_t {
    mutex_id mut_c, mut_r, mut_w;
    cond_id cond;
    int opt;
    int qdepth, msgsize, used;
    rbuf_declare_ext(rbuf);
    unsigned char *pool;
} msg_desc_t;

#define SIZE_OF_MSGQ_POOL 5 /* MME 1, SGW 2, PGW 2 */

pool_declare(msgqpool, msg_desc_t, SIZE_OF_MSGQ_POOL);

status_t msgq_init(void)
{
    pool_init(&msgqpool, SIZE_OF_MSGQ_POOL);
    return CORE_OK;
}

status_t msgq_final(void)
{
    pool_final(&msgqpool);
    return CORE_OK;
}

msgq_id msgq_create(int qdepth, int msgsize, int opt)
{
    msg_desc_t *md;
    int s;
    status_t rv;

    if (qdepth == 0 || msgsize == 0)
        return 0;

    pool_alloc_node(&msgqpool, &md);
    d_assert(md != NULL, return 0, "empty msgq pool");

    memset((void*)md, 0, sizeof(msg_desc_t));

    rv = mutex_create(&md->mut_c, MUTEX_DEFAULT);
    d_assert(rv == CORE_OK,
            goto error_final, "mutex creation failed");
    rv = mutex_create(&md->mut_r, MUTEX_DEFAULT);
    d_assert(rv == CORE_OK,
            goto error_final, "mutex creation failed");

    rv = mutex_create(&md->mut_w, MUTEX_DEFAULT);
    d_assert(rv == CORE_OK,
            goto error_final, "mutex creation failed");

    rv = cond_create(&md->cond);
    d_assert(rv == CORE_OK,
            goto error_final, "mutex creation failed");

    s = qdepth * msgsize;
    md->pool = core_malloc(s);
    d_assert(md->pool != NULL,
            goto error_final, "can't allocate msg q buffer %d bytes", s);

    rbuf_init_ext(&(md->rbuf), s, md->pool);

    md->opt = opt;
    md->qdepth = qdepth;
    md->msgsize = msgsize;
    md->used = 0;

    return (msgq_id)md;

error_final:
    if (md->pool) CORE_FREE(md->pool);
    if (md->mut_c) mutex_delete(md->mut_c);
    if (md->mut_r) mutex_delete(md->mut_r);
    if (md->mut_w) mutex_delete(md->mut_w);
    if (md->cond) cond_delete(md->cond);

    pool_free_node(&msgqpool, md);

    return 0;
}

status_t msgq_delete(msgq_id id)
{
    msg_desc_t *md = (msg_desc_t*)id;

    d_assert(md != NULL, return CORE_ERROR, "param 'id' is null");

    if (md->pool) CORE_FREE(md->pool);
    if (md->mut_c) mutex_delete(md->mut_c);
    if (md->mut_r) mutex_delete(md->mut_r);
    if (md->mut_w) mutex_delete(md->mut_w);
    if (md->cond) cond_delete(md->cond);

    pool_free_node(&msgqpool, md);

    return CORE_OK;
}

status_t msgq_send(msgq_id id, const char *msg, int msglen)
{
    msg_desc_t *md = (msg_desc_t*)id;
    int n;

    d_assert(md != NULL, return CORE_ERROR, "param 'id' is null");
    d_assert(msg != NULL, return CORE_ERROR, "param 'msg' is null");
    d_assert(msglen <= md->msgsize, return CORE_ERROR,
            "'msglen' is bigger than the msg size of queue");

    d_assert(md->pool != NULL, return CORE_ERROR, "msgq has no ring buffer");

    mutex_lock(md->mut_w);

    n = rbuf_free_bytes(&md->rbuf);
    if (n == 0)
    {
        mutex_unlock(md->mut_w);
        return CORE_EAGAIN;
    }

    n = rbuf_write(&md->rbuf, msg, msglen);
    d_trace(2, "ring write. head:%d tail:%d size:%d len:%d\n",
            md->rbuf.h.head, md->rbuf.h.tail, md->rbuf.h.size, msglen);
    d_assert(n == msglen,
            mutex_unlock(md->mut_w); return CORE_ERROR,
            "msgq integrity broken n:%d len:%d", n, msglen);

    d_trace(1, "msg (%d bytes) pushed.\n", msglen);

    mutex_unlock(md->mut_w);
    mutex_lock(md->mut_c);
    cond_signal(md->cond);
    mutex_unlock(md->mut_c);

    return CORE_OK;
}

status_t msgq_recv(msgq_id id, char *msg, int msglen)
{
    msg_desc_t *md = (msg_desc_t*)id;
    int n;

    d_assert(md != NULL, return CORE_ERROR, "param 'id' is null");
    d_assert(msg != NULL, return CORE_ERROR, "param 'msg' is null");
    d_assert(msglen >= md->msgsize, return CORE_ERROR,
            "'msglen' is smaller than msgsize");

    d_assert(md->pool != NULL, return CORE_ERROR, "msgq has no ring buffer");

    mutex_lock(md->mut_r);

    n = rbuf_bytes(&md->rbuf);

    if (!(md->opt & MSGQ_O_NONBLOCK) && (n < md->msgsize))
    {
        mutex_lock(md->mut_c);
        while(rbuf_is_empty(&md->rbuf) && 
                cond_wait(md->cond, md->mut_c) == CORE_OK);
        mutex_unlock(md->mut_c);

        n = rbuf_bytes(&md->rbuf);

        d_assert(n >= md->msgsize,
                mutex_unlock(md->mut_r); return CORE_ERROR,
                "msgq integrity broken");
    }
    else if (n < md->msgsize)
    {
        mutex_unlock(md->mut_r);
        return CORE_EAGAIN;
    }

    n = rbuf_read(&md->rbuf, msg, msglen);
    d_trace(2, "ring read. head:%d tail:%d size:%d len:%d\n",
            md->rbuf.h.head, md->rbuf.h.tail, md->rbuf.h.size, msglen);

    d_assert(n == msglen,
            mutex_unlock(md->mut_r); return CORE_ERROR,
            "msgq integrity broken n:%d len:%d", n, msglen);

    d_trace(1, "msg (%d bytes) pop.\n", msglen);

    mutex_unlock(md->mut_r);

    return CORE_OK;
}

status_t msgq_timedrecv(msgq_id id, char *msg, int msglen, c_time_t timeout)
{
    msg_desc_t *md = (msg_desc_t*)id;
    int n;
    status_t rv;

    d_assert(md != NULL, return CORE_ERROR, "param 'id' is null");
    d_assert(msg != NULL, return CORE_ERROR, "param 'msg' is null");
    d_assert(msglen >= md->msgsize, return CORE_ERROR,
            "'msglen' is smaller than msgsize");

    d_assert(md->pool != NULL, return CORE_ERROR, "msgq has no ring buffer");

    mutex_lock(md->mut_r);

    n = rbuf_bytes(&md->rbuf);

    if (!(md->opt & MSGQ_O_NONBLOCK) && (n < md->msgsize))
    {
        mutex_lock(md->mut_c);
        while(rbuf_is_empty(&md->rbuf) && 
                (rv = cond_timedwait(md->cond, md->mut_c, timeout)) == CORE_OK);
        mutex_unlock(md->mut_c);

        if (rv == CORE_TIMEUP)
        {
            mutex_unlock(md->mut_r);
            return CORE_TIMEUP;
        }

        n = rbuf_bytes(&md->rbuf);
        d_assert(n >= md->msgsize,
                mutex_unlock(md->mut_r); return CORE_ERROR,
                "msgq integrity broken");
    }
    else if (n < md->msgsize)
    {
        mutex_unlock(md->mut_r);
        return CORE_EAGAIN;
    }

    n = rbuf_read(&md->rbuf, msg, msglen);
    d_trace(2, "ring read. head:%d tail:%d size:%d len:%d\n",
            md->rbuf.h.head, md->rbuf.h.tail, md->rbuf.h.size, msglen);

    d_assert(n == msglen,
            mutex_unlock(md->mut_r); return CORE_ERROR,
            "msgq integrity broken n:%d len:%d", n, msglen);

    d_trace(1, "msg (%d bytes) pop.\n", msglen);

    mutex_unlock(md->mut_r);

    return CORE_OK;
}

#define TRACE_MODULE _sgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "3gpp_common.h"
#include "gtp_path.h"

#include "sgw_context.h"

static sgw_context_t self;

index_declare(sgw_sess_pool, sgw_sess_t, MAX_NUM_OF_UE);

static int context_initialized = 0;

status_t sgw_context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    memset(&self, 0, sizeof(sgw_context_t));

    index_init(&sgw_sess_pool, MAX_NUM_OF_UE);
    list_init(&self.sess_list);

    self.s11_addr = inet_addr("127.0.0.1");
    self.s11_port = GTPV2_C_UDP_PORT + 1;
    self.s11_node.addr = inet_addr("127.0.0.1");
    self.s11_node.port = GTPV2_C_UDP_PORT;
    list_init(&self.s11_node.local_list);
    list_init(&self.s11_node.remote_list);

    self.s5c_addr = inet_addr("127.0.0.1");
    self.s5c_port = GTPV2_C_UDP_PORT + 2;
    self.s5c_node.addr = inet_addr("127.0.0.1");
    self.s5c_node.port = GTPV2_C_UDP_PORT + 3;
    list_init(&self.s5c_node.local_list);
    list_init(&self.s5c_node.remote_list);

    self.s5u_addr = inet_addr("127.0.0.1");
    self.s5u_port = GTPV1_U_UDP_PORT;
    self.s5u_node.addr = inet_addr("127.0.0.1");
    self.s5u_node.port = GTPV1_U_UDP_PORT + 1;
    
    context_initialized = 1;

    return CORE_OK;
}

status_t sgw_context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    gtp_xact_delete_all(&self.s11_node);
    gtp_xact_delete_all(&self.s5c_node);

    sgw_sess_remove_all();

    d_print("%d not freed in sgw_sess_pool[%d] in SGW-Context\n",
            index_size(&sgw_sess_pool) - pool_avail(&sgw_sess_pool),
            index_size(&sgw_sess_pool));
    index_final(&sgw_sess_pool);

    context_initialized = 0;
    
    return CORE_OK;
}

sgw_context_t* sgw_self()
{
    return &self;
}

sgw_sess_t *sgw_sess_add()
{
    sgw_sess_t *sess = NULL;

    index_alloc(&sgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->sgw_s11_teid = sess->index;  /* derived from an index */
    sess->sgw_s11_addr = sgw_self()->s11_addr;

    sess->sgw_s5c_teid = sess->index;  /* derived from an index */
    sess->sgw_s5c_addr = sgw_self()->s5c_addr;

    list_append(&self.sess_list, sess);

    return sess;
}

status_t sgw_sess_remove(sgw_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");

    list_remove(&self.sess_list, sess);
    index_free(&sgw_sess_pool, sess);

    return CORE_OK;
}

status_t sgw_sess_remove_all()
{
    sgw_sess_t *enb = NULL, *next_enb = NULL;
    
    enb = sgw_sess_first();
    while (enb)
    {
        next_enb = sgw_sess_next(enb);

        sgw_sess_remove(enb);

        enb = next_enb;
    }

    return CORE_OK;
}

sgw_sess_t* sgw_sess_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&sgw_sess_pool, index);
}

sgw_sess_t* sgw_sess_find_by_teid(c_uint32_t teid)
{
    return sgw_sess_find(teid);
}

sgw_sess_t* sgw_sess_first()
{
    return list_first(&self.sess_list);
}

sgw_sess_t* sgw_sess_next(sgw_sess_t *enb)
{
    return list_next(enb);
}

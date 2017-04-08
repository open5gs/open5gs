#define TRACE_MODULE _sgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "3gpp_common.h"
#include "gtp_path.h"

#include "sgw_context.h"

static sgw_context_t self;

pool_declare(sgw_sess_pool, sgw_sess_t, MAX_NUM_OF_UE);

static int context_initialized = 0;

status_t sgw_context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    memset(&self, 0, sizeof(sgw_context_t));

    pool_init(&sgw_sess_pool, MAX_NUM_OF_UE);
    self.sess_hash = hash_make();

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

    d_assert(self.sess_hash, , "Null param");
    hash_destroy(self.sess_hash);

    d_print("%d not freed in sgw_sess_pool[%d] in SGW-Context\n",
            pool_size(&sgw_sess_pool) - pool_avail(&sgw_sess_pool),
            pool_size(&sgw_sess_pool));
    pool_final(&sgw_sess_pool);

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

    d_assert(self.sess_hash, return NULL, "Null param");

    pool_alloc_node(&sgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    memset(sess, 0, sizeof(sgw_sess_t));

    sess->teid = NEXT_ID(self.sess_tunnel_id, 0xffffffff);
    hash_set(self.sess_hash, &sess->teid, sizeof(sess->teid), sess);

    return sess;
}

status_t sgw_sess_remove(sgw_sess_t *sess)
{
    d_assert(self.sess_hash, return CORE_ERROR, "Null param");
    d_assert(sess, return CORE_ERROR, "Null param");
    hash_set(self.sess_hash, &sess->teid, sizeof(sess->teid), NULL);

    pool_free_node(&sgw_sess_pool, sess);

    return CORE_OK;
}

status_t sgw_sess_remove_all()
{
    hash_index_t *hi = NULL;
    sgw_sess_t *sess = NULL;

    for (hi = sgw_sess_first(); hi; hi = sgw_sess_next(hi))
    {
        sess = sgw_sess_this(hi);
        sgw_sess_remove(sess);
    }

    return CORE_OK;
}

sgw_sess_t *sgw_sess_find(c_uint32_t teid)
{
    d_assert(self.sess_hash, return NULL, "Null param");
    return hash_get(self.sess_hash, &teid, sizeof(teid));
}

hash_index_t *sgw_sess_first()
{
    d_assert(self.sess_hash, return NULL, "Null param");
    return hash_first(self.sess_hash);
}

hash_index_t *sgw_sess_next(hash_index_t *hi)
{
    return hash_next(hi);
}

sgw_sess_t *sgw_sess_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

unsigned int sgw_sess_count()
{
    d_assert(self.sess_hash, return 0, "Null param");
    return hash_count(self.sess_hash);
}

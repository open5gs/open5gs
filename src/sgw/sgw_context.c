#define TRACE_MODULE _sgw_ctx

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "3gpp_common.h"
#include "gtp_path.h"

#include "sgw_context.h"

static sgw_ctx_t self;

pool_declare(sgw_gtpc_pool, sgw_gtpc_t, MAX_NUM_OF_UE);
static hash_t *gtpc_hash;
#define SGW_NEXT_TUNNEL_ID(__id) \
    ((__id) = ((__id) == 0xffffffff ? 1 : ((__id) + 1)))
static c_uint32_t g_gtpc_tunnel_id = 0;

static int ctx_initialized = 0;

status_t sgw_ctx_init()
{
    d_assert(ctx_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    memset(&self, 0, sizeof(sgw_ctx_t));

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
    
    pool_init(&sgw_gtpc_pool, MAX_NUM_OF_UE);

    gtpc_hash = hash_make();

    ctx_initialized = 1;

    return CORE_OK;
}

status_t sgw_ctx_final()
{
    d_assert(ctx_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    hash_destroy(gtpc_hash);

    d_print("%d not freed in sgw_gtpc_pool[%d] of SGW-Context\n",
            pool_size(&sgw_gtpc_pool) - pool_avail(&sgw_gtpc_pool),
            pool_size(&sgw_gtpc_pool));
    pool_final(&sgw_gtpc_pool);

    ctx_initialized = 0;
    
    return CORE_OK;
}

sgw_ctx_t* sgw_self()
{
    return &self;
}

sgw_gtpc_t *sgw_ctx_gtpc_add()
{
    sgw_gtpc_t *gtpc = NULL;

    pool_alloc_node(&sgw_gtpc_pool, &gtpc);
    d_assert(gtpc, return NULL, "Null param");

    memset(gtpc, 0, sizeof(sgw_gtpc_t));

    gtpc->teid = SGW_NEXT_TUNNEL_ID(g_gtpc_tunnel_id);
    hash_set(gtpc_hash, &gtpc->teid, sizeof(gtpc->teid), gtpc);

    return gtpc;
}

status_t sgw_ctx_gtpc_remove(sgw_gtpc_t *gtpc)
{
    d_assert(gtpc, return CORE_ERROR, "Null param");
    hash_set(gtpc_hash, &gtpc->teid, sizeof(gtpc->teid), NULL);

    pool_free_node(&sgw_gtpc_pool, gtpc);

    return CORE_OK;
}

status_t sgw_ctx_gtpc_remove_all()
{
    hash_index_t *hi = NULL;
    sgw_gtpc_t *gtpc = NULL;

    for (hi = sgw_ctx_gtpc_first(); hi; hi = sgw_ctx_gtpc_next(hi))
    {
        gtpc = sgw_ctx_gtpc_this(hi);
        sgw_ctx_gtpc_remove(gtpc);
    }

    return CORE_OK;
}

sgw_gtpc_t *sgw_ctx_gtpc_find(c_uint32_t teid)
{
    return hash_get(gtpc_hash, &teid, sizeof(teid));
}

hash_index_t *sgw_ctx_gtpc_first()
{
    return hash_first(gtpc_hash);
}

hash_index_t *sgw_ctx_gtpc_next(hash_index_t *hi)
{
    return hash_next(hi);
}

sgw_gtpc_t *sgw_ctx_gtpc_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

unsigned int sgw_ctx_gtpc_count()
{
    return hash_count(gtpc_hash);
}

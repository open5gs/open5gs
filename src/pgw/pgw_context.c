#define TRACE_MODULE _pgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "gtp_path.h"

#include "pgw_context.h"

static pgw_context_t self;

pool_declare(pgw_gtpc_pool, pgw_gtpc_t, MAX_NUM_OF_UE);

static int context_initiaized = 0;

status_t pgw_context_init()
{
    d_assert(context_initiaized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    memset(&self, 0, sizeof(pgw_context_t));

    pool_init(&pgw_gtpc_pool, MAX_NUM_OF_UE);
    self.gtpc_hash = hash_make();

    self.s5c_addr = inet_addr("127.0.0.1");
    self.s5c_port = GTPV2_C_UDP_PORT + 3;
    self.s5c_node.addr = inet_addr("127.0.0.1");
    self.s5c_node.port = GTPV2_C_UDP_PORT + 2;
    list_init(&self.s5c_node.local_list);
    list_init(&self.s5c_node.remote_list);

    self.s5u_addr = inet_addr("127.0.0.1");
    self.s5u_port = GTPV1_U_UDP_PORT + 1;
    self.s5u_node.addr = inet_addr("127.0.0.1");
    self.s5u_node.port = GTPV1_U_UDP_PORT;

    context_initiaized = 1;

    return CORE_OK;
}

status_t pgw_context_final()
{
    d_assert(context_initiaized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    d_assert(self.gtpc_hash, , "Null param");
    hash_destroy(self.gtpc_hash);

    d_print("%d not freed in pgw_gtpc_pool[%d] in PGW-Context\n",
            pool_size(&pgw_gtpc_pool) - pool_avail(&pgw_gtpc_pool),
            pool_size(&pgw_gtpc_pool));
    pool_final(&pgw_gtpc_pool);

    context_initiaized = 0;
    
    return CORE_OK;
}

pgw_context_t* pgw_self()
{
    return &self;
}

pgw_gtpc_t *pgw_gtpc_add()
{
    pgw_gtpc_t *gtpc = NULL;

    d_assert(self.gtpc_hash, return NULL, "Null param");

    pool_alloc_node(&pgw_gtpc_pool, &gtpc);
    d_assert(gtpc, return NULL, "Null param");

    memset(gtpc, 0, sizeof(pgw_gtpc_t));

    gtpc->teid = NEXT_ID(self.gtpc_tunnel_id, 0xffffffff);
    hash_set(self.gtpc_hash, &gtpc->teid, sizeof(gtpc->teid), gtpc);

    return gtpc;
}

status_t pgw_gtpc_remove(pgw_gtpc_t *gtpc)
{
    d_assert(self.gtpc_hash, return CORE_ERROR, "Null param");
    d_assert(gtpc, return CORE_ERROR, "Null param");
    hash_set(self.gtpc_hash, &gtpc->teid, sizeof(gtpc->teid), NULL);

    pool_free_node(&pgw_gtpc_pool, gtpc);

    return CORE_OK;
}

status_t pgw_gtpc_remove_all()
{
    hash_index_t *hi = NULL;
    pgw_gtpc_t *gtpc = NULL;

    for (hi = pgw_gtpc_first(); hi; hi = pgw_gtpc_next(hi))
    {
        gtpc = pgw_gtpc_this(hi);
        pgw_gtpc_remove(gtpc);
    }

    return CORE_OK;
}

pgw_gtpc_t *pgw_gtpc_find(c_uint32_t teid)
{
    d_assert(self.gtpc_hash, return NULL, "Null param");
    return hash_get(self.gtpc_hash, &teid, sizeof(teid));
}

hash_index_t *pgw_gtpc_first()
{
    d_assert(self.gtpc_hash, return NULL, "Null param");
    return hash_first(self.gtpc_hash);
}

hash_index_t *pgw_gtpc_next(hash_index_t *hi)
{
    return hash_next(hi);
}

pgw_gtpc_t *pgw_gtpc_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

unsigned int pgw_gtpc_count()
{
    d_assert(self.gtpc_hash, return 0, "Null param");
    return hash_count(self.gtpc_hash);
}

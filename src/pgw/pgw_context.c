#define TRACE_MODULE _pgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "gtp_path.h"

#include "pgw_context.h"

static pgw_context_t self;

pool_declare(pgw_sess_pool, pgw_sess_t, MAX_NUM_OF_UE);

static int context_initiaized = 0;

status_t pgw_context_init()
{
    d_assert(context_initiaized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    memset(&self, 0, sizeof(pgw_context_t));

    pool_init(&pgw_sess_pool, MAX_NUM_OF_UE);
    self.sess_hash = hash_make();

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

    d_assert(self.sess_hash, , "Null param");
    hash_destroy(self.sess_hash);

    d_print("%d not freed in pgw_sess_pool[%d] in PGW-Context\n",
            pool_size(&pgw_sess_pool) - pool_avail(&pgw_sess_pool),
            pool_size(&pgw_sess_pool));
    pool_final(&pgw_sess_pool);

    context_initiaized = 0;
    
    return CORE_OK;
}

pgw_context_t* pgw_self()
{
    return &self;
}

pgw_sess_t *pgw_sess_add()
{
    pgw_sess_t *sess = NULL;

    d_assert(self.sess_hash, return NULL, "Null param");

    pool_alloc_node(&pgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    memset(sess, 0, sizeof(pgw_sess_t));

    sess->teid = NEXT_ID(self.sess_tunnel_id, 0xffffffff);
    hash_set(self.sess_hash, &sess->teid, sizeof(sess->teid), sess);

    return sess;
}

status_t pgw_sess_remove(pgw_sess_t *sess)
{
    d_assert(self.sess_hash, return CORE_ERROR, "Null param");
    d_assert(sess, return CORE_ERROR, "Null param");
    hash_set(self.sess_hash, &sess->teid, sizeof(sess->teid), NULL);

    pool_free_node(&pgw_sess_pool, sess);

    return CORE_OK;
}

status_t pgw_sess_remove_all()
{
    hash_index_t *hi = NULL;
    pgw_sess_t *sess = NULL;

    for (hi = pgw_sess_first(); hi; hi = pgw_sess_next(hi))
    {
        sess = pgw_sess_this(hi);
        pgw_sess_remove(sess);
    }

    return CORE_OK;
}

pgw_sess_t *pgw_sess_find(c_uint32_t teid)
{
    d_assert(self.sess_hash, return NULL, "Null param");
    return hash_get(self.sess_hash, &teid, sizeof(teid));
}

hash_index_t *pgw_sess_first()
{
    d_assert(self.sess_hash, return NULL, "Null param");
    return hash_first(self.sess_hash);
}

hash_index_t *pgw_sess_next(hash_index_t *hi)
{
    return hash_next(hi);
}

pgw_sess_t *pgw_sess_this(hash_index_t *hi)
{
    d_assert(hi, return NULL, "Null param");
    return hash_this_val(hi);
}

unsigned int pgw_sess_count()
{
    d_assert(self.sess_hash, return 0, "Null param");
    return hash_count(self.sess_hash);
}

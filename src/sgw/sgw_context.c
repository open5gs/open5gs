#define TRACE_MODULE _sgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "3gpp_common.h"
#include "gtp_path.h"

#include "sgw_context.h"

static sgw_context_t self;

index_declare(sgw_sess_pool, sgw_sess_t, MAX_NUM_OF_UE);
index_declare(sgw_bearer_pool, sgw_bearer_t, MAX_NUM_OF_UE_BEARER);

static int context_initialized = 0;

/* FIXME : Global IP information and port.
 * This should be read from configuration file or arguments
 */
static char g_mme_ip_addr[20] = "10.1.35.215";
static unsigned int g_mme_gtp_c_port = GTPV2_C_UDP_PORT;

static char g_sgw_s11_ip_addr[20] = "10.1.35.216";
static unsigned int g_sgw_s11_port = GTPV2_C_UDP_PORT;

static char g_sgw_s5c_ip_addr[20] = "10.1.35.217";
static unsigned int g_sgw_s5c_port = GTPV2_C_UDP_PORT;

static char g_pgw_ip_addr[20] = "10.1.35.219";
static unsigned int g_pgw_s5c_port = GTPV2_C_UDP_PORT;

static char g_sgw_s1u_ip_addr[20] = "10.1.35.216";/* same as g_sgw_s11_ip_addr */
static unsigned int g_sgw_s1u_port = GTPV1_U_UDP_PORT;

static char g_sgw_s5u_ip_addr[20] = "10.1.35.217"; /* same as g_sgw_s5c_ip_addr */
static unsigned int g_sgw_s5u_port = GTPV1_U_UDP_PORT;

status_t sgw_context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    memset(&self, 0, sizeof(sgw_context_t));

    index_init(&sgw_sess_pool, MAX_NUM_OF_UE);
    index_init(&sgw_bearer_pool, MAX_NUM_OF_UE_BEARER);
    list_init(&self.sess_list);

    self.sgw_addr = inet_addr(g_sgw_s11_ip_addr);

    /* S11 address and port of SGW */
    self.s11_addr = inet_addr(g_sgw_s11_ip_addr);
    self.s11_port = g_sgw_s11_port;

    /* FIXME : It shoud be removed : Peer MME ? */
    self.s11_node.addr = inet_addr(g_mme_ip_addr); 
    self.s11_node.port = g_mme_gtp_c_port;
    list_init(&self.s11_node.local_list);
    list_init(&self.s11_node.remote_list);

    /* S5C address and port of SGW */
    self.s5c_addr = inet_addr(g_sgw_s5c_ip_addr);
    self.s5c_port = g_sgw_s5c_port;

    /* FIXME : It shoud be removed : Peer PGW ? */
    self.s5c_node.addr = inet_addr(g_pgw_ip_addr);
    self.s5c_node.port = g_pgw_s5c_port;
    list_init(&self.s5c_node.local_list);
    list_init(&self.s5c_node.remote_list);

    /* S1U address and port of SGW */
    self.s1u_addr = inet_addr(g_sgw_s1u_ip_addr);
    self.s1u_port = g_sgw_s1u_port;

    self.s5u_addr = inet_addr(g_sgw_s5u_ip_addr);
    self.s5u_port = g_sgw_s5u_port;

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
    index_final(&sgw_bearer_pool);
    index_final(&sgw_sess_pool);

    context_initialized = 0;
    
    return CORE_OK;
}

sgw_context_t* sgw_self()
{
    return &self;
}

sgw_bearer_t *sgw_sess_add(c_uint8_t id)
{
    sgw_sess_t *sess = NULL;
    sgw_bearer_t *bearer = NULL;

    index_alloc(&sgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->sgw_s11_teid = sess->index;  /* derived from an index */
    sess->sgw_s11_addr = sgw_self()->s11_addr;

    sess->sgw_s5c_teid = sess->index;  /* derived from an index */
    sess->sgw_s5c_addr = sgw_self()->s5c_addr;

    list_init(&sess->bearer_list);
    list_append(&self.sess_list, sess);

    bearer = sgw_bearer_add(sess, id);
    d_assert(bearer, sgw_sess_remove(sess); return NULL, 
            "Can't add default bearer context");

    return bearer;
}

status_t sgw_sess_remove(sgw_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");

    sgw_bearer_remove_all(sess);

    list_remove(&self.sess_list, sess);
    index_free(&sgw_sess_pool, sess);

    return CORE_OK;
}

status_t sgw_sess_remove_all()
{
    sgw_sess_t *sess = NULL, *next_sess = NULL;
    
    sess = sgw_sess_first();
    while (sess)
    {
        next_sess = sgw_sess_next(sess);

        sgw_sess_remove(sess);

        sess = next_sess;
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

sgw_sess_t* sgw_sess_next(sgw_sess_t *sess)
{
    return list_next(sess);
}

sgw_bearer_t* sgw_bearer_add(sgw_sess_t *sess, c_uint8_t id)
{
    sgw_bearer_t *bearer = NULL;

    d_assert(sess, return NULL, "Null param");

    index_alloc(&sgw_bearer_pool, &bearer);
    d_assert(bearer, return NULL, "Bearer context allocation failed");

    bearer->id = id;
    bearer->sgw_s1u_teid = bearer->index;
    bearer->sgw_s1u_addr = sgw_self()->s1u_addr;
    bearer->sgw_s5u_teid = bearer->index;
    bearer->sgw_s5u_addr = sgw_self()->s5u_addr;
    
    bearer->sess = sess;
    list_append(&sess->bearer_list, bearer);

    return bearer;
}

status_t sgw_bearer_remove(sgw_bearer_t *bearer)
{
    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(bearer->sess, return CORE_ERROR, "Null param");

    list_remove(&bearer->sess->bearer_list, bearer);
    index_free(&sgw_bearer_pool, bearer);

    return CORE_OK;
}

status_t sgw_bearer_remove_all(sgw_sess_t *sess)
{
    sgw_bearer_t *bearer = NULL, *next_bearer = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    
    bearer = list_first(&sess->bearer_list);
    while (bearer)
    {
        next_bearer = list_next(bearer);

        sgw_bearer_remove(bearer);

        bearer = next_bearer;
    }

    return CORE_OK;
}

sgw_bearer_t* sgw_bearer_find(index_t index)
{
    d_assert(index && index < MAX_NUM_OF_UE_BEARER, return NULL, 
            "Invalid Index(%d)",index);

    return index_find(&sgw_bearer_pool, index);
}

sgw_bearer_t* sgw_bearer_find_by_sgw_s5u_teid(c_uint32_t sgw_s5u_teid)
{
    return sgw_bearer_find(sgw_s5u_teid);
}

sgw_bearer_t* sgw_bearer_find_by_sgw_s1u_teid(c_uint32_t sgw_s1u_teid)
{
    return sgw_bearer_find(sgw_s1u_teid);
}

sgw_bearer_t* sgw_bearer_find_by_id(sgw_sess_t *sess, c_uint8_t id)
{
    sgw_bearer_t *bearer = NULL;
    
    d_assert(sess, return NULL, "Null param");

    bearer = list_first(&sess->bearer_list);
    while (bearer)
    {
        if (bearer->id == id)
            break;

        bearer = list_next(bearer);
    }

    return bearer;
}

sgw_bearer_t* sgw_default_bearer_in_sess(sgw_sess_t *sess)
{
    return sgw_bearer_first(sess);
}

sgw_bearer_t* sgw_bearer_first(sgw_sess_t *sess)
{
    d_assert(sess, return NULL, "Null param");
    return list_first(&sess->bearer_list);
}

sgw_bearer_t* sgw_bearer_next(sgw_bearer_t *bearer)
{
    return list_next(bearer);
}


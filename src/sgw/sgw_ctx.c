#define TRACE_MODULE _sgw_ctx

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "gtp_path.h"

#include "sgw_ctx.h"

#define UE_PER_ENB                  128
#define S11_PER_UE                  8
#define S5C_PER_UE                  S11_PER_UE

#define SIZE_OF_ENB_POOL            128
#define SIZE_OF_UE_POOL             (SIZE_OF_ENB_POOL * UE_PER_ENB)
#define SIZE_OF_S11_POOL            (SIZE_OF_UE_POOL * S11_PER_UE)
#define SIZE_OF_S5C_POOL            (SIZE_OF_UE_POOL * S5C_PER_UE)

static sgw_ctx_t self;

static int ctx_initialized = 0;

static list_t s11_list;
static list_t s5c_list;

pool_declare(s11_pool, s11_ctx_t, SIZE_OF_S11_POOL);
pool_declare(s5c_pool, s5c_ctx_t, SIZE_OF_S5C_POOL);

status_t sgw_ctx_init()
{
    d_assert(ctx_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    pool_init(&s11_pool, SIZE_OF_S11_POOL);
    pool_init(&s5c_pool, SIZE_OF_S5C_POOL);

    list_init(&s11_list);
    list_init(&s5c_list);

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

    ctx_initialized = 1;

    return CORE_OK;
}

status_t sgw_ctx_final()
{
    d_assert(ctx_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    sgw_ctx_s11_remove_all();
    sgw_ctx_s5c_remove_all();

    pool_final(&s11_pool);
    pool_final(&s5c_pool);

    ctx_initialized = 0;
    
    return CORE_OK;
}

sgw_ctx_t* sgw_self()
{
    return &self;
}

s11_ctx_t* sgw_ctx_s11_add()
{
    s11_ctx_t *s11 = NULL;

    pool_alloc_node(&s11_pool, &s11);
    d_assert(s11, return NULL, "Null param");

    memset(s11, 0, sizeof(s11_ctx_t));

    list_append(&s11_list, s11);
    
    return s11;
}

status_t sgw_ctx_s11_remove(s11_ctx_t *s11)
{
    d_assert(s11, return CORE_ERROR, "Null param");

    list_remove(&s11_list, s11);
    pool_free_node(&s11_pool, s11);

    return CORE_OK;
}

status_t sgw_ctx_s11_remove_all()
{
    s11_ctx_t *s11 = NULL, *next_s11 = NULL;
    
    s11 = sgw_ctx_s11_first();
    while (s11)
    {
        next_s11 = sgw_ctx_s11_next(s11);

        sgw_ctx_s11_remove(s11);

        s11 = next_s11;
    }

    return CORE_OK;
}

s11_ctx_t* sgw_ctx_s11_find_by_teid(c_uint32_t teid)
{
    s11_ctx_t *s11 = NULL;
    
    s11 = sgw_ctx_s11_first();
    while (s11)
    {
        if (teid == s11->teid)
            break;

        s11 = sgw_ctx_s11_next(s11);
    }

    return s11;
}

s11_ctx_t* sgw_ctx_s11_first()
{
    return list_first(&s11_list);
}

s11_ctx_t* sgw_ctx_s11_next(s11_ctx_t *s11)
{
    return list_next(s11);
}

s5c_ctx_t* sgw_ctx_s5c_add()
{
    s5c_ctx_t *s5c = NULL;

    pool_alloc_node(&s5c_pool, &s5c);
    d_assert(s5c, return NULL, "Null param");

    memset(s5c, 0, sizeof(s5c_ctx_t));

    list_append(&s5c_list, s5c);
    
    return s5c;
}

status_t sgw_ctx_s5c_remove(s5c_ctx_t *s5c)
{
    d_assert(s5c, return CORE_ERROR, "Null param");

    list_remove(&s5c_list, s5c);
    pool_free_node(&s5c_pool, s5c);

    return CORE_OK;
}

status_t sgw_ctx_s5c_remove_all()
{
    s5c_ctx_t *s5c = NULL, *next_s5c = NULL;
    
    s5c = sgw_ctx_s5c_first();
    while (s5c)
    {
        next_s5c = sgw_ctx_s5c_next(s5c);

        sgw_ctx_s5c_remove(s5c);

        s5c = next_s5c;
    }

    return CORE_OK;
}

s5c_ctx_t* sgw_ctx_s5c_find_by_teid(c_uint32_t teid)
{
    s5c_ctx_t *s5c = NULL;
    
    s5c = sgw_ctx_s5c_first();
    while (s5c)
    {
        if (teid == s5c->teid)
            break;

        s5c = sgw_ctx_s5c_next(s5c);
    }

    return s5c;
}

s5c_ctx_t* sgw_ctx_s5c_first()
{
    return list_first(&s5c_list);
}

s5c_ctx_t* sgw_ctx_s5c_next(s5c_ctx_t *s5c)
{
    return list_next(s5c);
}

#define TRACE_MODULE _pgw_ctx

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "context.h"
#include "gtp_path.h"

static pgw_ctx_t self;

static int ctx_initialized = 0;

status_t pgw_ctx_init()
{
    d_assert(ctx_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    memset(&self, 0, sizeof(pgw_ctx_t));

    self.s5c_addr = inet_addr("127.0.0.1");
    self.s5c_port = GTPV2_C_UDP_PORT + 3;
    self.s5c_node.addr = inet_addr("127.0.0.1");
    self.s5c_node.port = GTPV2_C_UDP_PORT + 2;
    list_init(&self.s5c_node.initial_list);
    list_init(&self.s5c_node.triggered_list);
    self.s5c_node.local_list = &self.s5c_node.initial_list;
    self.s5c_node.remote_list = &self.s5c_node.triggered_list;

    self.s5u_addr = inet_addr("127.0.0.1");
    self.s5u_port = GTPV1_U_UDP_PORT + 1;
    self.s5u_node.addr = inet_addr("127.0.0.1");
    self.s5u_node.port = GTPV1_U_UDP_PORT;

    ctx_initialized = 1;

    return CORE_OK;
}

status_t pgw_ctx_final()
{
    d_assert(ctx_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    ctx_initialized = 0;
    
    return CORE_OK;
}

pgw_ctx_t* pgw_self()
{
    return &self;
}

#define TRACE_MODULE _sgw_ctx

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "gtp_path.h"

#include "sgw_ctx.h"

static sgw_ctx_t self;

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

    ctx_initialized = 1;

    return CORE_OK;
}

status_t sgw_ctx_final()
{
    d_assert(ctx_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    ctx_initialized = 0;
    
    return CORE_OK;
}

sgw_ctx_t* sgw_self()
{
    return &self;
}

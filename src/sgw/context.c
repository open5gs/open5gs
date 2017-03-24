#define TRACE_MODULE _sgw_ctx

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "context.h"
#include "gtp_path.h"

static sgw_ctx_t self;

static int ctx_initialized = 0;

status_t sgw_ctx_init()
{
    d_assert(ctx_initialized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    /* Initialize MME context */
    memset(&self, 0, sizeof(sgw_ctx_t));

    self.sgw_local_addr = inet_addr("127.0.0.1");
    self.mme_remote_addr = inet_addr("127.0.0.1");
    self.pgw_remote_addr = inet_addr("127.0.0.1");

    self.s11_local_port = S11_UDP_PORT + 1;
    self.s11_remote_port = S11_UDP_PORT;

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


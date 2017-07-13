#define TRACE_MODULE _context

#include "context.h"

static context_t self;

static int context_initialized = 0;

status_t context_init()
{
    d_assert(context_initialized == 0, return CORE_ERROR,
            "Context already has been context_initialized");

    /* Initialize context */
    memset(&self, 0, sizeof(context_t));

    context_initialized = 1;

    return CORE_OK;
}

status_t context_final()
{
    d_assert(context_initialized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    context_initialized = 0;

    return CORE_OK;
}

context_t* context_self()
{
    return &self;
}

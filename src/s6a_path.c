#define TRACE_MODULE _s6a_path

#include "core_debug.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#include "s6a_path.h"

status_t s6a_open()
{
    int ret;
    ret = fd_core_initialize ();
    if (ret != 0) 
    {
        d_error("fd_core_initialize() failed");
        return ret;
    } 
    return CORE_OK;
}

status_t s6a_close()
{
    int ret;
    ret = fd_core_shutdown();
    if (ret != 0) 
    {
        d_error("fd_core_shutdown() failed");
    }

    return CORE_OK;
}

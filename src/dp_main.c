#include "core.h"
#define TRACE_MODULE _dp_main
#include "core_thread.h"
#include "core_net.h"

void *THREAD_FUNC dp_main(void *data)
{
    while (!thread_should_stop())
    {
        net_fds_read_run(50); 
    }

    return NULL;
}

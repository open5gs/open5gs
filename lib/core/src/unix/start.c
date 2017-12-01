#include "core.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pool.h"
#include "core_msgq.h"
#include "core_tlv.h"
#include "core_timer.h"
#include "core_atomic.h"
#include "core_mutex.h"
#include "core_cond.h"
#include "core_rwlock.h"
#include "core_semaphore.h"
#include "core_thread.h"
#include "core_network.h"
#include "core_file.h"
#include "core_pkbuf.h"
#include "core_signal.h"

status_t core_app_initialize(int *argc,
                            const char * const * *argv,
                            const char * const * *env)
{
    /* An absolute noop.  At present, only Win32 requires this stub, but it's
     * required in order to move command arguments passed through the service
     * control manager into the process, and it's required to fix the char*
     * data passed in from win32 unicode into utf-8, win32's core internal fmt.
     */
    return core_initialize();
}

static int initialized = 0;

status_t core_initialize(void)
{
    if (initialized++)
    {
        return CORE_OK;
    }

    /* IMPORTANT: Mutex should be initialized firtly because node-pool
     * framework uses mutex and it will be used by other xxx_init() */
    mutex_init();
    semaphore_init();
    cond_init();
    rwlock_init();
    atomic_init();
    thread_init();
    network_init();
    file_init();
    pkbuf_init();
    tlv_init();
    tm_init();
    msgq_init();
    signal_init();

    return CORE_OK;
}

void core_terminate(void)
{
    initialized--;
    if (initialized) {
        return;
    }

    /* Reverse ordered finalization */
    msgq_final();
    tm_final();
    tlv_final();
    pkbuf_final();
    file_final();
    network_final();
    thread_final();
    atomic_final();
    rwlock_final();
    cond_final();
    semaphore_final();
    mutex_final();

    return;
}

void core_terminate2(void)
{
    core_terminate();
}

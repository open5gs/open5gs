#define TRACE_MODULE _pgw_fd

#include "fd_lib.h"

#include "pgw_context.h"

int pgw_fd_init(void)
{
    fd_context_init(FD_MODE_CLIENT);

    CHECK_FCT( fd_init(pgw_self()->fd_conf_path) );

	return 0;
}

void pgw_fd_final(void)
{
    fd_final();
}

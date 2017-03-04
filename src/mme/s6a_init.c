#define TRACE_MODULE _s6a_mme_init

#include "core_debug.h"

#include "s6a_app.h"

struct session_handler *s6a_mme_reg = NULL;

status_t s6a_mme_init(void)
{
    status_t rv;

    rv = s6a_init(MODE_MME);
    if (rv != CORE_OK) return rv;

	d_assert(fd_sess_handler_create(&s6a_mme_reg, 
            (void *)free, NULL, NULL) == 0, return -1,);
	
	return CORE_OK;
}

void s6a_mme_final(void)
{
	d_assert(fd_sess_handler_destroy(&s6a_mme_reg, NULL) == 0,,);

    s6a_final();
}

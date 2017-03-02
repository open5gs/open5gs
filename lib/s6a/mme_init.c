#define TRACE_MODULE _s6a_mme_init

#include "core_debug.h"

#include "s6a_app.h"

struct session_handler *s6a_mme_reg = NULL;

int mme_init(void)
{
	d_assert(fd_sess_handler_create(&s6a_mme_reg, 
            (void *)free, NULL, NULL) == 0, return -1,);
	
	return 0;
}

void mme_final(void)
{
	d_assert(fd_sess_handler_destroy(&s6a_mme_reg, NULL) == 0,,);
}

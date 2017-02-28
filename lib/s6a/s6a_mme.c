#define TRACE_MODULE _s6a_mme

#include "s6a_app.h"

struct session_handler *s6a_mme_reg = NULL;

int s6a_mme_init(void)
{
	CHECK_FCT(fd_sess_handler_create(&s6a_mme_reg, (void *)free, NULL, NULL));
	
	return 0;
}

void s6a_mme_final(void)
{
	CHECK_FCT_DO(fd_sess_handler_destroy(&s6a_mme_reg, NULL), /* continue */ );
	
	return;
};

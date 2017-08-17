#define TRACE_MODULE _pgw_gx_handler

#include "core_debug.h"
#include "core_pool.h"

#include "fd_lib.h"
#include "gx_lib.h"

#include "pgw_gx_handler.h"

#define MAX_NUM_SESSION_STATE 32

static struct session_handler *pgw_gx_reg = NULL;

struct sess_state {
#if 0 /* FIXME */
    mme_ue_t *mme_ue;
#endif
    struct timespec ts; /* Time of sending the message */
};

pool_declare(pgw_gx_sess_pool, struct sess_state, MAX_NUM_SESSION_STATE);

/* PGW Sends Credit Control Request to PCRF */
#if 0
void pgw_gx_send_air(mme_ue_t *mme_ue)
{
    return;
}
#endif

int pgw_gx_init(void)
{
    pool_init(&pgw_gx_sess_pool, MAX_NUM_SESSION_STATE);

    CHECK_FCT( fd_init(FD_MODE_CLIENT, pgw_self()->fd_conf_path) );

	/* Install objects definitions for this application */
	CHECK_FCT( gx_dict_init() );

	CHECK_FCT( fd_sess_handler_create(&pgw_gx_reg, (void *)free, NULL, NULL) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(gx_application, fd_vendor, 1, 0) );
	
	return 0;
}

void pgw_gx_final(void)
{
	CHECK_FCT_DO( fd_sess_handler_destroy(&pgw_gx_reg, NULL), );

    fd_final();

    if (pool_size(&pgw_gx_sess_pool) != pool_avail(&pgw_gx_sess_pool))
        d_error("%d not freed in pgw_gx_sess_pool[%d] of S6A-SM",
                pool_size(&pgw_gx_sess_pool) - pool_avail(&pgw_gx_sess_pool),
                pool_size(&pgw_gx_sess_pool));

    d_trace(3, "%d not freed in pgw_gx_sess_pool[%d] of S6A-SM\n",
            pool_size(&pgw_gx_sess_pool) - pool_avail(&pgw_gx_sess_pool),
            pool_size(&pgw_gx_sess_pool));

    pool_final(&pgw_gx_sess_pool);
}

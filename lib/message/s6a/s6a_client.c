#define TRACE_MODULE _s6a_client

#include "core_debug.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/extension.h"

#include "s6a_message.h"

static struct session_handler *s6a_cli_reg = NULL;

struct sess_state {
	c_int32_t randval; /* a random value to store in Test-AVP */
	struct timespec ts;	/* Time of sending the message */
} ;

status_t s6a_client_start()
{
	CHECK_FCT(fd_sess_handler_create(&s6a_cli_reg, (void *)free, NULL, NULL));

    return CORE_OK;
}

void s6a_client_stop()
{
	CHECK_FCT_DO(fd_sess_handler_destroy(&s6a_cli_reg, NULL), /* continue */ );
}

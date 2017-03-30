#define TRACE_MODULE _pgw_handler

#include "core_debug.h"

#include "event.h"
#include "pgw_path.h"
#include "s5c_handler.h"

void pgw_s5c_handle_create_session_request(
        gtp_xact_t *xact, gtp_create_session_request_t *req)
{
    d_info("pgw_s5c_handle_create_session_request");
}

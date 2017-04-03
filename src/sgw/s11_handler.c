#define TRACE_MODULE _sgw_handler

#include "core_debug.h"

#include "event.h"
#include "sgw_path.h"
#include "s11_handler.h"

void sgw_s11_handle_create_session_request(
        gtp_xact_t *xact, gtp_message_t *gtp_message)
{
    d_assert(sgw_s5c_send_to_pgw(NULL, xact, gtp_message) == CORE_OK, return, 
            "failed to send message");
}

void sgw_s11_handle_create_session_response(
        gtp_xact_t *xact, gtp_message_t *gtp_message)
{
    d_info("handle create_session response");
#if 0
    d_assert(sgw_s11_send_to_mme(xact, gtp_message) == CORE_OK, return, 
            "failed to send message");
#endif
}

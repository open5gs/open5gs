#define TRACE_MODULE _sgw_handler

#include "core_debug.h"

#include "sgw_event.h"
#include "sgw_ctx.h"
#include "sgw_path.h"
#include "sgw_handler.h"

void sgw_handle_create_session_request(
        gtp_xact_t *xact, c_uint8_t type, gtp_message_t *gtp_message)
{
    gtp_create_session_request_t *req = NULL;
    sgw_gtpc_t *gtpc = NULL;

    d_assert(xact, return, "gtpc_add failed");
    d_assert(gtp_message, return, "gtpc_add failed");

    req = &gtp_message->create_session_request;

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No Sender F-TEID for control plance");
        return;
    }

    gtpc = sgw_ctx_gtpc_add();
    d_assert(gtpc, return, "gtpc_add failed");

    memcpy(&gtpc->mme, req->sender_f_teid_for_control_plane.data,
                req->sender_f_teid_for_control_plane.len);

    d_assert(sgw_s5c_send_to_pgw(xact, type, gtp_message) == CORE_OK, 
            return, "failed to send message");
}

void sgw_handle_create_session_response(
        gtp_xact_t *xact, c_uint8_t type, gtp_message_t *gtp_message)
{
    d_info("handle create_session response");
    d_assert(sgw_s11_send_to_mme(xact, type, gtp_message) == CORE_OK, 
            return, "failed to send message");
}

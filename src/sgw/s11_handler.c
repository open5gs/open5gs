#define TRACE_MODULE _sgw_handler

#include "core_debug.h"

#include "event.h"
#include "sgw_path.h"
#include "s11_handler.h"

void sgw_handle_create_session_request(
        gtp_xact_t *xact, gtp_create_session_request_t *req)
{
    status_t rv;
    pkbuf_t *pkbuf;

    rv = tlv_build_msg(&pkbuf, &tlv_desc_create_session_request, req,
            TLV_MODE_T1_L2_I1);
    d_assert(rv == CORE_OK, return, "failed to build GTP TLV");

    d_assert(sgw_s5c_send_to_pgw(GTP_CREATE_SESSION_REQUEST_TYPE, pkbuf, xact),
            return, "failed to send message");
}

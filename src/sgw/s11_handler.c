#define TRACE_MODULE _sgw_handler

#include "core_debug.h"

#include "event.h"

#include "s11_handler.h"

void sgw_handle_create_session_request(
        gtp_xact_t *xact, gtp_create_session_request_t *req)
{
    d_info("sgw_handle_create_session_request");
    d_print_hex(req->imsi.data, req->imsi.len);
    d_print_hex(req->msisdn.data, req->msisdn.len);
    d_print_hex(req->me_identity.data, req->me_identity.len);

}

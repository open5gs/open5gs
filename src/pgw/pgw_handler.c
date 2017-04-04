#define TRACE_MODULE _pgw_handler

#include "core_debug.h"

#include "pgw_event.h"
#include "pgw_path.h"
#include "pgw_handler.h"

void pgw_handle_create_session_request(
        gtp_xact_t *xact, gtp_create_session_request_t *req)
{
    gtp_message_t gtp_message;
    gtp_create_session_response_t *rsp = &gtp_message.create_session_response;

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    rsp->cause.presence = 1;
    rsp->cause.data = (c_uint8_t *)"\x55\x15";
    rsp->cause.len = 2;

    pgw_s5c_send_to_sgw(xact, GTP_CREATE_SESSION_RESPONSE_TYPE, &gtp_message);
}

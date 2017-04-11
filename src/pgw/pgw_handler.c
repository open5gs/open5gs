#define TRACE_MODULE _pgw_handler

#include "core_debug.h"

#include "gtp_types.h"

#include "pgw_event.h"
#include "pgw_context.h"
#include "pgw_path.h"
#include "pgw_handler.h"

void pgw_handle_create_session_request(
        gtp_xact_t *xact, gtp_create_session_request_t *req)
{
    status_t rv;
    pkbuf_t *pkbuf;
    gtp_message_t gtp_message;
    c_uint8_t type = GTP_CREATE_SESSION_RESPONSE_TYPE;
    gtp_create_session_response_t *rsp = &gtp_message.create_session_response;

    gtp_cause_t cause;
    gtp_f_teid_t *sgw_f_teid;
    gtp_f_teid_t pgw_f_teid;

    pgw_sess_t *sess = NULL;

    d_assert(xact, return, "Null param");
    d_assert(req, return, "Null param");

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No Sender F-TEID for control plance");
        return;
    }

    sgw_f_teid = req->sender_f_teid_for_control_plane.data;
    if (!(sgw_f_teid->ipv4 == 1 && sgw_f_teid->ipv6 == 0 &&
            sgw_f_teid->interface_type == GTP_F_TEID_S5_S8_SGW_GTP_C))
    {
        d_error("Invalid Parameter(ipv4:%d,ipv6:%d,type:%d",
            sgw_f_teid->ipv4, sgw_f_teid->ipv6,
            sgw_f_teid->interface_type);
        return;
    }

    sess = pgw_sess_add();
    d_assert(sess, return, "sess_add failed");

    sess->sgw_teid = ntohl(sgw_f_teid->teid);
    sess->sgw_addr = sgw_f_teid->ipv4_addr;

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    memset(&cause, 0, sizeof(cause));
    cause.cause_value = GTP_CAUSE_REQUEST_ACCEPTED;
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    memset(&pgw_f_teid, 0, sizeof(gtp_f_teid_t));
    pgw_f_teid.ipv4 = 1;
    pgw_f_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_C;
    pgw_f_teid.ipv4_addr = pgw_self()->s5c_addr;
    pgw_f_teid.teid = htonl(sess->teid);
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        presence = 1;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        data = &pgw_f_teid;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        len = GTP_F_TEID_IPV4_LEN;

    rv = gtp_build_msg(&pkbuf, type, &gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    pgw_s5c_send_to_sgw(xact, type, sess->sgw_teid, pkbuf);
}

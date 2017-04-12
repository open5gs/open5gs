#define TRACE_MODULE _sgw_handler

#include "core_debug.h"

#include "gtp_types.h"

#include "sgw_event.h"
#include "sgw_context.h"
#include "sgw_path.h"
#include "sgw_handler.h"

void sgw_handle_create_session_request(
        gtp_xact_t *xact, c_uint8_t type, gtp_message_t *gtp_message)
{
    status_t rv;
    gtp_create_session_request_t *req = NULL;
    sgw_sess_t *sess = NULL;
    pkbuf_t *pkbuf = NULL;
    gtp_f_teid_t *f_teid = NULL;

    d_assert(xact, return, "Null param");
    d_assert(gtp_message, return, "Null param");

    req = &gtp_message->create_session_request;

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No Sender F-TEID for control plance");
        return;
    }

    f_teid = req->sender_f_teid_for_control_plane.data;
    if (!(f_teid->ipv4 == 1 && f_teid->ipv6 == 0 &&
            f_teid->interface_type == GTP_F_TEID_S11_MME_GTP_C))
    {
        d_error("Invalid Parameter(ipv4:%d,ipv6:%d,type:%d",
            f_teid->ipv4, f_teid->ipv6,
            f_teid->interface_type);
        return;
    }

    sess = sgw_sess_add();
    d_assert(sess, return, "sess_add failed");

    sess->mme_s11_teid = ntohl(f_teid->teid);
    sess->mme_s11_addr = f_teid->ipv4_addr;

    f_teid->teid = htonl(sess->teid);
    f_teid->ipv4_addr = sgw_self()->s5c_addr;
    f_teid->interface_type = GTP_F_TEID_S5_S8_SGW_GTP_C;

    rv = gtp_build_msg(&pkbuf, type, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    d_assert(sgw_s5c_send_to_pgw(xact, type, 0, pkbuf) == CORE_OK, 
            return, "failed to send message");
}

void sgw_handle_create_session_response(gtp_xact_t *xact, 
    sgw_sess_t *sess, c_uint8_t type, gtp_message_t *gtp_message)
{
    status_t rv;
    gtp_create_session_response_t *rsp = NULL;
    pkbuf_t *pkbuf = NULL;
    gtp_f_teid_t *pgw_f_teid = NULL;
    gtp_f_teid_t sgw_f_teid;

    d_assert(sess, return, "Null param");
    d_assert(xact, return, "Null param");
    d_assert(gtp_message, return, "Null param");

    rsp = &gtp_message->create_session_response;

    if (rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
            presence == 0)
    {
        d_error("No PGW F-TEID for GTP-based control plance");
        return;
    }

    pgw_f_teid = rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
                data;
    if (!(pgw_f_teid->ipv4 == 1 && pgw_f_teid->ipv6 == 0 &&
            pgw_f_teid->interface_type == GTP_F_TEID_S5_S8_PGW_GTP_C))
    {
        d_error("Invalid Parameter(ipv4:%d,ipv6:%d,type:%d",
            pgw_f_teid->ipv4, pgw_f_teid->ipv6,
            pgw_f_teid->interface_type);
        return;
    }

    sess->pgw_s5c_teid = ntohl(pgw_f_teid->teid);
    sess->pgw_s5c_addr = pgw_f_teid->ipv4_addr;

    memset(&sgw_f_teid, 0, sizeof(gtp_f_teid_t));
    sgw_f_teid.ipv4 = 1;
    sgw_f_teid.interface_type = GTP_F_TEID_S11_S4_SGW_GTP_C;
    sgw_f_teid.ipv4_addr = sgw_self()->s11_addr;
    sgw_f_teid.teid = htonl(sess->teid);

    rsp->sender_f_teid_for_control_plane.presence = 1;
    rsp->sender_f_teid_for_control_plane.data = &sgw_f_teid;
    rsp->sender_f_teid_for_control_plane.len = GTP_F_TEID_IPV4_LEN;

    rv = gtp_build_msg(&pkbuf, type, gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    d_assert(sgw_s11_send_to_mme(
            xact, type, sess->mme_s11_teid, pkbuf) == CORE_OK, return, 
            "failed to send message");
}

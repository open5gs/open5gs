#define TRACE_MODULE _pgw_handler

#include "core_debug.h"

#include "gtp_types.h"

#include "pgw_event.h"
#include "pgw_context.h"
#include "pgw_path.h"
#include "pgw_handler.h"

c_int16_t pgw_pco_build(c_uint8_t *pco_buf, tlv_pco_t *tlv_pco)
{
    pco_t ue, pgw;
    pco_ipcp_t pco_ipcp;
    c_int8_t size = 0;
    int i = 0;

    d_assert(pco_buf, return -1, "Null param");
    d_assert(tlv_pco, return -1, "Null param");

    size = pco_parse(&ue, tlv_pco->data, tlv_pco->len);
    d_assert(size, return -1, "pco parse failed");

    memset(&pgw, 0, sizeof(pco_t));
    pgw.ext = ue.ext;
    pgw.configuration_protocol = ue.configuration_protocol;

    for(i = 0; i < ue.num_of_id; i++)
    {
        c_uint8_t *data = ue.ids[i].data;
        switch(ue.ids[i].id)
        {
            case PCO_ID_CHALLENGE_HANDSHAKE_AUTHENTICATION_PROTOCOL:
            {
                if (data[0] == 2) /* Code : Response */
                {
                    pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                    pgw.ids[pgw.num_of_id].len = 4;
                    pgw.ids[pgw.num_of_id].data =
                        (c_uint8_t *)"\x03\x00\x00\x04"; /* Code : Success */
                    pgw.num_of_id++;
                }
                break;
            }
            case PCO_ID_INTERNET_PROTOCOL_CONTROL_PROTOCOL:
            {
                if (data[0] == 1) /* Code : Configuration Request */
                {
                    c_uint16_t len = 16;

                    memset(&pco_ipcp, 0, sizeof(pco_ipcp_t));
                    pco_ipcp.code = 2; /* Code : Configuration Ack */
                    pco_ipcp.len = htons(len);

                    /* Primary DNS Server IP Address */
                    pco_ipcp.options[0].type = 129; 
                    pco_ipcp.options[0].len = 6; 
                    pco_ipcp.options[0].addr = pgw_self()->primary_dns_addr;

                    /* Secondary DNS Server IP Address */
                    pco_ipcp.options[1].type = 131; 
                    pco_ipcp.options[1].len = 6; 
                    pco_ipcp.options[1].addr = pgw_self()->secondary_dns_addr;

                    pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                    pgw.ids[pgw.num_of_id].len = len;
                    pgw.ids[pgw.num_of_id].data = (c_uint8_t *)&pco_ipcp;

                    pgw.num_of_id++;
                }
                break;
            }
            case PCO_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST:
            {
                pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                pgw.ids[pgw.num_of_id].len = 4;
                pgw.ids[pgw.num_of_id].data = &pgw_self()->primary_dns_addr;
                pgw.num_of_id++;

                pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                pgw.ids[pgw.num_of_id].len = 4;
                pgw.ids[pgw.num_of_id].data = &pgw_self()->secondary_dns_addr;

                pgw.num_of_id++;
                break;
            }
            case PCO_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING:
                break;
            default:
                d_warn("Unknown PCO ID:(0x%x)", ue.ids[i].id);
        }
    }

    size = pco_build(pco_buf, MAX_PCO_LEN, &pgw);
    return size;
}

void pgw_handle_create_session_request(
        gtp_xact_t *xact, gtp_create_session_request_t *req)
{
    status_t rv;
    pkbuf_t *pkbuf;
    gtp_message_t gtp_message;
    c_uint8_t type = GTP_CREATE_SESSION_RESPONSE_TYPE;
    gtp_create_session_response_t *rsp = &gtp_message.create_session_response;

    gtp_cause_t cause;
    gtp_f_teid_t *sgw_s5c_teid, *sgw_s5u_teid;
    gtp_f_teid_t pgw_s5c_teid, pgw_s5u_teid;
    c_int8_t apn[MAX_APN_LEN];
    c_uint8_t pco_buf[MAX_PCO_LEN];
    c_int16_t pco_len;

    pgw_sess_t *sess = NULL;
    pgw_bearer_t *bearer = NULL;

    d_assert(xact, return, "Null param");
    d_assert(req, return, "Null param");

    if (req->access_point_name.presence == 0)
    {
        d_error("No APN");
        return;
    }
    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No TEID");
        return;
    }
    if (req->bearer_contexts_to_be_created.presence == 0)
    {
        d_error("No Bearer");
        return;
    }
    if (req->bearer_contexts_to_be_created.eps_bearer_id.presence == 0)
    {
        d_error("No EPS Bearer ID");
        return;
    }
    if (req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.presence == 0)
    {
        d_error("No TEID");
        return;
    }

    /* Generate Control Plane(UL) : PGW-S5C */
    memcpy(apn, req->access_point_name.data, req->access_point_name.len);
    apn[req->access_point_name.len] = 0;
    sess = pgw_sess_find_by_apn(apn);
    if (!sess)
    {
        bearer = pgw_sess_add(apn,
                req->bearer_contexts_to_be_created.eps_bearer_id.u8);
        d_assert(bearer, return, "No Bearer Context");
        sess = bearer->sess;
    }
    d_assert(sess, return, "No Session Context");

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    memset(&cause, 0, sizeof(cause));
    cause.value = GTP_CAUSE_REQUEST_ACCEPTED;
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    /* Receive Control Plane(DL) : SGW-S5C */
    sgw_s5c_teid = req->sender_f_teid_for_control_plane.data;
    sess->sgw_s5c_teid = ntohl(sgw_s5c_teid->teid);
    sess->sgw_s5c_addr = sgw_s5c_teid->ipv4_addr;

    /* Receive Control Plane(DL) : SGW-S5U */
    sgw_s5u_teid = req->bearer_contexts_to_be_created.s5_s8_u_sgw_f_teid.data;
    bearer->sgw_s5u_teid = ntohl(sgw_s5u_teid->teid);
    bearer->sgw_s5u_addr = sgw_s5u_teid->ipv4_addr;

    d_trace(3, "[GTP] Create Session Reqeust : "
            "SGW[%d] --> PGW[%d]\n", sess->sgw_s5c_teid, sess->pgw_s5c_teid);

    /* Send Control Plane(UL) : PGW-S5C */
    memset(&pgw_s5c_teid, 0, sizeof(gtp_f_teid_t));
    pgw_s5c_teid.ipv4 = 1;
    pgw_s5c_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_C;
    pgw_s5c_teid.ipv4_addr = sess->pgw_s5c_addr;
    pgw_s5c_teid.teid = htonl(sess->pgw_s5c_teid);
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        presence = 1;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        data = &pgw_s5c_teid;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        len = GTP_F_TEID_IPV4_LEN;

    d_assert(sess->ip_pool, pgw_sess_remove(sess); return, "No IP Pool");
    sess->paa.pdn_type = GTP_PDN_TYPE_IPV4;
    sess->paa.ipv4_addr = sess->ip_pool->ue_addr;

    rsp->pdn_address_allocation.presence = 1;
    rsp->pdn_address_allocation.data = &sess->paa;
    rsp->pdn_address_allocation.len = PAA_IPV4_LEN;

    rsp->apn_restriction.presence = 1;
    rsp->apn_restriction.u8 = GTP_APN_NO_RESTRICTION;

    if (req->protocol_configuration_options.presence == 1)
    {
        pco_len = pgw_pco_build(pco_buf, &req->protocol_configuration_options);
        d_assert(pco_len > 0, pgw_sess_remove(sess); return, "pco build failed");
        rsp->protocol_configuration_options.presence = 1;
        rsp->protocol_configuration_options.data = pco_buf;
        rsp->protocol_configuration_options.len = pco_len;
    }

    rsp->bearer_contexts_created.presence = 1;
    rsp->bearer_contexts_created.eps_bearer_id.presence = 1;
    rsp->bearer_contexts_created.eps_bearer_id.u8 = bearer->id;

    /* Send Data Plane(UL) : PGW-S5U */
    memset(&pgw_s5u_teid, 0, sizeof(gtp_f_teid_t));
    pgw_s5u_teid.ipv4 = 1;
    pgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_U;
    pgw_s5u_teid.ipv4_addr = bearer->pgw_s5u_addr;
    pgw_s5u_teid.teid = htonl(bearer->pgw_s5u_teid);
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.presence = 1;
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.data = &pgw_s5u_teid;
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.len = 
        GTP_F_TEID_IPV4_LEN;

    rv = gtp_build_msg(&pkbuf, type, &gtp_message);
    d_assert(rv == CORE_OK, pgw_sess_remove(sess); return, "gtp build failed");

    pgw_s5c_send_to_sgw(xact, type, sess->sgw_s5c_teid, pkbuf);
}

void pgw_handle_delete_session_request(
        gtp_xact_t *xact, pgw_sess_t *sess, gtp_delete_session_request_t *req)
{
    status_t rv;
    pkbuf_t *pkbuf;
    gtp_message_t gtp_message;
    c_uint8_t type = GTP_DELETE_SESSION_RESPONSE_TYPE;
    gtp_delete_session_response_t *rsp = &gtp_message.delete_session_response;
    c_uint32_t sgw_s5c_teid;

    gtp_cause_t cause;
    c_uint8_t pco_buf[MAX_PCO_LEN];
    c_int16_t pco_len;
    
    d_assert(xact, return, "Null param");
    /* sess can be NULL */
    d_assert(req, return, "Null param");

    /* prepare cause */
    memset(&cause, 0, sizeof(cause));
    cause.value = GTP_CAUSE_REQUEST_ACCEPTED;

    /* Remove a pgw session */
    if (sess)
    {
        /* backup sgw_s5c_teid in session context */
        sgw_s5c_teid = sess->sgw_s5c_teid;

        if (pgw_sess_remove(sess) != CORE_OK)
        {
            d_error("Error on PGW session %d removal", sess->index);
            cause.value = GTP_CAUSE_CONTEXT_NOT_FOUND;
        }
    }
    else
    {
        cause.value = GTP_CAUSE_INVALID_PEER;
    }

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    /* Cause */
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    /* Recovery */

    /* PCO */
    if (req->protocol_configuration_options.presence == 1)
    {
        pco_len = pgw_pco_build(pco_buf, &req->protocol_configuration_options);
        d_assert(pco_len > 0, return, "pco build failed");
        rsp->protocol_configuration_options.presence = 1;
        rsp->protocol_configuration_options.data = pco_buf;
        rsp->protocol_configuration_options.len = pco_len;
    }

    /* Private Extension */

    /* build */
    rv = gtp_build_msg(&pkbuf, type, &gtp_message);
    d_assert(rv == CORE_OK, return, "gtp build failed");

    /* send */
    pgw_s5c_send_to_sgw(xact, type, sgw_s5c_teid, pkbuf);
}

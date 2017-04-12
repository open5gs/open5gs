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
    gtp_f_teid_t *sgw_s5c_teid;
    gtp_f_teid_t pgw_s5c_teid, pgw_s5u_teid;
    c_int8_t apn[MAX_APN_LEN];
    pdn_t *pdn = NULL;
    c_uint8_t pco_buf[MAX_PCO_LEN];
    c_int16_t pco_len;
    bearer_t *bearer = NULL;

    pgw_sess_t *sess = NULL;

    d_assert(xact, return, "Null param");
    d_assert(req, return, "Null param");

    if (req->sender_f_teid_for_control_plane.presence == 0)
    {
        d_error("No Sender F-TEID for control plance");
        return;
    }
    if (req->access_point_name.presence == 0)
    {
        d_error("No APN");
        return;
    }

    if (req->protocol_configuration_options.presence == 0)
    {
        d_error("No Protocol Configuration Options");
        return;
    }

    if (req->bearer_contexts_to_be_created.presence == 0 &&
        req->bearer_contexts_to_be_created.eps_bearer_id.presence == 0)
    {
        d_error("No Bearer");
        return;
    }

    sgw_s5c_teid = req->sender_f_teid_for_control_plane.data;
    if (!(sgw_s5c_teid->ipv4 == 1 && sgw_s5c_teid->ipv6 == 0 &&
            sgw_s5c_teid->interface_type == GTP_F_TEID_S5_S8_SGW_GTP_C))
    {
        d_error("Invalid Parameter(ipv4:%d,ipv6:%d,type:%d",
            sgw_s5c_teid->ipv4, sgw_s5c_teid->ipv6,
            sgw_s5c_teid->interface_type);
        return;
    }

    sess = pgw_sess_add();
    d_assert(sess, return, "No Session Context");

    sess->sgw_s5c_teid = ntohl(sgw_s5c_teid->teid);
    sess->sgw_s5c_addr = sgw_s5c_teid->ipv4_addr;

    memcpy(apn, req->access_point_name.data, req->access_point_name.len);
    apn[req->access_point_name.len] = 0;
    pdn = pgw_pdn_find_by_apn(sess, apn);
    if (!pdn)
    {
        pdn = pgw_pdn_add(sess, apn);
    }
    d_assert(pdn, pgw_sess_remove(sess); return, "No PDN Context");

    bearer = pgw_bearer_find_by_id(sess, 
                req->bearer_contexts_to_be_created.eps_bearer_id.u8);
    if (!bearer)
    {
        bearer = pgw_bearer_add(sess,
                req->bearer_contexts_to_be_created.eps_bearer_id.u8);
    }
    d_assert(bearer, pgw_sess_remove(sess); return, "No Bearer Context");

    memset(&gtp_message, 0, sizeof(gtp_message_t));

    memset(&cause, 0, sizeof(cause));
    cause.cause_value = GTP_CAUSE_REQUEST_ACCEPTED;
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    memset(&pgw_s5c_teid, 0, sizeof(gtp_f_teid_t));
    pgw_s5c_teid.ipv4 = 1;
    pgw_s5c_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_C;
    pgw_s5c_teid.ipv4_addr = pgw_self()->s5c_addr;
    pgw_s5c_teid.teid = htonl(sess->teid);
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        presence = 1;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        data = &pgw_s5c_teid;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        len = GTP_F_TEID_IPV4_LEN;

    pdn->paa.gtp_type = GTP_PDN_TYPE_IPV4;
    pdn->paa.ipv4_addr = inet_addr("45.45.45.113"); /* FIXME */

    rsp->pdn_address_allocation.presence = 1;
    rsp->pdn_address_allocation.data = &pdn->paa;
    rsp->pdn_address_allocation.len = PAA_IPV4_LEN;

    rsp->apn_restriction.presence = 1;
    rsp->apn_restriction.u8 = GTP_APN_NO_RESTRICTION;

    pco_len = pgw_pco_build(pco_buf, &req->protocol_configuration_options);
    d_assert(pco_len > 0, pgw_sess_remove(sess); return, "pco build failed");
    rsp->protocol_configuration_options.presence = 1;
    rsp->protocol_configuration_options.data = pco_buf;
    rsp->protocol_configuration_options.len = pco_len;

    rsp->bearer_contexts_created.presence = 1;
    rsp->bearer_contexts_created.eps_bearer_id.presence = 1;
    rsp->bearer_contexts_created.eps_bearer_id.u8 = bearer->id;

    memset(&pgw_s5u_teid, 0, sizeof(gtp_f_teid_t));
    pgw_s5u_teid.ipv4 = 1;
    pgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_U;
    pgw_s5u_teid.ipv4_addr = pgw_self()->s5u_addr;
    pgw_s5u_teid.teid = htonl(sess->teid);
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.presence = 1;
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.data = &pgw_s5u_teid;
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.len = 
        GTP_F_TEID_IPV4_LEN;

    rv = gtp_build_msg(&pkbuf, type, &gtp_message);
    d_assert(rv == CORE_OK, pgw_sess_remove(sess); return, "gtp build failed");

    pgw_s5c_send_to_sgw(xact, type, sess->sgw_s5c_teid, pkbuf);
}

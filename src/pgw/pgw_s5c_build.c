#define TRACE_MODULE _pgw_s5c_build

#include "core_debug.h"

#include "gtp_types.h"
#include "gtp_message.h"
#include "gx_message.h"

#include "types.h"
#include "pgw_context.h"

static c_int16_t pgw_pco_build(c_uint8_t *pco_buf, tlv_pco_t *tlv_pco);

status_t pgw_s5c_build_create_session_response(
        pkbuf_t **pkbuf, c_uint8_t type, pgw_sess_t *sess,
        gx_cca_message_t *cca_message, gtp_create_session_request_t *req)
{
    status_t rv;
    pgw_bearer_t *bearer = NULL;

    gtp_message_t gtp_message;
    gtp_create_session_response_t *rsp = NULL;

    gtp_cause_t cause;
    gtp_f_teid_t pgw_s5c_teid, pgw_s5u_teid;
    c_uint8_t pco_buf[MAX_PCO_LEN];
    c_int16_t pco_len;

    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(req, return CORE_ERROR, "Null param");

    bearer = pgw_default_bearer_in_sess(sess);
    d_assert(bearer, return CORE_ERROR, "Null param");

    rsp = &gtp_message.create_session_response;
    memset(&gtp_message, 0, sizeof(gtp_message_t));

    /* Set Cause */
    memset(&cause, 0, sizeof(cause));
    cause.value = GTP_CAUSE_REQUEST_ACCEPTED;
    rsp->cause.presence = 1;
    rsp->cause.len = sizeof(cause);
    rsp->cause.data = &cause;

    /* Control Plane(UL) : PGW-S5C */
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

    /* PDN Address Allocation */
    d_assert(sess->ip_pool, return CORE_ERROR, "No IP Pool");
    sess->pdn.paa.pdn_type = GTP_PDN_TYPE_IPV4;
    sess->pdn.paa.ipv4_addr = sess->ip_pool->ue_addr;

    rsp->pdn_address_allocation.presence = 1;
    rsp->pdn_address_allocation.data = &sess->pdn.paa;
    rsp->pdn_address_allocation.len = PAA_IPV4_LEN;

    /* APN Restriction */
    rsp->apn_restriction.presence = 1;
    rsp->apn_restriction.u8 = GTP_APN_NO_RESTRICTION;
    
    /* TODO : APN-AMBR
     * if PCRF changes APN-AMBR, this should be included. */

    /* PCO */
    if (req->protocol_configuration_options.presence == 1)
    {
        pco_len = pgw_pco_build(pco_buf, &req->protocol_configuration_options);
        d_assert(pco_len > 0, return CORE_ERROR, "pco build failed");
        rsp->protocol_configuration_options.presence = 1;
        rsp->protocol_configuration_options.data = pco_buf;
        rsp->protocol_configuration_options.len = pco_len;
    }

    /* Bearer EBI */
    rsp->bearer_contexts_created.presence = 1;
    rsp->bearer_contexts_created.eps_bearer_id.presence = 1;
    rsp->bearer_contexts_created.eps_bearer_id.u8 = bearer->ebi;

    /* TODO : Bearer QoS 
     * if PCRF changes Bearer QoS, this should be included. */

    /* Data Plane(UL) : PGW-S5U */
    memset(&pgw_s5u_teid, 0, sizeof(gtp_f_teid_t));
    pgw_s5u_teid.ipv4 = 1;
    pgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_U;
    pgw_s5u_teid.ipv4_addr = bearer->pgw_s5u_addr;
    pgw_s5u_teid.teid = htonl(bearer->pgw_s5u_teid);
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.presence = 1;
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.data = &pgw_s5u_teid;
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.len = GTP_F_TEID_IPV4_LEN;

    gtp_message.h.type = type;
    rv = gtp_build_msg(pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

status_t pgw_s5c_build_delete_session_response(
        pkbuf_t **pkbuf, c_uint8_t type, pgw_sess_t *sess,
        gx_cca_message_t *cca_message, gtp_delete_session_request_t *req)
{
    status_t rv;

    gtp_message_t gtp_message;
    gtp_delete_session_response_t *rsp = NULL;

    gtp_cause_t cause;
    c_uint8_t pco_buf[MAX_PCO_LEN];
    c_int16_t pco_len;
    
    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(cca_message, return CORE_ERROR, "Null param");
    d_assert(req, return CORE_ERROR, "Null param");

    /* prepare cause */
    memset(&cause, 0, sizeof(cause));
    cause.value = GTP_CAUSE_REQUEST_ACCEPTED;

    rsp = &gtp_message.delete_session_response;
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
        d_assert(pco_len > 0, return CORE_ERROR, "pco build failed");
        rsp->protocol_configuration_options.presence = 1;
        rsp->protocol_configuration_options.data = pco_buf;
        rsp->protocol_configuration_options.len = pco_len;
    }

    /* Private Extension */

    /* build */
    gtp_message.h.type = type;
    rv = gtp_build_msg(pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

status_t pgw_s5c_build_create_bearer_request(
        pkbuf_t **pkbuf, c_uint8_t type, pgw_bearer_t *bearer)
{
    status_t rv;
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *linked_bearer = NULL;

    gtp_message_t gtp_message;
    gtp_create_bearer_request_t *req = NULL;

    gtp_f_teid_t pgw_s5u_teid;
    gtp_bearer_qos_t bearer_qos;
    char bearer_qos_buf[GTP_BEARER_QOS_LEN];

    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");
    linked_bearer = pgw_default_bearer_in_sess(sess);
    d_assert(linked_bearer, return CORE_ERROR, "Null param");

    req = &gtp_message.create_bearer_request;
    memset(&gtp_message, 0, sizeof(gtp_message_t));
 
    /* Linked EBI */
    req->linked_eps_bearer_id.presence = 1;
    req->linked_eps_bearer_id.u8 = linked_bearer->ebi;

    /* Bearer EBI */
    req->bearer_contexts.presence = 1;
    req->bearer_contexts.eps_bearer_id.presence = 1;
    req->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

    /* Data Plane(UL) : PGW_S5U */
    memset(&pgw_s5u_teid, 0, sizeof(gtp_f_teid_t));
    pgw_s5u_teid.ipv4 = 1;
    pgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_U;
    pgw_s5u_teid.ipv4_addr = bearer->pgw_s5u_addr;
    pgw_s5u_teid.teid = htonl(bearer->pgw_s5u_teid);
    req->bearer_contexts.s5_s8_u_sgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.data = &pgw_s5u_teid;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.len = GTP_F_TEID_IPV4_LEN;

    /* Bearer QoS */
    memset(&bearer_qos, 0, sizeof(bearer_qos));
    bearer_qos.qci = bearer->qos.qci;
    bearer_qos.priority_level = bearer->qos.arp.priority_level;
    bearer_qos.pre_emption_capability = bearer->qos.arp.pre_emption_capability;
    bearer_qos.pre_emption_vulnerability =
        bearer->qos.arp.pre_emption_vulnerability;
    bearer_qos.dl_mbr = bearer->qos.mbr.downlink;
    bearer_qos.ul_mbr = bearer->qos.mbr.uplink;
    bearer_qos.dl_gbr = bearer->qos.gbr.downlink;
    bearer_qos.ul_gbr = bearer->qos.gbr.uplink;

    req->bearer_contexts.bearer_level_qos.presence = 1;
    gtp_build_bearer_qos(&req->bearer_contexts.bearer_level_qos,
            &bearer_qos, bearer_qos_buf, GTP_BEARER_QOS_LEN);

    gtp_message.h.type = type;
    rv = gtp_build_msg(pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

static c_int16_t pgw_pco_build(c_uint8_t *pco_buf, tlv_pco_t *tlv_pco)
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

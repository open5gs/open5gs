#define TRACE_MODULE _pgw_s5c_build

#include "core_debug.h"

#include "3gpp_types.h"
#include "gtp/gtp_types.h"
#include "gtp/gtp_conv.h"
#include "gtp/gtp_message.h"
#include "fd/gx/gx_message.h"

#include "ipfw/ipfw2.h"

#include "pgw_context.h"

static c_int16_t pgw_pco_build(c_uint8_t *pco_buf, tlv_pco_t *tlv_pco);

status_t pgw_s5c_build_create_session_response(
        pkbuf_t **pkbuf, c_uint8_t type, pgw_sess_t *sess,
        gx_message_t *gx_message, gtp_create_session_request_t *req)
{
    status_t rv;
    pgw_bearer_t *bearer = NULL;

    gtp_message_t gtp_message;
    gtp_create_session_response_t *rsp = NULL;

    gtp_cause_t cause;
    gtp_f_teid_t pgw_s5c_teid, pgw_s5u_teid;
    int len;
    c_uint8_t pco_buf[MAX_PCO_LEN];
    c_int16_t pco_len;

    d_trace(3, "[PGW] Create Session Response\n");

    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(req, return CORE_ERROR, "Null param");
    bearer = pgw_default_bearer_in_sess(sess);
    d_assert(bearer, return CORE_ERROR, "Null param");

    d_trace(5, "    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]\n",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    d_trace(5, "    SGW_S5U_TEID[%d] PGW_S5U_TEID[%d]\n",
            bearer->sgw_s5u_teid, bearer->pgw_s5u_teid);

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
    pgw_s5c_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_C;
    pgw_s5c_teid.teid = htonl(sess->pgw_s5c_teid);
    rv = gtp_sockaddr_to_f_teid(
        pgw_self()->gtpc_addr, pgw_self()->gtpc_addr6, &pgw_s5c_teid, &len);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        presence = 1;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        data = &pgw_s5c_teid;
    rsp->pgw_s5_s8__s2a_s2b_f_teid_for_pmip_based_interface_or_for_gtp_based_control_plane_interface.
        len = len;

    /* PDN Address Allocation */
    rsp->pdn_address_allocation.data = &sess->pdn.paa;
    if (sess->ipv4 && sess->ipv6)
        rsp->pdn_address_allocation.len = PAA_IPV4V6_LEN;
    else if (sess->ipv4)
        rsp->pdn_address_allocation.len = PAA_IPV4_LEN;
    else if (sess->ipv6)
        rsp->pdn_address_allocation.len = PAA_IPV6_LEN;
    else
        d_assert(0, return CORE_ERROR, "No IP Pool");
    rsp->pdn_address_allocation.presence = 1;

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
    pgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_U;
    pgw_s5u_teid.teid = htonl(bearer->pgw_s5u_teid);
    rv = gtp_sockaddr_to_f_teid(
        pgw_self()->gtpu_addr, pgw_self()->gtpu_addr6, &pgw_s5u_teid, &len);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.presence = 1;
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.data = &pgw_s5u_teid;
    rsp->bearer_contexts_created.s5_s8_u_sgw_f_teid.len = len;

    gtp_message.h.type = type;
    rv = gtp_build_msg(pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

status_t pgw_s5c_build_delete_session_response(
        pkbuf_t **pkbuf, c_uint8_t type,
        gx_message_t *gx_message, gtp_delete_session_request_t *req)
{
    status_t rv;

    gtp_message_t gtp_message;
    gtp_delete_session_response_t *rsp = NULL;

    gtp_cause_t cause;
    c_uint8_t pco_buf[MAX_PCO_LEN];
    c_int16_t pco_len;
    
    d_assert(gx_message, return CORE_ERROR, "Null param");
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

static void encode_traffic_flow_template(gtp_tft_t *tft, pgw_bearer_t *bearer)
{
    int i, j, len;
    pgw_pf_t *pf = NULL;

    d_assert(tft, return,);
    d_assert(bearer, return,);

    memset(tft, 0, sizeof(*tft));
    tft->code = GTP_TFT_CODE_CREATE_NEW_TFT;

    i = 0;
    pf = pgw_pf_first(bearer);
    while(pf)
    {
        tft->pf[i].direction = pf->direction;
        tft->pf[i].identifier = pf->identifier - 1;
        tft->pf[i].precedence = i+1;

        j = 0, len = 0;
        if (pf->rule.proto)
        {
            tft->pf[i].component[j].type = 
                GTP_PACKET_FILTER_PROTOCOL_IDENTIFIER_NEXT_HEADER_TYPE;
            tft->pf[i].component[j].proto = pf->rule.proto;
            j++; len += 2;
        }

        if (pf->rule.ipv4_local)
        {
            tft->pf[i].component[j].type = 
                GTP_PACKET_FILTER_IPV4_LOCAL_ADDRESS_TYPE;
            tft->pf[i].component[j].ipv4.addr = pf->rule.ip.local.addr[0];
            tft->pf[i].component[j].ipv4.mask = pf->rule.ip.local.mask[0];
            j++; len += 9;
        }

        if (pf->rule.ipv4_remote)
        {
            tft->pf[i].component[j].type = 
                GTP_PACKET_FILTER_IPV4_REMOTE_ADDRESS_TYPE;
            tft->pf[i].component[j].ipv4.addr = pf->rule.ip.remote.addr[0];
            tft->pf[i].component[j].ipv4.mask = pf->rule.ip.remote.mask[0];
            j++; len += 9;
        }

        if (pf->rule.ipv6_local)
        {
            tft->pf[i].component[j].type = 
                GTP_PACKET_FILTER_IPV6_LOCAL_ADDRESS_PREFIX_LENGTH_TYPE;
            memcpy(tft->pf[i].component[j].ipv6.addr, pf->rule.ip.local.addr,
                    sizeof pf->rule.ip.local.addr);
            tft->pf[i].component[j].ipv6.prefixlen =
                contigmask((c_uint8_t *)pf->rule.ip.local.mask, 128);
            j++; len += 18;
        }

        if (pf->rule.ipv6_remote)
        {
            tft->pf[i].component[j].type = 
                GTP_PACKET_FILTER_IPV6_REMOTE_ADDRESS_PREFIX_LENGTH_TYPE;
            memcpy(tft->pf[i].component[j].ipv6.addr, pf->rule.ip.remote.addr,
                    sizeof pf->rule.ip.remote.addr);
            tft->pf[i].component[j].ipv6.prefixlen =
                contigmask((c_uint8_t *)pf->rule.ip.remote.mask, 128);
            j++; len += 18;
        }

        if (pf->rule.port.local.low)
        {
            if (pf->rule.port.local.low == pf->rule.port.local.high)
            {
                tft->pf[i].component[j].type = 
                    GTP_PACKET_FILTER_SINGLE_LOCAL_PORT_TYPE;
                tft->pf[i].component[j].port.low = pf->rule.port.local.low;
                j++; len += 3;
            }
            else
            {
                tft->pf[i].component[j].type = 
                    GTP_PACKET_FILTER_LOCAL_PORT_RANGE_TYPE;
                tft->pf[i].component[j].port.low = pf->rule.port.local.low;
                tft->pf[i].component[j].port.high = pf->rule.port.local.high;
                j++; len += 5;
            }
        }

        if (pf->rule.port.remote.low)
        {
            if (pf->rule.port.remote.low == pf->rule.port.remote.high)
            {
                tft->pf[i].component[j].type = 
                    GTP_PACKET_FILTER_SINGLE_REMOTE_PORT_TYPE;
                tft->pf[i].component[j].port.low = pf->rule.port.remote.low;
                j++; len += 3;
            }
            else
            {
                tft->pf[i].component[j].type = 
                    GTP_PACKET_FILTER_REMOTE_PORT_RANGE_TYPE;
                tft->pf[i].component[j].port.low = pf->rule.port.remote.low;
                tft->pf[i].component[j].port.high = pf->rule.port.remote.high;
                j++; len += 5;
            }
        }

        tft->pf[i].num_of_component = j;
        tft->pf[i].length = len;
        i++;

        pf = pgw_pf_next(pf);
    }
    tft->num_of_packet_filter = i;
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
    gtp_tft_t tft;
    int len;
    char tft_buf[GTP_MAX_TRAFFIC_FLOW_TEMPLATE];

    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");
    linked_bearer = pgw_default_bearer_in_sess(sess);
    d_assert(linked_bearer, return CORE_ERROR, "Null param");

    d_trace(3, "[PGW] Create Bearer Request\n");
    d_trace(5, "    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]\n",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);

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
    pgw_s5u_teid.interface_type = GTP_F_TEID_S5_S8_PGW_GTP_U;
    pgw_s5u_teid.teid = htonl(bearer->pgw_s5u_teid);
    rv = gtp_sockaddr_to_f_teid(
        pgw_self()->gtpu_addr, pgw_self()->gtpu_addr6, &pgw_s5u_teid, &len);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    req->bearer_contexts.s5_s8_u_sgw_f_teid.presence = 1;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.data = &pgw_s5u_teid;
    req->bearer_contexts.s5_s8_u_sgw_f_teid.len = len;

    /* Bearer QoS */
    memset(&bearer_qos, 0, sizeof(bearer_qos));
    bearer_qos.qci = bearer->qos.qci;
    bearer_qos.priority_level = bearer->qos.arp.priority_level;
    bearer_qos.pre_emption_capability = 
        bearer->qos.arp.pre_emption_capability;
    bearer_qos.pre_emption_vulnerability =
        bearer->qos.arp.pre_emption_vulnerability;
    bearer_qos.dl_mbr = bearer->qos.mbr.downlink;
    bearer_qos.ul_mbr = bearer->qos.mbr.uplink;
    bearer_qos.dl_gbr = bearer->qos.gbr.downlink;
    bearer_qos.ul_gbr = bearer->qos.gbr.uplink;

    req->bearer_contexts.bearer_level_qos.presence = 1;
    gtp_build_bearer_qos(&req->bearer_contexts.bearer_level_qos,
            &bearer_qos, bearer_qos_buf, GTP_BEARER_QOS_LEN);

    /* Bearer TFT */
    encode_traffic_flow_template(&tft, bearer);
    req->bearer_contexts.tft.presence = 1;
    gtp_build_tft(&req->bearer_contexts.tft,
            &tft, tft_buf, GTP_MAX_TRAFFIC_FLOW_TEMPLATE);

    gtp_message.h.type = type;
    rv = gtp_build_msg(pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

status_t pgw_s5c_build_update_bearer_request(
        pkbuf_t **pkbuf, c_uint8_t type, pgw_bearer_t *bearer,
        int qos_presence, int tft_presence)
{
    status_t rv;
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *linked_bearer = NULL;

    gtp_message_t gtp_message;
    gtp_update_bearer_request_t *req = NULL;

    gtp_bearer_qos_t bearer_qos;
    char bearer_qos_buf[GTP_BEARER_QOS_LEN];
    gtp_tft_t tft;
    char tft_buf[GTP_MAX_TRAFFIC_FLOW_TEMPLATE];

    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");
    linked_bearer = pgw_default_bearer_in_sess(sess);
    d_assert(linked_bearer, return CORE_ERROR, "Null param");

    d_trace(3, "[PGW] Update Bearer Request\n");
    d_trace(5, "    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]\n",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    req = &gtp_message.update_bearer_request;
    memset(&gtp_message, 0, sizeof(gtp_message_t));
 
    /* Bearer EBI */
    req->bearer_contexts.presence = 1;
    req->bearer_contexts.eps_bearer_id.presence = 1;
    req->bearer_contexts.eps_bearer_id.u8 = bearer->ebi;

    /* Bearer QoS */
    if (qos_presence == 1)
    {
        memset(&bearer_qos, 0, sizeof(bearer_qos));
        bearer_qos.qci = bearer->qos.qci;
        bearer_qos.priority_level = bearer->qos.arp.priority_level;
        bearer_qos.pre_emption_capability = 
            bearer->qos.arp.pre_emption_capability;
        bearer_qos.pre_emption_vulnerability =
            bearer->qos.arp.pre_emption_vulnerability;
        bearer_qos.dl_mbr = bearer->qos.mbr.downlink;
        bearer_qos.ul_mbr = bearer->qos.mbr.uplink;
        bearer_qos.dl_gbr = bearer->qos.gbr.downlink;
        bearer_qos.ul_gbr = bearer->qos.gbr.uplink;

        req->bearer_contexts.bearer_level_qos.presence = 1;
        gtp_build_bearer_qos(&req->bearer_contexts.bearer_level_qos,
                &bearer_qos, bearer_qos_buf, GTP_BEARER_QOS_LEN);
    }

    /* Bearer TFT */
    if (tft_presence == 1)
    {
        encode_traffic_flow_template(&tft, bearer);
        req->bearer_contexts.tft.presence = 1;
        gtp_build_tft(&req->bearer_contexts.tft,
                &tft, tft_buf, GTP_MAX_TRAFFIC_FLOW_TEMPLATE);
    }

    gtp_message.h.type = type;
    rv = gtp_build_msg(pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

status_t pgw_s5c_build_delete_bearer_request(
        pkbuf_t **pkbuf, c_uint8_t type, pgw_bearer_t *bearer)
{
    status_t rv;
    pgw_sess_t *sess = NULL;
    pgw_bearer_t *linked_bearer = NULL;

    gtp_message_t gtp_message;
    gtp_delete_bearer_request_t *req = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");
    linked_bearer = pgw_default_bearer_in_sess(sess);
    d_assert(linked_bearer, return CORE_ERROR, "Null param");

    d_trace(3, "[PGW] Delete Bearer Request\n");
    d_trace(5, "    SGW_S5C_TEID[0x%x] PGW_S5C_TEID[0x%x]\n",
            sess->sgw_s5c_teid, sess->pgw_s5c_teid);
    req = &gtp_message.delete_bearer_request;
    memset(&gtp_message, 0, sizeof(gtp_message_t));
 
    if (bearer->ebi == linked_bearer->ebi)
    {
        /* Linked EBI */
        req->linked_eps_bearer_id.presence = 1;
        req->linked_eps_bearer_id.u8 = bearer->ebi;
    }
    else
    {
        /* Bearer EBI */
        req->eps_bearer_ids.presence = 1;
        req->eps_bearer_ids.u8 = bearer->ebi;
    }

    gtp_message.h.type = type;
    rv = gtp_build_msg(pkbuf, &gtp_message);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp build failed");

    return CORE_OK;
}

static c_int16_t pgw_pco_build(c_uint8_t *pco_buf, tlv_pco_t *tlv_pco)
{
    status_t rv;
    pco_t ue, pgw;
    pco_ipcp_t pco_ipcp;
    ipsubnet_t dns_primary, dns_secondary, dns6_primary, dns6_secondary;
    ipsubnet_t p_cscf, p_cscf6;
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
                    if (pgw_self()->dns[0])
                    {
                        rv = core_ipsubnet(
                                &dns_primary, pgw_self()->dns[0], NULL);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                        pco_ipcp.options[0].type = 129; 
                        pco_ipcp.options[0].len = 6; 
                        pco_ipcp.options[0].addr = dns_primary.sub[0];
                    }

                    /* Secondary DNS Server IP Address */
                    if (pgw_self()->dns[1])
                    {
                        rv = core_ipsubnet(
                                &dns_secondary, pgw_self()->dns[1], NULL);
                        d_assert(rv == CORE_OK, return CORE_ERROR,);
                        pco_ipcp.options[1].type = 131; 
                        pco_ipcp.options[1].len = 6; 
                        pco_ipcp.options[1].addr = dns_secondary.sub[0];
                    }

                    pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                    pgw.ids[pgw.num_of_id].len = len;
                    pgw.ids[pgw.num_of_id].data = (c_uint8_t *)&pco_ipcp;

                    pgw.num_of_id++;
                }
                break;
            }
            case PCO_ID_DNS_SERVER_IPV4_ADDRESS_REQUEST:
            {
                if (pgw_self()->dns[0])
                {
                    rv = core_ipsubnet(
                            &dns_primary, pgw_self()->dns[0], NULL);
                    d_assert(rv == CORE_OK, return CORE_ERROR,);
                    pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                    pgw.ids[pgw.num_of_id].len = IPV4_LEN;
                    pgw.ids[pgw.num_of_id].data = dns_primary.sub;
                    pgw.num_of_id++;
                }

                if (pgw_self()->dns[1])
                {
                    rv = core_ipsubnet(
                            &dns_secondary, pgw_self()->dns[1], NULL);
                    d_assert(rv == CORE_OK, return CORE_ERROR,);
                    pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                    pgw.ids[pgw.num_of_id].len = IPV4_LEN;
                    pgw.ids[pgw.num_of_id].data = dns_secondary.sub;
                    pgw.num_of_id++;
                }
                break;
            }
            case PCO_ID_DNS_SERVER_IPV6_ADDRESS_REQUEST:
            {
                if (pgw_self()->dns6[0])
                {
                    rv = core_ipsubnet(
                            &dns6_primary, pgw_self()->dns6[0], NULL);
                    d_assert(rv == CORE_OK, return CORE_ERROR,);
                    pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                    pgw.ids[pgw.num_of_id].len = IPV6_LEN;
                    pgw.ids[pgw.num_of_id].data = dns6_primary.sub;
                    pgw.num_of_id++;
                }

                if (pgw_self()->dns6[1])
                {
                    rv = core_ipsubnet(
                            &dns6_secondary, pgw_self()->dns6[1], NULL);
                    d_assert(rv == CORE_OK, return CORE_ERROR,);
                    pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                    pgw.ids[pgw.num_of_id].len = IPV6_LEN;
                    pgw.ids[pgw.num_of_id].data = dns6_secondary.sub;
                    pgw.num_of_id++;
                }
                break;
            }
            case PCO_ID_P_CSCF_IPV4_ADDRESS_REQUEST:
            {
                if (pgw_self()->num_of_p_cscf)
                {
                    rv = core_ipsubnet(&p_cscf,
                        pgw_self()->p_cscf[pgw_self()->p_cscf_index], NULL);
                    d_assert(rv == CORE_OK, return CORE_ERROR,);
                    pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                    pgw.ids[pgw.num_of_id].len = IPV4_LEN;
                    pgw.ids[pgw.num_of_id].data = p_cscf.sub;
                    pgw.num_of_id++;

                    pgw_self()->p_cscf_index++;
                    pgw_self()->p_cscf_index %= pgw_self()->num_of_p_cscf;
                }
                break;
            }
            case PCO_ID_P_CSCF_IPV6_ADDRESS_REQUEST:
            {
                if (pgw_self()->num_of_p_cscf6)
                {
                    rv = core_ipsubnet(&p_cscf6,
                        pgw_self()->p_cscf6[pgw_self()->p_cscf6_index], NULL);
                    d_assert(rv == CORE_OK, return CORE_ERROR,);
                    pgw.ids[pgw.num_of_id].id = ue.ids[i].id;
                    pgw.ids[pgw.num_of_id].len = IPV6_LEN;
                    pgw.ids[pgw.num_of_id].data = p_cscf6.sub;
                    pgw.num_of_id++;

                    pgw_self()->p_cscf6_index++;
                    pgw_self()->p_cscf6_index %= pgw_self()->num_of_p_cscf6;
                }
                break;
            }
            case PCO_ID_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING:
                /* TODO */
                break;
            case PCO_ID_IPV4_LINK_MTU_REQUEST:
                /* TODO */
                break;
            default:
                d_warn("Unknown PCO ID:(0x%x)", ue.ids[i].id);
        }
    }

    size = pco_build(pco_buf, MAX_PCO_LEN, &pgw);
    return size;
}

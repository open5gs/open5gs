#define TRACE_MODULE _hss_fd_path

#include "core_debug.h"
#include "core_lib.h"
#include "core_sha2.h"
#include "core_network.h"

#include "fd/fd_lib.h"
#include "fd/s6a/s6a_dict.h"
#include "fd/s6a/s6a_message.h"

#include "hss_context.h"
#include "hss_kdf.h"
#include "milenage.h"

/* handler for fallback cb */
static struct disp_hdl *hdl_s6a_fb = NULL; 
/* handler for Authentication-Information-Request cb */
static struct disp_hdl *hdl_s6a_air = NULL; 
/* handler for Update-Location-Request cb */
static struct disp_hdl *hdl_s6a_ulr = NULL; 

/* Default callback for the application. */
static int hss_s6a_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *session, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Authentication-Information-Request messages */
static int hss_s6a_air_cb( struct msg **msg, struct avp *avp, 
        struct session *session, void *opaque, enum disp_action *act)
{
	struct msg *ans, *qry;
    struct avp *avpch;
    struct avp *avp_e_utran_vector, *avp_xres, *avp_kasme, *avp_rand, *avp_autn;
    struct avp_hdr *hdr;
    union avp_value val;

    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];
    c_uint8_t opc[HSS_KEY_LEN];
    c_uint8_t sqn[HSS_SQN_LEN];
    c_uint8_t autn[AUTN_LEN];
    c_uint8_t ik[HSS_KEY_LEN];
    c_uint8_t ck[HSS_KEY_LEN];
    c_uint8_t ak[HSS_AK_LEN];
    c_uint8_t xres[MAX_RES_LEN];
    c_uint8_t kasme[SHA256_DIGEST_SIZE];
    size_t xres_len = 8;

#define MAC_S_LEN 8
    c_uint8_t mac_s[MAC_S_LEN];

    hss_db_auth_info_t auth_info;
    c_uint8_t zero[RAND_LEN];
    status_t rv;
    c_uint32_t result_code = 0;
	
    d_assert(msg, return EINVAL,);
	
	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0) );
    ans = *msg;

    CHECK_FCT( fd_msg_search_avp(qry, fd_user_name, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
    core_cpystrn(imsi_bcd, (char*)hdr->avp_value->os.data, 
        c_min(hdr->avp_value->os.len, MAX_IMSI_BCD_LEN)+1);

    rv = hss_db_auth_info(imsi_bcd, &auth_info);
    if (rv != CORE_OK)
    {
        d_trace(3, "Cannot get Auth-Info for IMSI:'%s'\n", imsi_bcd);
        result_code = S6A_DIAMETER_ERROR_USER_UNKNOWN;
        goto out;
    }

    memset(zero, 0, sizeof(zero));
    if (memcmp(auth_info.rand, zero, RAND_LEN) == 0)
    {
        core_generate_random_bytes(auth_info.rand, RAND_LEN);
    }

    if (auth_info.use_opc)
        memcpy(opc, auth_info.opc, sizeof(opc));
    else
        milenage_opc(auth_info.k, auth_info.op, opc);

    CHECK_FCT( fd_msg_search_avp(qry, s6a_req_eutran_auth_info, &avp) );
    if (avp)
    {
        CHECK_FCT( fd_avp_search_avp(avp, 
                s6a_re_synchronization_info, &avpch) );
        if (avpch)
        {
            CHECK_FCT( fd_msg_avp_hdr(avpch, &hdr) );
            hss_kdf_sqn(opc, auth_info.k, hdr->avp_value->os.data, sqn, mac_s);
            if (memcmp(mac_s, hdr->avp_value->os.data +
                        RAND_LEN + HSS_SQN_LEN, MAC_S_LEN) == 0)
            {
                core_generate_random_bytes(auth_info.rand, RAND_LEN);
                auth_info.sqn = core_buffer_to_uint64(sqn, HSS_SQN_LEN);
                /* 33.102 C.3.4 Guide : IND + 1 */
                auth_info.sqn = (auth_info.sqn + 32 + 1) & HSS_MAX_SQN;
            }
            else
            {
                d_error("Re-synch MAC failed for IMSI:`%s`", imsi_bcd);
                d_print("MAC_S: ");
                d_print_hex(mac_s, MAC_S_LEN);
                d_print_hex(hdr->avp_value->os.data +
                        RAND_LEN + HSS_SQN_LEN, MAC_S_LEN);
                d_print("SQN: ");
                d_print_hex(sqn, HSS_SQN_LEN);
                result_code = S6A_DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE;
                goto out;
            }
        }
    }

    rv = hss_db_update_rand_and_sqn(imsi_bcd, auth_info.rand, auth_info.sqn);
    if (rv != CORE_OK)
    {
        d_error("Cannot update rand and sqn for IMSI:'%s'", imsi_bcd);
        result_code = S6A_DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE;
        goto out;
    }

    rv = hss_db_increment_sqn(imsi_bcd);
    if (rv != CORE_OK)
    {
        d_error("Cannot increment sqn for IMSI:'%s'", imsi_bcd);
        result_code = S6A_DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE;
        goto out;
    }

    CHECK_FCT( fd_msg_search_avp(qry, s6a_visited_plmn_id, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
#if 0  // TODO : check visited_plmn_id
    memcpy(visited_plmn_id, hdr->avp_value->os.data, hdr->avp_value->os.len);
#endif

    milenage_generate(opc, auth_info.amf, auth_info.k,
        core_uint64_to_buffer(auth_info.sqn, HSS_SQN_LEN, sqn), auth_info.rand,
        autn, ik, ck, ak, xres, &xres_len);
    hss_kdf_kasme(ck, ik, hdr->avp_value->os.data, sqn, ak, kasme);

    /* Set the Authentication-Info */
    CHECK_FCT( fd_msg_avp_new(s6a_authentication_info, 0, &avp) );
    CHECK_FCT( fd_msg_avp_new(s6a_e_utran_vector, 0, &avp_e_utran_vector) );

    CHECK_FCT( fd_msg_avp_new(s6a_rand, 0, &avp_rand) );
    val.os.data = auth_info.rand;
    val.os.len = HSS_KEY_LEN;
    CHECK_FCT( fd_msg_avp_setvalue(avp_rand, &val) );
    CHECK_FCT(
        fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_rand) );

    CHECK_FCT( fd_msg_avp_new(s6a_xres, 0, &avp_xres) );
    val.os.data = xres;
    val.os.len = xres_len;
    CHECK_FCT( fd_msg_avp_setvalue(avp_xres, &val) );
    CHECK_FCT(
        fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_xres) );

    CHECK_FCT( fd_msg_avp_new(s6a_autn, 0, &avp_autn) );
    val.os.data = autn;
    val.os.len = AUTN_LEN;
    CHECK_FCT( fd_msg_avp_setvalue(avp_autn, &val) );
    CHECK_FCT(
        fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_autn) );

    CHECK_FCT( fd_msg_avp_new(s6a_kasme, 0, &avp_kasme) );
    val.os.data = kasme;
    val.os.len = SHA256_DIGEST_SIZE;
    CHECK_FCT( fd_msg_avp_setvalue(avp_kasme, &val) );
    CHECK_FCT(
        fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_kasme) );

    CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_e_utran_vector) );
    CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) );

    /* Set the Auth-Session-State AVP */
    CHECK_FCT( fd_msg_avp_new(fd_auth_session_state, 0, &avp) );
    val.i32 = 1;
    CHECK_FCT( fd_msg_avp_setvalue(avp, &val) );
    CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );

    /* Set Vendor-Specific-Application-Id AVP */
    CHECK_FCT_DO( fd_message_vendor_specific_appid_set(
                ans, S6A_APPLICATION_ID), goto out );

	/* Send the answer */
	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );
	
	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
	fd_logger_self()->stats.nb_echoed++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

	return 0;

out:
    CHECK_FCT( fd_message_experimental_rescode_set(ans, result_code) );

    /* Set the Auth-Session-State AVP */
    CHECK_FCT( fd_msg_avp_new(fd_auth_session_state, 0, &avp) );
    val.i32 = 1;
    CHECK_FCT( fd_msg_avp_setvalue(avp, &val) );
    CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );

    /* Set Vendor-Specific-Application-Id AVP */
    CHECK_FCT_DO( fd_message_vendor_specific_appid_set(
                ans, S6A_APPLICATION_ID), goto out );

	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

    return 0;
}

/* Callback for incoming Update-Location-Request messages */
static int hss_s6a_ulr_cb( struct msg **msg, struct avp *avp, 
        struct session *session, void *opaque, enum disp_action *act)
{
	struct msg *ans, *qry;

    struct avp_hdr *hdr;
    union avp_value val;

    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

    status_t rv;
    c_uint32_t result_code = 0;
    s6a_subscription_data_t subscription_data;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;

    d_assert(msg, return EINVAL,);
	
	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0) );
    ans = *msg;

    CHECK_FCT( fd_msg_search_avp(qry, fd_user_name, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
    core_cpystrn(imsi_bcd, (char*)hdr->avp_value->os.data, 
        c_min(hdr->avp_value->os.len, MAX_IMSI_BCD_LEN)+1);

    rv = hss_db_subscription_data(imsi_bcd, &subscription_data);
    if (rv != CORE_OK)
    {
        d_error("Cannot get Subscription-Data for IMSI:'%s'", imsi_bcd);
        result_code = S6A_DIAMETER_ERROR_USER_UNKNOWN;
        goto out;
    }

    CHECK_FCT( fd_msg_search_avp(qry, s6a_visited_plmn_id, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
#if 0  // TODO : check visited_plmn_id
    memcpy(visited_plmn_id, hdr->avp_value->os.data, hdr->avp_value->os.len);
#endif

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) );

    /* Set the Auth-Session-State AVP */
    CHECK_FCT( fd_msg_avp_new(fd_auth_session_state, 0, &avp) );
    val.i32 = 1;
    CHECK_FCT( fd_msg_avp_setvalue(avp, &val) );
    CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );

    /* Set the ULA Flags */
    CHECK_FCT( fd_msg_avp_new(s6a_ula_flags, 0, &avp) );
    val.i32 = S6A_ULA_FLAGS_MME_REGISTERED_FOR_SMS;
    CHECK_FCT( fd_msg_avp_setvalue(avp, &val) );
    CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );

    CHECK_FCT( fd_msg_search_avp(qry, s6a_ulr_flags, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
    if (!(hdr->avp_value->u32 & S6A_ULR_SKIP_SUBSCRIBER_DATA))
    {
        struct avp *avp_access_restriction_data;
        struct avp *avp_subscriber_status, *avp_network_access_mode;
        struct avp *avp_ambr, *avp_max_bandwidth_ul, *avp_max_bandwidth_dl;
        int i;

        /* Set the Subscription Data */
        CHECK_FCT( fd_msg_avp_new(s6a_subscription_data, 0, &avp) );

        if (subscription_data.access_restriction_data)
        {
            CHECK_FCT( fd_msg_avp_new(s6a_access_restriction_data, 0,
                    &avp_access_restriction_data) );
            val.i32 = subscription_data.access_restriction_data;
            CHECK_FCT( fd_msg_avp_setvalue(
                    avp_access_restriction_data, &val) );
            CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                    avp_access_restriction_data) );
        }

        CHECK_FCT( fd_msg_avp_new(s6a_subscriber_status, 0, 
                    &avp_subscriber_status) );
        val.i32 = subscription_data.subscriber_status;
        CHECK_FCT( fd_msg_avp_setvalue(avp_subscriber_status, &val) );
        CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                    avp_subscriber_status) );

        CHECK_FCT( fd_msg_avp_new(s6a_network_access_mode, 0, 
                    &avp_network_access_mode) );
        val.i32 = subscription_data.network_access_mode;
        CHECK_FCT( fd_msg_avp_setvalue(avp_network_access_mode, &val) );
        CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                avp_network_access_mode) );

            /* Set the AMBR */
        CHECK_FCT( fd_msg_avp_new(s6a_ambr, 0, &avp_ambr) );
        CHECK_FCT( fd_msg_avp_new(s6a_max_bandwidth_ul, 0, 
                    &avp_max_bandwidth_ul) );
        val.u32 = subscription_data.ambr.uplink;
        CHECK_FCT( fd_msg_avp_setvalue(avp_max_bandwidth_ul, &val) );
        CHECK_FCT( fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                    avp_max_bandwidth_ul) );
        CHECK_FCT( fd_msg_avp_new(s6a_max_bandwidth_dl, 0, 
                    &avp_max_bandwidth_dl) );
        val.u32 = subscription_data.ambr.downlink;
        CHECK_FCT( fd_msg_avp_setvalue(avp_max_bandwidth_dl, &val) );
        CHECK_FCT( fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                    avp_max_bandwidth_dl) );
        CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_ambr) );

        if (subscription_data.num_of_pdn)
        {
            /* Set the APN Configuration Profile */
            struct avp *apn_configuration_profile;
            struct avp *context_identifier;
            struct avp *all_apn_configuration_included_indicator;

            CHECK_FCT( fd_msg_avp_new(s6a_apn_configuration_profile, 0, 
                    &apn_configuration_profile) );

            CHECK_FCT( fd_msg_avp_new(s6a_context_identifier, 0, 
                    &context_identifier) );
            val.i32 = 1; /* Context Identifier : 1 */
            CHECK_FCT( fd_msg_avp_setvalue(context_identifier, &val) );
            CHECK_FCT( fd_msg_avp_add(apn_configuration_profile, 
                    MSG_BRW_LAST_CHILD, context_identifier) );

            CHECK_FCT( fd_msg_avp_new(
                    s6a_all_apn_configuration_included_indicator, 0, 
                    &all_apn_configuration_included_indicator) );
            val.i32 = 0;
            CHECK_FCT( fd_msg_avp_setvalue(
                    all_apn_configuration_included_indicator, &val) );
            CHECK_FCT( fd_msg_avp_add(apn_configuration_profile, 
                    MSG_BRW_LAST_CHILD, 
                    all_apn_configuration_included_indicator) );

            for (i = 0; i < subscription_data.num_of_pdn; i++)
            {
                /* Set the APN Configuration */
                struct avp *apn_configuration, *context_identifier;
                struct avp *pdn_type, *service_selection;
                struct avp *eps_subscribed_qos_profile, *qos_class_identifier;
                struct avp *allocation_retention_priority, *priority_level;
                struct avp *pre_emption_capability, *pre_emption_vulnerability;
                struct avp *mip6_agent_info, *mip_home_agent_address;

                pdn_t *pdn = &subscription_data.pdn[i];
                d_assert(pdn, goto out,);
                pdn->context_identifier = i+1;

                CHECK_FCT( fd_msg_avp_new(s6a_apn_configuration, 0, 
                    &apn_configuration) );

                /* Set Context-Identifier */
                CHECK_FCT( fd_msg_avp_new(s6a_context_identifier, 0, 
                        &context_identifier) );
                val.i32 = pdn->context_identifier;
                CHECK_FCT( fd_msg_avp_setvalue(context_identifier, &val) );
                CHECK_FCT( fd_msg_avp_add(apn_configuration, 
                        MSG_BRW_LAST_CHILD, context_identifier) );

                /* Set PDN-Type */
                CHECK_FCT( fd_msg_avp_new(s6a_pdn_type, 0, &pdn_type) );
                val.i32 = pdn->pdn_type;
                CHECK_FCT( fd_msg_avp_setvalue(pdn_type, &val) );
                CHECK_FCT( fd_msg_avp_add(apn_configuration, 
                        MSG_BRW_LAST_CHILD, pdn_type) );

                /* Set Service-Selection */
                CHECK_FCT( fd_msg_avp_new(s6a_service_selection, 0, 
                        &service_selection) );
                val.os.data = (c_uint8_t *)pdn->apn;
                val.os.len = strlen(pdn->apn);
                CHECK_FCT( fd_msg_avp_setvalue(service_selection, &val) );
                CHECK_FCT( fd_msg_avp_add(apn_configuration, 
                        MSG_BRW_LAST_CHILD, service_selection) );

                /* Set the EPS Subscribed QoS Profile */
                CHECK_FCT( fd_msg_avp_new(s6a_eps_subscribed_qos_profile, 0, 
                        &eps_subscribed_qos_profile) );

                CHECK_FCT( fd_msg_avp_new(s6a_qos_class_identifier, 0, 
                        &qos_class_identifier) );
                val.i32 = pdn->qos.qci;
                CHECK_FCT( fd_msg_avp_setvalue(qos_class_identifier, &val) );
                CHECK_FCT( fd_msg_avp_add(eps_subscribed_qos_profile, 
                    MSG_BRW_LAST_CHILD, qos_class_identifier) );

                        /* Set Allocation retention priority */
                CHECK_FCT( fd_msg_avp_new(s6a_allocation_retention_priority, 0, 
                        &allocation_retention_priority) );

                CHECK_FCT( fd_msg_avp_new(s6a_priority_level, 0, 
                        &priority_level) );
                val.u32 = pdn->qos.arp.priority_level;
                CHECK_FCT( fd_msg_avp_setvalue(priority_level, &val) );
                CHECK_FCT( fd_msg_avp_add(allocation_retention_priority, 
                    MSG_BRW_LAST_CHILD, priority_level) );

                CHECK_FCT( fd_msg_avp_new(s6a_pre_emption_capability, 0, 
                        &pre_emption_capability) );
                val.u32 = pdn->qos.arp.pre_emption_capability;
                CHECK_FCT( fd_msg_avp_setvalue(pre_emption_capability, &val) );
                CHECK_FCT( fd_msg_avp_add(allocation_retention_priority, 
                    MSG_BRW_LAST_CHILD, pre_emption_capability) );

                CHECK_FCT( fd_msg_avp_new(s6a_pre_emption_vulnerability, 0, 
                        &pre_emption_vulnerability) );
                val.u32 = pdn->qos.arp.pre_emption_vulnerability;
                CHECK_FCT(
                    fd_msg_avp_setvalue(pre_emption_vulnerability, &val) );
                CHECK_FCT( fd_msg_avp_add(allocation_retention_priority, 
                    MSG_BRW_LAST_CHILD, pre_emption_vulnerability) );

                CHECK_FCT( fd_msg_avp_add(eps_subscribed_qos_profile, 
                    MSG_BRW_LAST_CHILD, allocation_retention_priority) );

                CHECK_FCT( fd_msg_avp_add(apn_configuration, 
                    MSG_BRW_LAST_CHILD, eps_subscribed_qos_profile) );

                /* Set MIP6-Agent-Info */
                if (pdn->pgw_ip.ipv4 || pdn->pgw_ip.ipv6)
                {
                    CHECK_FCT( fd_msg_avp_new(fd_mip6_agent_info, 0,
                                &mip6_agent_info) );

                    if (pdn->pgw_ip.ipv4)
                    {
                        CHECK_FCT( fd_msg_avp_new(fd_mip_home_agent_address, 0,
                                    &mip_home_agent_address) );
                        sin.sin_family = AF_INET;
                        sin.sin_addr.s_addr = pdn->pgw_ip.both.addr;
                        CHECK_FCT( fd_msg_avp_value_encode (
                                    &sin, mip_home_agent_address ) );
                        CHECK_FCT( fd_msg_avp_add(mip6_agent_info,
                                MSG_BRW_LAST_CHILD, mip_home_agent_address) );
                    }

                    if (pdn->pgw_ip.ipv6)
                    {
                        CHECK_FCT( fd_msg_avp_new(fd_mip_home_agent_address, 0,
                                    &mip_home_agent_address) );
                        sin6.sin6_family = AF_INET6;
                        memcpy(sin6.sin6_addr.s6_addr, pdn->pgw_ip.both.addr6,
                                sizeof pdn->pgw_ip.both.addr6);
                        CHECK_FCT( fd_msg_avp_value_encode (
                                    &sin6, mip_home_agent_address ) );
                        CHECK_FCT( fd_msg_avp_add(mip6_agent_info,
                                MSG_BRW_LAST_CHILD, mip_home_agent_address) );
                    }

                    CHECK_FCT( fd_msg_avp_add(apn_configuration, 
                            MSG_BRW_LAST_CHILD, mip6_agent_info) );
                }

                /* Set AMBR */
                if (pdn->ambr.downlink || pdn->ambr.uplink)
                {
                    CHECK_FCT( fd_msg_avp_new(s6a_ambr, 0, &avp_ambr) );
                    CHECK_FCT( fd_msg_avp_new(s6a_max_bandwidth_ul, 0, 
                                &avp_max_bandwidth_ul) );
                    val.u32 = pdn->ambr.uplink;
                    CHECK_FCT( fd_msg_avp_setvalue(avp_max_bandwidth_ul, &val) );
                    CHECK_FCT( fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                                avp_max_bandwidth_ul) );
                    CHECK_FCT( fd_msg_avp_new(s6a_max_bandwidth_dl, 0, 
                                &avp_max_bandwidth_dl) );
                    val.u32 = pdn->ambr.downlink;
                    CHECK_FCT( fd_msg_avp_setvalue(avp_max_bandwidth_dl, &val) );
                    CHECK_FCT( fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                                avp_max_bandwidth_dl) );

                    CHECK_FCT( fd_msg_avp_add(apn_configuration, 
                            MSG_BRW_LAST_CHILD, avp_ambr) );
                }

                CHECK_FCT( fd_msg_avp_add(apn_configuration_profile, 
                        MSG_BRW_LAST_CHILD, apn_configuration) );
            }
            CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                    apn_configuration_profile) );
        }

        CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );
    }

    CHECK_FCT( fd_msg_avp_new(s6a_subscribed_rau_tau_timer, 0, &avp) );
    val.i32 = subscription_data.subscribed_rau_tau_timer * 60; /* seconds */
    CHECK_FCT( fd_msg_avp_setvalue(avp, &val) );
    CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );

    /* Set Vendor-Specific-Application-Id AVP */
    CHECK_FCT_DO( fd_message_vendor_specific_appid_set(
                ans, S6A_APPLICATION_ID), goto out );

	/* Send the answer */
	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );
	
	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
	fd_logger_self()->stats.nb_echoed++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

	return 0;

out:
    CHECK_FCT( fd_message_experimental_rescode_set(ans, result_code) );

    /* Set the Auth-Session-State AVP */
    CHECK_FCT( fd_msg_avp_new(fd_auth_session_state, 0, &avp) );
    val.i32 = 1;
    CHECK_FCT( fd_msg_avp_setvalue(avp, &val) );
    CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );

    /* Set Vendor-Specific-Application-Id AVP */
    CHECK_FCT_DO( fd_message_vendor_specific_appid_set(
                ans, S6A_APPLICATION_ID), goto out );

	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

    return 0;
}

int hss_fd_init(void)
{
	struct disp_when data;

    CHECK_FCT( fd_init(FD_MODE_SERVER, hss_self()->fd_conf_path) );

	/* Install objects definitions for this application */
	CHECK_FCT( s6a_dict_init() );

	memset(&data, 0, sizeof(data));
	data.app = s6a_application;
	
	/* fallback CB if command != unexpected message received */
	CHECK_FCT( fd_disp_register(hss_s6a_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_s6a_fb) );
	
	/* specific handler for Authentication-Information-Request */
	data.command = s6a_cmd_air;
	CHECK_FCT( fd_disp_register(hss_s6a_air_cb, DISP_HOW_CC, &data, NULL,
                &hdl_s6a_air) );

	/* specific handler for Location-Update-Request */
	data.command = s6a_cmd_ulr;
	CHECK_FCT( fd_disp_register(hss_s6a_ulr_cb, DISP_HOW_CC, &data, NULL, 
                &hdl_s6a_ulr) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(s6a_application, fd_vendor, 1, 0) );

	return 0;
}

void hss_fd_final(void)
{
	if (hdl_s6a_fb) {
		(void) fd_disp_unregister(&hdl_s6a_fb, NULL);
	}
	if (hdl_s6a_air) {
		(void) fd_disp_unregister(&hdl_s6a_air, NULL);
	}
	if (hdl_s6a_ulr) {
		(void) fd_disp_unregister(&hdl_s6a_ulr, NULL);
	}

    fd_final();
}

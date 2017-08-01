#define TRACE_MODULE _hss_s6a_handler

#include "core_debug.h"
#include "core_lib.h"
#include "core_sha2.h"

#include "s6a_lib.h"

#include "hss_context.h"
#include "hss_kdf.h"
#include "milenage.h"

#define HSS_SQN_LEN 6
#define HSS_AK_LEN 6

/* handler for fallback cb */
static struct disp_hdl *hdl_fb = NULL; 
/* handler for Authentication-Information-Request cb */
static struct disp_hdl *hdl_air = NULL; 
/* handler for Update-Location-Request cb */
static struct disp_hdl *hdl_ulr = NULL; 

/* Default callback for the application. */
static int hss_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Authentication-Information-Request messages */
static int hss_air_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	struct msg *ans, *qry;
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

    hss_db_auth_info_t auth_info;
    c_uint8_t zero[RAND_LEN];
    status_t rv;
	
    d_assert(msg, return EINVAL,);
	
	/* Create answer header */
	qry = *msg;
	fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ans = *msg;

    d_assert(fd_msg_search_avp(qry, s6a_user_name, &avp) == 0 && avp, 
            goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr,,);

    memcpy(imsi_bcd, (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    imsi_bcd[hdr->avp_value->os.len] = 0;

    rv = hss_db_auth_info(imsi_bcd, &auth_info);
    if (rv != CORE_OK)
    {
        d_error("Cannot get Auth-Info for IMSI:'%s'", imsi_bcd);
        goto out;
    }

    memset(zero, 0, sizeof(zero));
    if (memcmp(auth_info.rand, zero, RAND_LEN) == 0)
    {
        core_generate_random_bytes(auth_info.rand, RAND_LEN);
    }

    rv = hss_db_update_rand_and_sqn(imsi_bcd, auth_info.rand, auth_info.sqn);
    if (rv != CORE_OK)
    {
        d_error("Cannot update rand and sqn for IMSI:'%s'", imsi_bcd);
        goto out;
    }

    rv = hss_db_increment_sqn(imsi_bcd);
    if (rv != CORE_OK)
    {
        d_error("Cannot increment sqn for IMSI:'%s'", imsi_bcd);
        goto out;
    }

    d_assert(fd_msg_search_avp(qry, s6a_visited_plmn_id, &avp) == 0 && 
            avp, goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr,,);

    if (hdr && hdr->avp_value && hdr->avp_value->os.data)
    {
#if 0  // TODO : check visited_plmn_id
        memcpy(visited_plmn_id,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
#endif
    }

    milenage_opc(auth_info.k, auth_info.op, opc);
    milenage_generate(opc, auth_info.amf, auth_info.k,
        core_uint64_to_buffer(auth_info.sqn, HSS_SQN_LEN, sqn), auth_info.rand,
        autn, ik, ck, ak, xres, &xres_len);
    hss_kdf_kasme(ck, ik, hdr->avp_value->os.data, sqn, ak, kasme);

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	d_assert(fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) == 0,
            goto out,);

    /* Set the Auth-Session-State AVP */
    d_assert(fd_msg_avp_new(s6a_auth_session_state, 0, &avp) == 0, goto out,);
    val.i32 = 1;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set the Authentication-Info */
    d_assert(fd_msg_avp_new(s6a_authentication_info, 0, &avp) == 0, goto out,);
    d_assert(fd_msg_avp_new(s6a_e_utran_vector, 0, &avp_e_utran_vector) == 0, 
            goto out,);

    d_assert(fd_msg_avp_new(s6a_rand, 0, &avp_rand) == 0, goto out,);
    val.os.data = auth_info.rand;
    val.os.len = HSS_KEY_LEN;
    d_assert(fd_msg_avp_setvalue(avp_rand, &val) == 0, goto out,);
    d_assert(
        fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_rand) == 0, 
        goto out,);

    d_assert(fd_msg_avp_new(s6a_xres, 0, &avp_xres) == 0, goto out,);
    val.os.data = xres;
    val.os.len = xres_len;
    d_assert(fd_msg_avp_setvalue(avp_xres, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_xres) == 0,
        goto out,);

    d_assert(fd_msg_avp_new(s6a_autn, 0, &avp_autn) == 0, goto out,);
    val.os.data = autn;
    val.os.len = AUTN_LEN;
    d_assert(fd_msg_avp_setvalue(avp_autn, &val) == 0, goto out,);
    d_assert(
        fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_autn) == 0,
        goto out,);

    d_assert(fd_msg_avp_new(s6a_kasme, 0, &avp_kasme) == 0, goto out,);
    val.os.data = kasme;
    val.os.len = SHA256_DIGEST_SIZE;
    d_assert(fd_msg_avp_setvalue(avp_kasme, &val) == 0, goto out,);
    d_assert(
        fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_kasme) == 0, 
        goto out,);

    d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_e_utran_vector) == 0, 
            goto out,);
    d_assert(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);
	
	/* Send the answer */
	fd_msg_send(msg, NULL, NULL);
	
	/* Add this value to the stats */
	pthread_mutex_lock(&s6a_config->stats_lock);
	s6a_config->stats.nb_echoed++;
	pthread_mutex_unlock(&s6a_config->stats_lock);

	return 0;

out:
	fd_msg_rescode_set(ans, "DIAMETER_AUTHENTICATION_REJECTED", NULL, NULL, 1);
	fd_msg_send(msg, NULL, NULL);

    return 0;
}

/* Callback for incoming Update-Location-Request messages */
static int hss_ulr_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	struct msg *ans, *qry;

    struct avp_hdr *hdr;
    union avp_value val;

    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];

    status_t rv;
    hss_db_subscription_data_t subscription_data;
	
    d_assert(msg, return EINVAL,);
	
	/* Create answer header */
	qry = *msg;
	fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ans = *msg;

    d_assert(fd_msg_search_avp(qry, s6a_user_name, &avp) == 0 && avp, 
            goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr,,);

    memcpy(imsi_bcd, (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    imsi_bcd[hdr->avp_value->os.len] = 0;

    rv = hss_db_subscription_data(imsi_bcd, &subscription_data);
    if (rv != CORE_OK)
    {
        d_error("Cannot get Subscription-Data for IMSI:'%s'", imsi_bcd);
        goto out;
    }

    d_assert(fd_msg_search_avp(qry, s6a_visited_plmn_id, &avp) == 0 && 
            avp, goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr,,);

    if (hdr && hdr->avp_value && hdr->avp_value->os.data)
    {
#if 0  // TODO : check visited_plmn_id
        memcpy(visited_plmn_id,
                hdr->avp_value->os.data, hdr->avp_value->os.len);
#endif
    }

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	d_assert(fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) == 0,
            goto out,);

    /* Set the Auth-Session-Statee AVP */
    d_assert(fd_msg_avp_new(s6a_auth_session_state, 0, &avp) == 0, goto out,);
    val.i32 = 1;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set the ULA Flags */
    d_assert(fd_msg_avp_new(s6a_ula_flags, 0, &avp) == 0, goto out,);
    val.i32 = S6A_ULA_MME_REGISTERED_FOR_SMS;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    d_assert(fd_msg_search_avp(qry, s6a_ulr_flags, &avp) == 0 && 
            avp, goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr,,);
    if (hdr && hdr->avp_value && 
        !(hdr->avp_value->u32 & S6A_ULR_SKIP_SUBSCRIBER_DATA))
    {
        struct avp *avp_msisdn, *avp_access_restriction_data;
        struct avp *avp_subscriber_status, *avp_network_access_mode;
        struct avp *avp_ambr, *avp_max_bandwidth_ul, *avp_max_bandwidth_dl;
        int i;
        c_uint8_t msisdn[MAX_IMSI_LEN];
        int msisdn_len;

        /* Set the Subscription Data */
        d_assert(fd_msg_avp_new(s6a_subscription_data, 0, &avp) == 0, 
                goto out,);

        d_assert(fd_msg_avp_new(s6a_msisdn, 0, &avp_msisdn) == 0, goto out,);
        core_bcd_to_buffer(imsi_bcd, msisdn, &msisdn_len);
        val.os.data = msisdn;
        val.os.len = msisdn_len;
        d_assert(fd_msg_avp_setvalue(avp_msisdn, &val) == 0, goto out,);
        d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_msisdn) == 0, 
                goto out,);

        if (subscription_data.access_restriction_data)
        {
            d_assert(fd_msg_avp_new(s6a_access_restriction_data, 0, 
                    &avp_access_restriction_data) == 0, goto out,);
            val.i32 = subscription_data.access_restriction_data;
            d_assert(fd_msg_avp_setvalue(
                    avp_access_restriction_data, &val) == 0, goto out,);
            d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                    avp_access_restriction_data) == 0, goto out,);
        }

        d_assert(fd_msg_avp_new(s6a_subscriber_status, 0, 
                    &avp_subscriber_status) == 0, goto out,);
        val.i32 = subscription_data.subscriber_status;
        d_assert(fd_msg_avp_setvalue(avp_subscriber_status, &val) == 0, 
            goto out,);
        d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                    avp_subscriber_status) == 0, goto out,);

        d_assert(fd_msg_avp_new(s6a_network_access_mode, 0, 
                    &avp_network_access_mode) == 0, goto out,);
        val.i32 = subscription_data.network_access_mode;
        d_assert(fd_msg_avp_setvalue(avp_network_access_mode, &val) == 0, 
                goto out,);
        d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                avp_network_access_mode) == 0, goto out,);

            /* Set the AMBR */
        d_assert(fd_msg_avp_new(s6a_ambr, 0, &avp_ambr) == 0, goto out,);
        d_assert(fd_msg_avp_new(s6a_max_bandwidth_ul, 0, 
                    &avp_max_bandwidth_ul) == 0, goto out,);
        val.i32 = subscription_data.max_bandwidth_ul * 1024; /* bits per second */
        d_assert(fd_msg_avp_setvalue(avp_max_bandwidth_ul, &val) == 0, 
                goto out,);
        d_assert(fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                    avp_max_bandwidth_ul) == 0, goto out,);
        d_assert(fd_msg_avp_new(s6a_max_bandwidth_dl, 0, 
                    &avp_max_bandwidth_dl) == 0, goto out,);
        val.i32 = subscription_data.max_bandwidth_dl * 1024; /* bitsper second */
        d_assert(fd_msg_avp_setvalue(avp_max_bandwidth_dl, &val) == 0, 
                goto out,);
        d_assert(fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                    avp_max_bandwidth_dl) == 0, goto out,);
        d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_ambr) == 0, 
                goto out,);

        if (subscription_data.num_of_pdn)
        {
            /* Set the APN Configuration Profile */
            struct avp *apn_configuration_profile;
            struct avp *context_identifier, *all_apn_conf_inc_ind;

            d_assert(fd_msg_avp_new(s6a_apn_configuration_profile, 0, 
                    &apn_configuration_profile) == 0, goto out,);

            d_assert(fd_msg_avp_new(s6a_context_identifier, 0, 
                    &context_identifier) == 0, goto out,);
            val.i32 = 0; /* FIXME : default PDN Context Identifier */
            d_assert(fd_msg_avp_setvalue(context_identifier, &val) == 0, 
                    goto out,);
            d_assert(fd_msg_avp_add(apn_configuration_profile, 
                    MSG_BRW_LAST_CHILD, context_identifier) == 0, goto out,);

            d_assert(fd_msg_avp_new(s6a_all_apn_conf_inc_ind, 0, 
                    &all_apn_conf_inc_ind) == 0, goto out,);
            val.i32 = 0;
            d_assert(fd_msg_avp_setvalue(all_apn_conf_inc_ind, &val) == 0, 
                    goto out,);
            d_assert(fd_msg_avp_add(apn_configuration_profile, 
                    MSG_BRW_LAST_CHILD, all_apn_conf_inc_ind) == 0, goto out,);

            for (i = 0; i < subscription_data.num_of_pdn; i++)
            {
                /* Set the APN Configuration */
                struct avp *apn_configuration, *context_identifier;
                struct avp *pdn_type, *service_selection;
                struct avp *eps_subscribed_qos_profile, *qos_class_identifier;
                struct avp *allocation_retention_priority, *priority_level;
                struct avp *pre_emption_capability, *pre_emption_vulnerability;

                pdn_t *pdn = &subscription_data.pdn[i];
                d_assert(pdn, goto out,);

                d_assert(fd_msg_avp_new(s6a_apn_configuration, 0, 
                    &apn_configuration) == 0, goto out,);

                d_assert(fd_msg_avp_new(s6a_context_identifier, 0, 
                        &context_identifier) == 0, goto out,);
                val.i32 = pdn->id;
                d_assert(fd_msg_avp_setvalue(context_identifier, &val) == 0, 
                        goto out,);
                d_assert(fd_msg_avp_add(apn_configuration, 
                        MSG_BRW_LAST_CHILD, context_identifier) == 0, goto out,);

                d_assert(fd_msg_avp_new(s6a_pdn_type, 0, 
                        &pdn_type) == 0, goto out,);
                val.i32 = pdn->s6a_type;
                d_assert(fd_msg_avp_setvalue(pdn_type, &val) == 0, 
                        goto out,);
                d_assert(fd_msg_avp_add(apn_configuration, 
                        MSG_BRW_LAST_CHILD, pdn_type) == 0, goto out,);

                d_assert(fd_msg_avp_new(s6a_service_selection, 0, 
                        &service_selection) == 0, goto out,);
                val.os.data = (c_uint8_t *)pdn->apn;
                val.os.len = strlen(pdn->apn);
                d_assert(fd_msg_avp_setvalue(service_selection, &val) == 0, 
                        goto out,);
                d_assert(fd_msg_avp_add(apn_configuration, 
                        MSG_BRW_LAST_CHILD, service_selection) == 0, goto out,);

                    /* Set the EPS Subscribed QoS Profile */
                d_assert(fd_msg_avp_new(s6a_eps_subscribed_qos_profile, 0, 
                        &eps_subscribed_qos_profile) == 0, goto out,);

                d_assert(fd_msg_avp_new(s6a_qos_class_identifier, 0, 
                        &qos_class_identifier) == 0, goto out,);
                val.i32 = pdn->qci;
                d_assert(fd_msg_avp_setvalue(qos_class_identifier, &val) == 0, 
                        goto out,);
                d_assert(fd_msg_avp_add(eps_subscribed_qos_profile, 
                    MSG_BRW_LAST_CHILD, qos_class_identifier) == 0, goto out,);

                        /* Set Allocation retention priority */
                d_assert(fd_msg_avp_new(s6a_allocation_retention_priority, 0, 
                        &allocation_retention_priority) == 0, goto out,);

                d_assert(fd_msg_avp_new(s6a_priority_level, 0, 
                        &priority_level) == 0, goto out,);
                val.u32 = pdn->priority_level;
                d_assert(fd_msg_avp_setvalue(priority_level, &val) == 0, 
                        goto out,);
                d_assert(fd_msg_avp_add(allocation_retention_priority, 
                    MSG_BRW_LAST_CHILD, priority_level) == 0, goto out,);

                d_assert(fd_msg_avp_new(s6a_pre_emption_capability, 0, 
                        &pre_emption_capability) == 0, goto out,);
                val.u32 = pdn->pre_emption_capability;
                d_assert(fd_msg_avp_setvalue(pre_emption_capability, &val) == 0, 
                        goto out,);
                d_assert(fd_msg_avp_add(allocation_retention_priority, 
                    MSG_BRW_LAST_CHILD, pre_emption_capability) == 0, 
                        goto out,);

                d_assert(fd_msg_avp_new(s6a_pre_emption_vulnerability, 0, 
                        &pre_emption_vulnerability) == 0, goto out,);
                val.u32 = pdn->pre_emption_vulnerability;
                d_assert(fd_msg_avp_setvalue(pre_emption_vulnerability, &val)
                        == 0, goto out,);
                d_assert(fd_msg_avp_add(allocation_retention_priority, 
                    MSG_BRW_LAST_CHILD, pre_emption_vulnerability) == 0, 
                        goto out,);

                d_assert(fd_msg_avp_add(eps_subscribed_qos_profile, 
                    MSG_BRW_LAST_CHILD, allocation_retention_priority) == 0, 
                        goto out,);

                d_assert(fd_msg_avp_add(apn_configuration, 
                    MSG_BRW_LAST_CHILD, eps_subscribed_qos_profile) == 0, 
                        goto out,);

                /* Set AMBR */
                d_assert(fd_msg_avp_new(s6a_ambr, 0, &avp_ambr) == 0, goto out,);
                d_assert(fd_msg_avp_new(s6a_max_bandwidth_ul, 0, 
                            &avp_max_bandwidth_ul) == 0, goto out,);
                val.i32 = pdn->max_bandwidth_ul * 1024; /* bits per second */
                d_assert(fd_msg_avp_setvalue(avp_max_bandwidth_ul, &val) == 0, 
                        goto out,);
                d_assert(fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                            avp_max_bandwidth_ul) == 0, goto out,);
                d_assert(fd_msg_avp_new(s6a_max_bandwidth_dl, 0, 
                            &avp_max_bandwidth_dl) == 0, goto out,);
                val.i32 = pdn->max_bandwidth_dl * 1024; /* bitsper second */
                d_assert(fd_msg_avp_setvalue(avp_max_bandwidth_dl, &val) == 0, 
                        goto out,);
                d_assert(fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                            avp_max_bandwidth_dl) == 0, goto out,);

                d_assert(fd_msg_avp_add(apn_configuration, 
                        MSG_BRW_LAST_CHILD, avp_ambr) == 0, goto out,);

                d_assert(fd_msg_avp_add(apn_configuration_profile, 
                        MSG_BRW_LAST_CHILD, apn_configuration) == 0, 
                        goto out,);
            }
            d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                    apn_configuration_profile) == 0, goto out,);
        }

        d_assert(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) == 0, 
                goto out,);
    }

    d_assert(fd_msg_avp_new(s6a_subscribed_rau_tau_timer, 0, &avp) == 0, goto out,);
    val.i32 = subscription_data.subscribed_rau_tau_timer * 60; /* seconds */
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

	/* Send the answer */
	fd_msg_send(msg, NULL, NULL);
	
	/* Add this value to the stats */
	pthread_mutex_lock(&s6a_config->stats_lock);
	s6a_config->stats.nb_echoed++;
	pthread_mutex_unlock(&s6a_config->stats_lock);

	return 0;

out:
	fd_msg_rescode_set(ans, "DIAMETER_AUTHENTICATION_REJECTED", NULL, NULL, 1);
	fd_msg_send(msg, NULL, NULL);

    return 0;
}

status_t hss_s6a_init(void)
{
	struct disp_when data;
    int ret;

    if (hss_self()->s6a_config_path == NULL)
    {
        /* This is default diameter configuration if there is no config file 
         * The Configuration : No TLS, Only TCP */
        s6a_config_init();

        s6a_config->cnf_diamid = HSS_IDENTITY;
        s6a_config->cnf_diamrlm = S6A_REALM;
        s6a_config->cnf_addr = hss_self()->hss_s6a_addr;
        s6a_config->cnf_port = hss_self()->hss_s6a_port;
        s6a_config->cnf_port_tls = hss_self()->hss_s6a_tls_port;

        s6a_config->pi_diamid = MME_IDENTITY;
        s6a_config->pi_addr = hss_self()->mme_s6a_addr;
        s6a_config->pic_port = hss_self()->mme_s6a_port;
    }

    ret = s6a_init(hss_self()->s6a_config_path);
    if (ret != 0) return CORE_ERROR;

	memset(&data, 0, sizeof(data));
	data.app = s6a_appli;
	
	/* fallback CB if command != unexpected message received */
	d_assert(fd_disp_register(hss_fb_cb, DISP_HOW_APPID, &data, NULL, 
                &hdl_fb) == 0, return CORE_ERROR,);
	
	/* specific handler for Authentication-Information-Request */
	data.command = s6a_cmd_air;
	d_assert(fd_disp_register(hss_air_cb, DISP_HOW_CC, &data, NULL, 
                &hdl_air) == 0, return CORE_ERROR,);

	/* specific handler for Location-Update-Request */
	data.command = s6a_cmd_ulr;
	d_assert(fd_disp_register(hss_ulr_cb, DISP_HOW_CC, &data, NULL, 
                &hdl_ulr) == 0, return CORE_ERROR,);

	return CORE_OK;
}

void hss_s6a_final(void)
{
	if (hdl_fb) {
		(void) fd_disp_unregister(&hdl_fb, NULL);
	}
	if (hdl_air) {
		(void) fd_disp_unregister(&hdl_air, NULL);
	}
	if (hdl_ulr) {
		(void) fd_disp_unregister(&hdl_ulr, NULL);
	}

    s6a_final();
}

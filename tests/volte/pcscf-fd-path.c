/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "gtp/gtp-xact.h"

#include "fd/fd-lib.h"
#include "fd/rx/rx-dict.h"
#include "fd/rx/rx-message.h"

#include "pcscf-fd-path.h"

#define MAX_NUM_SESSION_STATE 32

static struct session_handler *pcscf_rx_reg = NULL;
static struct disp_hdl *hdl_rx_fb = NULL; 
static struct disp_hdl *hdl_rx_asr = NULL; 
static fd_config_t fd_config;

struct sess_state {
    os0_t       sid;            /* Rx Session-Id */

    struct timespec ts;         /* Time of sending the message */
};

static void pcscf_rx_aaa_cb(void *data, struct msg **msg);
static void pcscf_rx_sta_cb(void *data, struct msg **msg);

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;

    new = ogs_calloc(1, sizeof(*new));
    new->sid = (os0_t)ogs_strdup((char *)sid);
    ogs_assert(new->sid);

    return new;
}

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    if (sess_data->sid)
        ogs_free(sess_data->sid);

    ogs_free(sess_data);
}

static int pcscf_rx_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	ogs_warn("Unexpected message received!");
	
	return ENOTSUP;
}

void pcscf_rx_send_aar(uint8_t **rx_sid, const char *ip,
        int qos_type, int flow_presence)
{
    int rv;
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch1, *avpch2;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;

    paa_t paa;
    ogs_ipsubnet_t ipsub;

    ogs_assert(rx_sid);

    ogs_assert(ip);
    rv = ogs_ipsubnet(&ipsub, ip, NULL);
    ogs_assert(rv == OGS_OK);

    /* Create the request */
    ret = fd_msg_new(rx_cmd_aar, MSGFL_ALLOC_ETEID, &req);
    ogs_assert(ret == 0);
    {
        struct msg_hdr * h;
        ret = fd_msg_hdr( req, &h );
        ogs_assert(ret == 0);
        h->msg_appl = RX_APPLICATION_ID;
    }

    /* Find Diameter Rx Session */
    if (*rx_sid) {
        /* Retrieve session by Session-Id */
        size_t sidlen = strlen((char *)*rx_sid);
		ret = fd_sess_fromsid_msg(*rx_sid, sidlen, &session, &new);
        ogs_assert(ret == 0);
        ogs_assert(new == 0);

        /* Add Session-Id to the message */
        ret = fd_message_session_id_set(req, *rx_sid, sidlen);
        ogs_assert(ret == 0);
        /* Save the session associated with the message */
        ret = fd_msg_sess_set(req, session);
    } else {
        /* Create a new session */
        #define RX_APP_SID_OPT  "app_rx"
        ret = fd_msg_new_session(req, (os0_t)RX_APP_SID_OPT, 
                CONSTSTRLEN(RX_APP_SID_OPT));
        ogs_assert(ret == 0);
        ret = fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL);
        ogs_assert(ret == 0);
    }

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(pcscf_rx_reg, session, &sess_data);
    if (!sess_data) {
        os0_t sid;
        size_t sidlen;

        ret = fd_sess_getsid(session, &sid, &sidlen);
        ogs_assert(ret == 0);

        /* Allocate new session state memory */
        sess_data = new_state(sid);
        ogs_assert(sess_data);

        /* Save Session-Id to PGW Session Context */
        *rx_sid = sess_data->sid;
    }
    
    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
    ogs_assert(ret == 0);
    
    /* Set the Destination-Realm AVP */
    ret = fd_msg_avp_new(fd_destination_realm, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(fd_auth_application_id, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = RX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set Subscription-Id */
    ret = fd_msg_avp_new(rx_subscription_id, 0, &avp);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(rx_subscription_id_type, 0, &avpch1);
    ogs_assert(ret == 0);
    val.i32 = RX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI;
    ret = fd_msg_avp_setvalue (avpch1, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    #define RX_APP_IMSI_BCD  "001010123456789"
    ret = fd_msg_avp_new(rx_subscription_id_data, 0, &avpch1);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)RX_APP_IMSI_BCD;
    val.os.len  = strlen(RX_APP_IMSI_BCD);
    ret = fd_msg_avp_setvalue (avpch1, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    if (ipsub.family == AF_INET) {
        /* Set Framed-IP-Address */
        ret = fd_msg_avp_new(rx_framed_ip_address, 0, &avp);
        ogs_assert(ret == 0);
        val.os.data = (uint8_t*)ipsub.sub;
        val.os.len = IPV4_LEN;
        ret = fd_msg_avp_setvalue(avp, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    } else if (ipsub.family == AF_INET6) {
        /* Set Framed-IPv6-Prefix */
        ret = fd_msg_avp_new(rx_framed_ipv6_prefix, 0, &avp);
        ogs_assert(ret == 0);
        memset(&paa, 0, sizeof(paa_t));

        memcpy(paa.addr6, ipsub.sub, IPV6_LEN);
        paa.pdn_type = 0x03;
#define FRAMED_IPV6_PREFIX_LENGTH 128  /* from spec document */
        paa.len = FRAMED_IPV6_PREFIX_LENGTH; 
        val.os.data = (uint8_t*)&paa;
        val.os.len = PAA_IPV6_LEN;
        ret = fd_msg_avp_setvalue(avp, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    }

    /* Set Media-Component-Description */
    ret = fd_msg_avp_new(rx_media_component_description, 0, &avp);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(rx_media_component_number, 0, &avpch1);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue (avpch1, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(rx_media_type, 0, &avpch1);
    ogs_assert(ret == 0);
    val.i32 = RX_MEDIA_TYPE_AUDIO;
    ret = fd_msg_avp_setvalue (avpch1, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    if (qos_type == 1) {
        ret = fd_msg_avp_new(rx_max_requested_bandwidth_dl, 0, &avpch1);
        ogs_assert(ret == 0);
        val.i32 = 96000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_max_requested_bandwidth_ul, 0, &avpch1);
        ogs_assert(ret == 0);
        val.i32 = 96000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_rr_bandwidth, 0, &avpch1);
        ogs_assert(ret == 0);
        val.i32 = 2400;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_rs_bandwidth, 0, &avpch1);
        ogs_assert(ret == 0);
        val.i32 = 2400;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);
    } else if (qos_type == 2) {
        ret = fd_msg_avp_new(rx_max_requested_bandwidth_dl, 0, &avpch1);
        ogs_assert(ret == 0);
        val.i32 = 96000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_max_requested_bandwidth_ul, 0, &avpch1);
        ogs_assert(ret == 0);
        val.i32 = 96000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_min_requested_bandwidth_dl, 0, &avpch1);
        ogs_assert(ret == 0);
        val.i32 = 88000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_min_requested_bandwidth_ul, 0, &avpch1);
        ogs_assert(ret == 0);
        val.i32 = 88000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);
    }

    if (flow_presence) {
        /* Set Media-Sub-Component #1 */
        ret = fd_msg_avp_new(rx_media_sub_component, 0, &avpch1);

        ret = fd_msg_avp_new(rx_flow_number, 0, &avpch2);
        ogs_assert(ret == 0);
        val.i32 = 1;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_flow_description, 0, &avpch2);
        ogs_assert(ret == 0);
        #define TEST_RX_FLOW_DESC1  \
            "permit out 17 from 172.20.166.84 to 172.18.128.20 20001"
        val.os.data = (uint8_t *)TEST_RX_FLOW_DESC1;
        val.os.len  = strlen(TEST_RX_FLOW_DESC1);
        ret = fd_msg_avp_setvalue (avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_flow_description, 0, &avpch2);
        ogs_assert(ret == 0);
        #define TEST_RX_FLOW_DESC2  \
            "permit in 17 from 172.18.128.20 to 172.20.166.84 20360"
        val.os.data = (uint8_t *)TEST_RX_FLOW_DESC2;
        val.os.len  = strlen(TEST_RX_FLOW_DESC2);
        ret = fd_msg_avp_setvalue (avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);

        /* Set Media-Sub-Component #2 */
        ret = fd_msg_avp_new(rx_media_sub_component, 0, &avpch1);

        ret = fd_msg_avp_new(rx_flow_number, 0, &avpch2);
        ogs_assert(ret == 0);
        val.i32 = 2;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_flow_usage, 0, &avpch2);
        ogs_assert(ret == 0);
        val.i32 = RX_FLOW_USAGE_RTCP;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_flow_description, 0, &avpch2);
        ogs_assert(ret == 0);
        #define TEST_RX_FLOW_DESC3  \
            "permit out 17 from 172.20.166.84 to 172.18.128.20 20002"
        val.os.data = (uint8_t *)TEST_RX_FLOW_DESC3;
        val.os.len  = strlen(TEST_RX_FLOW_DESC3);
        ret = fd_msg_avp_setvalue (avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(rx_flow_description, 0, &avpch2);
        ogs_assert(ret == 0);
        #define TEST_RX_FLOW_DESC4  \
            "permit in 17 from 172.18.128.20 to 172.20.166.84 20361"
        val.os.data = (uint8_t *)TEST_RX_FLOW_DESC4;
        val.os.len  = strlen(TEST_RX_FLOW_DESC4);
        ret = fd_msg_avp_setvalue (avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);
    }

    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);
    ogs_assert(ret == 0);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(pcscf_rx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);
    
    /* Send the request */
    ret = fd_msg_send(&req, pcscf_rx_aaa_cb, svg);
    ogs_assert(ret == 0);

    /* Increment the counter */
    ogs_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0);
    fd_logger_self()->stats.nb_sent++;
    ogs_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0);
}

static void pcscf_rx_aaa_cb(void *data, struct msg **msg)
{
    int ret;

    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    int result_code = 0;

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    ogs_assert(ret == 0);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    ogs_assert(ret == 0);
    ogs_assert(new == 0);
    
    ret = fd_sess_state_retrieve(pcscf_rx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data);
    ogs_assert((void *)sess_data == data);

    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, fd_result_code, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        result_code = hdr->avp_value->i32;
        ogs_debug("Result Code: %d\n", hdr->avp_value->i32);
    } else {
        ret = fd_msg_search_avp(*msg, fd_experimental_result, &avp);
        ogs_assert(ret == 0);
        if (avp) {
            ret = fd_avp_search_avp(avp, fd_experimental_result_code, &avpch1);
            ogs_assert(ret == 0);
            if (avpch1) {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                ogs_assert(ret == 0);
                result_code = hdr->avp_value->i32;
                ogs_debug("Experimental Result Code: %d\n",
                        result_code);
            }
        } else {
            ogs_error("no Result-Code");
            error++;
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, fd_origin_host, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("From '%.*s' ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, fd_origin_realm, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("('%.*s') ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Realm ");
        error++;
    }

    if (result_code != ER_DIAMETER_SUCCESS) {
        ogs_warn("ERROR DIAMETER Result Code(%d)", result_code);
        error++;
        goto out;
    }

out:
    /* Free the message */
    ogs_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0);
    dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    if (fd_logger_self()->stats.nb_recv) {
        /* Ponderate in the avg */
        fd_logger_self()->stats.avg = (fd_logger_self()->stats.avg * 
            fd_logger_self()->stats.nb_recv + dur) /
            (fd_logger_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < fd_logger_self()->stats.shortest)
            fd_logger_self()->stats.shortest = dur;
        if (dur > fd_logger_self()->stats.longest)
            fd_logger_self()->stats.longest = dur;
    } else {
        fd_logger_self()->stats.shortest = dur;
        fd_logger_self()->stats.longest = dur;
        fd_logger_self()->stats.avg = dur;
    }
    if (error)
        fd_logger_self()->stats.nb_errs++;
    else 
        fd_logger_self()->stats.nb_recv++;

    ogs_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0);
    
    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        ogs_debug("in %d.%06ld sec\n", 
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        ogs_debug("in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);

    ret = fd_sess_state_store(pcscf_rx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);
    
    ret = fd_msg_free(*msg);
    ogs_assert(ret == 0);
    *msg = NULL;

    return;
}

static int pcscf_rx_asr_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
    int rv;
    int ret;

	struct msg *ans, *qry;
    struct avp *avpch1, *avpch2, *avpch3;
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;
    os0_t sid;
    size_t sidlen;

    ogs_assert(msg);
    ogs_assert(sess);

    ret = fd_sess_state_retrieve(pcscf_rx_reg, sess, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data);

	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(fd_auth_application_id, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = RX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Auth-Request-Type AVP */
    ret = fd_msg_avp_new(fd_auth_request_type, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Get Abort-Cause */
    ret = fd_msg_search_avp(qry, rx_abort_cause, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
    } else {
        ogs_error("no_Abort-Cause ");
    }

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	ret = fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1);
    ogs_assert(ret == 0);

    /* Store this value in the session */
    sid = sess_data->sid;
    ogs_assert(sid);

    ret = fd_sess_state_store(pcscf_rx_reg, sess, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

	/* Add this value to the stats */
	ogs_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0);
	fd_logger_self()->stats.nb_echoed++;
	ogs_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0);

    pcscf_rx_send_str(sid);

    return 0;
}

void pcscf_rx_send_str(uint8_t *rx_sid)
{
    int rv;
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch1, *avpch2;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;

    ogs_assert(rx_sid);

    /* Create the request */
    ret = fd_msg_new(rx_cmd_str, MSGFL_ALLOC_ETEID, &req);
    ogs_assert(ret == 0);
    {
        struct msg_hdr * h;
        ret = fd_msg_hdr( req, &h );
        ogs_assert(ret == 0);
        h->msg_appl = RX_APPLICATION_ID;
    }

    /* Retrieve session by Session-Id */
    size_t sidlen = strlen((char*)rx_sid);
    ret = fd_sess_fromsid_msg(rx_sid, sidlen, &session, &new);
    ogs_assert(ret == 0);
    ogs_assert(new == 0);

    /* Add Session-Id to the message */
    ret = fd_message_session_id_set(req, rx_sid, sidlen);
    ogs_assert(ret == 0);
    /* Save the session associated with the message */
    ret = fd_msg_sess_set(req, session);

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(pcscf_rx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data);
    
    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
    ogs_assert(ret == 0);
    
    /* Set the Destination-Realm AVP */
    ret = fd_msg_avp_new(fd_destination_realm, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(fd_auth_application_id, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = RX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Termination-Cause AVP */
    ret = fd_msg_avp_new(rx_termination_cause, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = RX_TERMINATION_CAUSE_DIAMETER_LOGOUT;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(pcscf_rx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);
    
    /* Send the request */
    ret = fd_msg_send(&req, pcscf_rx_sta_cb, svg);
    ogs_assert(ret == 0);

    /* Increment the counter */
    ogs_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0);
    fd_logger_self()->stats.nb_sent++;
    ogs_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0);
}

static void pcscf_rx_sta_cb(void *data, struct msg **msg)
{
    int ret;

    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    int result_code = 0;

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    ogs_assert(ret == 0);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    ogs_assert(ret == 0);
    ogs_assert(new == 0);
    
    ret = fd_sess_state_retrieve(pcscf_rx_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data && (void *)sess_data == data);

    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, fd_result_code, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        result_code = hdr->avp_value->i32;
        ogs_debug("Result Code: %d\n", hdr->avp_value->i32);
    } else {
        ret = fd_msg_search_avp(*msg, fd_experimental_result, &avp);
        ogs_assert(ret == 0);
        if (avp) {
            ret = fd_avp_search_avp(avp, fd_experimental_result_code, &avpch1);
            ogs_assert(ret == 0);
            if (avpch1) {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                ogs_assert(ret == 0);
                result_code = hdr->avp_value->i32;
                ogs_debug("Experimental Result Code: %d\n",
                        result_code);
            }
        } else {
            ogs_error("no Result-Code");
            error++;
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, fd_origin_host, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("From '%.*s' ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, fd_origin_realm, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("('%.*s') ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Realm ");
        error++;
    }

    if (result_code != ER_DIAMETER_SUCCESS) {
        ogs_warn("ERROR DIAMETER Result Code(%d)", result_code);
        error++;
        goto out;
    }

out:
    /* Free the message */
    ogs_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0);
    dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    if (fd_logger_self()->stats.nb_recv) {
        /* Ponderate in the avg */
        fd_logger_self()->stats.avg = (fd_logger_self()->stats.avg * 
            fd_logger_self()->stats.nb_recv + dur) /
            (fd_logger_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < fd_logger_self()->stats.shortest)
            fd_logger_self()->stats.shortest = dur;
        if (dur > fd_logger_self()->stats.longest)
            fd_logger_self()->stats.longest = dur;
    } else {
        fd_logger_self()->stats.shortest = dur;
        fd_logger_self()->stats.longest = dur;
        fd_logger_self()->stats.avg = dur;
    }
    if (error)
        fd_logger_self()->stats.nb_errs++;
    else 
        fd_logger_self()->stats.nb_recv++;

    ogs_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0);
    
    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        ogs_trace("in %d.%06ld sec\n", 
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        ogs_trace("in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);

    state_cleanup(sess_data, NULL, NULL);
    
    ret = fd_msg_free(*msg);
    ogs_assert(ret == 0);
    *msg = NULL;

    return;
}

void pcscf_fd_config()
{
    memset(&fd_config, 0, sizeof(fd_config_t));

    fd_config.cnf_diamid = "pcscf.open-ims.test";
    fd_config.cnf_diamrlm = "open-ims.test";
    fd_config.cnf_port = DIAMETER_PORT;
    fd_config.cnf_port_tls = DIAMETER_SECURE_PORT;
    fd_config.cnf_flags.no_sctp = 1;
    fd_config.cnf_addr = "127.0.0.1";

    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dbg_msg_dumps/.libs/dbg_msg_dumps.so";
    fd_config.ext[fd_config.num_of_ext].conf = "0x8888";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_rfc5777/.libs/dict_rfc5777.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_mip6i/.libs/dict_mip6i.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_nasreq/.libs/dict_nasreq.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_nas_mipv6/.libs/dict_nas_mipv6.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_dcca/.libs/dict_dcca.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_dcca_3gpp/.libs/dict_dcca_3gpp.so";
    fd_config.num_of_ext++;

    fd_config.conn[fd_config.num_of_conn].identity = "pcrf.open-ims.test";
    fd_config.conn[fd_config.num_of_conn].addr = "127.0.0.5";
    fd_config.num_of_conn++;
}

int pcscf_fd_init(void)
{
    int ret;
	struct disp_when data;

    pcscf_fd_config();

    ret = fd_init(FD_MODE_CLIENT, NULL, &fd_config);
    ogs_assert(ret == 0);

	/* Install objects definitions for this application */
	ret = rx_dict_init();
    ogs_assert(ret == 0);

    /* Create handler for sessions */
	ret = fd_sess_handler_create(&pcscf_rx_reg, state_cleanup, NULL, NULL);
    ogs_assert(ret == 0);

	/* Fallback CB if command != unexpected message received */
	memset(&data, 0, sizeof(data));
	data.app = rx_application;

	ret = fd_disp_register(pcscf_rx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_rx_fb);
    ogs_assert(ret == 0);
	
	/* Specific handler for Abort-Session-Request */
	data.command = rx_cmd_asr;
	ret = fd_disp_register(pcscf_rx_asr_cb, DISP_HOW_CC, &data, NULL,
                &hdl_rx_asr);
    ogs_assert(ret == 0);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(rx_application, fd_vendor, 1, 0);
    ogs_assert(ret == 0);
	
	return 0;
}

void pcscf_fd_final(void)
{
    int ret;
	ret = fd_sess_handler_destroy(&pcscf_rx_reg, NULL);
    ogs_assert(ret == 0);

	if (hdl_rx_fb)
		(void) fd_disp_unregister(&hdl_rx_fb, NULL);
	if (hdl_rx_asr)
		(void) fd_disp_unregister(&hdl_rx_asr, NULL);

    fd_final();
}

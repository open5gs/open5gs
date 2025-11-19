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

#include "ogs-diameter-s6b.h"

#include "test-common.h"
#include "test-fd-path.h"

struct sess_state {
    os0_t       sid;                /* S6B Session-Id */

    struct timespec ts;             /* Time of sending the message */
};

static OGS_POOL(sess_state_pool, struct sess_state);
static ogs_thread_mutex_t sess_state_mutex;

static struct session_handler *test_s6b_reg = NULL;
static struct disp_hdl *hdl_s6b_fb = NULL;
static struct disp_hdl *hdl_s6b_aar = NULL;
static struct disp_hdl *hdl_s6b_str = NULL;

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;

    ogs_assert(sid);

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_alloc(&sess_state_pool, &new);
    if (!new) {
        ogs_error("ogs_pool_alloc() failed");
        ogs_thread_mutex_unlock(&sess_state_mutex);
        return NULL;
    }
    memset(new, 0, sizeof(*new));

    new->sid = (os0_t)ogs_strdup((char *)sid);
    if (!new->sid) {
        ogs_error("ogs_strdup() failed");
        ogs_pool_free(&sess_state_pool, new);
        ogs_thread_mutex_unlock(&sess_state_mutex);
        return NULL;
    }

    ogs_thread_mutex_unlock(&sess_state_mutex);

    return new;
}

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    if (!sess_data) {
        ogs_error("No session state");
        return;
    }

    if (sess_data->sid)
        ogs_free(sess_data->sid);

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_free(&sess_state_pool, sess_data);
    ogs_thread_mutex_unlock(&sess_state_mutex);
}

static int test_s6b_fb_cb(struct msg **msg, struct avp *avp,
        struct session *sess, void *opaque, enum disp_action *act)
{
    /* This CB should never be called */
    ogs_warn("Unexpected message received!");

    return ENOTSUP;
}

static int test_s6b_aar_cb( struct msg **msg, struct avp *avp,
        struct session *sess, void *opaque, enum disp_action *act)
{
    int rv;
    int ret = 0, i;

    struct msg *ans, *qry;
    struct avp *avpch1, *avpch2;
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;

    uint32_t result_code = OGS_DIAM_MISSING_AVP;

    ogs_assert(msg);

    /* Create answer header */
    qry = *msg;
    ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_application_id, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_S6B_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Auth-Request-Type AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_request_type, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_AUTH_REQUEST_TYPE_AUTHORIZE_ONLY;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Find Session */
    ret = fd_sess_state_retrieve(test_s6b_reg, sess, &sess_data);
    ogs_assert(ret == 0);

    if (!sess_data) {
        os0_t sid;
        size_t sidlen;

        ret = fd_sess_getsid(sess, &sid, &sidlen);
        ogs_assert(ret == 0);

        sess_data = new_state(sid);
        ogs_assert(sess_data);
    }

    /* Get Origin-Host */
    ret = fd_msg_search_avp(qry, ogs_diam_origin_host, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
    } else {
        ogs_error("no_CC-Request-Type ");
        result_code = OGS_DIAM_MISSING_AVP;
        goto out;
    }

    /* Set the Session-Timeout AVP */
    ret = fd_msg_avp_new(ogs_diam_session_timeout, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 7200;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
    ret = fd_msg_rescode_set(ans, (char *)"DIAMETER_SUCCESS", NULL, NULL, 1);
    ogs_assert(ret == 0);

    /* Store this value in the session */
    ret = fd_sess_state_store(test_s6b_reg, sess, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

    /* Send the answer */
    ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    /* Add this value to the stats */
    ogs_assert(pthread_mutex_lock(&ogs_diam_stats_self()->stats_lock) == 0);
    ogs_diam_stats_self()->stats.nb_echoed++;
    ogs_assert(pthread_mutex_unlock(&ogs_diam_stats_self()->stats_lock) ==0);

    return 0;

out:
    /* Set the Result-Code */
    if (result_code == OGS_DIAM_AVP_UNSUPPORTED) {
        ret = fd_msg_rescode_set(ans,
                    (char *)"DIAMETER_AVP_UNSUPPORTED", NULL, NULL, 1);
        ogs_assert(ret == 0);
    } else if (result_code == OGS_DIAM_UNKNOWN_SESSION_ID) {
        ret = fd_msg_rescode_set(ans,
                    (char *)"DIAMETER_UNKNOWN_SESSION_ID", NULL, NULL, 1);
        ogs_assert(ret == 0);
    } else if (result_code == OGS_DIAM_MISSING_AVP) {
        ret = fd_msg_rescode_set(ans,
                    (char *)"DIAMETER_MISSING_AVP", NULL, NULL, 1);
        ogs_assert(ret == 0);
    } else {
        ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
        ogs_assert(ret == 0);
    }

    if (sess_data) {
        /* Store this value in the session */
        ret = fd_sess_state_store(test_s6b_reg, sess, &sess_data);
        ogs_assert(sess_data == NULL);
    }

    ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    return 0;
}

static int test_s6b_str_cb( struct msg **msg, struct avp *avp,
        struct session *sess, void *opaque, enum disp_action *act)
{
    int rv;
    int ret = 0, i;

    struct msg *ans, *qry;
    struct avp *avpch1, *avpch2;
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;

    uint32_t result_code = OGS_DIAM_MISSING_AVP;

    ogs_assert(msg);

    /* Create answer header */
    qry = *msg;
    ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    /* Find Session */
    ret = fd_sess_state_retrieve(test_s6b_reg, sess, &sess_data);
    ogs_assert(ret == 0);

    if (!sess_data) {
        os0_t sid;
        size_t sidlen;

        ret = fd_sess_getsid(sess, &sid, &sidlen);
        ogs_assert(ret == 0);

        sess_data = new_state(sid);
        ogs_assert(sess_data);
    }

    /* Get Origin-Host */
    ret = fd_msg_search_avp(qry, ogs_diam_origin_host, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
    } else {
        ogs_error("no_CC-Request-Type ");
        result_code = OGS_DIAM_MISSING_AVP;
        goto out;
    }

    /* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
    ret = fd_msg_rescode_set(ans, (char *)"DIAMETER_SUCCESS", NULL, NULL, 1);
    ogs_assert(ret == 0);

    /* Store this value in the session */
    ret = fd_sess_state_store(test_s6b_reg, sess, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

    /* Send the answer */
    ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    /* Add this value to the stats */
    ogs_assert(pthread_mutex_lock(&ogs_diam_stats_self()->stats_lock) == 0);
    ogs_diam_stats_self()->stats.nb_echoed++;
    ogs_assert(pthread_mutex_unlock(&ogs_diam_stats_self()->stats_lock) ==0);

    return 0;

out:
    /* Set the Result-Code */
    if (result_code == OGS_DIAM_AVP_UNSUPPORTED) {
        ret = fd_msg_rescode_set(ans,
                    (char *)"DIAMETER_AVP_UNSUPPORTED", NULL, NULL, 1);
        ogs_assert(ret == 0);
    } else if (result_code == OGS_DIAM_UNKNOWN_SESSION_ID) {
        ret = fd_msg_rescode_set(ans,
                    (char *)"DIAMETER_UNKNOWN_SESSION_ID", NULL, NULL, 1);
        ogs_assert(ret == 0);
    } else if (result_code == OGS_DIAM_MISSING_AVP) {
        ret = fd_msg_rescode_set(ans,
                    (char *)"DIAMETER_MISSING_AVP", NULL, NULL, 1);
        ogs_assert(ret == 0);
    } else {
        ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
        ogs_assert(ret == 0);
    }

    if (sess_data) {
        /* Store this value in the session */
        ret = fd_sess_state_store(test_s6b_reg, sess, &sess_data);
        ogs_assert(sess_data == NULL);
    }

    ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    return 0;
}

int test_s6b_init(void)
{
    int ret;
    struct disp_when data;

    ogs_thread_mutex_init(&sess_state_mutex);
    ogs_pool_init(&sess_state_pool, ogs_app()->pool.sess);

    /* Install objects definitions for this application */
    ret = ogs_diam_s6b_init();
    ogs_assert(ret == 0);

    /* Create handler for sessions */
    ret = fd_sess_handler_create(&test_s6b_reg, &state_cleanup, NULL, NULL);
    ogs_assert(ret == 0);

    /* Fallback CB if command != unexpected message received */
    memset(&data, 0, sizeof(data));
    data.app = ogs_diam_s6b_application;

    ret = fd_disp_register(test_s6b_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_s6b_fb);
    ogs_assert(ret == 0);

    data.command = ogs_diam_rx_cmd_aar;
    ret = fd_disp_register(test_s6b_aar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_s6b_aar);
    ogs_assert(ret == 0);

    data.command = ogs_diam_rx_cmd_str;
    ret = fd_disp_register(test_s6b_str_cb, DISP_HOW_CC, &data, NULL,
                &hdl_s6b_str);
    ogs_assert(ret == 0);

    /* Advertise the support for the application in the peer */
    ret = fd_disp_app_support(ogs_diam_s6b_application, ogs_diam_vendor, 1, 0);
    ogs_assert(ret == 0);

    return 0;
}

void test_s6b_final(void)
{
    int ret;

    ret = fd_sess_handler_destroy(&test_s6b_reg, NULL);
    ogs_assert(ret == OGS_OK);

    if (hdl_s6b_fb)
        (void) fd_disp_unregister(&hdl_s6b_fb, NULL);
    if (hdl_s6b_aar)
        (void) fd_disp_unregister(&hdl_s6b_aar, NULL);
    if (hdl_s6b_str)
        (void) fd_disp_unregister(&hdl_s6b_str, NULL);

    ogs_pool_final(&sess_state_pool);
    ogs_thread_mutex_destroy(&sess_state_mutex);
}

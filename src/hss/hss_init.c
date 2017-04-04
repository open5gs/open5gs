#define TRACE_MODULE _hss_init

#include "core_debug.h"
#include "core_lib.h"
#include "core_sha2.h"

#include "s6a_lib.h"

#include "hss_ctx.h"
#include "hss_kdf.h"
#include "milenage.h"

#define HSS_SQN_LEN 6
#define HSS_AK_LEN 6

static struct disp_hdl *hdl_fb = NULL; /* handler for fallback cb */
static struct disp_hdl *hdl_air = NULL; /* handler for Auth-Info-Request cb */

/* Default callback for the application. */
static int hss_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Test-Request messages */
static int hss_air_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	struct msg *ans, *qry;
    struct avp *avpch1, *avpch2;
    struct avp_hdr *hdr;
    union avp_value val;

    user_ctx_t *user = NULL;
    c_uint8_t sqn[HSS_SQN_LEN];
    c_uint8_t autn[AUTN_LEN];
    c_uint8_t ik[HSS_KEY_LEN];
    c_uint8_t ck[HSS_KEY_LEN];
    c_uint8_t ak[HSS_AK_LEN];
    c_uint8_t xres[MAX_RES_LEN];
    c_uint8_t kasme[SHA256_DIGEST_SIZE];
    size_t xres_len = 8;
	
    d_assert(msg, return EINVAL,);
	
	/* Create answer header */
	qry = *msg;
	fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ans = *msg;

    d_assert(fd_msg_search_avp(qry, s6a_user_name, &avp) == 0 && avp, 
            goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr,,);

    user = hss_ctx_user_find_by_imsi(
            hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!user)
    {
        char imsi[MAX_IMSI_LEN];
        strncpy(imsi, (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
        d_warn("Cannot find IMSI:%s\n", imsi);
        goto out;
    }

    d_assert(fd_msg_search_avp(qry, s6a_visited_plmn_id, &avp) == 0 && 
            avp, goto out,);
    d_assert(fd_msg_avp_hdr(avp, &hdr) == 0 && hdr,,);

    milenage_opc(user->k, user->op, user->opc);
    milenage_generate(user->opc, user->amf, user->k, 
        core_uint64_to_buffer(user->sqn, HSS_SQN_LEN, sqn), user->rand, 
        autn, ik, ck, ak, xres, &xres_len);
    hss_kdf_kasme(ck, ik, hdr->avp_value->os.data, sqn, ak, kasme);

    user->sqn = (user->sqn + 32) & 0x7ffffffffff;
	
	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	d_assert(fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) == 0,
            goto out,);

    /* Set the Auth-Session-Statee AVP */
    d_assert(fd_msg_avp_new(s6a_auth_session_state, 0, &avp) == 0, goto out,);
    val.i32 = 1;
    d_assert(fd_msg_avp_setvalue(avp, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) == 0, goto out,);

    /* Set the Authentication-Info */
    d_assert(fd_msg_avp_new(s6a_authentication_info, 0, &avp) == 0, goto out,);
    d_assert(fd_msg_avp_new(s6a_e_utran_vector, 0, &avpch1) == 0, goto out,);

    d_assert(fd_msg_avp_new(s6a_rand, 0, &avpch2) == 0, goto out,);
    val.os.data = user->rand;
    val.os.len = HSS_KEY_LEN;
    d_assert(fd_msg_avp_setvalue(avpch2, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2) == 0, 
            goto out,);

    d_assert(fd_msg_avp_new(s6a_xres, 0, &avpch2) == 0, goto out,);
    val.os.data = xres;
    val.os.len = xres_len;
    d_assert(fd_msg_avp_setvalue(avpch2, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2) == 0,
            goto out,);

    d_assert(fd_msg_avp_new(s6a_autn, 0, &avpch2) == 0, goto out,);
    val.os.data = autn;
    val.os.len = AUTN_LEN;
    d_assert(fd_msg_avp_setvalue(avpch2, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2) == 0,
            goto out,);

    d_assert(fd_msg_avp_new(s6a_kasme, 0, &avpch2) == 0, goto out,);
    val.os.data = kasme;
    val.os.len = SHA256_DIGEST_SIZE;
    d_assert(fd_msg_avp_setvalue(avpch2, &val) == 0, goto out,);
    d_assert(fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2) == 0, 
            goto out,);

    d_assert(fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1) == 0, goto out,);
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

status_t hss_initialize(void)
{
    status_t rv;
    int ret;
	struct disp_when data;

    ret = s6a_init(MODE_HSS);
    if (ret != 0) return CORE_ERROR;

    rv = hss_ctx_init();
    if (rv != CORE_OK) return rv;

	memset(&data, 0, sizeof(data));
	data.app = s6a_appli;
	data.command = s6a_cmd_air;
	
	/* fallback CB if command != unexpected message received */
	d_assert(fd_disp_register(hss_fb_cb, DISP_HOW_APPID, &data, NULL, 
                &hdl_fb) == 0, return CORE_ERROR,);
	
	/* Now specific handler for Authentication-Information-Request */
	d_assert(fd_disp_register(hss_air_cb, DISP_HOW_CC, &data, NULL, 
                &hdl_air) == 0, return CORE_ERROR,);

	return CORE_OK;
}

void hss_terminate(void)
{
	if (hdl_fb) {
		(void) fd_disp_unregister(&hdl_fb, NULL);
	}
	if (hdl_air) {
		(void) fd_disp_unregister(&hdl_air, NULL);
	}

    hss_ctx_final();
    s6a_final();
	
	return;
}

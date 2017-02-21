#ifndef _S6A_MESSAGE_H__
#define _S6A_MESSAGE_H__

#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S6A_SDU_SIZE 2048

typedef struct _s6a_conf_t {
    char *identity;
    char *realm;
} s6a_conf_t;

CORE_DECLARE(status_t) s6a_initialize();

CORE_DECLARE(status_t) s6a_conf_handle(s6a_conf_t *conf);
CORE_DECLARE(void) s6a_conf_show();

CORE_DECLARE(status_t) s6a_thread_start();
CORE_DECLARE(void) s6a_thread_stop();

CORE_DECLARE(status_t) s6a_dict_init();

extern struct dict_object *s6a_vendor;
extern struct dict_object *s6a_app;

extern struct dict_object *s6a_air;
extern struct dict_object *s6a_aia;
extern struct dict_object *s6a_ulr;
extern struct dict_object *s6a_ula;
extern struct dict_object *s6a_pur;
extern struct dict_object *s6a_pua;
extern struct dict_object *s6a_clr;
extern struct dict_object *s6a_cla;

extern struct dict_object *s6a_destination_host;
extern struct dict_object *s6a_destination_realm;
extern struct dict_object *s6a_user_name;
extern struct dict_object *s6a_session_id;
extern struct dict_object *s6a_auth_session_state;
extern struct dict_object *s6a_result_code;
extern struct dict_object *s6a_experimental_result;

extern struct dict_object *s6a_visited_plmn_id;
extern struct dict_object *s6a_rat_type;
extern struct dict_object *s6a_ulr_flags;
extern struct dict_object *s6a_ula_flags;
extern struct dict_object *s6a_subscription_data;
extern struct dict_object *s6a_req_eutran_auth_info;
extern struct dict_object *s6a_number_of_requested_vectors;
extern struct dict_object *s6a_immediate_response_pref;
extern struct dict_object *s6a_authentication_info;
extern struct dict_object *s6a_re_synchronization_info;
extern struct dict_object *s6a_service_selection;
extern struct dict_object *s6a_ue_srvcc_cap;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


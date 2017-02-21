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

CORE_DECLARE(status_t) s6a_dict_init();

CORE_DECLARE(status_t) s6a_thread_start();
CORE_DECLARE(void) s6a_thread_stop();

CORE_DECLARE(status_t) s6a_server_start();
CORE_DECLARE(void) s6a_server_stop();
CORE_DECLARE(status_t) s6a_client_start();
CORE_DECLARE(void) s6a_client_stop();

extern struct dict_object *s6a_vendor;
extern struct dict_object *s6a_appli;

extern struct dict_object *s6a_air;
extern struct dict_object *s6a_aia;
extern struct dict_object *s6a_ulr;
extern struct dict_object *s6a_ula;
extern struct dict_object *s6a_pur;
extern struct dict_object *s6a_pua;
extern struct dict_object *s6a_clr;
extern struct dict_object *s6a_cla;

extern struct dict_object *s6a_origin_host;
extern struct dict_object *s6a_origin_realm;
extern struct dict_object *s6a_destination_host;
extern struct dict_object *s6a_destination_realm;
extern struct dict_object *s6a_user_name;
extern struct dict_object *s6a_session_id;
extern struct dict_object *s6a_auth_session_state;
extern struct dict_object *s6a_result_code;
extern struct dict_object *s6a_experimental_result;
extern struct dict_object *s6a_vendor_id;
extern struct dict_object *s6a_experimental_result_code;

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
extern struct dict_object *s6a_imsi;
extern struct dict_object *s6a_imei;
extern struct dict_object *s6a_software_version;
extern struct dict_object *s6a_supported_features;
extern struct dict_object *s6a_req_e_utran_auth_info;
extern struct dict_object *s6a_req_resync_info;
extern struct dict_object *s6a_req_nb_of_req_vectors;
extern struct dict_object *s6a_req_geran_auth_info;
extern struct dict_object *s6a_immediate_response_pref;
extern struct dict_object *s6a_visited_plmn_id;
extern struct dict_object *s6a_auth_session_state;
extern struct dict_object *s6a_authentication_info;
extern struct dict_object *s6a_e_utran_vector;
extern struct dict_object *s6a_rand;
extern struct dict_object *s6a_xres;
extern struct dict_object *s6a_autn;
extern struct dict_object *s6a_kasme;
extern struct dict_object *s6a_ulr_flags;
extern struct dict_object *s6a_ula_flags;
extern struct dict_object *s6a_pur_flags;
extern struct dict_object *s6a_pua_flags;
extern struct dict_object *s6a_rat_type;
extern struct dict_object *s6a_terminal_info;
extern struct dict_object *s6a_ue_srvcc_cap;
extern struct dict_object *s6a_gmlc_addr;
extern struct dict_object *s6a_subscription_data;
extern struct dict_object *s6a_subscriber_status;
extern struct dict_object *s6a_msisdn;
extern struct dict_object *s6a_ambr;
extern struct dict_object *s6a_network_access_mode;
extern struct dict_object *s6a_access_restriction_data;
extern struct dict_object *s6a_apn_configuration_profile;
extern struct dict_object *s6a_subscribed_rau_tau_timer;
extern struct dict_object *s6a_context_identifier;
extern struct dict_object *s6a_all_apn_conf_inc_ind;
extern struct dict_object *s6a_apn_configuration;
extern struct dict_object *s6a_max_bandwidth_ul;
extern struct dict_object *s6a_max_bandwidth_dl;
extern struct dict_object *s6a_pdn_type;
extern struct dict_object *s6a_service_selection;
extern struct dict_object *s6a_eps_subscribed_qos_profile;
extern struct dict_object *s6a_qos_class_identifier;
extern struct dict_object *s6a_allocation_retention_priority;
extern struct dict_object *s6a_priority_level;
extern struct dict_object *s6a_pre_emption_capability;
extern struct dict_object *s6a_pre_emption_vulnerability;
extern struct dict_object *s6a_served_party_ip_addr;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


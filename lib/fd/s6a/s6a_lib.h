#ifndef __S6A_LIB_H__
#define __S6A_LIB_H__

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/extension.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S6A_RAT_TYPE_WLAN                   0
#define S6A_RAT_TYPE_VIRTUAL                1
#define S6A_RAT_TYPE_UTRAN                  1000
#define S6A_RAT_TYPE_GERAN                  1001
#define S6A_RAT_TYPE_GAN                    1002
#define S6A_RAT_TYPE_HSPA_EVOLUTION         1003
#define S6A_RAT_TYPE_EUTRAN                 1004
#define S6A_RAT_TYPE_EUTRAN_NB_IOT          1005
#define S6A_RAT_TYPE_CDMA2000_1X            2000
#define S6A_RAT_TYPE_HRPD                   2001
#define S6A_RAT_TYPE_UMB                    2002
#define S6A_RAT_TYPE_EHRPD                  2003

#define S6A_ULR_SINGLE_REGISTRATION_IND     (1)
#define S6A_ULR_S6A_S6D_INDICATOR           (1 << 1)
#define S6A_ULR_SKIP_SUBSCRIBER_DATA        (1 << 2)
#define S6A_ULR_GPRS_SUBSCRIPTION_DATA_IND  (1 << 3)
#define S6A_ULR_NODE_TYPE_IND               (1 << 4)
#define S6A_ULR_INITIAL_ATTACH_IND          (1 << 5)
#define S6A_ULR_PS_LCS_SUPPORTED_BY_UE      (1 << 6)

#define S6A_ULA_SEPARATION_INDICATION       (0)
#define S6A_ULA_MME_REGISTERED_FOR_SMS      (1)

#define S6A_UE_SRVCC_NOT_SUPPORTED          (0)
#define S6A_UE_SRVCC_SUPPORTED              (1)

/* Some global variables for dictionary */
extern struct dict_object *s6a_vendor;
extern struct dict_object *s6a_appli;

extern struct dict_object *s6a_cmd_air;
extern struct dict_object *s6a_cmd_aia;
extern struct dict_object *s6a_cmd_ulr;
extern struct dict_object *s6a_cmd_ula;
extern struct dict_object *s6a_cmd_pur;
extern struct dict_object *s6a_cmd_pua;
extern struct dict_object *s6a_cmd_clr;
extern struct dict_object *s6a_cmd_cla;

extern struct dict_object *s6a_origin_host;
extern struct dict_object *s6a_origin_realm;
extern struct dict_object *s6a_destination_host;
extern struct dict_object *s6a_destination_realm;
extern struct dict_object *s6a_user_name;
extern struct dict_object *s6a_auth_session_state;
extern struct dict_object *s6a_result_code;

extern struct dict_object *s6a_visited_plmn_id;
extern struct dict_object *s6a_rat_type;
extern struct dict_object *s6a_ulr_flags;
extern struct dict_object *s6a_ula_flags;
extern struct dict_object *s6a_subscription_data;
extern struct dict_object *s6a_req_eutran_auth_info;
extern struct dict_object *s6a_number_of_requested_vectors;
extern struct dict_object *s6a_immediate_response_preferred;
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
extern struct dict_object *s6a_e_utran_vector;
extern struct dict_object *s6a_rand;
extern struct dict_object *s6a_xres;
extern struct dict_object *s6a_autn;
extern struct dict_object *s6a_kasme;
extern struct dict_object *s6a_pur_flags;
extern struct dict_object *s6a_pua_flags;
extern struct dict_object *s6a_terminal_info;
extern struct dict_object *s6a_gmlc_addr;
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
extern struct dict_object *s6a_eps_subscribed_qos_profile;
extern struct dict_object *s6a_qos_class_identifier;
extern struct dict_object *s6a_allocation_retention_priority;
extern struct dict_object *s6a_priority_level;
extern struct dict_object *s6a_pre_emption_capability;
extern struct dict_object *s6a_pre_emption_vulnerability;
extern struct dict_object *s6a_served_party_ip_addr;

int s6a_dict_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __S6A_LIB_H__ */

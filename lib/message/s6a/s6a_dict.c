#define TRACE_MODULE _s6a_dict

#include "core_debug.h"
#include "core_lib.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"
#include "freeDiameter/extension.h"

#include "s6a_message.h"

#define VENDOR_3GPP (10415)
#define APP_S6A     (16777251)

struct dict_object *s6a_vendor;
struct dict_object *s6a_app;

struct dict_object *s6a_air;
struct dict_object *s6a_aia;
struct dict_object *s6a_ulr;
struct dict_object *s6a_ula;
struct dict_object *s6a_pur;
struct dict_object *s6a_pua;
struct dict_object *s6a_clr;
struct dict_object *s6a_cla;

struct dict_object *s6a_destination_host;
struct dict_object *s6a_destination_realm;
struct dict_object *s6a_user_name;
struct dict_object *s6a_session_id;
struct dict_object *s6a_auth_session_state;
struct dict_object *s6a_result_code;
struct dict_object *s6a_experimental_result;

struct dict_object *s6a_visited_plmn_id;
struct dict_object *s6a_rat_type;
struct dict_object *s6a_ulr_flags;
struct dict_object *s6a_ula_flags;
struct dict_object *s6a_subscription_data;
struct dict_object *s6a_req_eutran_auth_info;
struct dict_object *s6a_number_of_requested_vectors;
struct dict_object *s6a_immediate_response_pref;
struct dict_object *s6a_authentication_info;
struct dict_object *s6a_re_synchronization_info;
struct dict_object *s6a_service_selection;
struct dict_object *s6a_ue_srvcc_cap;

status_t s6a_ext_init()
{
    int ret;

    int fd_ext_dict_nas_mipv6_init(int major, int minor, char *conffile);
    int fd_ext_dict_s6a_init(int major, int minor, char *conffile);

    ret = fd_ext_dict_nas_mipv6_init(
            FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR, NULL);
    if (ret != 0) 
    {
        d_error("fd_ext_init_nas_mipv6() failed");
        return CORE_ERROR;
    } 
    ret = fd_ext_dict_s6a_init(
            FD_PROJECT_VERSION_MAJOR, FD_PROJECT_VERSION_MINOR, NULL);
    if (ret != 0) 
    {
        d_error("fd_ext_init_dict_s6a() failed");
        return CORE_ERROR;
    } 

    return CORE_OK;
}

status_t s6a_dict_init()
{
    int ret;
    vendor_id_t vendor_3gpp = VENDOR_3GPP;
    application_id_t app_s6a = APP_S6A;

    ret = s6a_ext_init();
    if (ret != 0)
    {
        return CORE_ERROR;
    }

  /*
   * Pre-loading vendor object
   */
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_VENDOR, VENDOR_BY_ID, 
        (void *)&vendor_3gpp, &s6a_vendor, ENOENT));
    /*
    * Pre-loading application object
    */
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_APPLICATION, 
        APPLICATION_BY_ID, (void *)&app_s6a, &s6a_app, ENOENT));
    /*
    * Pre-loading commands objects
    */
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Authentication-Information-Request", &s6a_air, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Authentication-Information-Answer", &s6a_aia, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Update-Location-Request", &s6a_ulr, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Update-Location-Answer", &s6a_ula, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Purge-UE-Request", &s6a_pur, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Purge-UE-Answer", &s6a_pua, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Cancel-Location-Request", &s6a_clr, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, 
        "Cancel-Location-Answer", &s6a_cla, ENOENT));
    /*
    * Pre-loading base avps
    */
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Destination-Host", &s6a_destination_host, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Destination-Realm", &s6a_destination_realm, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "User-Name", &s6a_user_name, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Session-Id", &s6a_session_id, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Auth-Session-State", &s6a_auth_session_state, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Result-Code", &s6a_result_code, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Experimental-Result", &s6a_experimental_result, ENOENT));
    /*
    * Pre-loading S6A specifics AVPs
    */
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Visited-PLMN-Id", 
        &s6a_visited_plmn_id, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "RAT-Type", 
        &s6a_rat_type, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "ULR-Flags", 
        &s6a_ulr_flags, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "ULA-Flags", 
        &s6a_ula_flags, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Subscription-Data", 
        &s6a_subscription_data, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Requested-EUTRAN-Authentication-Info", 
        &s6a_req_eutran_auth_info, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Number-Of-Requested-Vectors", 
        &s6a_number_of_requested_vectors, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Immediate-Response-Preferred", 
        &s6a_immediate_response_pref, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Authentication-Info", 
        &s6a_authentication_info, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Re-Synchronization-Info", 
        &s6a_re_synchronization_info, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "Service-Selection", 
        &s6a_service_selection, ENOENT));
    CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, 
        AVP_BY_NAME_ALL_VENDORS, "UE-SRVCC-Capability", 
        &s6a_ue_srvcc_cap, ENOENT));

    return CORE_OK;
}

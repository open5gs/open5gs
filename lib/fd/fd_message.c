#define TRACE_MODULE _fd_message

#include "fd_message.h"

#define FD_3GPP_VENDOR_ID       10415      /* 3GPP Vendor ID */

struct dict_object *fd_origin_host = NULL;
struct dict_object *fd_origin_realm = NULL;
struct dict_object *fd_destination_host = NULL;
struct dict_object *fd_destination_realm = NULL;
struct dict_object *fd_user_name = NULL;
struct dict_object *fd_auth_session_state = NULL;
struct dict_object *fd_result_code = NULL;
struct dict_object *fd_experimental_result = NULL;
struct dict_object *fd_experimental_result_code = NULL;

struct dict_object *fd_vendor = NULL;
struct dict_object *fd_vendor_id = NULL;

int fd_message_init()
{
    struct dict_vendor_data vendor_data = { FD_3GPP_VENDOR_ID, "3GPP" };
    CHECK_FCT( fd_dict_new(fd_g_config->cnf_dict, DICT_VENDOR,
                &vendor_data, NULL, &fd_vendor) );

    CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Vendor-Id", &fd_vendor_id, ENOENT) );

    CHECK_FCT( fd_dict_search (fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Origin-Host", &fd_origin_host, ENOENT) );
    CHECK_FCT( fd_dict_search (fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Origin-Realm", &fd_origin_realm, ENOENT) );
    CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Destination-Host", &fd_destination_host, ENOENT) );
    CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Destination-Realm", &fd_destination_realm, ENOENT) );
    CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "User-Name", &fd_user_name, ENOENT) );
    CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Auth-Session-State", &fd_auth_session_state, ENOENT) );
    CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Result-Code", &fd_result_code, ENOENT) );
    CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Experimental-Result", &fd_experimental_result, ENOENT) );
    CHECK_FCT( fd_dict_search(fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, 
        "Experimental-Result-Code", &fd_experimental_result_code, ENOENT) );

    return 0;
}

int fd_message_experimental_rescode_set(
        struct msg *msg, c_uint32_t result_code)
{
    struct avp *avp;
    struct avp *avp_vendor;
    struct avp *avp_experimental_result_code;
    union avp_value value;

    CHECK_FCT( fd_msg_avp_new(fd_experimental_result, 0, &avp) );

    CHECK_FCT( fd_msg_avp_new(fd_vendor_id, 0, &avp_vendor) );
    value.u32 = FD_3GPP_VENDOR_ID;
    CHECK_FCT( fd_msg_avp_setvalue(avp_vendor, &value) );
    CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_vendor) );

    CHECK_FCT( fd_msg_avp_new(
            fd_experimental_result_code, 0, &avp_experimental_result_code) );
    value.u32 = result_code;
    CHECK_FCT( fd_msg_avp_setvalue(avp_experimental_result_code, &value) );
    CHECK_FCT( fd_msg_avp_add(
            avp, MSG_BRW_LAST_CHILD, avp_experimental_result_code) );

    CHECK_FCT( fd_msg_avp_add(msg, MSG_BRW_LAST_CHILD, avp) );

    CHECK_FCT( fd_msg_add_origin(msg, 0) );

    return 0;
}

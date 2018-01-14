#define TRACE_MODULE _fd_message

#include "fd_message.h"

#define CHECK_dict_search( _type, _criteria, _what, _result )	\
	CHECK_FCT(  fd_dict_search( fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT) );

#define FD_3GPP_VENDOR_ID 10415

struct dict_object *fd_session_id = NULL;
struct dict_object *fd_origin_host = NULL;
struct dict_object *fd_origin_realm = NULL;
struct dict_object *fd_destination_host = NULL;
struct dict_object *fd_destination_realm = NULL;
struct dict_object *fd_user_name = NULL;
struct dict_object *fd_auth_session_state = NULL;
struct dict_object *fd_auth_application_id = NULL;
struct dict_object *fd_auth_request_type = NULL;
struct dict_object *fd_re_auth_request_type = NULL;
struct dict_object *fd_result_code = NULL;
struct dict_object *fd_experimental_result = NULL;
struct dict_object *fd_experimental_result_code = NULL;
struct dict_object *fd_vendor_specific_application_id = NULL;
struct dict_object *fd_mip6_agent_info = NULL;
struct dict_object *fd_mip_home_agent_address = NULL;

struct dict_object *fd_vendor = NULL;
struct dict_object *fd_vendor_id = NULL;

int fd_message_init()
{
    vendor_id_t id = FD_3GPP_VENDOR_ID;

    CHECK_dict_search( DICT_VENDOR, VENDOR_BY_ID, (void *)&id, &fd_vendor);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Vendor-Id", &fd_vendor_id);

    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Session-Id", &fd_session_id);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Origin-Host", &fd_origin_host);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Origin-Realm", &fd_origin_realm);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Destination-Host", &fd_destination_host);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Destination-Realm", &fd_destination_realm);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "User-Name", &fd_user_name);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Auth-Session-State", &fd_auth_session_state);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Auth-Application-Id", &fd_auth_application_id);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Auth-Request-Type", &fd_auth_request_type);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Re-Auth-Request-Type", &fd_re_auth_request_type);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Result-Code", &fd_result_code);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Experimental-Result", &fd_experimental_result);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Experimental-Result-Code", &fd_experimental_result_code);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "Vendor-Specific-Application-Id", &fd_vendor_specific_application_id);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "MIP6-Agent-Info", &fd_mip6_agent_info);
    CHECK_dict_search( DICT_AVP, AVP_BY_NAME, "MIP-Home-Agent-Address", &fd_mip_home_agent_address);

    return 0;
}

int fd_message_session_id_set(
        struct msg *msg, c_uint8_t *sid, size_t sidlen)
{
    struct avp *avp;
    union avp_value val;

	/* Create an AVP to hold it */
	CHECK_FCT( fd_msg_avp_new( fd_session_id, 0, &avp ) );
	
	/* Set its value */
	memset(&val, 0, sizeof(val));
	val.os.data = sid;
	val.os.len  = sidlen;
	CHECK_FCT( fd_msg_avp_setvalue( avp, &val ) );
	
	/* Add it to the message */
	CHECK_FCT( fd_msg_avp_add( msg, MSG_BRW_FIRST_CHILD, avp ) );

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

int fd_message_vendor_specific_appid_set(struct msg *msg, c_uint32_t app_id)
{
    struct avp *avp;
    struct avp *avp_vendor;
    struct avp *avp_vendor_specific_application_id;
    union avp_value value;

    CHECK_FCT( fd_msg_avp_new(fd_vendor_specific_application_id, 0, &avp) );

    CHECK_FCT( fd_msg_avp_new(fd_vendor_id, 0, &avp_vendor) );
    value.u32 = FD_3GPP_VENDOR_ID;
    CHECK_FCT( fd_msg_avp_setvalue(avp_vendor, &value) );
    CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_vendor) );

    CHECK_FCT( fd_msg_avp_new(
            fd_auth_application_id, 0, &avp_vendor_specific_application_id) );
    value.u32 = app_id;
    CHECK_FCT(
            fd_msg_avp_setvalue(avp_vendor_specific_application_id, &value) );
    CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                avp_vendor_specific_application_id) );

    CHECK_FCT( fd_msg_avp_add(msg, MSG_BRW_LAST_CHILD, avp) );

    return 0;
}

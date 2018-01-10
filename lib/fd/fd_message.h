#ifndef __FD_MESSAGE_H__
#define __FD_MESSAGE_H__

#include "core_errno.h"

#include "freeDiameter/freeDiameter-host.h"
#include "freeDiameter/libfdcore.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Result-Code AVP */
#define FD_DIAMETER_AVP_UNSUPPORTED                     5001
#define FD_DIAMETER_UNKNOWN_SESSION_ID                  5002
#define FD_DIAMETER_MISSING_AVP                         5004
#define FD_DIAMETER_INVALID_AVP_VALUE                   5005

extern struct dict_object *fd_session_id;
extern struct dict_object *fd_origin_host;
extern struct dict_object *fd_origin_realm;
extern struct dict_object *fd_destination_host;
extern struct dict_object *fd_destination_realm;
extern struct dict_object *fd_user_name;
extern struct dict_object *fd_auth_session_state;
extern struct dict_object *fd_auth_application_id;
extern struct dict_object *fd_auth_request_type;
extern struct dict_object *fd_result_code;
extern struct dict_object *fd_experimental_result;
extern struct dict_object *fd_experimental_result_code;
extern struct dict_object *fd_vendor_specific_application_id;
extern struct dict_object *fd_mip6_agent_info;
extern struct dict_object *fd_mip_home_agent_address;

extern struct dict_object *fd_vendor;
extern struct dict_object *fd_vendor_id;

CORE_DECLARE(int) fd_message_init(void);
CORE_DECLARE(int) fd_message_session_id_set(
        struct msg *msg, c_uint8_t *sid, size_t sidlen);
CORE_DECLARE(int) fd_message_experimental_rescode_set(
        struct msg *msg, c_uint32_t result_code);
CORE_DECLARE(int) fd_message_vendor_specific_appid_set(
        struct msg *msg, c_uint32_t app_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ! __FD_MESSAGE_H__ */

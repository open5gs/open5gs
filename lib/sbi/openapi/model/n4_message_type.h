/*
 * n4_message_type.h
 *
 *
 */

#ifndef _OpenAPI_n4_message_type_H_
#define _OpenAPI_n4_message_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n4_message_type_NULL = 0, OpenAPI_n4_message_type_PFCP_SES_EST_REQ, OpenAPI_n4_message_type_PFCP_SES_EST_RSP, OpenAPI_n4_message_type_PFCP_SES_MOD_REQ, OpenAPI_n4_message_type_PFCP_SES_MOD_RSP, OpenAPI_n4_message_type_PFCP_SES_DEL_REQ, OpenAPI_n4_message_type_PFCP_SES_DEL_RSP, OpenAPI_n4_message_type_PFCP_SES_REP_REQ, OpenAPI_n4_message_type_PFCP_SES_REP_RSP } OpenAPI_n4_message_type_e;

char* OpenAPI_n4_message_type_ToString(OpenAPI_n4_message_type_e n4_message_type);

OpenAPI_n4_message_type_e OpenAPI_n4_message_type_FromString(char* n4_message_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n4_message_type_H_ */


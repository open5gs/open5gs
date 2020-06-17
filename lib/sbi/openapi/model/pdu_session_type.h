/*
 * pdu_session_type.h
 *
 *
 */

#ifndef _OpenAPI_pdu_session_type_H_
#define _OpenAPI_pdu_session_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pdu_session_type_NULL = 0, OpenAPI_pdu_session_type_IPV4, OpenAPI_pdu_session_type_IPV6, OpenAPI_pdu_session_type_IPV4V6, OpenAPI_pdu_session_type_UNSTRUCTURED, OpenAPI_pdu_session_type_ETHERNET } OpenAPI_pdu_session_type_e;

char* OpenAPI_pdu_session_type_ToString(OpenAPI_pdu_session_type_e pdu_session_type);

OpenAPI_pdu_session_type_e OpenAPI_pdu_session_type_FromString(char* pdu_session_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_type_H_ */


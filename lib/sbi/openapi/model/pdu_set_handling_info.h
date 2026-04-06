/*
 * pdu_set_handling_info.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_set_handling_info_H_
#define _OpenAPI_pdu_set_handling_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pdu_set_handling_info_NULL = 0, OpenAPI_pdu_set_handling_info_ALL_PDUS_NEEDED, OpenAPI_pdu_set_handling_info_ALL_PDUS_NOT_NEEDED } OpenAPI_pdu_set_handling_info_e;

char* OpenAPI_pdu_set_handling_info_ToString(OpenAPI_pdu_set_handling_info_e pdu_set_handling_info);

OpenAPI_pdu_set_handling_info_e OpenAPI_pdu_set_handling_info_FromString(char* pdu_set_handling_info);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_set_handling_info_H_ */


/*
 * rtp_header_ext_type.h
 *
 * 
 */

#ifndef _OpenAPI_rtp_header_ext_type_H_
#define _OpenAPI_rtp_header_ext_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_rtp_header_ext_type_NULL = 0, OpenAPI_rtp_header_ext_type_PDU_SET_MARKING, OpenAPI_rtp_header_ext_type_DYN_CHANGING_TRAFFIC_CHAR, OpenAPI_rtp_header_ext_type_EXPEDITED_TRANSFER_IND } OpenAPI_rtp_header_ext_type_e;

char* OpenAPI_rtp_header_ext_type_ToString(OpenAPI_rtp_header_ext_type_e rtp_header_ext_type);

OpenAPI_rtp_header_ext_type_e OpenAPI_rtp_header_ext_type_FromString(char* rtp_header_ext_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rtp_header_ext_type_H_ */


/*
 * rtp_payload_format.h
 *
 * 
 */

#ifndef _OpenAPI_rtp_payload_format_H_
#define _OpenAPI_rtp_payload_format_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_rtp_payload_format_NULL = 0, OpenAPI_rtp_payload_format_H264, OpenAPI_rtp_payload_format_H265 } OpenAPI_rtp_payload_format_e;

char* OpenAPI_rtp_payload_format_ToString(OpenAPI_rtp_payload_format_e rtp_payload_format);

OpenAPI_rtp_payload_format_e OpenAPI_rtp_payload_format_FromString(char* rtp_payload_format);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rtp_payload_format_H_ */


/*
 * af_sig_protocol.h
 *
 * 
 */

#ifndef _OpenAPI_af_sig_protocol_H_
#define _OpenAPI_af_sig_protocol_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_af_sig_protocol_NULL = 0, OpenAPI_af_sig_protocol_NO_INFORMATION, OpenAPI_af_sig_protocol_SIP } OpenAPI_af_sig_protocol_e;

char* OpenAPI_af_sig_protocol_ToString(OpenAPI_af_sig_protocol_e af_sig_protocol);

OpenAPI_af_sig_protocol_e OpenAPI_af_sig_protocol_FromString(char* af_sig_protocol);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_sig_protocol_H_ */


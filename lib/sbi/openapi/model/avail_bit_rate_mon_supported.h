/*
 * avail_bit_rate_mon_supported.h
 *
 * 
 */

#ifndef _OpenAPI_avail_bit_rate_mon_supported_H_
#define _OpenAPI_avail_bit_rate_mon_supported_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_avail_bit_rate_mon_supported_NULL = 0, OpenAPI_avail_bit_rate_mon_supported_SUPPORTED, OpenAPI_avail_bit_rate_mon_supported_NOT_SUPPORTED, OpenAPI_avail_bit_rate_mon_supported_UNKNOWN } OpenAPI_avail_bit_rate_mon_supported_e;

char* OpenAPI_avail_bit_rate_mon_supported_ToString(OpenAPI_avail_bit_rate_mon_supported_e avail_bit_rate_mon_supported);

OpenAPI_avail_bit_rate_mon_supported_e OpenAPI_avail_bit_rate_mon_supported_FromString(char* avail_bit_rate_mon_supported);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_avail_bit_rate_mon_supported_H_ */


/*
 * radio_resources_indicator.h
 *
 * 
 */

#ifndef _OpenAPI_radio_resources_indicator_H_
#define _OpenAPI_radio_resources_indicator_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_radio_resources_indicator_NULL = 0, OpenAPI_radio_resources_indicator_OPERATOR_PROVIDED, OpenAPI_radio_resources_indicator_CONFIGURED } OpenAPI_radio_resources_indicator_e;

char* OpenAPI_radio_resources_indicator_ToString(OpenAPI_radio_resources_indicator_e radio_resources_indicator);

OpenAPI_radio_resources_indicator_e OpenAPI_radio_resources_indicator_FromString(char* radio_resources_indicator);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_radio_resources_indicator_H_ */


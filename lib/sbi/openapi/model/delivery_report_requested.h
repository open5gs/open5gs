/*
 * delivery_report_requested.h
 *
 * 
 */

#ifndef _OpenAPI_delivery_report_requested_H_
#define _OpenAPI_delivery_report_requested_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_delivery_report_requested_NULL = 0, OpenAPI_delivery_report_requested__true, OpenAPI_delivery_report_requested__false } OpenAPI_delivery_report_requested_e;

char* OpenAPI_delivery_report_requested_ToString(OpenAPI_delivery_report_requested_e delivery_report_requested);

OpenAPI_delivery_report_requested_e OpenAPI_delivery_report_requested_FromString(char* delivery_report_requested);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_delivery_report_requested_H_ */


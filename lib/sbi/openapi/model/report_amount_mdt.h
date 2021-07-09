/*
 * report_amount_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_report_amount_mdt_H_
#define _OpenAPI_report_amount_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_report_amount_mdt_NULL = 0, OpenAPI_report_amount_mdt__1, OpenAPI_report_amount_mdt__2, OpenAPI_report_amount_mdt__4, OpenAPI_report_amount_mdt__8, OpenAPI_report_amount_mdt__16, OpenAPI_report_amount_mdt__32, OpenAPI_report_amount_mdt__64, OpenAPI_report_amount_mdt_infinity } OpenAPI_report_amount_mdt_e;

char* OpenAPI_report_amount_mdt_ToString(OpenAPI_report_amount_mdt_e report_amount_mdt);

OpenAPI_report_amount_mdt_e OpenAPI_report_amount_mdt_FromString(char* report_amount_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_report_amount_mdt_H_ */


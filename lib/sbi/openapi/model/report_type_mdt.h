/*
 * report_type_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_report_type_mdt_H_
#define _OpenAPI_report_type_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_report_type_mdt_NULL = 0, OpenAPI_report_type_mdt_PERIODICAL, OpenAPI_report_type_mdt_EVENT_TRIGGED } OpenAPI_report_type_mdt_e;

char* OpenAPI_report_type_mdt_ToString(OpenAPI_report_type_mdt_e report_type_mdt);

OpenAPI_report_type_mdt_e OpenAPI_report_type_mdt_FromString(char* report_type_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_report_type_mdt_H_ */


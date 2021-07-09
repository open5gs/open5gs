/*
 * report_interval_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_report_interval_mdt_H_
#define _OpenAPI_report_interval_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_report_interval_mdt_NULL = 0, OpenAPI_report_interval_mdt__120, OpenAPI_report_interval_mdt__240, OpenAPI_report_interval_mdt__480, OpenAPI_report_interval_mdt__640, OpenAPI_report_interval_mdt__1024, OpenAPI_report_interval_mdt__2048, OpenAPI_report_interval_mdt__5120, OpenAPI_report_interval_mdt__10240, OpenAPI_report_interval_mdt__60000, OpenAPI_report_interval_mdt__360000, OpenAPI_report_interval_mdt__720000, OpenAPI_report_interval_mdt__1800000, OpenAPI_report_interval_mdt__3600000 } OpenAPI_report_interval_mdt_e;

char* OpenAPI_report_interval_mdt_ToString(OpenAPI_report_interval_mdt_e report_interval_mdt);

OpenAPI_report_interval_mdt_e OpenAPI_report_interval_mdt_FromString(char* report_interval_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_report_interval_mdt_H_ */


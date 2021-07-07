/*
 * job_type.h
 *
 * 
 */

#ifndef _OpenAPI_job_type_H_
#define _OpenAPI_job_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_job_type_NULL = 0, OpenAPI_job_type_IMMEDIATE_MDT_ONLY, OpenAPI_job_type_LOGGED_MDT_ONLY, OpenAPI_job_type_TRACE_ONLY, OpenAPI_job_type_IMMEDIATE_MDT_AND_TRACE, OpenAPI_job_type_RLF_REPORTS_ONLY, OpenAPI_job_type_RCEF_REPORTS_ONLY, OpenAPI_job_type_LOGGED_MBSFN_MDT } OpenAPI_job_type_e;

char* OpenAPI_job_type_ToString(OpenAPI_job_type_e job_type);

OpenAPI_job_type_e OpenAPI_job_type_FromString(char* job_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_job_type_H_ */


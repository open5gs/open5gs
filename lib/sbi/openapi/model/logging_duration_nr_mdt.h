/*
 * logging_duration_nr_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_logging_duration_nr_mdt_H_
#define _OpenAPI_logging_duration_nr_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_logging_duration_nr_mdt_NULL = 0, OpenAPI_logging_duration_nr_mdt__600, OpenAPI_logging_duration_nr_mdt__1200, OpenAPI_logging_duration_nr_mdt__2400, OpenAPI_logging_duration_nr_mdt__3600, OpenAPI_logging_duration_nr_mdt__5400, OpenAPI_logging_duration_nr_mdt__7200 } OpenAPI_logging_duration_nr_mdt_e;

char* OpenAPI_logging_duration_nr_mdt_ToString(OpenAPI_logging_duration_nr_mdt_e logging_duration_nr_mdt);

OpenAPI_logging_duration_nr_mdt_e OpenAPI_logging_duration_nr_mdt_FromString(char* logging_duration_nr_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_logging_duration_nr_mdt_H_ */


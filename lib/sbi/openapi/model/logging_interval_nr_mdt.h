/*
 * logging_interval_nr_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_logging_interval_nr_mdt_H_
#define _OpenAPI_logging_interval_nr_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_logging_interval_nr_mdt_NULL = 0, OpenAPI_logging_interval_nr_mdt__128, OpenAPI_logging_interval_nr_mdt__256, OpenAPI_logging_interval_nr_mdt__512, OpenAPI_logging_interval_nr_mdt__1024, OpenAPI_logging_interval_nr_mdt__2048, OpenAPI_logging_interval_nr_mdt__3072, OpenAPI_logging_interval_nr_mdt__4096, OpenAPI_logging_interval_nr_mdt__6144, OpenAPI_logging_interval_nr_mdt__320, OpenAPI_logging_interval_nr_mdt__640, OpenAPI_logging_interval_nr_mdt_infinity } OpenAPI_logging_interval_nr_mdt_e;

char* OpenAPI_logging_interval_nr_mdt_ToString(OpenAPI_logging_interval_nr_mdt_e logging_interval_nr_mdt);

OpenAPI_logging_interval_nr_mdt_e OpenAPI_logging_interval_nr_mdt_FromString(char* logging_interval_nr_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_logging_interval_nr_mdt_H_ */


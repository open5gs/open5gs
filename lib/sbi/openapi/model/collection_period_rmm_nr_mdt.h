/*
 * collection_period_rmm_nr_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_collection_period_rmm_nr_mdt_H_
#define _OpenAPI_collection_period_rmm_nr_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_collection_period_rmm_nr_mdt_NULL = 0, OpenAPI_collection_period_rmm_nr_mdt__1024, OpenAPI_collection_period_rmm_nr_mdt__2048, OpenAPI_collection_period_rmm_nr_mdt__5120, OpenAPI_collection_period_rmm_nr_mdt__10240, OpenAPI_collection_period_rmm_nr_mdt__60000 } OpenAPI_collection_period_rmm_nr_mdt_e;

char* OpenAPI_collection_period_rmm_nr_mdt_ToString(OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nr_mdt);

OpenAPI_collection_period_rmm_nr_mdt_e OpenAPI_collection_period_rmm_nr_mdt_FromString(char* collection_period_rmm_nr_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_collection_period_rmm_nr_mdt_H_ */


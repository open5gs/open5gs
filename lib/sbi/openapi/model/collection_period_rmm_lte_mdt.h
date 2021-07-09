/*
 * collection_period_rmm_lte_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_collection_period_rmm_lte_mdt_H_
#define _OpenAPI_collection_period_rmm_lte_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_collection_period_rmm_lte_mdt_NULL = 0, OpenAPI_collection_period_rmm_lte_mdt__1024, OpenAPI_collection_period_rmm_lte_mdt__1280, OpenAPI_collection_period_rmm_lte_mdt__2048, OpenAPI_collection_period_rmm_lte_mdt__2560, OpenAPI_collection_period_rmm_lte_mdt__5120, OpenAPI_collection_period_rmm_lte_mdt__10240, OpenAPI_collection_period_rmm_lte_mdt__60000 } OpenAPI_collection_period_rmm_lte_mdt_e;

char* OpenAPI_collection_period_rmm_lte_mdt_ToString(OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lte_mdt);

OpenAPI_collection_period_rmm_lte_mdt_e OpenAPI_collection_period_rmm_lte_mdt_FromString(char* collection_period_rmm_lte_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_collection_period_rmm_lte_mdt_H_ */


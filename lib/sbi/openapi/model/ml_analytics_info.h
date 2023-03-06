/*
 * ml_analytics_info.h
 *
 * ML Analytics Filter information supported by the Nnwdaf_MLModelProvision service
 */

#ifndef _OpenAPI_ml_analytics_info_H_
#define _OpenAPI_ml_analytics_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nwdaf_event.h"
#include "snssai.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ml_analytics_info_s OpenAPI_ml_analytics_info_t;
typedef struct OpenAPI_ml_analytics_info_s {
    OpenAPI_list_t *ml_analytics_ids;
    OpenAPI_list_t *snssai_list;
    OpenAPI_list_t *tracking_area_list;
} OpenAPI_ml_analytics_info_t;

OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_create(
    OpenAPI_list_t *ml_analytics_ids,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *tracking_area_list
);
void OpenAPI_ml_analytics_info_free(OpenAPI_ml_analytics_info_t *ml_analytics_info);
OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_parseFromJSON(cJSON *ml_analytics_infoJSON);
cJSON *OpenAPI_ml_analytics_info_convertToJSON(OpenAPI_ml_analytics_info_t *ml_analytics_info);
OpenAPI_ml_analytics_info_t *OpenAPI_ml_analytics_info_copy(OpenAPI_ml_analytics_info_t *dst, OpenAPI_ml_analytics_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ml_analytics_info_H_ */


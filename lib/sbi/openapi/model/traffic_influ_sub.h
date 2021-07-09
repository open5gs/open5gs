/*
 * traffic_influ_sub.h
 *
 * 
 */

#ifndef _OpenAPI_traffic_influ_sub_H_
#define _OpenAPI_traffic_influ_sub_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_influ_sub_s OpenAPI_traffic_influ_sub_t;
typedef struct OpenAPI_traffic_influ_sub_s {
    OpenAPI_list_t *dnns;
    OpenAPI_list_t *snssais;
    OpenAPI_list_t *internal_group_ids;
    OpenAPI_list_t *supis;
    char *notification_uri;
    char *expiry;
    char *supported_features;
} OpenAPI_traffic_influ_sub_t;

OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_create(
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *snssais,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t *supis,
    char *notification_uri,
    char *expiry,
    char *supported_features
);
void OpenAPI_traffic_influ_sub_free(OpenAPI_traffic_influ_sub_t *traffic_influ_sub);
OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_parseFromJSON(cJSON *traffic_influ_subJSON);
cJSON *OpenAPI_traffic_influ_sub_convertToJSON(OpenAPI_traffic_influ_sub_t *traffic_influ_sub);
OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_copy(OpenAPI_traffic_influ_sub_t *dst, OpenAPI_traffic_influ_sub_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_influ_sub_H_ */


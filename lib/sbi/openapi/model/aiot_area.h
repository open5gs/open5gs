/*
 * aiot_area.h
 *
 * Contains a list of AIoT Area ID(s).
 */

#ifndef _OpenAPI_aiot_area_H_
#define _OpenAPI_aiot_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_aiot_area_s OpenAPI_aiot_area_t;
#include "aiot_area_id.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_aiot_area_s {
    OpenAPI_list_t *area_ids;
};

OpenAPI_aiot_area_t *OpenAPI_aiot_area_create(
    OpenAPI_list_t *area_ids
);
void OpenAPI_aiot_area_free(OpenAPI_aiot_area_t *aiot_area);
OpenAPI_aiot_area_t *OpenAPI_aiot_area_parseFromJSON(cJSON *aiot_areaJSON);
cJSON *OpenAPI_aiot_area_convertToJSON(OpenAPI_aiot_area_t *aiot_area);
OpenAPI_aiot_area_t *OpenAPI_aiot_area_copy(OpenAPI_aiot_area_t *dst, OpenAPI_aiot_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_aiot_area_H_ */


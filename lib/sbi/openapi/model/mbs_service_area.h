/*
 * mbs_service_area.h
 *
 * MBS Service Area
 */

#ifndef _OpenAPI_mbs_service_area_H_
#define _OpenAPI_mbs_service_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ncgi_tai.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mbs_service_area_s OpenAPI_mbs_service_area_t;
typedef struct OpenAPI_mbs_service_area_s {
    OpenAPI_list_t *ncgi_list;
    OpenAPI_list_t *tai_list;
} OpenAPI_mbs_service_area_t;

OpenAPI_mbs_service_area_t *OpenAPI_mbs_service_area_create(
    OpenAPI_list_t *ncgi_list,
    OpenAPI_list_t *tai_list
);
void OpenAPI_mbs_service_area_free(OpenAPI_mbs_service_area_t *mbs_service_area);
OpenAPI_mbs_service_area_t *OpenAPI_mbs_service_area_parseFromJSON(cJSON *mbs_service_areaJSON);
cJSON *OpenAPI_mbs_service_area_convertToJSON(OpenAPI_mbs_service_area_t *mbs_service_area);
OpenAPI_mbs_service_area_t *OpenAPI_mbs_service_area_copy(OpenAPI_mbs_service_area_t *dst, OpenAPI_mbs_service_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_service_area_H_ */


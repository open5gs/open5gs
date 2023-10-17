/*
 * mbs_service_area_info.h
 *
 * MBS Service Area Information for location dependent MBS session
 */

#ifndef _OpenAPI_mbs_service_area_info_H_
#define _OpenAPI_mbs_service_area_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mbs_service_area.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mbs_service_area_info_s OpenAPI_mbs_service_area_info_t;
typedef struct OpenAPI_mbs_service_area_info_s {
    int area_session_id;
    struct OpenAPI_mbs_service_area_s *mbs_service_area;
} OpenAPI_mbs_service_area_info_t;

OpenAPI_mbs_service_area_info_t *OpenAPI_mbs_service_area_info_create(
    int area_session_id,
    OpenAPI_mbs_service_area_t *mbs_service_area
);
void OpenAPI_mbs_service_area_info_free(OpenAPI_mbs_service_area_info_t *mbs_service_area_info);
OpenAPI_mbs_service_area_info_t *OpenAPI_mbs_service_area_info_parseFromJSON(cJSON *mbs_service_area_infoJSON);
cJSON *OpenAPI_mbs_service_area_info_convertToJSON(OpenAPI_mbs_service_area_info_t *mbs_service_area_info);
OpenAPI_mbs_service_area_info_t *OpenAPI_mbs_service_area_info_copy(OpenAPI_mbs_service_area_info_t *dst, OpenAPI_mbs_service_area_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_service_area_info_H_ */


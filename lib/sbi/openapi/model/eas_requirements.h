/*
 * eas_requirements.h
 *
 * 
 */

#ifndef _OpenAPI_eas_requirements_H_
#define _OpenAPI_eas_requirements_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_eas_requirements_s OpenAPI_eas_requirements_t;
#include "affinity_anti_affinity.h"
#include "serving_location.h"
#include "software_image_info.h"
#include "virtual_resource.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_eas_requirements_s {
    struct OpenAPI_serving_location_s *required_ea_sserving_location;
    struct OpenAPI_software_image_info_s *software_image_info;
    struct OpenAPI_affinity_anti_affinity_s *affinity_anti_affinity;
    bool is_service_continuity;
    int service_continuity;
    struct OpenAPI_virtual_resource_s *virtual_resource;
};

OpenAPI_eas_requirements_t *OpenAPI_eas_requirements_create(
    OpenAPI_serving_location_t *required_ea_sserving_location,
    OpenAPI_software_image_info_t *software_image_info,
    OpenAPI_affinity_anti_affinity_t *affinity_anti_affinity,
    bool is_service_continuity,
    int service_continuity,
    OpenAPI_virtual_resource_t *virtual_resource
);
void OpenAPI_eas_requirements_free(OpenAPI_eas_requirements_t *eas_requirements);
OpenAPI_eas_requirements_t *OpenAPI_eas_requirements_parseFromJSON(cJSON *eas_requirementsJSON);
cJSON *OpenAPI_eas_requirements_convertToJSON(OpenAPI_eas_requirements_t *eas_requirements);
OpenAPI_eas_requirements_t *OpenAPI_eas_requirements_copy(OpenAPI_eas_requirements_t *dst, OpenAPI_eas_requirements_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eas_requirements_H_ */


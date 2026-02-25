/*
 * ue_location_service_ind.h
 *
 * Specifies location service types requested by UE.
 */

#ifndef _OpenAPI_ue_location_service_ind_H_
#define _OpenAPI_ue_location_service_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ue_location_service_ind_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_location_service_ind_s OpenAPI_ue_location_service_ind_t;
typedef struct OpenAPI_ue_location_service_ind_s {
} OpenAPI_ue_location_service_ind_t;

OpenAPI_ue_location_service_ind_t *OpenAPI_ue_location_service_ind_create(void);
void OpenAPI_ue_location_service_ind_free(OpenAPI_ue_location_service_ind_t *ue_location_service_ind);
OpenAPI_ue_location_service_ind_t *OpenAPI_ue_location_service_ind_parseFromJSON(cJSON *ue_location_service_indJSON);
cJSON *OpenAPI_ue_location_service_ind_convertToJSON(OpenAPI_ue_location_service_ind_t *ue_location_service_ind);
OpenAPI_ue_location_service_ind_t *OpenAPI_ue_location_service_ind_copy(OpenAPI_ue_location_service_ind_t *dst, OpenAPI_ue_location_service_ind_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_location_service_ind_H_ */


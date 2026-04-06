/*
 * dnn_ladn_service_areas.h
 *
 * Contains a list of DNN LADN Service Areas
 */

#ifndef _OpenAPI_dnn_ladn_service_areas_H_
#define _OpenAPI_dnn_ladn_service_areas_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dnn_ladn_service_areas_s OpenAPI_dnn_ladn_service_areas_t;
#include "dnn_ladn_service_area.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dnn_ladn_service_areas_s {
    OpenAPI_list_t *dnn_ladn_service_areas;
};

OpenAPI_dnn_ladn_service_areas_t *OpenAPI_dnn_ladn_service_areas_create(
    OpenAPI_list_t *dnn_ladn_service_areas
);
void OpenAPI_dnn_ladn_service_areas_free(OpenAPI_dnn_ladn_service_areas_t *dnn_ladn_service_areas);
OpenAPI_dnn_ladn_service_areas_t *OpenAPI_dnn_ladn_service_areas_parseFromJSON(cJSON *dnn_ladn_service_areasJSON);
cJSON *OpenAPI_dnn_ladn_service_areas_convertToJSON(OpenAPI_dnn_ladn_service_areas_t *dnn_ladn_service_areas);
OpenAPI_dnn_ladn_service_areas_t *OpenAPI_dnn_ladn_service_areas_copy(OpenAPI_dnn_ladn_service_areas_t *dst, OpenAPI_dnn_ladn_service_areas_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_ladn_service_areas_H_ */


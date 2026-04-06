/*
 * dnn_ladn_service_area.h
 *
 * Contains DNN and LADN Service Area
 */

#ifndef _OpenAPI_dnn_ladn_service_area_H_
#define _OpenAPI_dnn_ladn_service_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_dnn_ladn_service_area_s OpenAPI_dnn_ladn_service_area_t;
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_dnn_ladn_service_area_s {
    char *dnn;
    OpenAPI_list_t *ladn_service_area;
};

OpenAPI_dnn_ladn_service_area_t *OpenAPI_dnn_ladn_service_area_create(
    char *dnn,
    OpenAPI_list_t *ladn_service_area
);
void OpenAPI_dnn_ladn_service_area_free(OpenAPI_dnn_ladn_service_area_t *dnn_ladn_service_area);
OpenAPI_dnn_ladn_service_area_t *OpenAPI_dnn_ladn_service_area_parseFromJSON(cJSON *dnn_ladn_service_areaJSON);
cJSON *OpenAPI_dnn_ladn_service_area_convertToJSON(OpenAPI_dnn_ladn_service_area_t *dnn_ladn_service_area);
OpenAPI_dnn_ladn_service_area_t *OpenAPI_dnn_ladn_service_area_copy(OpenAPI_dnn_ladn_service_area_t *dst, OpenAPI_dnn_ladn_service_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_ladn_service_area_H_ */


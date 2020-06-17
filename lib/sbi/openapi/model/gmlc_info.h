/*
 * gmlc_info.h
 *
 *
 */

#ifndef _OpenAPI_gmlc_info_H_
#define _OpenAPI_gmlc_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "external_client_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_gmlc_info_s OpenAPI_gmlc_info_t;
typedef struct OpenAPI_gmlc_info_s {
    OpenAPI_list_t *serving_client_types;
} OpenAPI_gmlc_info_t;

OpenAPI_gmlc_info_t *OpenAPI_gmlc_info_create(
    OpenAPI_list_t *serving_client_types
    );
void OpenAPI_gmlc_info_free(OpenAPI_gmlc_info_t *gmlc_info);
OpenAPI_gmlc_info_t *OpenAPI_gmlc_info_parseFromJSON(cJSON *gmlc_infoJSON);
cJSON *OpenAPI_gmlc_info_convertToJSON(OpenAPI_gmlc_info_t *gmlc_info);
OpenAPI_gmlc_info_t *OpenAPI_gmlc_info_copy(OpenAPI_gmlc_info_t *dst, OpenAPI_gmlc_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gmlc_info_H_ */


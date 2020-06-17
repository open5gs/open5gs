/*
 * service_id_to_layer2_id.h
 *
 *
 */

#ifndef _OpenAPI_service_id_to_layer2_id_H_
#define _OpenAPI_service_id_to_layer2_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_id_to_layer2_id_s OpenAPI_service_id_to_layer2_id_t;
typedef struct OpenAPI_service_id_to_layer2_id_s {
    OpenAPI_list_t *ser_ids;
    char *des_layer2_id;
} OpenAPI_service_id_to_layer2_id_t;

OpenAPI_service_id_to_layer2_id_t *OpenAPI_service_id_to_layer2_id_create(
    OpenAPI_list_t *ser_ids,
    char *des_layer2_id
    );
void OpenAPI_service_id_to_layer2_id_free(OpenAPI_service_id_to_layer2_id_t *service_id_to_layer2_id);
OpenAPI_service_id_to_layer2_id_t *OpenAPI_service_id_to_layer2_id_parseFromJSON(cJSON *service_id_to_layer2_idJSON);
cJSON *OpenAPI_service_id_to_layer2_id_convertToJSON(OpenAPI_service_id_to_layer2_id_t *service_id_to_layer2_id);
OpenAPI_service_id_to_layer2_id_t *OpenAPI_service_id_to_layer2_id_copy(OpenAPI_service_id_to_layer2_id_t *dst, OpenAPI_service_id_to_layer2_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_id_to_layer2_id_H_ */


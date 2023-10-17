/*
 * nf_service_instance.h
 *
 * NF service instance
 */

#ifndef _OpenAPI_nf_service_instance_H_
#define _OpenAPI_nf_service_instance_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_service_instance_s OpenAPI_nf_service_instance_t;
typedef struct OpenAPI_nf_service_instance_s {
    char *service_instance_id;
    char *nf_instance_id;
    char *nf_service_set_id;
} OpenAPI_nf_service_instance_t;

OpenAPI_nf_service_instance_t *OpenAPI_nf_service_instance_create(
    char *service_instance_id,
    char *nf_instance_id,
    char *nf_service_set_id
);
void OpenAPI_nf_service_instance_free(OpenAPI_nf_service_instance_t *nf_service_instance);
OpenAPI_nf_service_instance_t *OpenAPI_nf_service_instance_parseFromJSON(cJSON *nf_service_instanceJSON);
cJSON *OpenAPI_nf_service_instance_convertToJSON(OpenAPI_nf_service_instance_t *nf_service_instance);
OpenAPI_nf_service_instance_t *OpenAPI_nf_service_instance_copy(OpenAPI_nf_service_instance_t *dst, OpenAPI_nf_service_instance_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_service_instance_H_ */


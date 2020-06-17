/*
 * service_name_cond.h
 *
 * Subscription to a set of NFs based on their support for a given Service Name
 */

#ifndef _OpenAPI_service_name_cond_H_
#define _OpenAPI_service_name_cond_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_name_cond_s OpenAPI_service_name_cond_t;
typedef struct OpenAPI_service_name_cond_s {
    char *service_name;
} OpenAPI_service_name_cond_t;

OpenAPI_service_name_cond_t *OpenAPI_service_name_cond_create(
    char *service_name
    );
void OpenAPI_service_name_cond_free(OpenAPI_service_name_cond_t *service_name_cond);
OpenAPI_service_name_cond_t *OpenAPI_service_name_cond_parseFromJSON(cJSON *service_name_condJSON);
cJSON *OpenAPI_service_name_cond_convertToJSON(OpenAPI_service_name_cond_t *service_name_cond);
OpenAPI_service_name_cond_t *OpenAPI_service_name_cond_copy(OpenAPI_service_name_cond_t *dst, OpenAPI_service_name_cond_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_name_cond_H_ */


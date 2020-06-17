/*
 * chf_service_info.h
 *
 * Information of primary and secondary CHF services
 */

#ifndef _OpenAPI_chf_service_info_H_
#define _OpenAPI_chf_service_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_chf_service_info_s OpenAPI_chf_service_info_t;
typedef struct OpenAPI_chf_service_info_s {
    char *primary_chf_service_instance;
    char *secondary_chf_service_instance;
} OpenAPI_chf_service_info_t;

OpenAPI_chf_service_info_t *OpenAPI_chf_service_info_create(
    char *primary_chf_service_instance,
    char *secondary_chf_service_instance
    );
void OpenAPI_chf_service_info_free(OpenAPI_chf_service_info_t *chf_service_info);
OpenAPI_chf_service_info_t *OpenAPI_chf_service_info_parseFromJSON(cJSON *chf_service_infoJSON);
cJSON *OpenAPI_chf_service_info_convertToJSON(OpenAPI_chf_service_info_t *chf_service_info);
OpenAPI_chf_service_info_t *OpenAPI_chf_service_info_copy(OpenAPI_chf_service_info_t *dst, OpenAPI_chf_service_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_chf_service_info_H_ */


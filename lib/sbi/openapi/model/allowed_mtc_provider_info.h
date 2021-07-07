/*
 * allowed_mtc_provider_info.h
 *
 * 
 */

#ifndef _OpenAPI_allowed_mtc_provider_info_H_
#define _OpenAPI_allowed_mtc_provider_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_allowed_mtc_provider_info_s OpenAPI_allowed_mtc_provider_info_t;
typedef struct OpenAPI_allowed_mtc_provider_info_s {
    char *mtc_provider_information;
    char *af_id;
} OpenAPI_allowed_mtc_provider_info_t;

OpenAPI_allowed_mtc_provider_info_t *OpenAPI_allowed_mtc_provider_info_create(
    char *mtc_provider_information,
    char *af_id
);
void OpenAPI_allowed_mtc_provider_info_free(OpenAPI_allowed_mtc_provider_info_t *allowed_mtc_provider_info);
OpenAPI_allowed_mtc_provider_info_t *OpenAPI_allowed_mtc_provider_info_parseFromJSON(cJSON *allowed_mtc_provider_infoJSON);
cJSON *OpenAPI_allowed_mtc_provider_info_convertToJSON(OpenAPI_allowed_mtc_provider_info_t *allowed_mtc_provider_info);
OpenAPI_allowed_mtc_provider_info_t *OpenAPI_allowed_mtc_provider_info_copy(OpenAPI_allowed_mtc_provider_info_t *dst, OpenAPI_allowed_mtc_provider_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_allowed_mtc_provider_info_H_ */


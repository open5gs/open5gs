/*
 * sdm_subs_modification.h
 *
 *
 */

#ifndef _OpenAPI_sdm_subs_modification_H_
#define _OpenAPI_sdm_subs_modification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sdm_subs_modification_s OpenAPI_sdm_subs_modification_t;
typedef struct OpenAPI_sdm_subs_modification_s {
    char *expires;
    OpenAPI_list_t *monitored_resource_uris;
} OpenAPI_sdm_subs_modification_t;

OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_create(
    char *expires,
    OpenAPI_list_t *monitored_resource_uris
    );
void OpenAPI_sdm_subs_modification_free(OpenAPI_sdm_subs_modification_t *sdm_subs_modification);
OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_parseFromJSON(cJSON *sdm_subs_modificationJSON);
cJSON *OpenAPI_sdm_subs_modification_convertToJSON(OpenAPI_sdm_subs_modification_t *sdm_subs_modification);
OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_copy(OpenAPI_sdm_subs_modification_t *dst, OpenAPI_sdm_subs_modification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sdm_subs_modification_H_ */


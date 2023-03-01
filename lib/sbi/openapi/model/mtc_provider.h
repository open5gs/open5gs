/*
 * mtc_provider.h
 *
 * MTC provider information.
 */

#ifndef _OpenAPI_mtc_provider_H_
#define _OpenAPI_mtc_provider_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mtc_provider_s OpenAPI_mtc_provider_t;
typedef struct OpenAPI_mtc_provider_s {
    char *mtc_provider_information;
    char *af_id;
} OpenAPI_mtc_provider_t;

OpenAPI_mtc_provider_t *OpenAPI_mtc_provider_create(
    char *mtc_provider_information,
    char *af_id
);
void OpenAPI_mtc_provider_free(OpenAPI_mtc_provider_t *mtc_provider);
OpenAPI_mtc_provider_t *OpenAPI_mtc_provider_parseFromJSON(cJSON *mtc_providerJSON);
cJSON *OpenAPI_mtc_provider_convertToJSON(OpenAPI_mtc_provider_t *mtc_provider);
OpenAPI_mtc_provider_t *OpenAPI_mtc_provider_copy(OpenAPI_mtc_provider_t *dst, OpenAPI_mtc_provider_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mtc_provider_H_ */


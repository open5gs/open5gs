/*
 * nf_service_version.h
 *
 * Contains the version details of an NF service
 */

#ifndef _OpenAPI_nf_service_version_H_
#define _OpenAPI_nf_service_version_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_service_version_s OpenAPI_nf_service_version_t;
typedef struct OpenAPI_nf_service_version_s {
    char *api_version_in_uri;
    char *api_full_version;
    char *expiry;
} OpenAPI_nf_service_version_t;

OpenAPI_nf_service_version_t *OpenAPI_nf_service_version_create(
    char *api_version_in_uri,
    char *api_full_version,
    char *expiry
    );
void OpenAPI_nf_service_version_free(OpenAPI_nf_service_version_t *nf_service_version);
OpenAPI_nf_service_version_t *OpenAPI_nf_service_version_parseFromJSON(cJSON *nf_service_versionJSON);
cJSON *OpenAPI_nf_service_version_convertToJSON(OpenAPI_nf_service_version_t *nf_service_version);
OpenAPI_nf_service_version_t *OpenAPI_nf_service_version_copy(OpenAPI_nf_service_version_t *dst, OpenAPI_nf_service_version_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_service_version_H_ */


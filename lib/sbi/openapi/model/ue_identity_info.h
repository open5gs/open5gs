/*
 * ue_identity_info.h
 *
 * Represents 5GS-Level UE identities.
 */

#ifndef _OpenAPI_ue_identity_info_H_
#define _OpenAPI_ue_identity_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_identity_info_s OpenAPI_ue_identity_info_t;
typedef struct OpenAPI_ue_identity_info_s {
    char *gpsi;
    char *pei;
    char *supi;
} OpenAPI_ue_identity_info_t;

OpenAPI_ue_identity_info_t *OpenAPI_ue_identity_info_create(
    char *gpsi,
    char *pei,
    char *supi
);
void OpenAPI_ue_identity_info_free(OpenAPI_ue_identity_info_t *ue_identity_info);
OpenAPI_ue_identity_info_t *OpenAPI_ue_identity_info_parseFromJSON(cJSON *ue_identity_infoJSON);
cJSON *OpenAPI_ue_identity_info_convertToJSON(OpenAPI_ue_identity_info_t *ue_identity_info);
OpenAPI_ue_identity_info_t *OpenAPI_ue_identity_info_copy(OpenAPI_ue_identity_info_t *dst, OpenAPI_ue_identity_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_identity_info_H_ */


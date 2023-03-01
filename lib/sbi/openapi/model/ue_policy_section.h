/*
 * ue_policy_section.h
 *
 * Contains the UE policy section.
 */

#ifndef _OpenAPI_ue_policy_section_H_
#define _OpenAPI_ue_policy_section_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_policy_section_s OpenAPI_ue_policy_section_t;
typedef struct OpenAPI_ue_policy_section_s {
    char *ue_policy_section_info;
    char *upsi;
} OpenAPI_ue_policy_section_t;

OpenAPI_ue_policy_section_t *OpenAPI_ue_policy_section_create(
    char *ue_policy_section_info,
    char *upsi
);
void OpenAPI_ue_policy_section_free(OpenAPI_ue_policy_section_t *ue_policy_section);
OpenAPI_ue_policy_section_t *OpenAPI_ue_policy_section_parseFromJSON(cJSON *ue_policy_sectionJSON);
cJSON *OpenAPI_ue_policy_section_convertToJSON(OpenAPI_ue_policy_section_t *ue_policy_section);
OpenAPI_ue_policy_section_t *OpenAPI_ue_policy_section_copy(OpenAPI_ue_policy_section_t *dst, OpenAPI_ue_policy_section_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_policy_section_H_ */


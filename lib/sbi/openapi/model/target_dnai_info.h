/*
 * target_dnai_info.h
 *
 * Target DNAI Information
 */

#ifndef _OpenAPI_target_dnai_info_H_
#define _OpenAPI_target_dnai_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "smf_selection_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_target_dnai_info_s OpenAPI_target_dnai_info_t;
typedef struct OpenAPI_target_dnai_info_s {
    char *target_dnai;
    OpenAPI_smf_selection_type_e smf_selection_type;
} OpenAPI_target_dnai_info_t;

OpenAPI_target_dnai_info_t *OpenAPI_target_dnai_info_create(
    char *target_dnai,
    OpenAPI_smf_selection_type_e smf_selection_type
);
void OpenAPI_target_dnai_info_free(OpenAPI_target_dnai_info_t *target_dnai_info);
OpenAPI_target_dnai_info_t *OpenAPI_target_dnai_info_parseFromJSON(cJSON *target_dnai_infoJSON);
cJSON *OpenAPI_target_dnai_info_convertToJSON(OpenAPI_target_dnai_info_t *target_dnai_info);
OpenAPI_target_dnai_info_t *OpenAPI_target_dnai_info_copy(OpenAPI_target_dnai_info_t *dst, OpenAPI_target_dnai_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_target_dnai_info_H_ */


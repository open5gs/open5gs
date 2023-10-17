/*
 * target_ue_information.h
 *
 * Identifies the target UE information.
 */

#ifndef _OpenAPI_target_ue_information_H_
#define _OpenAPI_target_ue_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_target_ue_information_s OpenAPI_target_ue_information_t;
typedef struct OpenAPI_target_ue_information_s {
    bool is_any_ue;
    int any_ue;
    OpenAPI_list_t *supis;
    OpenAPI_list_t *gpsis;
    OpenAPI_list_t *int_group_ids;
} OpenAPI_target_ue_information_t;

OpenAPI_target_ue_information_t *OpenAPI_target_ue_information_create(
    bool is_any_ue,
    int any_ue,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *int_group_ids
);
void OpenAPI_target_ue_information_free(OpenAPI_target_ue_information_t *target_ue_information);
OpenAPI_target_ue_information_t *OpenAPI_target_ue_information_parseFromJSON(cJSON *target_ue_informationJSON);
cJSON *OpenAPI_target_ue_information_convertToJSON(OpenAPI_target_ue_information_t *target_ue_information);
OpenAPI_target_ue_information_t *OpenAPI_target_ue_information_copy(OpenAPI_target_ue_information_t *dst, OpenAPI_target_ue_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_target_ue_information_H_ */


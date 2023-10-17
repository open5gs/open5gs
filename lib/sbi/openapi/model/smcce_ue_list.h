/*
 * smcce_ue_list.h
 *
 * Represents the List of UEs classified based on experience level of Session Management  congestion control. 
 */

#ifndef _OpenAPI_smcce_ue_list_H_
#define _OpenAPI_smcce_ue_list_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smcce_ue_list_s OpenAPI_smcce_ue_list_t;
typedef struct OpenAPI_smcce_ue_list_s {
    OpenAPI_list_t *high_level;
    OpenAPI_list_t *medium_level;
    OpenAPI_list_t *low_level;
} OpenAPI_smcce_ue_list_t;

OpenAPI_smcce_ue_list_t *OpenAPI_smcce_ue_list_create(
    OpenAPI_list_t *high_level,
    OpenAPI_list_t *medium_level,
    OpenAPI_list_t *low_level
);
void OpenAPI_smcce_ue_list_free(OpenAPI_smcce_ue_list_t *smcce_ue_list);
OpenAPI_smcce_ue_list_t *OpenAPI_smcce_ue_list_parseFromJSON(cJSON *smcce_ue_listJSON);
cJSON *OpenAPI_smcce_ue_list_convertToJSON(OpenAPI_smcce_ue_list_t *smcce_ue_list);
OpenAPI_smcce_ue_list_t *OpenAPI_smcce_ue_list_copy(OpenAPI_smcce_ue_list_t *dst, OpenAPI_smcce_ue_list_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smcce_ue_list_H_ */


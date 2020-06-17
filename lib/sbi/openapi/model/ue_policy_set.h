/*
 * ue_policy_set.h
 *
 * Contains the UE policy data for a given subscriber.
 */

#ifndef _OpenAPI_ue_policy_set_H_
#define _OpenAPI_ue_policy_set_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_route_selection_descriptor.h"
#include "presence_info.h"
#include "ue_policy_section.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_policy_set_s OpenAPI_ue_policy_set_t;
typedef struct OpenAPI_ue_policy_set_s {
    OpenAPI_list_t* pra_infos;
    OpenAPI_list_t *subsc_cats;
    OpenAPI_list_t* ue_policy_sections;
    OpenAPI_list_t *upsis;
    OpenAPI_list_t* allowed_route_sel_descs;
    int andsp_ind;
    char *pei;
    OpenAPI_list_t *os_ids;
} OpenAPI_ue_policy_set_t;

OpenAPI_ue_policy_set_t *OpenAPI_ue_policy_set_create(
    OpenAPI_list_t* pra_infos,
    OpenAPI_list_t *subsc_cats,
    OpenAPI_list_t* ue_policy_sections,
    OpenAPI_list_t *upsis,
    OpenAPI_list_t* allowed_route_sel_descs,
    int andsp_ind,
    char *pei,
    OpenAPI_list_t *os_ids
    );
void OpenAPI_ue_policy_set_free(OpenAPI_ue_policy_set_t *ue_policy_set);
OpenAPI_ue_policy_set_t *OpenAPI_ue_policy_set_parseFromJSON(cJSON *ue_policy_setJSON);
cJSON *OpenAPI_ue_policy_set_convertToJSON(OpenAPI_ue_policy_set_t *ue_policy_set);
OpenAPI_ue_policy_set_t *OpenAPI_ue_policy_set_copy(OpenAPI_ue_policy_set_t *dst, OpenAPI_ue_policy_set_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_policy_set_H_ */


/*
 * policy_association.h
 *
 *
 */

#ifndef _OpenAPI_policy_association_H_
#define _OpenAPI_policy_association_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr.h"
#include "policy_association_request.h"
#include "presence_info.h"
#include "request_trigger.h"
#include "service_area_restriction.h"
#include "smf_selection_data.h"
#include "wireline_service_area_restriction.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_policy_association_s OpenAPI_policy_association_t;
typedef struct OpenAPI_policy_association_s {
    struct OpenAPI_policy_association_request_s *request;
    OpenAPI_list_t *triggers;
    struct OpenAPI_service_area_restriction_s *serv_area_res;
    struct OpenAPI_wireline_service_area_restriction_s *wl_serv_area_res;
    int rfsp;
    struct OpenAPI_smf_selection_data_s *smf_sel_info;
    struct OpenAPI_ambr_s *ue_ambr;
    struct OpenAPI_ambr_s *rg_tmbr;
    OpenAPI_list_t* pras;
    char *supp_feat;
} OpenAPI_policy_association_t;

OpenAPI_policy_association_t *OpenAPI_policy_association_create(
    OpenAPI_policy_association_request_t *request,
    OpenAPI_list_t *triggers,
    OpenAPI_service_area_restriction_t *serv_area_res,
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res,
    int rfsp,
    OpenAPI_smf_selection_data_t *smf_sel_info,
    OpenAPI_ambr_t *ue_ambr,
    OpenAPI_ambr_t *rg_tmbr,
    OpenAPI_list_t* pras,
    char *supp_feat
    );
void OpenAPI_policy_association_free(OpenAPI_policy_association_t *policy_association);
OpenAPI_policy_association_t *OpenAPI_policy_association_parseFromJSON(cJSON *policy_associationJSON);
cJSON *OpenAPI_policy_association_convertToJSON(OpenAPI_policy_association_t *policy_association);
OpenAPI_policy_association_t *OpenAPI_policy_association_copy(OpenAPI_policy_association_t *dst, OpenAPI_policy_association_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_association_H_ */


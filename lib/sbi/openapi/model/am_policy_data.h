/*
 * am_policy_data.h
 *
 * Contains the AM policy data for a given subscriber.
 */

#ifndef _OpenAPI_am_policy_data_H_
#define _OpenAPI_am_policy_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_am_policy_data_s OpenAPI_am_policy_data_t;
#include "charging_information.h"
#include "policy_counter_info.h"
#include "presence_info.h"
#include "restricted_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_am_policy_data_s {
    OpenAPI_list_t* pra_infos;
    OpenAPI_list_t *subsc_cats;
    struct OpenAPI_charging_information_s *chf_info;
    char *chf_group_id;
    bool is_subsc_spending_limits;
    int subsc_spending_limits;
    OpenAPI_list_t* spend_lim_info;
    OpenAPI_list_t *restri_status;
    char *supp_feat;
};

OpenAPI_am_policy_data_t *OpenAPI_am_policy_data_create(
    OpenAPI_list_t* pra_infos,
    OpenAPI_list_t *subsc_cats,
    OpenAPI_charging_information_t *chf_info,
    char *chf_group_id,
    bool is_subsc_spending_limits,
    int subsc_spending_limits,
    OpenAPI_list_t* spend_lim_info,
    OpenAPI_list_t *restri_status,
    char *supp_feat
);
void OpenAPI_am_policy_data_free(OpenAPI_am_policy_data_t *am_policy_data);
OpenAPI_am_policy_data_t *OpenAPI_am_policy_data_parseFromJSON(cJSON *am_policy_dataJSON);
cJSON *OpenAPI_am_policy_data_convertToJSON(OpenAPI_am_policy_data_t *am_policy_data);
OpenAPI_am_policy_data_t *OpenAPI_am_policy_data_copy(OpenAPI_am_policy_data_t *dst, OpenAPI_am_policy_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_am_policy_data_H_ */


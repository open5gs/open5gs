/*
 * sm_policy_delete_data.h
 *
 *
 */

#ifndef _OpenAPI_sm_policy_delete_data_H_
#define _OpenAPI_sm_policy_delete_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "accu_usage_report.h"
#include "pdu_session_rel_cause.h"
#include "plmn_id_nid.h"
#include "ran_nas_rel_cause.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_policy_delete_data_s OpenAPI_sm_policy_delete_data_t;
typedef struct OpenAPI_sm_policy_delete_data_s {
    struct OpenAPI_user_location_s *user_location_info;
    char *ue_time_zone;
    struct OpenAPI_plmn_id_nid_s *serving_network;
    char *user_location_info_time;
    OpenAPI_list_t *ran_nas_rel_causes;
    OpenAPI_list_t *accu_usage_reports;
    struct OpenAPI_pdu_session_rel_cause_s *pdu_sess_rel_cause;
} OpenAPI_sm_policy_delete_data_t;

OpenAPI_sm_policy_delete_data_t *OpenAPI_sm_policy_delete_data_create(
    OpenAPI_user_location_t *user_location_info,
    char *ue_time_zone,
    OpenAPI_plmn_id_nid_t *serving_network,
    char *user_location_info_time,
    OpenAPI_list_t *ran_nas_rel_causes,
    OpenAPI_list_t *accu_usage_reports,
    OpenAPI_pdu_session_rel_cause_t *pdu_sess_rel_cause
    );
void OpenAPI_sm_policy_delete_data_free(OpenAPI_sm_policy_delete_data_t *sm_policy_delete_data);
OpenAPI_sm_policy_delete_data_t *OpenAPI_sm_policy_delete_data_parseFromJSON(cJSON *sm_policy_delete_dataJSON);
cJSON *OpenAPI_sm_policy_delete_data_convertToJSON(OpenAPI_sm_policy_delete_data_t *sm_policy_delete_data);
OpenAPI_sm_policy_delete_data_t *OpenAPI_sm_policy_delete_data_copy(OpenAPI_sm_policy_delete_data_t *dst, OpenAPI_sm_policy_delete_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_policy_delete_data_H_ */


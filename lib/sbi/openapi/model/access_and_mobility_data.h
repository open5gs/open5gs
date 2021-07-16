/*
 * access_and_mobility_data.h
 *
 * 
 */

#ifndef _OpenAPI_access_and_mobility_data_H_
#define _OpenAPI_access_and_mobility_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "cm_info.h"
#include "plmn_id_1.h"
#include "rat_type.h"
#include "rm_info.h"
#include "sms_support.h"
#include "ue_reachability.h"
#include "user_location.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_and_mobility_data_s OpenAPI_access_and_mobility_data_t;
typedef struct OpenAPI_access_and_mobility_data_s {
    struct OpenAPI_user_location_s *location;
    char *location_ts;
    char *time_zone;
    char *time_zone_ts;
    OpenAPI_access_type_e access_type;
    OpenAPI_list_t *reg_states;
    char *reg_states_ts;
    OpenAPI_list_t *conn_states;
    char *conn_states_ts;
    struct OpenAPI_ue_reachability_s *reachability_status;
    char *reachability_status_ts;
    OpenAPI_sms_support_e sms_over_nas_status;
    char *sms_over_nas_status_ts;
    bool is_roaming_status;
    int roaming_status;
    char *roaming_status_ts;
    struct OpenAPI_plmn_id_1_s *current_plmn;
    char *current_plmn_ts;
    OpenAPI_list_t *rat_type;
    char *rat_types_ts;
    char *supp_feat;
} OpenAPI_access_and_mobility_data_t;

OpenAPI_access_and_mobility_data_t *OpenAPI_access_and_mobility_data_create(
    OpenAPI_user_location_t *location,
    char *location_ts,
    char *time_zone,
    char *time_zone_ts,
    OpenAPI_access_type_e access_type,
    OpenAPI_list_t *reg_states,
    char *reg_states_ts,
    OpenAPI_list_t *conn_states,
    char *conn_states_ts,
    OpenAPI_ue_reachability_t *reachability_status,
    char *reachability_status_ts,
    OpenAPI_sms_support_e sms_over_nas_status,
    char *sms_over_nas_status_ts,
    bool is_roaming_status,
    int roaming_status,
    char *roaming_status_ts,
    OpenAPI_plmn_id_1_t *current_plmn,
    char *current_plmn_ts,
    OpenAPI_list_t *rat_type,
    char *rat_types_ts,
    char *supp_feat
);
void OpenAPI_access_and_mobility_data_free(OpenAPI_access_and_mobility_data_t *access_and_mobility_data);
OpenAPI_access_and_mobility_data_t *OpenAPI_access_and_mobility_data_parseFromJSON(cJSON *access_and_mobility_dataJSON);
cJSON *OpenAPI_access_and_mobility_data_convertToJSON(OpenAPI_access_and_mobility_data_t *access_and_mobility_data);
OpenAPI_access_and_mobility_data_t *OpenAPI_access_and_mobility_data_copy(OpenAPI_access_and_mobility_data_t *dst, OpenAPI_access_and_mobility_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_and_mobility_data_H_ */


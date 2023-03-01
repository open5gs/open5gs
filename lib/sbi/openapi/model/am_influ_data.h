/*
 * am_influ_data.h
 *
 * Represents the AM Influence Data.
 */

#ifndef _OpenAPI_am_influ_data_H_
#define _OpenAPI_am_influ_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "am_influ_event.h"
#include "dnn_snssai_information.h"
#include "service_area_coverage_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_am_influ_data_s OpenAPI_am_influ_data_t;
typedef struct OpenAPI_am_influ_data_s {
    OpenAPI_list_t *app_ids;
    OpenAPI_list_t *dnn_snssai_infos;
    char *inter_group_id;
    char *supi;
    bool is_any_ue_ind;
    int any_ue_ind;
    bool is_policy_duration;
    int policy_duration;
    OpenAPI_list_t *ev_subs;
    char *notif_uri;
    char *notif_corr_id;
    OpenAPI_list_t *headers;
    bool is_thru_req;
    int thru_req;
    OpenAPI_list_t *cov_req;
    char *supported_features;
    char *res_uri;
    OpenAPI_list_t *reset_ids;
} OpenAPI_am_influ_data_t;

OpenAPI_am_influ_data_t *OpenAPI_am_influ_data_create(
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *dnn_snssai_infos,
    char *inter_group_id,
    char *supi,
    bool is_any_ue_ind,
    int any_ue_ind,
    bool is_policy_duration,
    int policy_duration,
    OpenAPI_list_t *ev_subs,
    char *notif_uri,
    char *notif_corr_id,
    OpenAPI_list_t *headers,
    bool is_thru_req,
    int thru_req,
    OpenAPI_list_t *cov_req,
    char *supported_features,
    char *res_uri,
    OpenAPI_list_t *reset_ids
);
void OpenAPI_am_influ_data_free(OpenAPI_am_influ_data_t *am_influ_data);
OpenAPI_am_influ_data_t *OpenAPI_am_influ_data_parseFromJSON(cJSON *am_influ_dataJSON);
cJSON *OpenAPI_am_influ_data_convertToJSON(OpenAPI_am_influ_data_t *am_influ_data);
OpenAPI_am_influ_data_t *OpenAPI_am_influ_data_copy(OpenAPI_am_influ_data_t *dst, OpenAPI_am_influ_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_am_influ_data_H_ */


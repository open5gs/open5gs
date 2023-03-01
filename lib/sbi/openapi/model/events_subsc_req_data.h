/*
 * events_subsc_req_data.h
 *
 * Identifies the events the application subscribes to.
 */

#ifndef _OpenAPI_events_subsc_req_data_H_
#define _OpenAPI_events_subsc_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event_subscription.h"
#include "qos_monitoring_information.h"
#include "requested_qos_monitoring_parameter.h"
#include "required_access_info.h"
#include "usage_threshold.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_events_subsc_req_data_s OpenAPI_events_subsc_req_data_t;
typedef struct OpenAPI_events_subsc_req_data_s {
    OpenAPI_list_t *events;
    char *notif_uri;
    OpenAPI_list_t *req_qos_mon_params;
    struct OpenAPI_qos_monitoring_information_s *qos_mon;
    OpenAPI_list_t *req_anis;
    struct OpenAPI_usage_threshold_s *usg_thres;
    char *notif_corre_id;
    OpenAPI_list_t *af_app_ids;
    bool is_direct_notif_ind;
    int direct_notif_ind;
} OpenAPI_events_subsc_req_data_t;

OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    OpenAPI_list_t *req_qos_mon_params,
    OpenAPI_qos_monitoring_information_t *qos_mon,
    OpenAPI_list_t *req_anis,
    OpenAPI_usage_threshold_t *usg_thres,
    char *notif_corre_id,
    OpenAPI_list_t *af_app_ids,
    bool is_direct_notif_ind,
    int direct_notif_ind
);
void OpenAPI_events_subsc_req_data_free(OpenAPI_events_subsc_req_data_t *events_subsc_req_data);
OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_parseFromJSON(cJSON *events_subsc_req_dataJSON);
cJSON *OpenAPI_events_subsc_req_data_convertToJSON(OpenAPI_events_subsc_req_data_t *events_subsc_req_data);
OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_copy(OpenAPI_events_subsc_req_data_t *dst, OpenAPI_events_subsc_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_events_subsc_req_data_H_ */


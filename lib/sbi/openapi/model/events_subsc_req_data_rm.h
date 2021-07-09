/*
 * events_subsc_req_data_rm.h
 *
 * this data type is defined in the same way as the EventsSubscReqData data type, but with the OpenAPI nullable property set to true.
 */

#ifndef _OpenAPI_events_subsc_req_data_rm_H_
#define _OpenAPI_events_subsc_req_data_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event_subscription.h"
#include "qos_monitoring_information_rm.h"
#include "requested_qos_monitoring_parameter.h"
#include "required_access_info.h"
#include "usage_threshold_rm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_events_subsc_req_data_rm_s OpenAPI_events_subsc_req_data_rm_t;
typedef struct OpenAPI_events_subsc_req_data_rm_s {
    OpenAPI_list_t *events;
    char *notif_uri;
    OpenAPI_list_t *req_qos_mon_params;
    struct OpenAPI_qos_monitoring_information_rm_s *qos_mon;
    OpenAPI_list_t *req_anis;
    struct OpenAPI_usage_threshold_rm_s *usg_thres;
    char *notif_corre_id;
} OpenAPI_events_subsc_req_data_rm_t;

OpenAPI_events_subsc_req_data_rm_t *OpenAPI_events_subsc_req_data_rm_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    OpenAPI_list_t *req_qos_mon_params,
    OpenAPI_qos_monitoring_information_rm_t *qos_mon,
    OpenAPI_list_t *req_anis,
    OpenAPI_usage_threshold_rm_t *usg_thres,
    char *notif_corre_id
);
void OpenAPI_events_subsc_req_data_rm_free(OpenAPI_events_subsc_req_data_rm_t *events_subsc_req_data_rm);
OpenAPI_events_subsc_req_data_rm_t *OpenAPI_events_subsc_req_data_rm_parseFromJSON(cJSON *events_subsc_req_data_rmJSON);
cJSON *OpenAPI_events_subsc_req_data_rm_convertToJSON(OpenAPI_events_subsc_req_data_rm_t *events_subsc_req_data_rm);
OpenAPI_events_subsc_req_data_rm_t *OpenAPI_events_subsc_req_data_rm_copy(OpenAPI_events_subsc_req_data_rm_t *dst, OpenAPI_events_subsc_req_data_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_events_subsc_req_data_rm_H_ */


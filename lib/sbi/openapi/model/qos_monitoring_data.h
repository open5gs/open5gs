/*
 * qos_monitoring_data.h
 *
 * Contains QoS monitoring related control information.
 */

#ifndef _OpenAPI_qos_monitoring_data_H_
#define _OpenAPI_qos_monitoring_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "reporting_frequency.h"
#include "requested_qos_monitoring_parameter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_monitoring_data_s OpenAPI_qos_monitoring_data_t;
typedef struct OpenAPI_qos_monitoring_data_s {
    char *qm_id;
    OpenAPI_list_t *req_qos_mon_params;
    OpenAPI_list_t *rep_freqs;
    bool is_rep_thresh_dl_null;
    bool is_rep_thresh_dl;
    int rep_thresh_dl;
    bool is_rep_thresh_ul_null;
    bool is_rep_thresh_ul;
    int rep_thresh_ul;
    bool is_rep_thresh_rp_null;
    bool is_rep_thresh_rp;
    int rep_thresh_rp;
    bool is_wait_time_null;
    bool is_wait_time;
    int wait_time;
    bool is_rep_period_null;
    bool is_rep_period;
    int rep_period;
    bool is_notify_uri_null;
    char *notify_uri;
    bool is_notify_corre_id_null;
    char *notify_corre_id;
    bool is_direct_notif_ind;
    int direct_notif_ind;
} OpenAPI_qos_monitoring_data_t;

OpenAPI_qos_monitoring_data_t *OpenAPI_qos_monitoring_data_create(
    char *qm_id,
    OpenAPI_list_t *req_qos_mon_params,
    OpenAPI_list_t *rep_freqs,
    bool is_rep_thresh_dl_null,
    bool is_rep_thresh_dl,
    int rep_thresh_dl,
    bool is_rep_thresh_ul_null,
    bool is_rep_thresh_ul,
    int rep_thresh_ul,
    bool is_rep_thresh_rp_null,
    bool is_rep_thresh_rp,
    int rep_thresh_rp,
    bool is_wait_time_null,
    bool is_wait_time,
    int wait_time,
    bool is_rep_period_null,
    bool is_rep_period,
    int rep_period,
    bool is_notify_uri_null,
    char *notify_uri,
    bool is_notify_corre_id_null,
    char *notify_corre_id,
    bool is_direct_notif_ind,
    int direct_notif_ind
);
void OpenAPI_qos_monitoring_data_free(OpenAPI_qos_monitoring_data_t *qos_monitoring_data);
OpenAPI_qos_monitoring_data_t *OpenAPI_qos_monitoring_data_parseFromJSON(cJSON *qos_monitoring_dataJSON);
cJSON *OpenAPI_qos_monitoring_data_convertToJSON(OpenAPI_qos_monitoring_data_t *qos_monitoring_data);
OpenAPI_qos_monitoring_data_t *OpenAPI_qos_monitoring_data_copy(OpenAPI_qos_monitoring_data_t *dst, OpenAPI_qos_monitoring_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_data_H_ */


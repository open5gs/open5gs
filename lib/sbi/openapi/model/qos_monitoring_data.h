/*
 * qos_monitoring_data.h
 *
 *
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
    int rep_thresh_dl;
    int rep_thresh_ul;
    int rep_thresh_rp;
    int wait_time;
    int rep_period;
    char *notify_uri;
    char *notify_corre_id;
} OpenAPI_qos_monitoring_data_t;

OpenAPI_qos_monitoring_data_t *OpenAPI_qos_monitoring_data_create(
    char *qm_id,
    OpenAPI_list_t *req_qos_mon_params,
    OpenAPI_list_t *rep_freqs,
    int rep_thresh_dl,
    int rep_thresh_ul,
    int rep_thresh_rp,
    int wait_time,
    int rep_period,
    char *notify_uri,
    char *notify_corre_id
    );
void OpenAPI_qos_monitoring_data_free(OpenAPI_qos_monitoring_data_t *qos_monitoring_data);
OpenAPI_qos_monitoring_data_t *OpenAPI_qos_monitoring_data_parseFromJSON(cJSON *qos_monitoring_dataJSON);
cJSON *OpenAPI_qos_monitoring_data_convertToJSON(OpenAPI_qos_monitoring_data_t *qos_monitoring_data);
OpenAPI_qos_monitoring_data_t *OpenAPI_qos_monitoring_data_copy(OpenAPI_qos_monitoring_data_t *dst, OpenAPI_qos_monitoring_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_data_H_ */


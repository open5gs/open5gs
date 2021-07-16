/*
 * qos_monitoring_information_rm.h
 *
 * this data type is defined in the same way as the QosMonitoringInformation data type, but with the OpenAPI nullable property set to true.
 */

#ifndef _OpenAPI_qos_monitoring_information_rm_H_
#define _OpenAPI_qos_monitoring_information_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_monitoring_information_rm_s OpenAPI_qos_monitoring_information_rm_t;
typedef struct OpenAPI_qos_monitoring_information_rm_s {
    bool is_rep_thresh_dl;
    int rep_thresh_dl;
    bool is_rep_thresh_ul;
    int rep_thresh_ul;
    bool is_rep_thresh_rp;
    int rep_thresh_rp;
} OpenAPI_qos_monitoring_information_rm_t;

OpenAPI_qos_monitoring_information_rm_t *OpenAPI_qos_monitoring_information_rm_create(
    bool is_rep_thresh_dl,
    int rep_thresh_dl,
    bool is_rep_thresh_ul,
    int rep_thresh_ul,
    bool is_rep_thresh_rp,
    int rep_thresh_rp
);
void OpenAPI_qos_monitoring_information_rm_free(OpenAPI_qos_monitoring_information_rm_t *qos_monitoring_information_rm);
OpenAPI_qos_monitoring_information_rm_t *OpenAPI_qos_monitoring_information_rm_parseFromJSON(cJSON *qos_monitoring_information_rmJSON);
cJSON *OpenAPI_qos_monitoring_information_rm_convertToJSON(OpenAPI_qos_monitoring_information_rm_t *qos_monitoring_information_rm);
OpenAPI_qos_monitoring_information_rm_t *OpenAPI_qos_monitoring_information_rm_copy(OpenAPI_qos_monitoring_information_rm_t *dst, OpenAPI_qos_monitoring_information_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_information_rm_H_ */


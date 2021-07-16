/*
 * qos_monitoring_information.h
 *
 * Indicates the QoS Monitoring information to report, i.e. UL and/or DL and or round trip delay.
 */

#ifndef _OpenAPI_qos_monitoring_information_H_
#define _OpenAPI_qos_monitoring_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_monitoring_information_s OpenAPI_qos_monitoring_information_t;
typedef struct OpenAPI_qos_monitoring_information_s {
    bool is_rep_thresh_dl;
    int rep_thresh_dl;
    bool is_rep_thresh_ul;
    int rep_thresh_ul;
    bool is_rep_thresh_rp;
    int rep_thresh_rp;
} OpenAPI_qos_monitoring_information_t;

OpenAPI_qos_monitoring_information_t *OpenAPI_qos_monitoring_information_create(
    bool is_rep_thresh_dl,
    int rep_thresh_dl,
    bool is_rep_thresh_ul,
    int rep_thresh_ul,
    bool is_rep_thresh_rp,
    int rep_thresh_rp
);
void OpenAPI_qos_monitoring_information_free(OpenAPI_qos_monitoring_information_t *qos_monitoring_information);
OpenAPI_qos_monitoring_information_t *OpenAPI_qos_monitoring_information_parseFromJSON(cJSON *qos_monitoring_informationJSON);
cJSON *OpenAPI_qos_monitoring_information_convertToJSON(OpenAPI_qos_monitoring_information_t *qos_monitoring_information);
OpenAPI_qos_monitoring_information_t *OpenAPI_qos_monitoring_information_copy(OpenAPI_qos_monitoring_information_t *dst, OpenAPI_qos_monitoring_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_information_H_ */


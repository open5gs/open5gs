/*
 * qos_monitoring_info.h
 *
 * QoS Monitoring Information
 */

#ifndef _OpenAPI_qos_monitoring_info_H_
#define _OpenAPI_qos_monitoring_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_monitoring_info_s OpenAPI_qos_monitoring_info_t;
typedef struct OpenAPI_qos_monitoring_info_s {
    bool is_qos_monitoring_ind;
    int qos_monitoring_ind;
} OpenAPI_qos_monitoring_info_t;

OpenAPI_qos_monitoring_info_t *OpenAPI_qos_monitoring_info_create(
    bool is_qos_monitoring_ind,
    int qos_monitoring_ind
);
void OpenAPI_qos_monitoring_info_free(OpenAPI_qos_monitoring_info_t *qos_monitoring_info);
OpenAPI_qos_monitoring_info_t *OpenAPI_qos_monitoring_info_parseFromJSON(cJSON *qos_monitoring_infoJSON);
cJSON *OpenAPI_qos_monitoring_info_convertToJSON(OpenAPI_qos_monitoring_info_t *qos_monitoring_info);
OpenAPI_qos_monitoring_info_t *OpenAPI_qos_monitoring_info_copy(OpenAPI_qos_monitoring_info_t *dst, OpenAPI_qos_monitoring_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_monitoring_info_H_ */


/*
 * qos_sustainability_info.h
 *
 * Represents the QoS Sustainability information.
 */

#ifndef _OpenAPI_qos_sustainability_info_H_
#define _OpenAPI_qos_sustainability_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_area_info.h"
#include "retainability_threshold.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_qos_sustainability_info_s OpenAPI_qos_sustainability_info_t;
typedef struct OpenAPI_qos_sustainability_info_s {
    struct OpenAPI_network_area_info_s *area_info;
    char *start_ts;
    char *end_ts;
    struct OpenAPI_retainability_threshold_s *qos_flow_ret_thd;
    char *ran_ue_throu_thd;
    struct OpenAPI_snssai_s *snssai;
    bool is_confidence;
    int confidence;
} OpenAPI_qos_sustainability_info_t;

OpenAPI_qos_sustainability_info_t *OpenAPI_qos_sustainability_info_create(
    OpenAPI_network_area_info_t *area_info,
    char *start_ts,
    char *end_ts,
    OpenAPI_retainability_threshold_t *qos_flow_ret_thd,
    char *ran_ue_throu_thd,
    OpenAPI_snssai_t *snssai,
    bool is_confidence,
    int confidence
);
void OpenAPI_qos_sustainability_info_free(OpenAPI_qos_sustainability_info_t *qos_sustainability_info);
OpenAPI_qos_sustainability_info_t *OpenAPI_qos_sustainability_info_parseFromJSON(cJSON *qos_sustainability_infoJSON);
cJSON *OpenAPI_qos_sustainability_info_convertToJSON(OpenAPI_qos_sustainability_info_t *qos_sustainability_info);
OpenAPI_qos_sustainability_info_t *OpenAPI_qos_sustainability_info_copy(OpenAPI_qos_sustainability_info_t *dst, OpenAPI_qos_sustainability_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_sustainability_info_H_ */


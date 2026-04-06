/*
 * qos_policy_assist_info.h
 *
 * The QoS and policy assistance analytics information.
 */

#ifndef _OpenAPI_qos_policy_assist_info_H_
#define _OpenAPI_qos_policy_assist_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_qos_policy_assist_info_s OpenAPI_qos_policy_assist_info_t;
#include "qos_policy_assist_sets_per_ts.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_qos_policy_assist_info_s {
    OpenAPI_list_t *qos_pol_assist_info;
};

OpenAPI_qos_policy_assist_info_t *OpenAPI_qos_policy_assist_info_create(
    OpenAPI_list_t *qos_pol_assist_info
);
void OpenAPI_qos_policy_assist_info_free(OpenAPI_qos_policy_assist_info_t *qos_policy_assist_info);
OpenAPI_qos_policy_assist_info_t *OpenAPI_qos_policy_assist_info_parseFromJSON(cJSON *qos_policy_assist_infoJSON);
cJSON *OpenAPI_qos_policy_assist_info_convertToJSON(OpenAPI_qos_policy_assist_info_t *qos_policy_assist_info);
OpenAPI_qos_policy_assist_info_t *OpenAPI_qos_policy_assist_info_copy(OpenAPI_qos_policy_assist_info_t *dst, OpenAPI_qos_policy_assist_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_policy_assist_info_H_ */


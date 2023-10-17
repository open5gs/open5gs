/*
 * pro_se_capability.h
 *
 * Indicate the supported ProSe Capability by the PCF.
 */

#ifndef _OpenAPI_pro_se_capability_H_
#define _OpenAPI_pro_se_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_capability_s OpenAPI_pro_se_capability_t;
typedef struct OpenAPI_pro_se_capability_s {
    bool is_prose_direct_discovey;
    int prose_direct_discovey;
    bool is_prose_direct_communication;
    int prose_direct_communication;
    bool is_prose_l2_ueto_network_relay;
    int prose_l2_ueto_network_relay;
    bool is_prose_l3_ueto_network_relay;
    int prose_l3_ueto_network_relay;
    bool is_prose_l2_remote_ue;
    int prose_l2_remote_ue;
    bool is_prose_l3_remote_ue;
    int prose_l3_remote_ue;
} OpenAPI_pro_se_capability_t;

OpenAPI_pro_se_capability_t *OpenAPI_pro_se_capability_create(
    bool is_prose_direct_discovey,
    int prose_direct_discovey,
    bool is_prose_direct_communication,
    int prose_direct_communication,
    bool is_prose_l2_ueto_network_relay,
    int prose_l2_ueto_network_relay,
    bool is_prose_l3_ueto_network_relay,
    int prose_l3_ueto_network_relay,
    bool is_prose_l2_remote_ue,
    int prose_l2_remote_ue,
    bool is_prose_l3_remote_ue,
    int prose_l3_remote_ue
);
void OpenAPI_pro_se_capability_free(OpenAPI_pro_se_capability_t *pro_se_capability);
OpenAPI_pro_se_capability_t *OpenAPI_pro_se_capability_parseFromJSON(cJSON *pro_se_capabilityJSON);
cJSON *OpenAPI_pro_se_capability_convertToJSON(OpenAPI_pro_se_capability_t *pro_se_capability);
OpenAPI_pro_se_capability_t *OpenAPI_pro_se_capability_copy(OpenAPI_pro_se_capability_t *dst, OpenAPI_pro_se_capability_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_capability_H_ */


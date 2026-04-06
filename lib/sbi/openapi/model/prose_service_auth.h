/*
 * prose_service_auth.h
 *
 * Indicates whether the UE is authorized to use related services. 
 */

#ifndef _OpenAPI_prose_service_auth_H_
#define _OpenAPI_prose_service_auth_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_prose_service_auth_s OpenAPI_prose_service_auth_t;
#include "ue_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_prose_service_auth_s {
    OpenAPI_ue_auth_e prose_direct_discovery_auth;
    OpenAPI_ue_auth_e prose_direct_communication_auth;
    OpenAPI_ue_auth_e prose_l2_relay_auth;
    OpenAPI_ue_auth_e prose_l3_relay_auth;
    OpenAPI_ue_auth_e prose_l2_remote_auth;
    OpenAPI_ue_auth_e prose_l3_remote_auth;
    OpenAPI_ue_auth_e prose_multipath_com_l2_remote_auth;
    OpenAPI_ue_auth_e prose_l2_ue_relay_auth;
    OpenAPI_ue_auth_e prose_l3_ue_relay_auth;
    OpenAPI_ue_auth_e prose_l2_end_auth;
    OpenAPI_ue_auth_e prose_l3_end_auth;
    OpenAPI_ue_auth_e prose_l3_intermediate_relay_auth;
    OpenAPI_ue_auth_e prose_l3_remote_multihop_auth;
    OpenAPI_ue_auth_e prose_l3_relay_multihop_auth;
    OpenAPI_ue_auth_e prose_l3_ue_relay_multihop_auth;
    OpenAPI_ue_auth_e prose_l3_end_multihop_auth;
    OpenAPI_ue_auth_e prose_l2_intermediate_relay_auth;
    OpenAPI_ue_auth_e prose_l2_remote_multihop_auth;
    OpenAPI_ue_auth_e prose_l2_relay_multihop_auth;
};

OpenAPI_prose_service_auth_t *OpenAPI_prose_service_auth_create(
    OpenAPI_ue_auth_e prose_direct_discovery_auth,
    OpenAPI_ue_auth_e prose_direct_communication_auth,
    OpenAPI_ue_auth_e prose_l2_relay_auth,
    OpenAPI_ue_auth_e prose_l3_relay_auth,
    OpenAPI_ue_auth_e prose_l2_remote_auth,
    OpenAPI_ue_auth_e prose_l3_remote_auth,
    OpenAPI_ue_auth_e prose_multipath_com_l2_remote_auth,
    OpenAPI_ue_auth_e prose_l2_ue_relay_auth,
    OpenAPI_ue_auth_e prose_l3_ue_relay_auth,
    OpenAPI_ue_auth_e prose_l2_end_auth,
    OpenAPI_ue_auth_e prose_l3_end_auth,
    OpenAPI_ue_auth_e prose_l3_intermediate_relay_auth,
    OpenAPI_ue_auth_e prose_l3_remote_multihop_auth,
    OpenAPI_ue_auth_e prose_l3_relay_multihop_auth,
    OpenAPI_ue_auth_e prose_l3_ue_relay_multihop_auth,
    OpenAPI_ue_auth_e prose_l3_end_multihop_auth,
    OpenAPI_ue_auth_e prose_l2_intermediate_relay_auth,
    OpenAPI_ue_auth_e prose_l2_remote_multihop_auth,
    OpenAPI_ue_auth_e prose_l2_relay_multihop_auth
);
void OpenAPI_prose_service_auth_free(OpenAPI_prose_service_auth_t *prose_service_auth);
OpenAPI_prose_service_auth_t *OpenAPI_prose_service_auth_parseFromJSON(cJSON *prose_service_authJSON);
cJSON *OpenAPI_prose_service_auth_convertToJSON(OpenAPI_prose_service_auth_t *prose_service_auth);
OpenAPI_prose_service_auth_t *OpenAPI_prose_service_auth_copy(OpenAPI_prose_service_auth_t *dst, OpenAPI_prose_service_auth_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_service_auth_H_ */


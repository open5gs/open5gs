
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prose_service_auth_1.h"

OpenAPI_prose_service_auth_1_t *OpenAPI_prose_service_auth_1_create(
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
)
{
    OpenAPI_prose_service_auth_1_t *prose_service_auth_1_local_var = ogs_malloc(sizeof(OpenAPI_prose_service_auth_1_t));
    ogs_assert(prose_service_auth_1_local_var);

    prose_service_auth_1_local_var->prose_direct_discovery_auth = prose_direct_discovery_auth;
    prose_service_auth_1_local_var->prose_direct_communication_auth = prose_direct_communication_auth;
    prose_service_auth_1_local_var->prose_l2_relay_auth = prose_l2_relay_auth;
    prose_service_auth_1_local_var->prose_l3_relay_auth = prose_l3_relay_auth;
    prose_service_auth_1_local_var->prose_l2_remote_auth = prose_l2_remote_auth;
    prose_service_auth_1_local_var->prose_l3_remote_auth = prose_l3_remote_auth;
    prose_service_auth_1_local_var->prose_multipath_com_l2_remote_auth = prose_multipath_com_l2_remote_auth;
    prose_service_auth_1_local_var->prose_l2_ue_relay_auth = prose_l2_ue_relay_auth;
    prose_service_auth_1_local_var->prose_l3_ue_relay_auth = prose_l3_ue_relay_auth;
    prose_service_auth_1_local_var->prose_l2_end_auth = prose_l2_end_auth;
    prose_service_auth_1_local_var->prose_l3_end_auth = prose_l3_end_auth;
    prose_service_auth_1_local_var->prose_l3_intermediate_relay_auth = prose_l3_intermediate_relay_auth;
    prose_service_auth_1_local_var->prose_l3_remote_multihop_auth = prose_l3_remote_multihop_auth;
    prose_service_auth_1_local_var->prose_l3_relay_multihop_auth = prose_l3_relay_multihop_auth;
    prose_service_auth_1_local_var->prose_l3_ue_relay_multihop_auth = prose_l3_ue_relay_multihop_auth;
    prose_service_auth_1_local_var->prose_l3_end_multihop_auth = prose_l3_end_multihop_auth;
    prose_service_auth_1_local_var->prose_l2_intermediate_relay_auth = prose_l2_intermediate_relay_auth;
    prose_service_auth_1_local_var->prose_l2_remote_multihop_auth = prose_l2_remote_multihop_auth;
    prose_service_auth_1_local_var->prose_l2_relay_multihop_auth = prose_l2_relay_multihop_auth;

    return prose_service_auth_1_local_var;
}

void OpenAPI_prose_service_auth_1_free(OpenAPI_prose_service_auth_1_t *prose_service_auth_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == prose_service_auth_1) {
        return;
    }
    ogs_free(prose_service_auth_1);
}

cJSON *OpenAPI_prose_service_auth_1_convertToJSON(OpenAPI_prose_service_auth_1_t *prose_service_auth_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (prose_service_auth_1 == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [ProseServiceAuth_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (prose_service_auth_1->prose_direct_discovery_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseDirectDiscoveryAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_direct_discovery_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_direct_discovery_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_direct_communication_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseDirectCommunicationAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_direct_communication_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_direct_communication_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l2_relay_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL2RelayAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l2_relay_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l2_relay_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l3_relay_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL3RelayAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l3_relay_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l3_relay_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l2_remote_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL2RemoteAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l2_remote_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l2_remote_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l3_remote_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL3RemoteAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l3_remote_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l3_remote_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_multipath_com_l2_remote_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseMultipathComL2RemoteAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_multipath_com_l2_remote_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_multipath_com_l2_remote_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l2_ue_relay_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL2UeRelayAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l2_ue_relay_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l2_ue_relay_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l3_ue_relay_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL3UeRelayAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l3_ue_relay_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l3_ue_relay_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l2_end_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL2EndAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l2_end_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l2_end_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l3_end_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL3EndAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l3_end_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l3_end_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l3_intermediate_relay_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL3IntermediateRelayAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l3_intermediate_relay_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l3_intermediate_relay_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l3_remote_multihop_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL3RemoteMultihopAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l3_remote_multihop_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l3_remote_multihop_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l3_relay_multihop_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL3RelayMultihopAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l3_relay_multihop_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l3_relay_multihop_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l3_ue_relay_multihop_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL3UeRelayMultihopAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l3_ue_relay_multihop_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l3_ue_relay_multihop_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l3_end_multihop_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL3EndMultihopAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l3_end_multihop_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l3_end_multihop_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l2_intermediate_relay_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL2IntermediateRelayAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l2_intermediate_relay_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l2_intermediate_relay_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l2_remote_multihop_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL2RemoteMultihopAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l2_remote_multihop_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l2_remote_multihop_auth]");
        goto end;
    }
    }

    if (prose_service_auth_1->prose_l2_relay_multihop_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "proseL2RelayMultihopAuth", OpenAPI_ue_auth_ToString(prose_service_auth_1->prose_l2_relay_multihop_auth)) == NULL) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed [prose_l2_relay_multihop_auth]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_prose_service_auth_1_t *OpenAPI_prose_service_auth_1_parseFromJSON(cJSON *prose_service_auth_1JSON)
{
    OpenAPI_prose_service_auth_1_t *prose_service_auth_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *prose_direct_discovery_auth = NULL;
    OpenAPI_ue_auth_e prose_direct_discovery_authVariable = 0;
    cJSON *prose_direct_communication_auth = NULL;
    OpenAPI_ue_auth_e prose_direct_communication_authVariable = 0;
    cJSON *prose_l2_relay_auth = NULL;
    OpenAPI_ue_auth_e prose_l2_relay_authVariable = 0;
    cJSON *prose_l3_relay_auth = NULL;
    OpenAPI_ue_auth_e prose_l3_relay_authVariable = 0;
    cJSON *prose_l2_remote_auth = NULL;
    OpenAPI_ue_auth_e prose_l2_remote_authVariable = 0;
    cJSON *prose_l3_remote_auth = NULL;
    OpenAPI_ue_auth_e prose_l3_remote_authVariable = 0;
    cJSON *prose_multipath_com_l2_remote_auth = NULL;
    OpenAPI_ue_auth_e prose_multipath_com_l2_remote_authVariable = 0;
    cJSON *prose_l2_ue_relay_auth = NULL;
    OpenAPI_ue_auth_e prose_l2_ue_relay_authVariable = 0;
    cJSON *prose_l3_ue_relay_auth = NULL;
    OpenAPI_ue_auth_e prose_l3_ue_relay_authVariable = 0;
    cJSON *prose_l2_end_auth = NULL;
    OpenAPI_ue_auth_e prose_l2_end_authVariable = 0;
    cJSON *prose_l3_end_auth = NULL;
    OpenAPI_ue_auth_e prose_l3_end_authVariable = 0;
    cJSON *prose_l3_intermediate_relay_auth = NULL;
    OpenAPI_ue_auth_e prose_l3_intermediate_relay_authVariable = 0;
    cJSON *prose_l3_remote_multihop_auth = NULL;
    OpenAPI_ue_auth_e prose_l3_remote_multihop_authVariable = 0;
    cJSON *prose_l3_relay_multihop_auth = NULL;
    OpenAPI_ue_auth_e prose_l3_relay_multihop_authVariable = 0;
    cJSON *prose_l3_ue_relay_multihop_auth = NULL;
    OpenAPI_ue_auth_e prose_l3_ue_relay_multihop_authVariable = 0;
    cJSON *prose_l3_end_multihop_auth = NULL;
    OpenAPI_ue_auth_e prose_l3_end_multihop_authVariable = 0;
    cJSON *prose_l2_intermediate_relay_auth = NULL;
    OpenAPI_ue_auth_e prose_l2_intermediate_relay_authVariable = 0;
    cJSON *prose_l2_remote_multihop_auth = NULL;
    OpenAPI_ue_auth_e prose_l2_remote_multihop_authVariable = 0;
    cJSON *prose_l2_relay_multihop_auth = NULL;
    OpenAPI_ue_auth_e prose_l2_relay_multihop_authVariable = 0;
    prose_direct_discovery_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseDirectDiscoveryAuth");
    if (prose_direct_discovery_auth) {
    if (!cJSON_IsString(prose_direct_discovery_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_direct_discovery_auth]");
        goto end;
    }
    prose_direct_discovery_authVariable = OpenAPI_ue_auth_FromString(prose_direct_discovery_auth->valuestring);
    }

    prose_direct_communication_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseDirectCommunicationAuth");
    if (prose_direct_communication_auth) {
    if (!cJSON_IsString(prose_direct_communication_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_direct_communication_auth]");
        goto end;
    }
    prose_direct_communication_authVariable = OpenAPI_ue_auth_FromString(prose_direct_communication_auth->valuestring);
    }

    prose_l2_relay_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL2RelayAuth");
    if (prose_l2_relay_auth) {
    if (!cJSON_IsString(prose_l2_relay_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l2_relay_auth]");
        goto end;
    }
    prose_l2_relay_authVariable = OpenAPI_ue_auth_FromString(prose_l2_relay_auth->valuestring);
    }

    prose_l3_relay_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL3RelayAuth");
    if (prose_l3_relay_auth) {
    if (!cJSON_IsString(prose_l3_relay_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l3_relay_auth]");
        goto end;
    }
    prose_l3_relay_authVariable = OpenAPI_ue_auth_FromString(prose_l3_relay_auth->valuestring);
    }

    prose_l2_remote_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL2RemoteAuth");
    if (prose_l2_remote_auth) {
    if (!cJSON_IsString(prose_l2_remote_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l2_remote_auth]");
        goto end;
    }
    prose_l2_remote_authVariable = OpenAPI_ue_auth_FromString(prose_l2_remote_auth->valuestring);
    }

    prose_l3_remote_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL3RemoteAuth");
    if (prose_l3_remote_auth) {
    if (!cJSON_IsString(prose_l3_remote_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l3_remote_auth]");
        goto end;
    }
    prose_l3_remote_authVariable = OpenAPI_ue_auth_FromString(prose_l3_remote_auth->valuestring);
    }

    prose_multipath_com_l2_remote_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseMultipathComL2RemoteAuth");
    if (prose_multipath_com_l2_remote_auth) {
    if (!cJSON_IsString(prose_multipath_com_l2_remote_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_multipath_com_l2_remote_auth]");
        goto end;
    }
    prose_multipath_com_l2_remote_authVariable = OpenAPI_ue_auth_FromString(prose_multipath_com_l2_remote_auth->valuestring);
    }

    prose_l2_ue_relay_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL2UeRelayAuth");
    if (prose_l2_ue_relay_auth) {
    if (!cJSON_IsString(prose_l2_ue_relay_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l2_ue_relay_auth]");
        goto end;
    }
    prose_l2_ue_relay_authVariable = OpenAPI_ue_auth_FromString(prose_l2_ue_relay_auth->valuestring);
    }

    prose_l3_ue_relay_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL3UeRelayAuth");
    if (prose_l3_ue_relay_auth) {
    if (!cJSON_IsString(prose_l3_ue_relay_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l3_ue_relay_auth]");
        goto end;
    }
    prose_l3_ue_relay_authVariable = OpenAPI_ue_auth_FromString(prose_l3_ue_relay_auth->valuestring);
    }

    prose_l2_end_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL2EndAuth");
    if (prose_l2_end_auth) {
    if (!cJSON_IsString(prose_l2_end_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l2_end_auth]");
        goto end;
    }
    prose_l2_end_authVariable = OpenAPI_ue_auth_FromString(prose_l2_end_auth->valuestring);
    }

    prose_l3_end_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL3EndAuth");
    if (prose_l3_end_auth) {
    if (!cJSON_IsString(prose_l3_end_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l3_end_auth]");
        goto end;
    }
    prose_l3_end_authVariable = OpenAPI_ue_auth_FromString(prose_l3_end_auth->valuestring);
    }

    prose_l3_intermediate_relay_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL3IntermediateRelayAuth");
    if (prose_l3_intermediate_relay_auth) {
    if (!cJSON_IsString(prose_l3_intermediate_relay_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l3_intermediate_relay_auth]");
        goto end;
    }
    prose_l3_intermediate_relay_authVariable = OpenAPI_ue_auth_FromString(prose_l3_intermediate_relay_auth->valuestring);
    }

    prose_l3_remote_multihop_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL3RemoteMultihopAuth");
    if (prose_l3_remote_multihop_auth) {
    if (!cJSON_IsString(prose_l3_remote_multihop_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l3_remote_multihop_auth]");
        goto end;
    }
    prose_l3_remote_multihop_authVariable = OpenAPI_ue_auth_FromString(prose_l3_remote_multihop_auth->valuestring);
    }

    prose_l3_relay_multihop_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL3RelayMultihopAuth");
    if (prose_l3_relay_multihop_auth) {
    if (!cJSON_IsString(prose_l3_relay_multihop_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l3_relay_multihop_auth]");
        goto end;
    }
    prose_l3_relay_multihop_authVariable = OpenAPI_ue_auth_FromString(prose_l3_relay_multihop_auth->valuestring);
    }

    prose_l3_ue_relay_multihop_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL3UeRelayMultihopAuth");
    if (prose_l3_ue_relay_multihop_auth) {
    if (!cJSON_IsString(prose_l3_ue_relay_multihop_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l3_ue_relay_multihop_auth]");
        goto end;
    }
    prose_l3_ue_relay_multihop_authVariable = OpenAPI_ue_auth_FromString(prose_l3_ue_relay_multihop_auth->valuestring);
    }

    prose_l3_end_multihop_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL3EndMultihopAuth");
    if (prose_l3_end_multihop_auth) {
    if (!cJSON_IsString(prose_l3_end_multihop_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l3_end_multihop_auth]");
        goto end;
    }
    prose_l3_end_multihop_authVariable = OpenAPI_ue_auth_FromString(prose_l3_end_multihop_auth->valuestring);
    }

    prose_l2_intermediate_relay_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL2IntermediateRelayAuth");
    if (prose_l2_intermediate_relay_auth) {
    if (!cJSON_IsString(prose_l2_intermediate_relay_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l2_intermediate_relay_auth]");
        goto end;
    }
    prose_l2_intermediate_relay_authVariable = OpenAPI_ue_auth_FromString(prose_l2_intermediate_relay_auth->valuestring);
    }

    prose_l2_remote_multihop_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL2RemoteMultihopAuth");
    if (prose_l2_remote_multihop_auth) {
    if (!cJSON_IsString(prose_l2_remote_multihop_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l2_remote_multihop_auth]");
        goto end;
    }
    prose_l2_remote_multihop_authVariable = OpenAPI_ue_auth_FromString(prose_l2_remote_multihop_auth->valuestring);
    }

    prose_l2_relay_multihop_auth = cJSON_GetObjectItemCaseSensitive(prose_service_auth_1JSON, "proseL2RelayMultihopAuth");
    if (prose_l2_relay_multihop_auth) {
    if (!cJSON_IsString(prose_l2_relay_multihop_auth)) {
        ogs_error("OpenAPI_prose_service_auth_1_parseFromJSON() failed [prose_l2_relay_multihop_auth]");
        goto end;
    }
    prose_l2_relay_multihop_authVariable = OpenAPI_ue_auth_FromString(prose_l2_relay_multihop_auth->valuestring);
    }

    prose_service_auth_1_local_var = OpenAPI_prose_service_auth_1_create (
        prose_direct_discovery_auth ? prose_direct_discovery_authVariable : 0,
        prose_direct_communication_auth ? prose_direct_communication_authVariable : 0,
        prose_l2_relay_auth ? prose_l2_relay_authVariable : 0,
        prose_l3_relay_auth ? prose_l3_relay_authVariable : 0,
        prose_l2_remote_auth ? prose_l2_remote_authVariable : 0,
        prose_l3_remote_auth ? prose_l3_remote_authVariable : 0,
        prose_multipath_com_l2_remote_auth ? prose_multipath_com_l2_remote_authVariable : 0,
        prose_l2_ue_relay_auth ? prose_l2_ue_relay_authVariable : 0,
        prose_l3_ue_relay_auth ? prose_l3_ue_relay_authVariable : 0,
        prose_l2_end_auth ? prose_l2_end_authVariable : 0,
        prose_l3_end_auth ? prose_l3_end_authVariable : 0,
        prose_l3_intermediate_relay_auth ? prose_l3_intermediate_relay_authVariable : 0,
        prose_l3_remote_multihop_auth ? prose_l3_remote_multihop_authVariable : 0,
        prose_l3_relay_multihop_auth ? prose_l3_relay_multihop_authVariable : 0,
        prose_l3_ue_relay_multihop_auth ? prose_l3_ue_relay_multihop_authVariable : 0,
        prose_l3_end_multihop_auth ? prose_l3_end_multihop_authVariable : 0,
        prose_l2_intermediate_relay_auth ? prose_l2_intermediate_relay_authVariable : 0,
        prose_l2_remote_multihop_auth ? prose_l2_remote_multihop_authVariable : 0,
        prose_l2_relay_multihop_auth ? prose_l2_relay_multihop_authVariable : 0
    );

    return prose_service_auth_1_local_var;
end:
    return NULL;
}

OpenAPI_prose_service_auth_1_t *OpenAPI_prose_service_auth_1_copy(OpenAPI_prose_service_auth_1_t *dst, OpenAPI_prose_service_auth_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_prose_service_auth_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_prose_service_auth_1_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_prose_service_auth_1_free(dst);
    dst = OpenAPI_prose_service_auth_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


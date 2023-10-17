
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_capability.h"

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
)
{
    OpenAPI_pro_se_capability_t *pro_se_capability_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_capability_t));
    ogs_assert(pro_se_capability_local_var);

    pro_se_capability_local_var->is_prose_direct_discovey = is_prose_direct_discovey;
    pro_se_capability_local_var->prose_direct_discovey = prose_direct_discovey;
    pro_se_capability_local_var->is_prose_direct_communication = is_prose_direct_communication;
    pro_se_capability_local_var->prose_direct_communication = prose_direct_communication;
    pro_se_capability_local_var->is_prose_l2_ueto_network_relay = is_prose_l2_ueto_network_relay;
    pro_se_capability_local_var->prose_l2_ueto_network_relay = prose_l2_ueto_network_relay;
    pro_se_capability_local_var->is_prose_l3_ueto_network_relay = is_prose_l3_ueto_network_relay;
    pro_se_capability_local_var->prose_l3_ueto_network_relay = prose_l3_ueto_network_relay;
    pro_se_capability_local_var->is_prose_l2_remote_ue = is_prose_l2_remote_ue;
    pro_se_capability_local_var->prose_l2_remote_ue = prose_l2_remote_ue;
    pro_se_capability_local_var->is_prose_l3_remote_ue = is_prose_l3_remote_ue;
    pro_se_capability_local_var->prose_l3_remote_ue = prose_l3_remote_ue;

    return pro_se_capability_local_var;
}

void OpenAPI_pro_se_capability_free(OpenAPI_pro_se_capability_t *pro_se_capability)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_capability) {
        return;
    }
    ogs_free(pro_se_capability);
}

cJSON *OpenAPI_pro_se_capability_convertToJSON(OpenAPI_pro_se_capability_t *pro_se_capability)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_capability == NULL) {
        ogs_error("OpenAPI_pro_se_capability_convertToJSON() failed [ProSeCapability]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pro_se_capability->is_prose_direct_discovey) {
    if (cJSON_AddBoolToObject(item, "proseDirectDiscovey", pro_se_capability->prose_direct_discovey) == NULL) {
        ogs_error("OpenAPI_pro_se_capability_convertToJSON() failed [prose_direct_discovey]");
        goto end;
    }
    }

    if (pro_se_capability->is_prose_direct_communication) {
    if (cJSON_AddBoolToObject(item, "proseDirectCommunication", pro_se_capability->prose_direct_communication) == NULL) {
        ogs_error("OpenAPI_pro_se_capability_convertToJSON() failed [prose_direct_communication]");
        goto end;
    }
    }

    if (pro_se_capability->is_prose_l2_ueto_network_relay) {
    if (cJSON_AddBoolToObject(item, "proseL2UetoNetworkRelay", pro_se_capability->prose_l2_ueto_network_relay) == NULL) {
        ogs_error("OpenAPI_pro_se_capability_convertToJSON() failed [prose_l2_ueto_network_relay]");
        goto end;
    }
    }

    if (pro_se_capability->is_prose_l3_ueto_network_relay) {
    if (cJSON_AddBoolToObject(item, "proseL3UetoNetworkRelay", pro_se_capability->prose_l3_ueto_network_relay) == NULL) {
        ogs_error("OpenAPI_pro_se_capability_convertToJSON() failed [prose_l3_ueto_network_relay]");
        goto end;
    }
    }

    if (pro_se_capability->is_prose_l2_remote_ue) {
    if (cJSON_AddBoolToObject(item, "proseL2RemoteUe", pro_se_capability->prose_l2_remote_ue) == NULL) {
        ogs_error("OpenAPI_pro_se_capability_convertToJSON() failed [prose_l2_remote_ue]");
        goto end;
    }
    }

    if (pro_se_capability->is_prose_l3_remote_ue) {
    if (cJSON_AddBoolToObject(item, "proseL3RemoteUe", pro_se_capability->prose_l3_remote_ue) == NULL) {
        ogs_error("OpenAPI_pro_se_capability_convertToJSON() failed [prose_l3_remote_ue]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pro_se_capability_t *OpenAPI_pro_se_capability_parseFromJSON(cJSON *pro_se_capabilityJSON)
{
    OpenAPI_pro_se_capability_t *pro_se_capability_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *prose_direct_discovey = NULL;
    cJSON *prose_direct_communication = NULL;
    cJSON *prose_l2_ueto_network_relay = NULL;
    cJSON *prose_l3_ueto_network_relay = NULL;
    cJSON *prose_l2_remote_ue = NULL;
    cJSON *prose_l3_remote_ue = NULL;
    prose_direct_discovey = cJSON_GetObjectItemCaseSensitive(pro_se_capabilityJSON, "proseDirectDiscovey");
    if (prose_direct_discovey) {
    if (!cJSON_IsBool(prose_direct_discovey)) {
        ogs_error("OpenAPI_pro_se_capability_parseFromJSON() failed [prose_direct_discovey]");
        goto end;
    }
    }

    prose_direct_communication = cJSON_GetObjectItemCaseSensitive(pro_se_capabilityJSON, "proseDirectCommunication");
    if (prose_direct_communication) {
    if (!cJSON_IsBool(prose_direct_communication)) {
        ogs_error("OpenAPI_pro_se_capability_parseFromJSON() failed [prose_direct_communication]");
        goto end;
    }
    }

    prose_l2_ueto_network_relay = cJSON_GetObjectItemCaseSensitive(pro_se_capabilityJSON, "proseL2UetoNetworkRelay");
    if (prose_l2_ueto_network_relay) {
    if (!cJSON_IsBool(prose_l2_ueto_network_relay)) {
        ogs_error("OpenAPI_pro_se_capability_parseFromJSON() failed [prose_l2_ueto_network_relay]");
        goto end;
    }
    }

    prose_l3_ueto_network_relay = cJSON_GetObjectItemCaseSensitive(pro_se_capabilityJSON, "proseL3UetoNetworkRelay");
    if (prose_l3_ueto_network_relay) {
    if (!cJSON_IsBool(prose_l3_ueto_network_relay)) {
        ogs_error("OpenAPI_pro_se_capability_parseFromJSON() failed [prose_l3_ueto_network_relay]");
        goto end;
    }
    }

    prose_l2_remote_ue = cJSON_GetObjectItemCaseSensitive(pro_se_capabilityJSON, "proseL2RemoteUe");
    if (prose_l2_remote_ue) {
    if (!cJSON_IsBool(prose_l2_remote_ue)) {
        ogs_error("OpenAPI_pro_se_capability_parseFromJSON() failed [prose_l2_remote_ue]");
        goto end;
    }
    }

    prose_l3_remote_ue = cJSON_GetObjectItemCaseSensitive(pro_se_capabilityJSON, "proseL3RemoteUe");
    if (prose_l3_remote_ue) {
    if (!cJSON_IsBool(prose_l3_remote_ue)) {
        ogs_error("OpenAPI_pro_se_capability_parseFromJSON() failed [prose_l3_remote_ue]");
        goto end;
    }
    }

    pro_se_capability_local_var = OpenAPI_pro_se_capability_create (
        prose_direct_discovey ? true : false,
        prose_direct_discovey ? prose_direct_discovey->valueint : 0,
        prose_direct_communication ? true : false,
        prose_direct_communication ? prose_direct_communication->valueint : 0,
        prose_l2_ueto_network_relay ? true : false,
        prose_l2_ueto_network_relay ? prose_l2_ueto_network_relay->valueint : 0,
        prose_l3_ueto_network_relay ? true : false,
        prose_l3_ueto_network_relay ? prose_l3_ueto_network_relay->valueint : 0,
        prose_l2_remote_ue ? true : false,
        prose_l2_remote_ue ? prose_l2_remote_ue->valueint : 0,
        prose_l3_remote_ue ? true : false,
        prose_l3_remote_ue ? prose_l3_remote_ue->valueint : 0
    );

    return pro_se_capability_local_var;
end:
    return NULL;
}

OpenAPI_pro_se_capability_t *OpenAPI_pro_se_capability_copy(OpenAPI_pro_se_capability_t *dst, OpenAPI_pro_se_capability_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_capability_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_capability_convertToJSON() failed");
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

    OpenAPI_pro_se_capability_free(dst);
    dst = OpenAPI_pro_se_capability_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


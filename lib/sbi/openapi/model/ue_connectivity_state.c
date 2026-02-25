
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_connectivity_state.h"

OpenAPI_ue_connectivity_state_t *OpenAPI_ue_connectivity_state_create(
    OpenAPI_access_type_e access_type,
    OpenAPI_cm_state_t *connectivitystate
)
{
    OpenAPI_ue_connectivity_state_t *ue_connectivity_state_local_var = ogs_malloc(sizeof(OpenAPI_ue_connectivity_state_t));
    ogs_assert(ue_connectivity_state_local_var);

    ue_connectivity_state_local_var->access_type = access_type;
    ue_connectivity_state_local_var->connectivitystate = connectivitystate;

    return ue_connectivity_state_local_var;
}

void OpenAPI_ue_connectivity_state_free(OpenAPI_ue_connectivity_state_t *ue_connectivity_state)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_connectivity_state) {
        return;
    }
    if (ue_connectivity_state->connectivitystate) {
        OpenAPI_cm_state_free(ue_connectivity_state->connectivitystate);
        ue_connectivity_state->connectivitystate = NULL;
    }
    ogs_free(ue_connectivity_state);
}

cJSON *OpenAPI_ue_connectivity_state_convertToJSON(OpenAPI_ue_connectivity_state_t *ue_connectivity_state)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_connectivity_state == NULL) {
        ogs_error("OpenAPI_ue_connectivity_state_convertToJSON() failed [UeConnectivityState]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_connectivity_state->access_type == OpenAPI_access_type_NULL) {
        ogs_error("OpenAPI_ue_connectivity_state_convertToJSON() failed [access_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(ue_connectivity_state->access_type)) == NULL) {
        ogs_error("OpenAPI_ue_connectivity_state_convertToJSON() failed [access_type]");
        goto end;
    }

    if (ue_connectivity_state->connectivitystate) {
    cJSON *connectivitystate_local_JSON = OpenAPI_cm_state_convertToJSON(ue_connectivity_state->connectivitystate);
    if (connectivitystate_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_connectivity_state_convertToJSON() failed [connectivitystate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "connectivitystate", connectivitystate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_connectivity_state_convertToJSON() failed [connectivitystate]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_connectivity_state_t *OpenAPI_ue_connectivity_state_parseFromJSON(cJSON *ue_connectivity_stateJSON)
{
    OpenAPI_ue_connectivity_state_t *ue_connectivity_state_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *connectivitystate = NULL;
    OpenAPI_cm_state_t *connectivitystate_local_nonprim = NULL;
    access_type = cJSON_GetObjectItemCaseSensitive(ue_connectivity_stateJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_ue_connectivity_state_parseFromJSON() failed [access_type]");
        goto end;
    }
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_ue_connectivity_state_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    connectivitystate = cJSON_GetObjectItemCaseSensitive(ue_connectivity_stateJSON, "connectivitystate");
    if (connectivitystate) {
    connectivitystate_local_nonprim = OpenAPI_cm_state_parseFromJSON(connectivitystate);
    if (!connectivitystate_local_nonprim) {
        ogs_error("OpenAPI_cm_state_parseFromJSON failed [connectivitystate]");
        goto end;
    }
    }

    ue_connectivity_state_local_var = OpenAPI_ue_connectivity_state_create (
        access_typeVariable,
        connectivitystate ? connectivitystate_local_nonprim : NULL
    );

    return ue_connectivity_state_local_var;
end:
    if (connectivitystate_local_nonprim) {
        OpenAPI_cm_state_free(connectivitystate_local_nonprim);
        connectivitystate_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_connectivity_state_t *OpenAPI_ue_connectivity_state_copy(OpenAPI_ue_connectivity_state_t *dst, OpenAPI_ue_connectivity_state_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_connectivity_state_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_connectivity_state_convertToJSON() failed");
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

    OpenAPI_ue_connectivity_state_free(dst);
    dst = OpenAPI_ue_connectivity_state_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


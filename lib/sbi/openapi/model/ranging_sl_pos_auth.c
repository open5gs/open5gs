
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ranging_sl_pos_auth.h"

OpenAPI_ranging_sl_pos_auth_t *OpenAPI_ranging_sl_pos_auth_create(
    OpenAPI_ue_auth_e rg_sl_pos_pc5_auth,
    OpenAPI_ue_auth_e rg_sl_pos_loc_auth,
    OpenAPI_ue_auth_e rg_sl_pos_client_auth,
    OpenAPI_ue_auth_e rg_sl_pos_server_auth
)
{
    OpenAPI_ranging_sl_pos_auth_t *ranging_sl_pos_auth_local_var = ogs_malloc(sizeof(OpenAPI_ranging_sl_pos_auth_t));
    ogs_assert(ranging_sl_pos_auth_local_var);

    ranging_sl_pos_auth_local_var->rg_sl_pos_pc5_auth = rg_sl_pos_pc5_auth;
    ranging_sl_pos_auth_local_var->rg_sl_pos_loc_auth = rg_sl_pos_loc_auth;
    ranging_sl_pos_auth_local_var->rg_sl_pos_client_auth = rg_sl_pos_client_auth;
    ranging_sl_pos_auth_local_var->rg_sl_pos_server_auth = rg_sl_pos_server_auth;

    return ranging_sl_pos_auth_local_var;
}

void OpenAPI_ranging_sl_pos_auth_free(OpenAPI_ranging_sl_pos_auth_t *ranging_sl_pos_auth)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ranging_sl_pos_auth) {
        return;
    }
    ogs_free(ranging_sl_pos_auth);
}

cJSON *OpenAPI_ranging_sl_pos_auth_convertToJSON(OpenAPI_ranging_sl_pos_auth_t *ranging_sl_pos_auth)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ranging_sl_pos_auth == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_convertToJSON() failed [RangingSlPosAuth]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ranging_sl_pos_auth->rg_sl_pos_pc5_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "rgSlPosPc5Auth", OpenAPI_ue_auth_ToString(ranging_sl_pos_auth->rg_sl_pos_pc5_auth)) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_convertToJSON() failed [rg_sl_pos_pc5_auth]");
        goto end;
    }
    }

    if (ranging_sl_pos_auth->rg_sl_pos_loc_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "rgSlPosLocAuth", OpenAPI_ue_auth_ToString(ranging_sl_pos_auth->rg_sl_pos_loc_auth)) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_convertToJSON() failed [rg_sl_pos_loc_auth]");
        goto end;
    }
    }

    if (ranging_sl_pos_auth->rg_sl_pos_client_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "rgSlPosClientAuth", OpenAPI_ue_auth_ToString(ranging_sl_pos_auth->rg_sl_pos_client_auth)) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_convertToJSON() failed [rg_sl_pos_client_auth]");
        goto end;
    }
    }

    if (ranging_sl_pos_auth->rg_sl_pos_server_auth != OpenAPI_ue_auth_NULL) {
    if (cJSON_AddStringToObject(item, "rgSlPosServerAuth", OpenAPI_ue_auth_ToString(ranging_sl_pos_auth->rg_sl_pos_server_auth)) == NULL) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_convertToJSON() failed [rg_sl_pos_server_auth]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ranging_sl_pos_auth_t *OpenAPI_ranging_sl_pos_auth_parseFromJSON(cJSON *ranging_sl_pos_authJSON)
{
    OpenAPI_ranging_sl_pos_auth_t *ranging_sl_pos_auth_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rg_sl_pos_pc5_auth = NULL;
    OpenAPI_ue_auth_e rg_sl_pos_pc5_authVariable = 0;
    cJSON *rg_sl_pos_loc_auth = NULL;
    OpenAPI_ue_auth_e rg_sl_pos_loc_authVariable = 0;
    cJSON *rg_sl_pos_client_auth = NULL;
    OpenAPI_ue_auth_e rg_sl_pos_client_authVariable = 0;
    cJSON *rg_sl_pos_server_auth = NULL;
    OpenAPI_ue_auth_e rg_sl_pos_server_authVariable = 0;
    rg_sl_pos_pc5_auth = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_authJSON, "rgSlPosPc5Auth");
    if (rg_sl_pos_pc5_auth) {
    if (!cJSON_IsString(rg_sl_pos_pc5_auth)) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_parseFromJSON() failed [rg_sl_pos_pc5_auth]");
        goto end;
    }
    rg_sl_pos_pc5_authVariable = OpenAPI_ue_auth_FromString(rg_sl_pos_pc5_auth->valuestring);
    }

    rg_sl_pos_loc_auth = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_authJSON, "rgSlPosLocAuth");
    if (rg_sl_pos_loc_auth) {
    if (!cJSON_IsString(rg_sl_pos_loc_auth)) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_parseFromJSON() failed [rg_sl_pos_loc_auth]");
        goto end;
    }
    rg_sl_pos_loc_authVariable = OpenAPI_ue_auth_FromString(rg_sl_pos_loc_auth->valuestring);
    }

    rg_sl_pos_client_auth = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_authJSON, "rgSlPosClientAuth");
    if (rg_sl_pos_client_auth) {
    if (!cJSON_IsString(rg_sl_pos_client_auth)) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_parseFromJSON() failed [rg_sl_pos_client_auth]");
        goto end;
    }
    rg_sl_pos_client_authVariable = OpenAPI_ue_auth_FromString(rg_sl_pos_client_auth->valuestring);
    }

    rg_sl_pos_server_auth = cJSON_GetObjectItemCaseSensitive(ranging_sl_pos_authJSON, "rgSlPosServerAuth");
    if (rg_sl_pos_server_auth) {
    if (!cJSON_IsString(rg_sl_pos_server_auth)) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_parseFromJSON() failed [rg_sl_pos_server_auth]");
        goto end;
    }
    rg_sl_pos_server_authVariable = OpenAPI_ue_auth_FromString(rg_sl_pos_server_auth->valuestring);
    }

    ranging_sl_pos_auth_local_var = OpenAPI_ranging_sl_pos_auth_create (
        rg_sl_pos_pc5_auth ? rg_sl_pos_pc5_authVariable : 0,
        rg_sl_pos_loc_auth ? rg_sl_pos_loc_authVariable : 0,
        rg_sl_pos_client_auth ? rg_sl_pos_client_authVariable : 0,
        rg_sl_pos_server_auth ? rg_sl_pos_server_authVariable : 0
    );

    return ranging_sl_pos_auth_local_var;
end:
    return NULL;
}

OpenAPI_ranging_sl_pos_auth_t *OpenAPI_ranging_sl_pos_auth_copy(OpenAPI_ranging_sl_pos_auth_t *dst, OpenAPI_ranging_sl_pos_auth_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ranging_sl_pos_auth_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ranging_sl_pos_auth_convertToJSON() failed");
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

    OpenAPI_ranging_sl_pos_auth_free(dst);
    dst = OpenAPI_ranging_sl_pos_auth_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


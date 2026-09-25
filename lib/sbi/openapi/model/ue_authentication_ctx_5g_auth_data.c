
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_authentication_ctx_5g_auth_data.h"

OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_create(
    OpenAPI_av5g_aka_t *av5g_aka,
    char *eap_payload
)
{
    OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_authentication_ctx_5g_auth_data_t));
    ogs_assert(ue_authentication_ctx_5g_auth_data_local_var);

    ue_authentication_ctx_5g_auth_data_local_var->av5g_aka = av5g_aka;
    ue_authentication_ctx_5g_auth_data_local_var->eap_payload = eap_payload;

    return ue_authentication_ctx_5g_auth_data_local_var;
}

void OpenAPI_ue_authentication_ctx_5g_auth_data_free(OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_authentication_ctx_5g_auth_data) {
        return;
    }
    if (ue_authentication_ctx_5g_auth_data->av5g_aka) {
        OpenAPI_av5g_aka_free(ue_authentication_ctx_5g_auth_data->av5g_aka);
        ue_authentication_ctx_5g_auth_data->av5g_aka = NULL;
    }
    if (ue_authentication_ctx_5g_auth_data->eap_payload) {
        ogs_free(ue_authentication_ctx_5g_auth_data->eap_payload);
        ue_authentication_ctx_5g_auth_data->eap_payload = NULL;
    }
    ogs_free(ue_authentication_ctx_5g_auth_data);
}

/*
 * oneOf union (x-open5gs-union): the members are the alternatives of the
 * original oneOf. Exactly one is set, and it is the JSON value itself
 * rather than a property of a wrapper object.
 */
cJSON *OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON(OpenAPI_ue_authentication_ctx_5g_auth_data_t *ue_authentication_ctx_5g_auth_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_authentication_ctx_5g_auth_data == NULL) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [UEAuthenticationCtx_5gAuthData]");
        return NULL;
    }

    if ((ue_authentication_ctx_5g_auth_data->av5g_aka != NULL) +
            (ue_authentication_ctx_5g_auth_data->eap_payload != NULL) != 1) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [UEAuthenticationCtx_5gAuthData]: exactly one alternative must be set");
        return NULL;
    }

    if (ue_authentication_ctx_5g_auth_data->av5g_aka) {
        return OpenAPI_av5g_aka_convertToJSON(ue_authentication_ctx_5g_auth_data->av5g_aka);
    }
    if (ue_authentication_ctx_5g_auth_data->eap_payload) {
        return cJSON_CreateString(ue_authentication_ctx_5g_auth_data->eap_payload);
    }

    ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed [UEAuthenticationCtx_5gAuthData]");
    return NULL;
}

OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON(cJSON *ue_authentication_ctx_5g_auth_dataJSON)
{
    OpenAPI_lnode_t *node = NULL;
    OpenAPI_av5g_aka_t *av5g_aka = NULL;
    char *eap_payload = NULL;

    if (cJSON_IsObject(ue_authentication_ctx_5g_auth_dataJSON)) {
        av5g_aka = OpenAPI_av5g_aka_parseFromJSON(ue_authentication_ctx_5g_auth_dataJSON);
        if (!av5g_aka) {
            ogs_error("OpenAPI_av5g_aka_parseFromJSON failed [av5g_aka]");
            goto end;
        }
    } else if (cJSON_IsString(ue_authentication_ctx_5g_auth_dataJSON)) {
        eap_payload = ogs_strdup(ue_authentication_ctx_5g_auth_dataJSON->valuestring);
        ogs_assert(eap_payload);
    } else {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON() failed [UEAuthenticationCtx_5gAuthData]");
        goto end;
    }

    return OpenAPI_ue_authentication_ctx_5g_auth_data_create(av5g_aka, eap_payload);
end:
    if (av5g_aka) {
        OpenAPI_av5g_aka_free(av5g_aka);
        av5g_aka = NULL;
    }
    if (eap_payload) {
        ogs_free(eap_payload);
        eap_payload = NULL;
    }
    return NULL;
}

OpenAPI_ue_authentication_ctx_5g_auth_data_t *OpenAPI_ue_authentication_ctx_5g_auth_data_copy(OpenAPI_ue_authentication_ctx_5g_auth_data_t *dst, OpenAPI_ue_authentication_ctx_5g_auth_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_authentication_ctx_5g_auth_data_convertToJSON() failed");
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

    OpenAPI_ue_authentication_ctx_5g_auth_data_free(dst);
    dst = OpenAPI_ue_authentication_ctx_5g_auth_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


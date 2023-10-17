
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authorization_info.h"

OpenAPI_authorization_info_t *OpenAPI_authorization_info_create(
    OpenAPI_snssai_t *snssai,
    char *dnn,
    char *mtc_provider_information,
    char *auth_update_callback_uri,
    char *af_id,
    char *nef_id,
    char *validity_time,
    OpenAPI_context_info_t *context_info
)
{
    OpenAPI_authorization_info_t *authorization_info_local_var = ogs_malloc(sizeof(OpenAPI_authorization_info_t));
    ogs_assert(authorization_info_local_var);

    authorization_info_local_var->snssai = snssai;
    authorization_info_local_var->dnn = dnn;
    authorization_info_local_var->mtc_provider_information = mtc_provider_information;
    authorization_info_local_var->auth_update_callback_uri = auth_update_callback_uri;
    authorization_info_local_var->af_id = af_id;
    authorization_info_local_var->nef_id = nef_id;
    authorization_info_local_var->validity_time = validity_time;
    authorization_info_local_var->context_info = context_info;

    return authorization_info_local_var;
}

void OpenAPI_authorization_info_free(OpenAPI_authorization_info_t *authorization_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == authorization_info) {
        return;
    }
    if (authorization_info->snssai) {
        OpenAPI_snssai_free(authorization_info->snssai);
        authorization_info->snssai = NULL;
    }
    if (authorization_info->dnn) {
        ogs_free(authorization_info->dnn);
        authorization_info->dnn = NULL;
    }
    if (authorization_info->mtc_provider_information) {
        ogs_free(authorization_info->mtc_provider_information);
        authorization_info->mtc_provider_information = NULL;
    }
    if (authorization_info->auth_update_callback_uri) {
        ogs_free(authorization_info->auth_update_callback_uri);
        authorization_info->auth_update_callback_uri = NULL;
    }
    if (authorization_info->af_id) {
        ogs_free(authorization_info->af_id);
        authorization_info->af_id = NULL;
    }
    if (authorization_info->nef_id) {
        ogs_free(authorization_info->nef_id);
        authorization_info->nef_id = NULL;
    }
    if (authorization_info->validity_time) {
        ogs_free(authorization_info->validity_time);
        authorization_info->validity_time = NULL;
    }
    if (authorization_info->context_info) {
        OpenAPI_context_info_free(authorization_info->context_info);
        authorization_info->context_info = NULL;
    }
    ogs_free(authorization_info);
}

cJSON *OpenAPI_authorization_info_convertToJSON(OpenAPI_authorization_info_t *authorization_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (authorization_info == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [AuthorizationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!authorization_info->snssai) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(authorization_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [snssai]");
        goto end;
    }

    if (!authorization_info->dnn) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", authorization_info->dnn) == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!authorization_info->mtc_provider_information) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [mtc_provider_information]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", authorization_info->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }

    if (!authorization_info->auth_update_callback_uri) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [auth_update_callback_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "authUpdateCallbackUri", authorization_info->auth_update_callback_uri) == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [auth_update_callback_uri]");
        goto end;
    }

    if (authorization_info->af_id) {
    if (cJSON_AddStringToObject(item, "afId", authorization_info->af_id) == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [af_id]");
        goto end;
    }
    }

    if (authorization_info->nef_id) {
    if (cJSON_AddStringToObject(item, "nefId", authorization_info->nef_id) == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [nef_id]");
        goto end;
    }
    }

    if (authorization_info->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", authorization_info->validity_time) == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (authorization_info->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(authorization_info->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed [context_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_authorization_info_t *OpenAPI_authorization_info_parseFromJSON(cJSON *authorization_infoJSON)
{
    OpenAPI_authorization_info_t *authorization_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *mtc_provider_information = NULL;
    cJSON *auth_update_callback_uri = NULL;
    cJSON *af_id = NULL;
    cJSON *nef_id = NULL;
    cJSON *validity_time = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(authorization_infoJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    dnn = cJSON_GetObjectItemCaseSensitive(authorization_infoJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [dnn]");
        goto end;
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(authorization_infoJSON, "mtcProviderInformation");
    if (!mtc_provider_information) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    if (!cJSON_IsString(mtc_provider_information)) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }

    auth_update_callback_uri = cJSON_GetObjectItemCaseSensitive(authorization_infoJSON, "authUpdateCallbackUri");
    if (!auth_update_callback_uri) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [auth_update_callback_uri]");
        goto end;
    }
    if (!cJSON_IsString(auth_update_callback_uri)) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [auth_update_callback_uri]");
        goto end;
    }

    af_id = cJSON_GetObjectItemCaseSensitive(authorization_infoJSON, "afId");
    if (af_id) {
    if (!cJSON_IsString(af_id) && !cJSON_IsNull(af_id)) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [af_id]");
        goto end;
    }
    }

    nef_id = cJSON_GetObjectItemCaseSensitive(authorization_infoJSON, "nefId");
    if (nef_id) {
    if (!cJSON_IsString(nef_id) && !cJSON_IsNull(nef_id)) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [nef_id]");
        goto end;
    }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(authorization_infoJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_authorization_info_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    context_info = cJSON_GetObjectItemCaseSensitive(authorization_infoJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    if (!context_info_local_nonprim) {
        ogs_error("OpenAPI_context_info_parseFromJSON failed [context_info]");
        goto end;
    }
    }

    authorization_info_local_var = OpenAPI_authorization_info_create (
        snssai_local_nonprim,
        ogs_strdup(dnn->valuestring),
        ogs_strdup(mtc_provider_information->valuestring),
        ogs_strdup(auth_update_callback_uri->valuestring),
        af_id && !cJSON_IsNull(af_id) ? ogs_strdup(af_id->valuestring) : NULL,
        nef_id && !cJSON_IsNull(nef_id) ? ogs_strdup(nef_id->valuestring) : NULL,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL
    );

    return authorization_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (context_info_local_nonprim) {
        OpenAPI_context_info_free(context_info_local_nonprim);
        context_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_authorization_info_t *OpenAPI_authorization_info_copy(OpenAPI_authorization_info_t *dst, OpenAPI_authorization_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authorization_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authorization_info_convertToJSON() failed");
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

    OpenAPI_authorization_info_free(dst);
    dst = OpenAPI_authorization_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


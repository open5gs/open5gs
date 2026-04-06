
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reauth_notification_info.h"

OpenAPI_reauth_notification_info_t *OpenAPI_reauth_notification_info_create(
    char *supi,
    OpenAPI_guami_t *old_guami,
    bool is_ausf_rediscovery_ind,
    int ausf_rediscovery_ind
)
{
    OpenAPI_reauth_notification_info_t *reauth_notification_info_local_var = ogs_malloc(sizeof(OpenAPI_reauth_notification_info_t));
    ogs_assert(reauth_notification_info_local_var);

    reauth_notification_info_local_var->supi = supi;
    reauth_notification_info_local_var->old_guami = old_guami;
    reauth_notification_info_local_var->is_ausf_rediscovery_ind = is_ausf_rediscovery_ind;
    reauth_notification_info_local_var->ausf_rediscovery_ind = ausf_rediscovery_ind;

    return reauth_notification_info_local_var;
}

void OpenAPI_reauth_notification_info_free(OpenAPI_reauth_notification_info_t *reauth_notification_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reauth_notification_info) {
        return;
    }
    if (reauth_notification_info->supi) {
        ogs_free(reauth_notification_info->supi);
        reauth_notification_info->supi = NULL;
    }
    if (reauth_notification_info->old_guami) {
        OpenAPI_guami_free(reauth_notification_info->old_guami);
        reauth_notification_info->old_guami = NULL;
    }
    ogs_free(reauth_notification_info);
}

cJSON *OpenAPI_reauth_notification_info_convertToJSON(OpenAPI_reauth_notification_info_t *reauth_notification_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reauth_notification_info == NULL) {
        ogs_error("OpenAPI_reauth_notification_info_convertToJSON() failed [ReauthNotificationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!reauth_notification_info->supi) {
        ogs_error("OpenAPI_reauth_notification_info_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", reauth_notification_info->supi) == NULL) {
        ogs_error("OpenAPI_reauth_notification_info_convertToJSON() failed [supi]");
        goto end;
    }

    if (reauth_notification_info->old_guami) {
    cJSON *old_guami_local_JSON = OpenAPI_guami_convertToJSON(reauth_notification_info->old_guami);
    if (old_guami_local_JSON == NULL) {
        ogs_error("OpenAPI_reauth_notification_info_convertToJSON() failed [old_guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "oldGuami", old_guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reauth_notification_info_convertToJSON() failed [old_guami]");
        goto end;
    }
    }

    if (reauth_notification_info->is_ausf_rediscovery_ind) {
    if (cJSON_AddBoolToObject(item, "ausfRediscoveryInd", reauth_notification_info->ausf_rediscovery_ind) == NULL) {
        ogs_error("OpenAPI_reauth_notification_info_convertToJSON() failed [ausf_rediscovery_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_reauth_notification_info_t *OpenAPI_reauth_notification_info_parseFromJSON(cJSON *reauth_notification_infoJSON)
{
    OpenAPI_reauth_notification_info_t *reauth_notification_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *old_guami = NULL;
    OpenAPI_guami_t *old_guami_local_nonprim = NULL;
    cJSON *ausf_rediscovery_ind = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(reauth_notification_infoJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_reauth_notification_info_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_reauth_notification_info_parseFromJSON() failed [supi]");
        goto end;
    }

    old_guami = cJSON_GetObjectItemCaseSensitive(reauth_notification_infoJSON, "oldGuami");
    if (old_guami) {
    old_guami_local_nonprim = OpenAPI_guami_parseFromJSON(old_guami);
    if (!old_guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [old_guami]");
        goto end;
    }
    }

    ausf_rediscovery_ind = cJSON_GetObjectItemCaseSensitive(reauth_notification_infoJSON, "ausfRediscoveryInd");
    if (ausf_rediscovery_ind) {
    if (!cJSON_IsBool(ausf_rediscovery_ind)) {
        ogs_error("OpenAPI_reauth_notification_info_parseFromJSON() failed [ausf_rediscovery_ind]");
        goto end;
    }
    }

    reauth_notification_info_local_var = OpenAPI_reauth_notification_info_create (
        ogs_strdup(supi->valuestring),
        old_guami ? old_guami_local_nonprim : NULL,
        ausf_rediscovery_ind ? true : false,
        ausf_rediscovery_ind ? ausf_rediscovery_ind->valueint : 0
    );

    return reauth_notification_info_local_var;
end:
    if (old_guami_local_nonprim) {
        OpenAPI_guami_free(old_guami_local_nonprim);
        old_guami_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_reauth_notification_info_t *OpenAPI_reauth_notification_info_copy(OpenAPI_reauth_notification_info_t *dst, OpenAPI_reauth_notification_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reauth_notification_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reauth_notification_info_convertToJSON() failed");
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

    OpenAPI_reauth_notification_info_free(dst);
    dst = OpenAPI_reauth_notification_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


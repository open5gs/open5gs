
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bdt_data_patch.h"

OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_create(
    OpenAPI_transfer_policy_t *trans_policy,
    OpenAPI_bdt_policy_status_e bdtp_status,
    bool is_warn_notif_enabled,
    int warn_notif_enabled,
    char *notif_uri,
    bool is_energy_ind,
    int energy_ind
)
{
    OpenAPI_bdt_data_patch_t *bdt_data_patch_local_var = ogs_malloc(sizeof(OpenAPI_bdt_data_patch_t));
    ogs_assert(bdt_data_patch_local_var);

    bdt_data_patch_local_var->trans_policy = trans_policy;
    bdt_data_patch_local_var->bdtp_status = bdtp_status;
    bdt_data_patch_local_var->is_warn_notif_enabled = is_warn_notif_enabled;
    bdt_data_patch_local_var->warn_notif_enabled = warn_notif_enabled;
    bdt_data_patch_local_var->notif_uri = notif_uri;
    bdt_data_patch_local_var->is_energy_ind = is_energy_ind;
    bdt_data_patch_local_var->energy_ind = energy_ind;

    return bdt_data_patch_local_var;
}

void OpenAPI_bdt_data_patch_free(OpenAPI_bdt_data_patch_t *bdt_data_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bdt_data_patch) {
        return;
    }
    if (bdt_data_patch->trans_policy) {
        OpenAPI_transfer_policy_free(bdt_data_patch->trans_policy);
        bdt_data_patch->trans_policy = NULL;
    }
    if (bdt_data_patch->notif_uri) {
        ogs_free(bdt_data_patch->notif_uri);
        bdt_data_patch->notif_uri = NULL;
    }
    ogs_free(bdt_data_patch);
}

cJSON *OpenAPI_bdt_data_patch_convertToJSON(OpenAPI_bdt_data_patch_t *bdt_data_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bdt_data_patch == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [BdtDataPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (bdt_data_patch->trans_policy) {
    cJSON *trans_policy_local_JSON = OpenAPI_transfer_policy_convertToJSON(bdt_data_patch->trans_policy);
    if (trans_policy_local_JSON == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [trans_policy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "transPolicy", trans_policy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [trans_policy]");
        goto end;
    }
    }

    if (bdt_data_patch->bdtp_status != OpenAPI_bdt_policy_status_NULL) {
    if (cJSON_AddStringToObject(item, "bdtpStatus", OpenAPI_bdt_policy_status_ToString(bdt_data_patch->bdtp_status)) == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [bdtp_status]");
        goto end;
    }
    }

    if (bdt_data_patch->is_warn_notif_enabled) {
    if (cJSON_AddBoolToObject(item, "warnNotifEnabled", bdt_data_patch->warn_notif_enabled) == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [warn_notif_enabled]");
        goto end;
    }
    }

    if (bdt_data_patch->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", bdt_data_patch->notif_uri) == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [notif_uri]");
        goto end;
    }
    }

    if (bdt_data_patch->is_energy_ind) {
    if (cJSON_AddBoolToObject(item, "energyInd", bdt_data_patch->energy_ind) == NULL) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed [energy_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_parseFromJSON(cJSON *bdt_data_patchJSON)
{
    OpenAPI_bdt_data_patch_t *bdt_data_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *trans_policy = NULL;
    OpenAPI_transfer_policy_t *trans_policy_local_nonprim = NULL;
    cJSON *bdtp_status = NULL;
    OpenAPI_bdt_policy_status_e bdtp_statusVariable = 0;
    cJSON *warn_notif_enabled = NULL;
    cJSON *notif_uri = NULL;
    cJSON *energy_ind = NULL;
    trans_policy = cJSON_GetObjectItemCaseSensitive(bdt_data_patchJSON, "transPolicy");
    if (trans_policy) {
    trans_policy_local_nonprim = OpenAPI_transfer_policy_parseFromJSON(trans_policy);
    if (!trans_policy_local_nonprim) {
        ogs_error("OpenAPI_transfer_policy_parseFromJSON failed [trans_policy]");
        goto end;
    }
    }

    bdtp_status = cJSON_GetObjectItemCaseSensitive(bdt_data_patchJSON, "bdtpStatus");
    if (bdtp_status) {
    if (!cJSON_IsString(bdtp_status)) {
        ogs_error("OpenAPI_bdt_data_patch_parseFromJSON() failed [bdtp_status]");
        goto end;
    }
    bdtp_statusVariable = OpenAPI_bdt_policy_status_FromString(bdtp_status->valuestring);
    }

    warn_notif_enabled = cJSON_GetObjectItemCaseSensitive(bdt_data_patchJSON, "warnNotifEnabled");
    if (warn_notif_enabled) {
    if (!cJSON_IsBool(warn_notif_enabled)) {
        ogs_error("OpenAPI_bdt_data_patch_parseFromJSON() failed [warn_notif_enabled]");
        goto end;
    }
    }

    notif_uri = cJSON_GetObjectItemCaseSensitive(bdt_data_patchJSON, "notifUri");
    if (notif_uri) {
    if (!cJSON_IsString(notif_uri) && !cJSON_IsNull(notif_uri)) {
        ogs_error("OpenAPI_bdt_data_patch_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }

    energy_ind = cJSON_GetObjectItemCaseSensitive(bdt_data_patchJSON, "energyInd");
    if (energy_ind) {
    if (!cJSON_IsBool(energy_ind)) {
        ogs_error("OpenAPI_bdt_data_patch_parseFromJSON() failed [energy_ind]");
        goto end;
    }
    }

    bdt_data_patch_local_var = OpenAPI_bdt_data_patch_create (
        trans_policy ? trans_policy_local_nonprim : NULL,
        bdtp_status ? bdtp_statusVariable : 0,
        warn_notif_enabled ? true : false,
        warn_notif_enabled ? warn_notif_enabled->valueint : 0,
        notif_uri && !cJSON_IsNull(notif_uri) ? ogs_strdup(notif_uri->valuestring) : NULL,
        energy_ind ? true : false,
        energy_ind ? energy_ind->valueint : 0
    );

    return bdt_data_patch_local_var;
end:
    if (trans_policy_local_nonprim) {
        OpenAPI_transfer_policy_free(trans_policy_local_nonprim);
        trans_policy_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_copy(OpenAPI_bdt_data_patch_t *dst, OpenAPI_bdt_data_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bdt_data_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bdt_data_patch_convertToJSON() failed");
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

    OpenAPI_bdt_data_patch_free(dst);
    dst = OpenAPI_bdt_data_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


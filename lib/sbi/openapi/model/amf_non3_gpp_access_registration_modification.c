
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_non3_gpp_access_registration_modification.h"

OpenAPI_amf_non3_gpp_access_registration_modification_t *OpenAPI_amf_non3_gpp_access_registration_modification_create(
    OpenAPI_guami_t *guami,
    bool is_purge_flag,
    int purge_flag,
    char *pei,
    OpenAPI_ims_vo_ps_e ims_vo_ps,
    OpenAPI_list_t *backup_amf_info
)
{
    OpenAPI_amf_non3_gpp_access_registration_modification_t *amf_non3_gpp_access_registration_modification_local_var = ogs_malloc(sizeof(OpenAPI_amf_non3_gpp_access_registration_modification_t));
    ogs_assert(amf_non3_gpp_access_registration_modification_local_var);

    amf_non3_gpp_access_registration_modification_local_var->guami = guami;
    amf_non3_gpp_access_registration_modification_local_var->is_purge_flag = is_purge_flag;
    amf_non3_gpp_access_registration_modification_local_var->purge_flag = purge_flag;
    amf_non3_gpp_access_registration_modification_local_var->pei = pei;
    amf_non3_gpp_access_registration_modification_local_var->ims_vo_ps = ims_vo_ps;
    amf_non3_gpp_access_registration_modification_local_var->backup_amf_info = backup_amf_info;

    return amf_non3_gpp_access_registration_modification_local_var;
}

void OpenAPI_amf_non3_gpp_access_registration_modification_free(OpenAPI_amf_non3_gpp_access_registration_modification_t *amf_non3_gpp_access_registration_modification)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_non3_gpp_access_registration_modification) {
        return;
    }
    if (amf_non3_gpp_access_registration_modification->guami) {
        OpenAPI_guami_free(amf_non3_gpp_access_registration_modification->guami);
        amf_non3_gpp_access_registration_modification->guami = NULL;
    }
    if (amf_non3_gpp_access_registration_modification->pei) {
        ogs_free(amf_non3_gpp_access_registration_modification->pei);
        amf_non3_gpp_access_registration_modification->pei = NULL;
    }
    if (amf_non3_gpp_access_registration_modification->backup_amf_info) {
        OpenAPI_list_for_each(amf_non3_gpp_access_registration_modification->backup_amf_info, node) {
            OpenAPI_backup_amf_info_free(node->data);
        }
        OpenAPI_list_free(amf_non3_gpp_access_registration_modification->backup_amf_info);
        amf_non3_gpp_access_registration_modification->backup_amf_info = NULL;
    }
    ogs_free(amf_non3_gpp_access_registration_modification);
}

cJSON *OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON(OpenAPI_amf_non3_gpp_access_registration_modification_t *amf_non3_gpp_access_registration_modification)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_non3_gpp_access_registration_modification == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed [AmfNon3GppAccessRegistrationModification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!amf_non3_gpp_access_registration_modification->guami) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed [guami]");
        return NULL;
    }
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(amf_non3_gpp_access_registration_modification->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed [guami]");
        goto end;
    }

    if (amf_non3_gpp_access_registration_modification->is_purge_flag) {
    if (cJSON_AddBoolToObject(item, "purgeFlag", amf_non3_gpp_access_registration_modification->purge_flag) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed [purge_flag]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration_modification->pei) {
    if (cJSON_AddStringToObject(item, "pei", amf_non3_gpp_access_registration_modification->pei) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration_modification->ims_vo_ps != OpenAPI_ims_vo_ps_NULL) {
    if (cJSON_AddStringToObject(item, "imsVoPs", OpenAPI_ims_vo_ps_ToString(amf_non3_gpp_access_registration_modification->ims_vo_ps)) == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed [ims_vo_ps]");
        goto end;
    }
    }

    if (amf_non3_gpp_access_registration_modification->backup_amf_info) {
    cJSON *backup_amf_infoList = cJSON_AddArrayToObject(item, "backupAmfInfo");
    if (backup_amf_infoList == NULL) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed [backup_amf_info]");
        goto end;
    }
    OpenAPI_list_for_each(amf_non3_gpp_access_registration_modification->backup_amf_info, node) {
        cJSON *itemLocal = OpenAPI_backup_amf_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed [backup_amf_info]");
            goto end;
        }
        cJSON_AddItemToArray(backup_amf_infoList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_amf_non3_gpp_access_registration_modification_t *OpenAPI_amf_non3_gpp_access_registration_modification_parseFromJSON(cJSON *amf_non3_gpp_access_registration_modificationJSON)
{
    OpenAPI_amf_non3_gpp_access_registration_modification_t *amf_non3_gpp_access_registration_modification_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *purge_flag = NULL;
    cJSON *pei = NULL;
    cJSON *ims_vo_ps = NULL;
    OpenAPI_ims_vo_ps_e ims_vo_psVariable = 0;
    cJSON *backup_amf_info = NULL;
    OpenAPI_list_t *backup_amf_infoList = NULL;
    guami = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registration_modificationJSON, "guami");
    if (!guami) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_parseFromJSON() failed [guami]");
        goto end;
    }
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }

    purge_flag = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registration_modificationJSON, "purgeFlag");
    if (purge_flag) {
    if (!cJSON_IsBool(purge_flag)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_parseFromJSON() failed [purge_flag]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registration_modificationJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    ims_vo_ps = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registration_modificationJSON, "imsVoPs");
    if (ims_vo_ps) {
    if (!cJSON_IsString(ims_vo_ps)) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_parseFromJSON() failed [ims_vo_ps]");
        goto end;
    }
    ims_vo_psVariable = OpenAPI_ims_vo_ps_FromString(ims_vo_ps->valuestring);
    }

    backup_amf_info = cJSON_GetObjectItemCaseSensitive(amf_non3_gpp_access_registration_modificationJSON, "backupAmfInfo");
    if (backup_amf_info) {
        cJSON *backup_amf_info_local = NULL;
        if (!cJSON_IsArray(backup_amf_info)) {
            ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_parseFromJSON() failed [backup_amf_info]");
            goto end;
        }

        backup_amf_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_amf_info_local, backup_amf_info) {
            if (!cJSON_IsObject(backup_amf_info_local)) {
                ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_parseFromJSON() failed [backup_amf_info]");
                goto end;
            }
            OpenAPI_backup_amf_info_t *backup_amf_infoItem = OpenAPI_backup_amf_info_parseFromJSON(backup_amf_info_local);
            if (!backup_amf_infoItem) {
                ogs_error("No backup_amf_infoItem");
                goto end;
            }
            OpenAPI_list_add(backup_amf_infoList, backup_amf_infoItem);
        }
    }

    amf_non3_gpp_access_registration_modification_local_var = OpenAPI_amf_non3_gpp_access_registration_modification_create (
        guami_local_nonprim,
        purge_flag ? true : false,
        purge_flag ? purge_flag->valueint : 0,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        ims_vo_ps ? ims_vo_psVariable : 0,
        backup_amf_info ? backup_amf_infoList : NULL
    );

    return amf_non3_gpp_access_registration_modification_local_var;
end:
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    if (backup_amf_infoList) {
        OpenAPI_list_for_each(backup_amf_infoList, node) {
            OpenAPI_backup_amf_info_free(node->data);
        }
        OpenAPI_list_free(backup_amf_infoList);
        backup_amf_infoList = NULL;
    }
    return NULL;
}

OpenAPI_amf_non3_gpp_access_registration_modification_t *OpenAPI_amf_non3_gpp_access_registration_modification_copy(OpenAPI_amf_non3_gpp_access_registration_modification_t *dst, OpenAPI_amf_non3_gpp_access_registration_modification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_modification_convertToJSON() failed");
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

    OpenAPI_amf_non3_gpp_access_registration_modification_free(dst);
    dst = OpenAPI_amf_non3_gpp_access_registration_modification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


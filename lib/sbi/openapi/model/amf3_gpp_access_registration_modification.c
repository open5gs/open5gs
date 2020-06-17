
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf3_gpp_access_registration_modification.h"

OpenAPI_amf3_gpp_access_registration_modification_t *OpenAPI_amf3_gpp_access_registration_modification_create(
    OpenAPI_guami_t *guami,
    int purge_flag,
    char *pei,
    OpenAPI_ims_vo_ps_t *ims_vo_ps,
    OpenAPI_list_t *backup_amf_info,
    OpenAPI_eps_interworking_info_t *eps_interworking_info,
    int ue_srvcc_capability
    )
{
    OpenAPI_amf3_gpp_access_registration_modification_t *amf3_gpp_access_registration_modification_local_var = OpenAPI_malloc(sizeof(OpenAPI_amf3_gpp_access_registration_modification_t));
    if (!amf3_gpp_access_registration_modification_local_var) {
        return NULL;
    }
    amf3_gpp_access_registration_modification_local_var->guami = guami;
    amf3_gpp_access_registration_modification_local_var->purge_flag = purge_flag;
    amf3_gpp_access_registration_modification_local_var->pei = pei;
    amf3_gpp_access_registration_modification_local_var->ims_vo_ps = ims_vo_ps;
    amf3_gpp_access_registration_modification_local_var->backup_amf_info = backup_amf_info;
    amf3_gpp_access_registration_modification_local_var->eps_interworking_info = eps_interworking_info;
    amf3_gpp_access_registration_modification_local_var->ue_srvcc_capability = ue_srvcc_capability;

    return amf3_gpp_access_registration_modification_local_var;
}

void OpenAPI_amf3_gpp_access_registration_modification_free(OpenAPI_amf3_gpp_access_registration_modification_t *amf3_gpp_access_registration_modification)
{
    if (NULL == amf3_gpp_access_registration_modification) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_guami_free(amf3_gpp_access_registration_modification->guami);
    ogs_free(amf3_gpp_access_registration_modification->pei);
    OpenAPI_ims_vo_ps_free(amf3_gpp_access_registration_modification->ims_vo_ps);
    OpenAPI_list_for_each(amf3_gpp_access_registration_modification->backup_amf_info, node) {
        OpenAPI_backup_amf_info_free(node->data);
    }
    OpenAPI_list_free(amf3_gpp_access_registration_modification->backup_amf_info);
    OpenAPI_eps_interworking_info_free(amf3_gpp_access_registration_modification->eps_interworking_info);
    ogs_free(amf3_gpp_access_registration_modification);
}

cJSON *OpenAPI_amf3_gpp_access_registration_modification_convertToJSON(OpenAPI_amf3_gpp_access_registration_modification_t *amf3_gpp_access_registration_modification)
{
    cJSON *item = NULL;

    if (amf3_gpp_access_registration_modification == NULL) {
        ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [Amf3GppAccessRegistrationModification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!amf3_gpp_access_registration_modification->guami) {
        ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(amf3_gpp_access_registration_modification->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [guami]");
        goto end;
    }

    if (amf3_gpp_access_registration_modification->purge_flag) {
        if (cJSON_AddBoolToObject(item, "purgeFlag", amf3_gpp_access_registration_modification->purge_flag) == NULL) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [purge_flag]");
            goto end;
        }
    }

    if (amf3_gpp_access_registration_modification->pei) {
        if (cJSON_AddStringToObject(item, "pei", amf3_gpp_access_registration_modification->pei) == NULL) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [pei]");
            goto end;
        }
    }

    if (amf3_gpp_access_registration_modification->ims_vo_ps) {
        cJSON *ims_vo_ps_local_JSON = OpenAPI_ims_vo_ps_convertToJSON(amf3_gpp_access_registration_modification->ims_vo_ps);
        if (ims_vo_ps_local_JSON == NULL) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [ims_vo_ps]");
            goto end;
        }
        cJSON_AddItemToObject(item, "imsVoPs", ims_vo_ps_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [ims_vo_ps]");
            goto end;
        }
    }

    if (amf3_gpp_access_registration_modification->backup_amf_info) {
        cJSON *backup_amf_infoList = cJSON_AddArrayToObject(item, "backupAmfInfo");
        if (backup_amf_infoList == NULL) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [backup_amf_info]");
            goto end;
        }

        OpenAPI_lnode_t *backup_amf_info_node;
        if (amf3_gpp_access_registration_modification->backup_amf_info) {
            OpenAPI_list_for_each(amf3_gpp_access_registration_modification->backup_amf_info, backup_amf_info_node) {
                cJSON *itemLocal = OpenAPI_backup_amf_info_convertToJSON(backup_amf_info_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [backup_amf_info]");
                    goto end;
                }
                cJSON_AddItemToArray(backup_amf_infoList, itemLocal);
            }
        }
    }

    if (amf3_gpp_access_registration_modification->eps_interworking_info) {
        cJSON *eps_interworking_info_local_JSON = OpenAPI_eps_interworking_info_convertToJSON(amf3_gpp_access_registration_modification->eps_interworking_info);
        if (eps_interworking_info_local_JSON == NULL) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [eps_interworking_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "epsInterworkingInfo", eps_interworking_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [eps_interworking_info]");
            goto end;
        }
    }

    if (amf3_gpp_access_registration_modification->ue_srvcc_capability) {
        if (cJSON_AddBoolToObject(item, "ueSrvccCapability", amf3_gpp_access_registration_modification->ue_srvcc_capability) == NULL) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed [ue_srvcc_capability]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_amf3_gpp_access_registration_modification_t *OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON(cJSON *amf3_gpp_access_registration_modificationJSON)
{
    OpenAPI_amf3_gpp_access_registration_modification_t *amf3_gpp_access_registration_modification_local_var = NULL;
    cJSON *guami = cJSON_GetObjectItemCaseSensitive(amf3_gpp_access_registration_modificationJSON, "guami");
    if (!guami) {
        ogs_error("OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON() failed [guami]");
        goto end;
    }

    OpenAPI_guami_t *guami_local_nonprim = NULL;

    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);

    cJSON *purge_flag = cJSON_GetObjectItemCaseSensitive(amf3_gpp_access_registration_modificationJSON, "purgeFlag");

    if (purge_flag) {
        if (!cJSON_IsBool(purge_flag)) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON() failed [purge_flag]");
            goto end;
        }
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(amf3_gpp_access_registration_modificationJSON, "pei");

    if (pei) {
        if (!cJSON_IsString(pei)) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON() failed [pei]");
            goto end;
        }
    }

    cJSON *ims_vo_ps = cJSON_GetObjectItemCaseSensitive(amf3_gpp_access_registration_modificationJSON, "imsVoPs");

    OpenAPI_ims_vo_ps_t *ims_vo_ps_local_nonprim = NULL;
    if (ims_vo_ps) {
        ims_vo_ps_local_nonprim = OpenAPI_ims_vo_ps_parseFromJSON(ims_vo_ps);
    }

    cJSON *backup_amf_info = cJSON_GetObjectItemCaseSensitive(amf3_gpp_access_registration_modificationJSON, "backupAmfInfo");

    OpenAPI_list_t *backup_amf_infoList;
    if (backup_amf_info) {
        cJSON *backup_amf_info_local_nonprimitive;
        if (!cJSON_IsArray(backup_amf_info)) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON() failed [backup_amf_info]");
            goto end;
        }

        backup_amf_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_amf_info_local_nonprimitive, backup_amf_info ) {
            if (!cJSON_IsObject(backup_amf_info_local_nonprimitive)) {
                ogs_error("OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON() failed [backup_amf_info]");
                goto end;
            }
            OpenAPI_backup_amf_info_t *backup_amf_infoItem = OpenAPI_backup_amf_info_parseFromJSON(backup_amf_info_local_nonprimitive);

            OpenAPI_list_add(backup_amf_infoList, backup_amf_infoItem);
        }
    }

    cJSON *eps_interworking_info = cJSON_GetObjectItemCaseSensitive(amf3_gpp_access_registration_modificationJSON, "epsInterworkingInfo");

    OpenAPI_eps_interworking_info_t *eps_interworking_info_local_nonprim = NULL;
    if (eps_interworking_info) {
        eps_interworking_info_local_nonprim = OpenAPI_eps_interworking_info_parseFromJSON(eps_interworking_info);
    }

    cJSON *ue_srvcc_capability = cJSON_GetObjectItemCaseSensitive(amf3_gpp_access_registration_modificationJSON, "ueSrvccCapability");

    if (ue_srvcc_capability) {
        if (!cJSON_IsBool(ue_srvcc_capability)) {
            ogs_error("OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON() failed [ue_srvcc_capability]");
            goto end;
        }
    }

    amf3_gpp_access_registration_modification_local_var = OpenAPI_amf3_gpp_access_registration_modification_create (
        guami_local_nonprim,
        purge_flag ? purge_flag->valueint : 0,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        ims_vo_ps ? ims_vo_ps_local_nonprim : NULL,
        backup_amf_info ? backup_amf_infoList : NULL,
        eps_interworking_info ? eps_interworking_info_local_nonprim : NULL,
        ue_srvcc_capability ? ue_srvcc_capability->valueint : 0
        );

    return amf3_gpp_access_registration_modification_local_var;
end:
    return NULL;
}

OpenAPI_amf3_gpp_access_registration_modification_t *OpenAPI_amf3_gpp_access_registration_modification_copy(OpenAPI_amf3_gpp_access_registration_modification_t *dst, OpenAPI_amf3_gpp_access_registration_modification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf3_gpp_access_registration_modification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf3_gpp_access_registration_modification_convertToJSON() failed");
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

    OpenAPI_amf3_gpp_access_registration_modification_free(dst);
    dst = OpenAPI_amf3_gpp_access_registration_modification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


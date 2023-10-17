
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_mbs_binding.h"

OpenAPI_pcf_mbs_binding_t *OpenAPI_pcf_mbs_binding_create(
    OpenAPI_mbs_session_id_t *mbs_session_id,
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level,
    char *recovery_time,
    char *supp_feat
)
{
    OpenAPI_pcf_mbs_binding_t *pcf_mbs_binding_local_var = ogs_malloc(sizeof(OpenAPI_pcf_mbs_binding_t));
    ogs_assert(pcf_mbs_binding_local_var);

    pcf_mbs_binding_local_var->mbs_session_id = mbs_session_id;
    pcf_mbs_binding_local_var->pcf_fqdn = pcf_fqdn;
    pcf_mbs_binding_local_var->pcf_ip_end_points = pcf_ip_end_points;
    pcf_mbs_binding_local_var->pcf_id = pcf_id;
    pcf_mbs_binding_local_var->pcf_set_id = pcf_set_id;
    pcf_mbs_binding_local_var->bind_level = bind_level;
    pcf_mbs_binding_local_var->recovery_time = recovery_time;
    pcf_mbs_binding_local_var->supp_feat = supp_feat;

    return pcf_mbs_binding_local_var;
}

void OpenAPI_pcf_mbs_binding_free(OpenAPI_pcf_mbs_binding_t *pcf_mbs_binding)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_mbs_binding) {
        return;
    }
    if (pcf_mbs_binding->mbs_session_id) {
        OpenAPI_mbs_session_id_free(pcf_mbs_binding->mbs_session_id);
        pcf_mbs_binding->mbs_session_id = NULL;
    }
    if (pcf_mbs_binding->pcf_fqdn) {
        ogs_free(pcf_mbs_binding->pcf_fqdn);
        pcf_mbs_binding->pcf_fqdn = NULL;
    }
    if (pcf_mbs_binding->pcf_ip_end_points) {
        OpenAPI_list_for_each(pcf_mbs_binding->pcf_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_mbs_binding->pcf_ip_end_points);
        pcf_mbs_binding->pcf_ip_end_points = NULL;
    }
    if (pcf_mbs_binding->pcf_id) {
        ogs_free(pcf_mbs_binding->pcf_id);
        pcf_mbs_binding->pcf_id = NULL;
    }
    if (pcf_mbs_binding->pcf_set_id) {
        ogs_free(pcf_mbs_binding->pcf_set_id);
        pcf_mbs_binding->pcf_set_id = NULL;
    }
    if (pcf_mbs_binding->recovery_time) {
        ogs_free(pcf_mbs_binding->recovery_time);
        pcf_mbs_binding->recovery_time = NULL;
    }
    if (pcf_mbs_binding->supp_feat) {
        ogs_free(pcf_mbs_binding->supp_feat);
        pcf_mbs_binding->supp_feat = NULL;
    }
    ogs_free(pcf_mbs_binding);
}

cJSON *OpenAPI_pcf_mbs_binding_convertToJSON(OpenAPI_pcf_mbs_binding_t *pcf_mbs_binding)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_mbs_binding == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [PcfMbsBinding]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pcf_mbs_binding->mbs_session_id) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [mbs_session_id]");
        return NULL;
    }
    cJSON *mbs_session_id_local_JSON = OpenAPI_mbs_session_id_convertToJSON(pcf_mbs_binding->mbs_session_id);
    if (mbs_session_id_local_JSON == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [mbs_session_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mbsSessionId", mbs_session_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [mbs_session_id]");
        goto end;
    }

    if (pcf_mbs_binding->pcf_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfFqdn", pcf_mbs_binding->pcf_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    if (pcf_mbs_binding->pcf_ip_end_points) {
    cJSON *pcf_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfIpEndPoints");
    if (pcf_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [pcf_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_mbs_binding->pcf_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [pcf_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_ip_end_pointsList, itemLocal);
    }
    }

    if (pcf_mbs_binding->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pcf_mbs_binding->pcf_id) == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (pcf_mbs_binding->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", pcf_mbs_binding->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (pcf_mbs_binding->bind_level != OpenAPI_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "bindLevel", OpenAPI_binding_level_ToString(pcf_mbs_binding->bind_level)) == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [bind_level]");
        goto end;
    }
    }

    if (pcf_mbs_binding->recovery_time) {
    if (cJSON_AddStringToObject(item, "recoveryTime", pcf_mbs_binding->recovery_time) == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [recovery_time]");
        goto end;
    }
    }

    if (pcf_mbs_binding->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", pcf_mbs_binding->supp_feat) == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_mbs_binding_t *OpenAPI_pcf_mbs_binding_parseFromJSON(cJSON *pcf_mbs_bindingJSON)
{
    OpenAPI_pcf_mbs_binding_t *pcf_mbs_binding_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbs_session_id = NULL;
    OpenAPI_mbs_session_id_t *mbs_session_id_local_nonprim = NULL;
    cJSON *pcf_fqdn = NULL;
    cJSON *pcf_ip_end_points = NULL;
    OpenAPI_list_t *pcf_ip_end_pointsList = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *bind_level = NULL;
    OpenAPI_binding_level_e bind_levelVariable = 0;
    cJSON *recovery_time = NULL;
    cJSON *supp_feat = NULL;
    mbs_session_id = cJSON_GetObjectItemCaseSensitive(pcf_mbs_bindingJSON, "mbsSessionId");
    if (!mbs_session_id) {
        ogs_error("OpenAPI_pcf_mbs_binding_parseFromJSON() failed [mbs_session_id]");
        goto end;
    }
    mbs_session_id_local_nonprim = OpenAPI_mbs_session_id_parseFromJSON(mbs_session_id);
    if (!mbs_session_id_local_nonprim) {
        ogs_error("OpenAPI_mbs_session_id_parseFromJSON failed [mbs_session_id]");
        goto end;
    }

    pcf_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_mbs_bindingJSON, "pcfFqdn");
    if (pcf_fqdn) {
    if (!cJSON_IsString(pcf_fqdn) && !cJSON_IsNull(pcf_fqdn)) {
        ogs_error("OpenAPI_pcf_mbs_binding_parseFromJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    pcf_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_mbs_bindingJSON, "pcfIpEndPoints");
    if (pcf_ip_end_points) {
        cJSON *pcf_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_ip_end_points)) {
            ogs_error("OpenAPI_pcf_mbs_binding_parseFromJSON() failed [pcf_ip_end_points]");
            goto end;
        }

        pcf_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_ip_end_points_local, pcf_ip_end_points) {
            if (!cJSON_IsObject(pcf_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_mbs_binding_parseFromJSON() failed [pcf_ip_end_points]");
                goto end;
            }
            OpenAPI_ip_end_point_t *pcf_ip_end_pointsItem = OpenAPI_ip_end_point_parseFromJSON(pcf_ip_end_points_local);
            if (!pcf_ip_end_pointsItem) {
                ogs_error("No pcf_ip_end_pointsItem");
                goto end;
            }
            OpenAPI_list_add(pcf_ip_end_pointsList, pcf_ip_end_pointsItem);
        }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(pcf_mbs_bindingJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pcf_mbs_binding_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(pcf_mbs_bindingJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_pcf_mbs_binding_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    bind_level = cJSON_GetObjectItemCaseSensitive(pcf_mbs_bindingJSON, "bindLevel");
    if (bind_level) {
    if (!cJSON_IsString(bind_level)) {
        ogs_error("OpenAPI_pcf_mbs_binding_parseFromJSON() failed [bind_level]");
        goto end;
    }
    bind_levelVariable = OpenAPI_binding_level_FromString(bind_level->valuestring);
    }

    recovery_time = cJSON_GetObjectItemCaseSensitive(pcf_mbs_bindingJSON, "recoveryTime");
    if (recovery_time) {
    if (!cJSON_IsString(recovery_time) && !cJSON_IsNull(recovery_time)) {
        ogs_error("OpenAPI_pcf_mbs_binding_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(pcf_mbs_bindingJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_pcf_mbs_binding_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    pcf_mbs_binding_local_var = OpenAPI_pcf_mbs_binding_create (
        mbs_session_id_local_nonprim,
        pcf_fqdn && !cJSON_IsNull(pcf_fqdn) ? ogs_strdup(pcf_fqdn->valuestring) : NULL,
        pcf_ip_end_points ? pcf_ip_end_pointsList : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        bind_level ? bind_levelVariable : 0,
        recovery_time && !cJSON_IsNull(recovery_time) ? ogs_strdup(recovery_time->valuestring) : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return pcf_mbs_binding_local_var;
end:
    if (mbs_session_id_local_nonprim) {
        OpenAPI_mbs_session_id_free(mbs_session_id_local_nonprim);
        mbs_session_id_local_nonprim = NULL;
    }
    if (pcf_ip_end_pointsList) {
        OpenAPI_list_for_each(pcf_ip_end_pointsList, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_ip_end_pointsList);
        pcf_ip_end_pointsList = NULL;
    }
    return NULL;
}

OpenAPI_pcf_mbs_binding_t *OpenAPI_pcf_mbs_binding_copy(OpenAPI_pcf_mbs_binding_t *dst, OpenAPI_pcf_mbs_binding_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_mbs_binding_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_mbs_binding_convertToJSON() failed");
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

    OpenAPI_pcf_mbs_binding_free(dst);
    dst = OpenAPI_pcf_mbs_binding_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


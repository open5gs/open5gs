
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_for_ue_info.h"

OpenAPI_pcf_for_ue_info_t *OpenAPI_pcf_for_ue_info_create(
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level
)
{
    OpenAPI_pcf_for_ue_info_t *pcf_for_ue_info_local_var = ogs_malloc(sizeof(OpenAPI_pcf_for_ue_info_t));
    ogs_assert(pcf_for_ue_info_local_var);

    pcf_for_ue_info_local_var->pcf_fqdn = pcf_fqdn;
    pcf_for_ue_info_local_var->pcf_ip_end_points = pcf_ip_end_points;
    pcf_for_ue_info_local_var->pcf_id = pcf_id;
    pcf_for_ue_info_local_var->pcf_set_id = pcf_set_id;
    pcf_for_ue_info_local_var->bind_level = bind_level;

    return pcf_for_ue_info_local_var;
}

void OpenAPI_pcf_for_ue_info_free(OpenAPI_pcf_for_ue_info_t *pcf_for_ue_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_for_ue_info) {
        return;
    }
    if (pcf_for_ue_info->pcf_fqdn) {
        ogs_free(pcf_for_ue_info->pcf_fqdn);
        pcf_for_ue_info->pcf_fqdn = NULL;
    }
    if (pcf_for_ue_info->pcf_ip_end_points) {
        OpenAPI_list_for_each(pcf_for_ue_info->pcf_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_for_ue_info->pcf_ip_end_points);
        pcf_for_ue_info->pcf_ip_end_points = NULL;
    }
    if (pcf_for_ue_info->pcf_id) {
        ogs_free(pcf_for_ue_info->pcf_id);
        pcf_for_ue_info->pcf_id = NULL;
    }
    if (pcf_for_ue_info->pcf_set_id) {
        ogs_free(pcf_for_ue_info->pcf_set_id);
        pcf_for_ue_info->pcf_set_id = NULL;
    }
    ogs_free(pcf_for_ue_info);
}

cJSON *OpenAPI_pcf_for_ue_info_convertToJSON(OpenAPI_pcf_for_ue_info_t *pcf_for_ue_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_for_ue_info == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_info_convertToJSON() failed [PcfForUeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcf_for_ue_info->pcf_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfFqdn", pcf_for_ue_info->pcf_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_info_convertToJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    if (pcf_for_ue_info->pcf_ip_end_points) {
    cJSON *pcf_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfIpEndPoints");
    if (pcf_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_info_convertToJSON() failed [pcf_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_for_ue_info->pcf_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_for_ue_info_convertToJSON() failed [pcf_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_ip_end_pointsList, itemLocal);
    }
    }

    if (pcf_for_ue_info->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pcf_for_ue_info->pcf_id) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_info_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (pcf_for_ue_info->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", pcf_for_ue_info->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_info_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (pcf_for_ue_info->bind_level != OpenAPI_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "bindLevel", OpenAPI_binding_level_ToString(pcf_for_ue_info->bind_level)) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_info_convertToJSON() failed [bind_level]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_for_ue_info_t *OpenAPI_pcf_for_ue_info_parseFromJSON(cJSON *pcf_for_ue_infoJSON)
{
    OpenAPI_pcf_for_ue_info_t *pcf_for_ue_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pcf_fqdn = NULL;
    cJSON *pcf_ip_end_points = NULL;
    OpenAPI_list_t *pcf_ip_end_pointsList = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *bind_level = NULL;
    OpenAPI_binding_level_e bind_levelVariable = 0;
    pcf_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_infoJSON, "pcfFqdn");
    if (pcf_fqdn) {
    if (!cJSON_IsString(pcf_fqdn) && !cJSON_IsNull(pcf_fqdn)) {
        ogs_error("OpenAPI_pcf_for_ue_info_parseFromJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    pcf_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_infoJSON, "pcfIpEndPoints");
    if (pcf_ip_end_points) {
        cJSON *pcf_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_ip_end_points)) {
            ogs_error("OpenAPI_pcf_for_ue_info_parseFromJSON() failed [pcf_ip_end_points]");
            goto end;
        }

        pcf_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_ip_end_points_local, pcf_ip_end_points) {
            if (!cJSON_IsObject(pcf_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_for_ue_info_parseFromJSON() failed [pcf_ip_end_points]");
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

    pcf_id = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_infoJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pcf_for_ue_info_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_infoJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_pcf_for_ue_info_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    bind_level = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_infoJSON, "bindLevel");
    if (bind_level) {
    if (!cJSON_IsString(bind_level)) {
        ogs_error("OpenAPI_pcf_for_ue_info_parseFromJSON() failed [bind_level]");
        goto end;
    }
    bind_levelVariable = OpenAPI_binding_level_FromString(bind_level->valuestring);
    }

    pcf_for_ue_info_local_var = OpenAPI_pcf_for_ue_info_create (
        pcf_fqdn && !cJSON_IsNull(pcf_fqdn) ? ogs_strdup(pcf_fqdn->valuestring) : NULL,
        pcf_ip_end_points ? pcf_ip_end_pointsList : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        bind_level ? bind_levelVariable : 0
    );

    return pcf_for_ue_info_local_var;
end:
    if (pcf_ip_end_pointsList) {
        OpenAPI_list_for_each(pcf_ip_end_pointsList, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_ip_end_pointsList);
        pcf_ip_end_pointsList = NULL;
    }
    return NULL;
}

OpenAPI_pcf_for_ue_info_t *OpenAPI_pcf_for_ue_info_copy(OpenAPI_pcf_for_ue_info_t *dst, OpenAPI_pcf_for_ue_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_for_ue_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_for_ue_info_convertToJSON() failed");
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

    OpenAPI_pcf_for_ue_info_free(dst);
    dst = OpenAPI_pcf_for_ue_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_addressing_info.h"

OpenAPI_pcf_addressing_info_t *OpenAPI_pcf_addressing_info_create(
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *binding_info
)
{
    OpenAPI_pcf_addressing_info_t *pcf_addressing_info_local_var = ogs_malloc(sizeof(OpenAPI_pcf_addressing_info_t));
    ogs_assert(pcf_addressing_info_local_var);

    pcf_addressing_info_local_var->pcf_fqdn = pcf_fqdn;
    pcf_addressing_info_local_var->pcf_ip_end_points = pcf_ip_end_points;
    pcf_addressing_info_local_var->binding_info = binding_info;

    return pcf_addressing_info_local_var;
}

void OpenAPI_pcf_addressing_info_free(OpenAPI_pcf_addressing_info_t *pcf_addressing_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_addressing_info) {
        return;
    }
    if (pcf_addressing_info->pcf_fqdn) {
        ogs_free(pcf_addressing_info->pcf_fqdn);
        pcf_addressing_info->pcf_fqdn = NULL;
    }
    if (pcf_addressing_info->pcf_ip_end_points) {
        OpenAPI_list_for_each(pcf_addressing_info->pcf_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_addressing_info->pcf_ip_end_points);
        pcf_addressing_info->pcf_ip_end_points = NULL;
    }
    if (pcf_addressing_info->binding_info) {
        ogs_free(pcf_addressing_info->binding_info);
        pcf_addressing_info->binding_info = NULL;
    }
    ogs_free(pcf_addressing_info);
}

cJSON *OpenAPI_pcf_addressing_info_convertToJSON(OpenAPI_pcf_addressing_info_t *pcf_addressing_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_addressing_info == NULL) {
        ogs_error("OpenAPI_pcf_addressing_info_convertToJSON() failed [PcfAddressingInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcf_addressing_info->pcf_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfFqdn", pcf_addressing_info->pcf_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_addressing_info_convertToJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    if (pcf_addressing_info->pcf_ip_end_points) {
    cJSON *pcf_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfIpEndPoints");
    if (pcf_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_addressing_info_convertToJSON() failed [pcf_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_addressing_info->pcf_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_addressing_info_convertToJSON() failed [pcf_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_ip_end_pointsList, itemLocal);
    }
    }

    if (pcf_addressing_info->binding_info) {
    if (cJSON_AddStringToObject(item, "bindingInfo", pcf_addressing_info->binding_info) == NULL) {
        ogs_error("OpenAPI_pcf_addressing_info_convertToJSON() failed [binding_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_addressing_info_t *OpenAPI_pcf_addressing_info_parseFromJSON(cJSON *pcf_addressing_infoJSON)
{
    OpenAPI_pcf_addressing_info_t *pcf_addressing_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pcf_fqdn = NULL;
    cJSON *pcf_ip_end_points = NULL;
    OpenAPI_list_t *pcf_ip_end_pointsList = NULL;
    cJSON *binding_info = NULL;
    pcf_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_addressing_infoJSON, "pcfFqdn");
    if (pcf_fqdn) {
    if (!cJSON_IsString(pcf_fqdn) && !cJSON_IsNull(pcf_fqdn)) {
        ogs_error("OpenAPI_pcf_addressing_info_parseFromJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    pcf_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_addressing_infoJSON, "pcfIpEndPoints");
    if (pcf_ip_end_points) {
        cJSON *pcf_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_ip_end_points)) {
            ogs_error("OpenAPI_pcf_addressing_info_parseFromJSON() failed [pcf_ip_end_points]");
            goto end;
        }

        pcf_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_ip_end_points_local, pcf_ip_end_points) {
            if (!cJSON_IsObject(pcf_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_addressing_info_parseFromJSON() failed [pcf_ip_end_points]");
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

    binding_info = cJSON_GetObjectItemCaseSensitive(pcf_addressing_infoJSON, "bindingInfo");
    if (binding_info) {
    if (!cJSON_IsString(binding_info) && !cJSON_IsNull(binding_info)) {
        ogs_error("OpenAPI_pcf_addressing_info_parseFromJSON() failed [binding_info]");
        goto end;
    }
    }

    pcf_addressing_info_local_var = OpenAPI_pcf_addressing_info_create (
        pcf_fqdn && !cJSON_IsNull(pcf_fqdn) ? ogs_strdup(pcf_fqdn->valuestring) : NULL,
        pcf_ip_end_points ? pcf_ip_end_pointsList : NULL,
        binding_info && !cJSON_IsNull(binding_info) ? ogs_strdup(binding_info->valuestring) : NULL
    );

    return pcf_addressing_info_local_var;
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

OpenAPI_pcf_addressing_info_t *OpenAPI_pcf_addressing_info_copy(OpenAPI_pcf_addressing_info_t *dst, OpenAPI_pcf_addressing_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_addressing_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_addressing_info_convertToJSON() failed");
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

    OpenAPI_pcf_addressing_info_free(dst);
    dst = OpenAPI_pcf_addressing_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


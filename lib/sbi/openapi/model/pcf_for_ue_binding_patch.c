
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_for_ue_binding_patch.h"

OpenAPI_pcf_for_ue_binding_patch_t *OpenAPI_pcf_for_ue_binding_patch_create(
    char *pcf_for_ue_fqdn,
    OpenAPI_list_t *pcf_for_ue_ip_end_points,
    char *pcf_id
)
{
    OpenAPI_pcf_for_ue_binding_patch_t *pcf_for_ue_binding_patch_local_var = ogs_malloc(sizeof(OpenAPI_pcf_for_ue_binding_patch_t));
    ogs_assert(pcf_for_ue_binding_patch_local_var);

    pcf_for_ue_binding_patch_local_var->pcf_for_ue_fqdn = pcf_for_ue_fqdn;
    pcf_for_ue_binding_patch_local_var->pcf_for_ue_ip_end_points = pcf_for_ue_ip_end_points;
    pcf_for_ue_binding_patch_local_var->pcf_id = pcf_id;

    return pcf_for_ue_binding_patch_local_var;
}

void OpenAPI_pcf_for_ue_binding_patch_free(OpenAPI_pcf_for_ue_binding_patch_t *pcf_for_ue_binding_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_for_ue_binding_patch) {
        return;
    }
    if (pcf_for_ue_binding_patch->pcf_for_ue_fqdn) {
        ogs_free(pcf_for_ue_binding_patch->pcf_for_ue_fqdn);
        pcf_for_ue_binding_patch->pcf_for_ue_fqdn = NULL;
    }
    if (pcf_for_ue_binding_patch->pcf_for_ue_ip_end_points) {
        OpenAPI_list_for_each(pcf_for_ue_binding_patch->pcf_for_ue_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_for_ue_binding_patch->pcf_for_ue_ip_end_points);
        pcf_for_ue_binding_patch->pcf_for_ue_ip_end_points = NULL;
    }
    if (pcf_for_ue_binding_patch->pcf_id) {
        ogs_free(pcf_for_ue_binding_patch->pcf_id);
        pcf_for_ue_binding_patch->pcf_id = NULL;
    }
    ogs_free(pcf_for_ue_binding_patch);
}

cJSON *OpenAPI_pcf_for_ue_binding_patch_convertToJSON(OpenAPI_pcf_for_ue_binding_patch_t *pcf_for_ue_binding_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_for_ue_binding_patch == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_patch_convertToJSON() failed [PcfForUeBindingPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcf_for_ue_binding_patch->pcf_for_ue_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfForUeFqdn", pcf_for_ue_binding_patch->pcf_for_ue_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_patch_convertToJSON() failed [pcf_for_ue_fqdn]");
        goto end;
    }
    }

    if (pcf_for_ue_binding_patch->pcf_for_ue_ip_end_points) {
    cJSON *pcf_for_ue_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfForUeIpEndPoints");
    if (pcf_for_ue_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_patch_convertToJSON() failed [pcf_for_ue_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_for_ue_binding_patch->pcf_for_ue_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_for_ue_binding_patch_convertToJSON() failed [pcf_for_ue_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_for_ue_ip_end_pointsList, itemLocal);
    }
    }

    if (pcf_for_ue_binding_patch->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pcf_for_ue_binding_patch->pcf_id) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_patch_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_for_ue_binding_patch_t *OpenAPI_pcf_for_ue_binding_patch_parseFromJSON(cJSON *pcf_for_ue_binding_patchJSON)
{
    OpenAPI_pcf_for_ue_binding_patch_t *pcf_for_ue_binding_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pcf_for_ue_fqdn = NULL;
    cJSON *pcf_for_ue_ip_end_points = NULL;
    OpenAPI_list_t *pcf_for_ue_ip_end_pointsList = NULL;
    cJSON *pcf_id = NULL;
    pcf_for_ue_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_binding_patchJSON, "pcfForUeFqdn");
    if (pcf_for_ue_fqdn) {
    if (!cJSON_IsString(pcf_for_ue_fqdn) && !cJSON_IsNull(pcf_for_ue_fqdn)) {
        ogs_error("OpenAPI_pcf_for_ue_binding_patch_parseFromJSON() failed [pcf_for_ue_fqdn]");
        goto end;
    }
    }

    pcf_for_ue_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_binding_patchJSON, "pcfForUeIpEndPoints");
    if (pcf_for_ue_ip_end_points) {
        cJSON *pcf_for_ue_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_for_ue_ip_end_points)) {
            ogs_error("OpenAPI_pcf_for_ue_binding_patch_parseFromJSON() failed [pcf_for_ue_ip_end_points]");
            goto end;
        }

        pcf_for_ue_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_for_ue_ip_end_points_local, pcf_for_ue_ip_end_points) {
            if (!cJSON_IsObject(pcf_for_ue_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_for_ue_binding_patch_parseFromJSON() failed [pcf_for_ue_ip_end_points]");
                goto end;
            }
            OpenAPI_ip_end_point_t *pcf_for_ue_ip_end_pointsItem = OpenAPI_ip_end_point_parseFromJSON(pcf_for_ue_ip_end_points_local);
            if (!pcf_for_ue_ip_end_pointsItem) {
                ogs_error("No pcf_for_ue_ip_end_pointsItem");
                goto end;
            }
            OpenAPI_list_add(pcf_for_ue_ip_end_pointsList, pcf_for_ue_ip_end_pointsItem);
        }
    }

    pcf_id = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_binding_patchJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pcf_for_ue_binding_patch_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_for_ue_binding_patch_local_var = OpenAPI_pcf_for_ue_binding_patch_create (
        pcf_for_ue_fqdn && !cJSON_IsNull(pcf_for_ue_fqdn) ? ogs_strdup(pcf_for_ue_fqdn->valuestring) : NULL,
        pcf_for_ue_ip_end_points ? pcf_for_ue_ip_end_pointsList : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL
    );

    return pcf_for_ue_binding_patch_local_var;
end:
    if (pcf_for_ue_ip_end_pointsList) {
        OpenAPI_list_for_each(pcf_for_ue_ip_end_pointsList, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_for_ue_ip_end_pointsList);
        pcf_for_ue_ip_end_pointsList = NULL;
    }
    return NULL;
}

OpenAPI_pcf_for_ue_binding_patch_t *OpenAPI_pcf_for_ue_binding_patch_copy(OpenAPI_pcf_for_ue_binding_patch_t *dst, OpenAPI_pcf_for_ue_binding_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_for_ue_binding_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_for_ue_binding_patch_convertToJSON() failed");
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

    OpenAPI_pcf_for_ue_binding_patch_free(dst);
    dst = OpenAPI_pcf_for_ue_binding_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


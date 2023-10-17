
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_mbs_binding_patch.h"

OpenAPI_pcf_mbs_binding_patch_t *OpenAPI_pcf_mbs_binding_patch_create(
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points,
    char *pcf_id
)
{
    OpenAPI_pcf_mbs_binding_patch_t *pcf_mbs_binding_patch_local_var = ogs_malloc(sizeof(OpenAPI_pcf_mbs_binding_patch_t));
    ogs_assert(pcf_mbs_binding_patch_local_var);

    pcf_mbs_binding_patch_local_var->pcf_fqdn = pcf_fqdn;
    pcf_mbs_binding_patch_local_var->pcf_ip_end_points = pcf_ip_end_points;
    pcf_mbs_binding_patch_local_var->pcf_id = pcf_id;

    return pcf_mbs_binding_patch_local_var;
}

void OpenAPI_pcf_mbs_binding_patch_free(OpenAPI_pcf_mbs_binding_patch_t *pcf_mbs_binding_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_mbs_binding_patch) {
        return;
    }
    if (pcf_mbs_binding_patch->pcf_fqdn) {
        ogs_free(pcf_mbs_binding_patch->pcf_fqdn);
        pcf_mbs_binding_patch->pcf_fqdn = NULL;
    }
    if (pcf_mbs_binding_patch->pcf_ip_end_points) {
        OpenAPI_list_for_each(pcf_mbs_binding_patch->pcf_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_mbs_binding_patch->pcf_ip_end_points);
        pcf_mbs_binding_patch->pcf_ip_end_points = NULL;
    }
    if (pcf_mbs_binding_patch->pcf_id) {
        ogs_free(pcf_mbs_binding_patch->pcf_id);
        pcf_mbs_binding_patch->pcf_id = NULL;
    }
    ogs_free(pcf_mbs_binding_patch);
}

cJSON *OpenAPI_pcf_mbs_binding_patch_convertToJSON(OpenAPI_pcf_mbs_binding_patch_t *pcf_mbs_binding_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_mbs_binding_patch == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_patch_convertToJSON() failed [PcfMbsBindingPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pcf_mbs_binding_patch->pcf_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfFqdn", pcf_mbs_binding_patch->pcf_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_patch_convertToJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    if (pcf_mbs_binding_patch->pcf_ip_end_points) {
    cJSON *pcf_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfIpEndPoints");
    if (pcf_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_patch_convertToJSON() failed [pcf_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_mbs_binding_patch->pcf_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_mbs_binding_patch_convertToJSON() failed [pcf_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_ip_end_pointsList, itemLocal);
    }
    }

    if (pcf_mbs_binding_patch->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pcf_mbs_binding_patch->pcf_id) == NULL) {
        ogs_error("OpenAPI_pcf_mbs_binding_patch_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_mbs_binding_patch_t *OpenAPI_pcf_mbs_binding_patch_parseFromJSON(cJSON *pcf_mbs_binding_patchJSON)
{
    OpenAPI_pcf_mbs_binding_patch_t *pcf_mbs_binding_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pcf_fqdn = NULL;
    cJSON *pcf_ip_end_points = NULL;
    OpenAPI_list_t *pcf_ip_end_pointsList = NULL;
    cJSON *pcf_id = NULL;
    pcf_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_mbs_binding_patchJSON, "pcfFqdn");
    if (pcf_fqdn) {
    if (!cJSON_IsString(pcf_fqdn) && !cJSON_IsNull(pcf_fqdn)) {
        ogs_error("OpenAPI_pcf_mbs_binding_patch_parseFromJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    pcf_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_mbs_binding_patchJSON, "pcfIpEndPoints");
    if (pcf_ip_end_points) {
        cJSON *pcf_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_ip_end_points)) {
            ogs_error("OpenAPI_pcf_mbs_binding_patch_parseFromJSON() failed [pcf_ip_end_points]");
            goto end;
        }

        pcf_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_ip_end_points_local, pcf_ip_end_points) {
            if (!cJSON_IsObject(pcf_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_mbs_binding_patch_parseFromJSON() failed [pcf_ip_end_points]");
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

    pcf_id = cJSON_GetObjectItemCaseSensitive(pcf_mbs_binding_patchJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pcf_mbs_binding_patch_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_mbs_binding_patch_local_var = OpenAPI_pcf_mbs_binding_patch_create (
        pcf_fqdn && !cJSON_IsNull(pcf_fqdn) ? ogs_strdup(pcf_fqdn->valuestring) : NULL,
        pcf_ip_end_points ? pcf_ip_end_pointsList : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL
    );

    return pcf_mbs_binding_patch_local_var;
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

OpenAPI_pcf_mbs_binding_patch_t *OpenAPI_pcf_mbs_binding_patch_copy(OpenAPI_pcf_mbs_binding_patch_t *dst, OpenAPI_pcf_mbs_binding_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_mbs_binding_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_mbs_binding_patch_convertToJSON() failed");
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

    OpenAPI_pcf_mbs_binding_patch_free(dst);
    dst = OpenAPI_pcf_mbs_binding_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_for_ue_binding.h"

OpenAPI_pcf_for_ue_binding_t *OpenAPI_pcf_for_ue_binding_create(
    char *supi,
    char *gpsi,
    char *pcf_for_ue_fqdn,
    OpenAPI_list_t *pcf_for_ue_ip_end_points,
    char *pcf_id,
    char *pcf_set_id,
    OpenAPI_binding_level_e bind_level,
    char *supp_feat
)
{
    OpenAPI_pcf_for_ue_binding_t *pcf_for_ue_binding_local_var = ogs_malloc(sizeof(OpenAPI_pcf_for_ue_binding_t));
    ogs_assert(pcf_for_ue_binding_local_var);

    pcf_for_ue_binding_local_var->supi = supi;
    pcf_for_ue_binding_local_var->gpsi = gpsi;
    pcf_for_ue_binding_local_var->pcf_for_ue_fqdn = pcf_for_ue_fqdn;
    pcf_for_ue_binding_local_var->pcf_for_ue_ip_end_points = pcf_for_ue_ip_end_points;
    pcf_for_ue_binding_local_var->pcf_id = pcf_id;
    pcf_for_ue_binding_local_var->pcf_set_id = pcf_set_id;
    pcf_for_ue_binding_local_var->bind_level = bind_level;
    pcf_for_ue_binding_local_var->supp_feat = supp_feat;

    return pcf_for_ue_binding_local_var;
}

void OpenAPI_pcf_for_ue_binding_free(OpenAPI_pcf_for_ue_binding_t *pcf_for_ue_binding)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_for_ue_binding) {
        return;
    }
    if (pcf_for_ue_binding->supi) {
        ogs_free(pcf_for_ue_binding->supi);
        pcf_for_ue_binding->supi = NULL;
    }
    if (pcf_for_ue_binding->gpsi) {
        ogs_free(pcf_for_ue_binding->gpsi);
        pcf_for_ue_binding->gpsi = NULL;
    }
    if (pcf_for_ue_binding->pcf_for_ue_fqdn) {
        ogs_free(pcf_for_ue_binding->pcf_for_ue_fqdn);
        pcf_for_ue_binding->pcf_for_ue_fqdn = NULL;
    }
    if (pcf_for_ue_binding->pcf_for_ue_ip_end_points) {
        OpenAPI_list_for_each(pcf_for_ue_binding->pcf_for_ue_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_for_ue_binding->pcf_for_ue_ip_end_points);
        pcf_for_ue_binding->pcf_for_ue_ip_end_points = NULL;
    }
    if (pcf_for_ue_binding->pcf_id) {
        ogs_free(pcf_for_ue_binding->pcf_id);
        pcf_for_ue_binding->pcf_id = NULL;
    }
    if (pcf_for_ue_binding->pcf_set_id) {
        ogs_free(pcf_for_ue_binding->pcf_set_id);
        pcf_for_ue_binding->pcf_set_id = NULL;
    }
    if (pcf_for_ue_binding->supp_feat) {
        ogs_free(pcf_for_ue_binding->supp_feat);
        pcf_for_ue_binding->supp_feat = NULL;
    }
    ogs_free(pcf_for_ue_binding);
}

cJSON *OpenAPI_pcf_for_ue_binding_convertToJSON(OpenAPI_pcf_for_ue_binding_t *pcf_for_ue_binding)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_for_ue_binding == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [PcfForUeBinding]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pcf_for_ue_binding->supi) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", pcf_for_ue_binding->supi) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [supi]");
        goto end;
    }

    if (pcf_for_ue_binding->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", pcf_for_ue_binding->gpsi) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (pcf_for_ue_binding->pcf_for_ue_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfForUeFqdn", pcf_for_ue_binding->pcf_for_ue_fqdn) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [pcf_for_ue_fqdn]");
        goto end;
    }
    }

    if (pcf_for_ue_binding->pcf_for_ue_ip_end_points) {
    cJSON *pcf_for_ue_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfForUeIpEndPoints");
    if (pcf_for_ue_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [pcf_for_ue_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(pcf_for_ue_binding->pcf_for_ue_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [pcf_for_ue_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_for_ue_ip_end_pointsList, itemLocal);
    }
    }

    if (pcf_for_ue_binding->pcf_id) {
    if (cJSON_AddStringToObject(item, "pcfId", pcf_for_ue_binding->pcf_id) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [pcf_id]");
        goto end;
    }
    }

    if (pcf_for_ue_binding->pcf_set_id) {
    if (cJSON_AddStringToObject(item, "pcfSetId", pcf_for_ue_binding->pcf_set_id) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    if (pcf_for_ue_binding->bind_level != OpenAPI_binding_level_NULL) {
    if (cJSON_AddStringToObject(item, "bindLevel", OpenAPI_binding_level_ToString(pcf_for_ue_binding->bind_level)) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [bind_level]");
        goto end;
    }
    }

    if (pcf_for_ue_binding->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", pcf_for_ue_binding->supp_feat) == NULL) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pcf_for_ue_binding_t *OpenAPI_pcf_for_ue_binding_parseFromJSON(cJSON *pcf_for_ue_bindingJSON)
{
    OpenAPI_pcf_for_ue_binding_t *pcf_for_ue_binding_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    cJSON *pcf_for_ue_fqdn = NULL;
    cJSON *pcf_for_ue_ip_end_points = NULL;
    OpenAPI_list_t *pcf_for_ue_ip_end_pointsList = NULL;
    cJSON *pcf_id = NULL;
    cJSON *pcf_set_id = NULL;
    cJSON *bind_level = NULL;
    OpenAPI_binding_level_e bind_levelVariable = 0;
    cJSON *supp_feat = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_bindingJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [supi]");
        goto end;
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_bindingJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    pcf_for_ue_fqdn = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_bindingJSON, "pcfForUeFqdn");
    if (pcf_for_ue_fqdn) {
    if (!cJSON_IsString(pcf_for_ue_fqdn) && !cJSON_IsNull(pcf_for_ue_fqdn)) {
        ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [pcf_for_ue_fqdn]");
        goto end;
    }
    }

    pcf_for_ue_ip_end_points = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_bindingJSON, "pcfForUeIpEndPoints");
    if (pcf_for_ue_ip_end_points) {
        cJSON *pcf_for_ue_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_for_ue_ip_end_points)) {
            ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [pcf_for_ue_ip_end_points]");
            goto end;
        }

        pcf_for_ue_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_for_ue_ip_end_points_local, pcf_for_ue_ip_end_points) {
            if (!cJSON_IsObject(pcf_for_ue_ip_end_points_local)) {
                ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [pcf_for_ue_ip_end_points]");
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

    pcf_id = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_bindingJSON, "pcfId");
    if (pcf_id) {
    if (!cJSON_IsString(pcf_id) && !cJSON_IsNull(pcf_id)) {
        ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [pcf_id]");
        goto end;
    }
    }

    pcf_set_id = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_bindingJSON, "pcfSetId");
    if (pcf_set_id) {
    if (!cJSON_IsString(pcf_set_id) && !cJSON_IsNull(pcf_set_id)) {
        ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [pcf_set_id]");
        goto end;
    }
    }

    bind_level = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_bindingJSON, "bindLevel");
    if (bind_level) {
    if (!cJSON_IsString(bind_level)) {
        ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [bind_level]");
        goto end;
    }
    bind_levelVariable = OpenAPI_binding_level_FromString(bind_level->valuestring);
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(pcf_for_ue_bindingJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_pcf_for_ue_binding_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    pcf_for_ue_binding_local_var = OpenAPI_pcf_for_ue_binding_create (
        ogs_strdup(supi->valuestring),
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        pcf_for_ue_fqdn && !cJSON_IsNull(pcf_for_ue_fqdn) ? ogs_strdup(pcf_for_ue_fqdn->valuestring) : NULL,
        pcf_for_ue_ip_end_points ? pcf_for_ue_ip_end_pointsList : NULL,
        pcf_id && !cJSON_IsNull(pcf_id) ? ogs_strdup(pcf_id->valuestring) : NULL,
        pcf_set_id && !cJSON_IsNull(pcf_set_id) ? ogs_strdup(pcf_set_id->valuestring) : NULL,
        bind_level ? bind_levelVariable : 0,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return pcf_for_ue_binding_local_var;
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

OpenAPI_pcf_for_ue_binding_t *OpenAPI_pcf_for_ue_binding_copy(OpenAPI_pcf_for_ue_binding_t *dst, OpenAPI_pcf_for_ue_binding_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_for_ue_binding_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_for_ue_binding_convertToJSON() failed");
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

    OpenAPI_pcf_for_ue_binding_free(dst);
    dst = OpenAPI_pcf_for_ue_binding_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


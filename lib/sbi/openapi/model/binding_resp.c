
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "binding_resp.h"

OpenAPI_binding_resp_t *OpenAPI_binding_resp_create(
    char *pcf_sm_fqdn,
    OpenAPI_list_t *pcf_sm_ip_end_points
)
{
    OpenAPI_binding_resp_t *binding_resp_local_var = ogs_malloc(sizeof(OpenAPI_binding_resp_t));
    ogs_assert(binding_resp_local_var);

    binding_resp_local_var->pcf_sm_fqdn = pcf_sm_fqdn;
    binding_resp_local_var->pcf_sm_ip_end_points = pcf_sm_ip_end_points;

    return binding_resp_local_var;
}

void OpenAPI_binding_resp_free(OpenAPI_binding_resp_t *binding_resp)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == binding_resp) {
        return;
    }
    if (binding_resp->pcf_sm_fqdn) {
        ogs_free(binding_resp->pcf_sm_fqdn);
        binding_resp->pcf_sm_fqdn = NULL;
    }
    if (binding_resp->pcf_sm_ip_end_points) {
        OpenAPI_list_for_each(binding_resp->pcf_sm_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(binding_resp->pcf_sm_ip_end_points);
        binding_resp->pcf_sm_ip_end_points = NULL;
    }
    ogs_free(binding_resp);
}

cJSON *OpenAPI_binding_resp_convertToJSON(OpenAPI_binding_resp_t *binding_resp)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (binding_resp == NULL) {
        ogs_error("OpenAPI_binding_resp_convertToJSON() failed [BindingResp]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (binding_resp->pcf_sm_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfSmFqdn", binding_resp->pcf_sm_fqdn) == NULL) {
        ogs_error("OpenAPI_binding_resp_convertToJSON() failed [pcf_sm_fqdn]");
        goto end;
    }
    }

    if (binding_resp->pcf_sm_ip_end_points) {
    cJSON *pcf_sm_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfSmIpEndPoints");
    if (pcf_sm_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_binding_resp_convertToJSON() failed [pcf_sm_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(binding_resp->pcf_sm_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_binding_resp_convertToJSON() failed [pcf_sm_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_sm_ip_end_pointsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_binding_resp_t *OpenAPI_binding_resp_parseFromJSON(cJSON *binding_respJSON)
{
    OpenAPI_binding_resp_t *binding_resp_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pcf_sm_fqdn = NULL;
    cJSON *pcf_sm_ip_end_points = NULL;
    OpenAPI_list_t *pcf_sm_ip_end_pointsList = NULL;
    pcf_sm_fqdn = cJSON_GetObjectItemCaseSensitive(binding_respJSON, "pcfSmFqdn");
    if (pcf_sm_fqdn) {
    if (!cJSON_IsString(pcf_sm_fqdn) && !cJSON_IsNull(pcf_sm_fqdn)) {
        ogs_error("OpenAPI_binding_resp_parseFromJSON() failed [pcf_sm_fqdn]");
        goto end;
    }
    }

    pcf_sm_ip_end_points = cJSON_GetObjectItemCaseSensitive(binding_respJSON, "pcfSmIpEndPoints");
    if (pcf_sm_ip_end_points) {
        cJSON *pcf_sm_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_sm_ip_end_points)) {
            ogs_error("OpenAPI_binding_resp_parseFromJSON() failed [pcf_sm_ip_end_points]");
            goto end;
        }

        pcf_sm_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_sm_ip_end_points_local, pcf_sm_ip_end_points) {
            if (!cJSON_IsObject(pcf_sm_ip_end_points_local)) {
                ogs_error("OpenAPI_binding_resp_parseFromJSON() failed [pcf_sm_ip_end_points]");
                goto end;
            }
            OpenAPI_ip_end_point_t *pcf_sm_ip_end_pointsItem = OpenAPI_ip_end_point_parseFromJSON(pcf_sm_ip_end_points_local);
            if (!pcf_sm_ip_end_pointsItem) {
                ogs_error("No pcf_sm_ip_end_pointsItem");
                goto end;
            }
            OpenAPI_list_add(pcf_sm_ip_end_pointsList, pcf_sm_ip_end_pointsItem);
        }
    }

    binding_resp_local_var = OpenAPI_binding_resp_create (
        pcf_sm_fqdn && !cJSON_IsNull(pcf_sm_fqdn) ? ogs_strdup(pcf_sm_fqdn->valuestring) : NULL,
        pcf_sm_ip_end_points ? pcf_sm_ip_end_pointsList : NULL
    );

    return binding_resp_local_var;
end:
    if (pcf_sm_ip_end_pointsList) {
        OpenAPI_list_for_each(pcf_sm_ip_end_pointsList, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(pcf_sm_ip_end_pointsList);
        pcf_sm_ip_end_pointsList = NULL;
    }
    return NULL;
}

OpenAPI_binding_resp_t *OpenAPI_binding_resp_copy(OpenAPI_binding_resp_t *dst, OpenAPI_binding_resp_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_binding_resp_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_binding_resp_convertToJSON() failed");
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

    OpenAPI_binding_resp_free(dst);
    dst = OpenAPI_binding_resp_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


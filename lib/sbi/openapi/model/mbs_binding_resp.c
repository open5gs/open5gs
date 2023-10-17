
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_binding_resp.h"

OpenAPI_mbs_binding_resp_t *OpenAPI_mbs_binding_resp_create(
    char *pcf_fqdn,
    OpenAPI_list_t *pcf_ip_end_points
)
{
    OpenAPI_mbs_binding_resp_t *mbs_binding_resp_local_var = ogs_malloc(sizeof(OpenAPI_mbs_binding_resp_t));
    ogs_assert(mbs_binding_resp_local_var);

    mbs_binding_resp_local_var->pcf_fqdn = pcf_fqdn;
    mbs_binding_resp_local_var->pcf_ip_end_points = pcf_ip_end_points;

    return mbs_binding_resp_local_var;
}

void OpenAPI_mbs_binding_resp_free(OpenAPI_mbs_binding_resp_t *mbs_binding_resp)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_binding_resp) {
        return;
    }
    if (mbs_binding_resp->pcf_fqdn) {
        ogs_free(mbs_binding_resp->pcf_fqdn);
        mbs_binding_resp->pcf_fqdn = NULL;
    }
    if (mbs_binding_resp->pcf_ip_end_points) {
        OpenAPI_list_for_each(mbs_binding_resp->pcf_ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(mbs_binding_resp->pcf_ip_end_points);
        mbs_binding_resp->pcf_ip_end_points = NULL;
    }
    ogs_free(mbs_binding_resp);
}

cJSON *OpenAPI_mbs_binding_resp_convertToJSON(OpenAPI_mbs_binding_resp_t *mbs_binding_resp)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_binding_resp == NULL) {
        ogs_error("OpenAPI_mbs_binding_resp_convertToJSON() failed [MbsBindingResp]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbs_binding_resp->pcf_fqdn) {
    if (cJSON_AddStringToObject(item, "pcfFqdn", mbs_binding_resp->pcf_fqdn) == NULL) {
        ogs_error("OpenAPI_mbs_binding_resp_convertToJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    if (mbs_binding_resp->pcf_ip_end_points) {
    cJSON *pcf_ip_end_pointsList = cJSON_AddArrayToObject(item, "pcfIpEndPoints");
    if (pcf_ip_end_pointsList == NULL) {
        ogs_error("OpenAPI_mbs_binding_resp_convertToJSON() failed [pcf_ip_end_points]");
        goto end;
    }
    OpenAPI_list_for_each(mbs_binding_resp->pcf_ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mbs_binding_resp_convertToJSON() failed [pcf_ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(pcf_ip_end_pointsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mbs_binding_resp_t *OpenAPI_mbs_binding_resp_parseFromJSON(cJSON *mbs_binding_respJSON)
{
    OpenAPI_mbs_binding_resp_t *mbs_binding_resp_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pcf_fqdn = NULL;
    cJSON *pcf_ip_end_points = NULL;
    OpenAPI_list_t *pcf_ip_end_pointsList = NULL;
    pcf_fqdn = cJSON_GetObjectItemCaseSensitive(mbs_binding_respJSON, "pcfFqdn");
    if (pcf_fqdn) {
    if (!cJSON_IsString(pcf_fqdn) && !cJSON_IsNull(pcf_fqdn)) {
        ogs_error("OpenAPI_mbs_binding_resp_parseFromJSON() failed [pcf_fqdn]");
        goto end;
    }
    }

    pcf_ip_end_points = cJSON_GetObjectItemCaseSensitive(mbs_binding_respJSON, "pcfIpEndPoints");
    if (pcf_ip_end_points) {
        cJSON *pcf_ip_end_points_local = NULL;
        if (!cJSON_IsArray(pcf_ip_end_points)) {
            ogs_error("OpenAPI_mbs_binding_resp_parseFromJSON() failed [pcf_ip_end_points]");
            goto end;
        }

        pcf_ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_ip_end_points_local, pcf_ip_end_points) {
            if (!cJSON_IsObject(pcf_ip_end_points_local)) {
                ogs_error("OpenAPI_mbs_binding_resp_parseFromJSON() failed [pcf_ip_end_points]");
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

    mbs_binding_resp_local_var = OpenAPI_mbs_binding_resp_create (
        pcf_fqdn && !cJSON_IsNull(pcf_fqdn) ? ogs_strdup(pcf_fqdn->valuestring) : NULL,
        pcf_ip_end_points ? pcf_ip_end_pointsList : NULL
    );

    return mbs_binding_resp_local_var;
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

OpenAPI_mbs_binding_resp_t *OpenAPI_mbs_binding_resp_copy(OpenAPI_mbs_binding_resp_t *dst, OpenAPI_mbs_binding_resp_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_binding_resp_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_binding_resp_convertToJSON() failed");
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

    OpenAPI_mbs_binding_resp_free(dst);
    dst = OpenAPI_mbs_binding_resp_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


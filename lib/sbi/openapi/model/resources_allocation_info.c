
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resources_allocation_info.h"

OpenAPI_resources_allocation_info_t *OpenAPI_resources_allocation_info_create(
    OpenAPI_media_component_resources_status_e mc_resourc_status,
    OpenAPI_list_t *flows,
    char *alt_ser_req
)
{
    OpenAPI_resources_allocation_info_t *resources_allocation_info_local_var = ogs_malloc(sizeof(OpenAPI_resources_allocation_info_t));
    ogs_assert(resources_allocation_info_local_var);

    resources_allocation_info_local_var->mc_resourc_status = mc_resourc_status;
    resources_allocation_info_local_var->flows = flows;
    resources_allocation_info_local_var->alt_ser_req = alt_ser_req;

    return resources_allocation_info_local_var;
}

void OpenAPI_resources_allocation_info_free(OpenAPI_resources_allocation_info_t *resources_allocation_info)
{
    if (NULL == resources_allocation_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(resources_allocation_info->flows, node) {
        OpenAPI_flows_free(node->data);
    }
    OpenAPI_list_free(resources_allocation_info->flows);
    ogs_free(resources_allocation_info->alt_ser_req);
    ogs_free(resources_allocation_info);
}

cJSON *OpenAPI_resources_allocation_info_convertToJSON(OpenAPI_resources_allocation_info_t *resources_allocation_info)
{
    cJSON *item = NULL;

    if (resources_allocation_info == NULL) {
        ogs_error("OpenAPI_resources_allocation_info_convertToJSON() failed [ResourcesAllocationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (resources_allocation_info->mc_resourc_status) {
    if (cJSON_AddStringToObject(item, "mcResourcStatus", OpenAPI_media_component_resources_status_ToString(resources_allocation_info->mc_resourc_status)) == NULL) {
        ogs_error("OpenAPI_resources_allocation_info_convertToJSON() failed [mc_resourc_status]");
        goto end;
    }
    }

    if (resources_allocation_info->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_resources_allocation_info_convertToJSON() failed [flows]");
        goto end;
    }

    OpenAPI_lnode_t *flows_node;
    if (resources_allocation_info->flows) {
        OpenAPI_list_for_each(resources_allocation_info->flows, flows_node) {
            cJSON *itemLocal = OpenAPI_flows_convertToJSON(flows_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_resources_allocation_info_convertToJSON() failed [flows]");
                goto end;
            }
            cJSON_AddItemToArray(flowsList, itemLocal);
        }
    }
    }

    if (resources_allocation_info->alt_ser_req) {
    if (cJSON_AddStringToObject(item, "altSerReq", resources_allocation_info->alt_ser_req) == NULL) {
        ogs_error("OpenAPI_resources_allocation_info_convertToJSON() failed [alt_ser_req]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_resources_allocation_info_t *OpenAPI_resources_allocation_info_parseFromJSON(cJSON *resources_allocation_infoJSON)
{
    OpenAPI_resources_allocation_info_t *resources_allocation_info_local_var = NULL;
    cJSON *mc_resourc_status = cJSON_GetObjectItemCaseSensitive(resources_allocation_infoJSON, "mcResourcStatus");

    OpenAPI_media_component_resources_status_e mc_resourc_statusVariable;
    if (mc_resourc_status) {
    if (!cJSON_IsString(mc_resourc_status)) {
        ogs_error("OpenAPI_resources_allocation_info_parseFromJSON() failed [mc_resourc_status]");
        goto end;
    }
    mc_resourc_statusVariable = OpenAPI_media_component_resources_status_FromString(mc_resourc_status->valuestring);
    }

    cJSON *flows = cJSON_GetObjectItemCaseSensitive(resources_allocation_infoJSON, "flows");

    OpenAPI_list_t *flowsList;
    if (flows) {
    cJSON *flows_local_nonprimitive;
    if (!cJSON_IsArray(flows)){
        ogs_error("OpenAPI_resources_allocation_info_parseFromJSON() failed [flows]");
        goto end;
    }

    flowsList = OpenAPI_list_create();

    cJSON_ArrayForEach(flows_local_nonprimitive, flows ) {
        if (!cJSON_IsObject(flows_local_nonprimitive)) {
            ogs_error("OpenAPI_resources_allocation_info_parseFromJSON() failed [flows]");
            goto end;
        }
        OpenAPI_flows_t *flowsItem = OpenAPI_flows_parseFromJSON(flows_local_nonprimitive);

        if (!flowsItem) {
            ogs_error("No flowsItem");
            OpenAPI_list_free(flowsList);
            goto end;
        }

        OpenAPI_list_add(flowsList, flowsItem);
    }
    }

    cJSON *alt_ser_req = cJSON_GetObjectItemCaseSensitive(resources_allocation_infoJSON, "altSerReq");

    if (alt_ser_req) {
    if (!cJSON_IsString(alt_ser_req)) {
        ogs_error("OpenAPI_resources_allocation_info_parseFromJSON() failed [alt_ser_req]");
        goto end;
    }
    }

    resources_allocation_info_local_var = OpenAPI_resources_allocation_info_create (
        mc_resourc_status ? mc_resourc_statusVariable : 0,
        flows ? flowsList : NULL,
        alt_ser_req ? ogs_strdup(alt_ser_req->valuestring) : NULL
    );

    return resources_allocation_info_local_var;
end:
    return NULL;
}

OpenAPI_resources_allocation_info_t *OpenAPI_resources_allocation_info_copy(OpenAPI_resources_allocation_info_t *dst, OpenAPI_resources_allocation_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_resources_allocation_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_resources_allocation_info_convertToJSON() failed");
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

    OpenAPI_resources_allocation_info_free(dst);
    dst = OpenAPI_resources_allocation_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


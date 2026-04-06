
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "l4s_support.h"

OpenAPI_l4s_support_t *OpenAPI_l4s_support_create(
    OpenAPI_l4s_notif_type_e notif_type,
    OpenAPI_list_t *flows
)
{
    OpenAPI_l4s_support_t *l4s_support_local_var = ogs_malloc(sizeof(OpenAPI_l4s_support_t));
    ogs_assert(l4s_support_local_var);

    l4s_support_local_var->notif_type = notif_type;
    l4s_support_local_var->flows = flows;

    return l4s_support_local_var;
}

void OpenAPI_l4s_support_free(OpenAPI_l4s_support_t *l4s_support)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == l4s_support) {
        return;
    }
    if (l4s_support->flows) {
        OpenAPI_list_for_each(l4s_support->flows, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(l4s_support->flows);
        l4s_support->flows = NULL;
    }
    ogs_free(l4s_support);
}

cJSON *OpenAPI_l4s_support_convertToJSON(OpenAPI_l4s_support_t *l4s_support)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (l4s_support == NULL) {
        ogs_error("OpenAPI_l4s_support_convertToJSON() failed [L4sSupport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (l4s_support->notif_type == OpenAPI_l4s_notif_type_NULL) {
        ogs_error("OpenAPI_l4s_support_convertToJSON() failed [notif_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifType", OpenAPI_l4s_notif_type_ToString(l4s_support->notif_type)) == NULL) {
        ogs_error("OpenAPI_l4s_support_convertToJSON() failed [notif_type]");
        goto end;
    }

    if (l4s_support->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_l4s_support_convertToJSON() failed [flows]");
        goto end;
    }
    OpenAPI_list_for_each(l4s_support->flows, node) {
        cJSON *itemLocal = OpenAPI_flows_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_l4s_support_convertToJSON() failed [flows]");
            goto end;
        }
        cJSON_AddItemToArray(flowsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_l4s_support_t *OpenAPI_l4s_support_parseFromJSON(cJSON *l4s_supportJSON)
{
    OpenAPI_l4s_support_t *l4s_support_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notif_type = NULL;
    OpenAPI_l4s_notif_type_e notif_typeVariable = 0;
    cJSON *flows = NULL;
    OpenAPI_list_t *flowsList = NULL;
    notif_type = cJSON_GetObjectItemCaseSensitive(l4s_supportJSON, "notifType");
    if (!notif_type) {
        ogs_error("OpenAPI_l4s_support_parseFromJSON() failed [notif_type]");
        goto end;
    }
    if (!cJSON_IsString(notif_type)) {
        ogs_error("OpenAPI_l4s_support_parseFromJSON() failed [notif_type]");
        goto end;
    }
    notif_typeVariable = OpenAPI_l4s_notif_type_FromString(notif_type->valuestring);

    flows = cJSON_GetObjectItemCaseSensitive(l4s_supportJSON, "flows");
    if (flows) {
        cJSON *flows_local = NULL;
        if (!cJSON_IsArray(flows)) {
            ogs_error("OpenAPI_l4s_support_parseFromJSON() failed [flows]");
            goto end;
        }

        flowsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flows_local, flows) {
            if (!cJSON_IsObject(flows_local)) {
                ogs_error("OpenAPI_l4s_support_parseFromJSON() failed [flows]");
                goto end;
            }
            OpenAPI_flows_t *flowsItem = OpenAPI_flows_parseFromJSON(flows_local);
            if (!flowsItem) {
                ogs_error("No flowsItem");
                goto end;
            }
            OpenAPI_list_add(flowsList, flowsItem);
        }
    }

    l4s_support_local_var = OpenAPI_l4s_support_create (
        notif_typeVariable,
        flows ? flowsList : NULL
    );

    return l4s_support_local_var;
end:
    if (flowsList) {
        OpenAPI_list_for_each(flowsList, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(flowsList);
        flowsList = NULL;
    }
    return NULL;
}

OpenAPI_l4s_support_t *OpenAPI_l4s_support_copy(OpenAPI_l4s_support_t *dst, OpenAPI_l4s_support_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_l4s_support_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_l4s_support_convertToJSON() failed");
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

    OpenAPI_l4s_support_free(dst);
    dst = OpenAPI_l4s_support_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


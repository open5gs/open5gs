
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bat_offset_info.h"

OpenAPI_bat_offset_info_t *OpenAPI_bat_offset_info_create(
    int ran_bat_offset_notif,
    bool is_adj_period,
    int adj_period,
    OpenAPI_list_t *flows
)
{
    OpenAPI_bat_offset_info_t *bat_offset_info_local_var = ogs_malloc(sizeof(OpenAPI_bat_offset_info_t));
    ogs_assert(bat_offset_info_local_var);

    bat_offset_info_local_var->ran_bat_offset_notif = ran_bat_offset_notif;
    bat_offset_info_local_var->is_adj_period = is_adj_period;
    bat_offset_info_local_var->adj_period = adj_period;
    bat_offset_info_local_var->flows = flows;

    return bat_offset_info_local_var;
}

void OpenAPI_bat_offset_info_free(OpenAPI_bat_offset_info_t *bat_offset_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bat_offset_info) {
        return;
    }
    if (bat_offset_info->flows) {
        OpenAPI_list_for_each(bat_offset_info->flows, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(bat_offset_info->flows);
        bat_offset_info->flows = NULL;
    }
    ogs_free(bat_offset_info);
}

cJSON *OpenAPI_bat_offset_info_convertToJSON(OpenAPI_bat_offset_info_t *bat_offset_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bat_offset_info == NULL) {
        ogs_error("OpenAPI_bat_offset_info_convertToJSON() failed [BatOffsetInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "ranBatOffsetNotif", bat_offset_info->ran_bat_offset_notif) == NULL) {
        ogs_error("OpenAPI_bat_offset_info_convertToJSON() failed [ran_bat_offset_notif]");
        goto end;
    }

    if (bat_offset_info->is_adj_period) {
    if (cJSON_AddNumberToObject(item, "adjPeriod", bat_offset_info->adj_period) == NULL) {
        ogs_error("OpenAPI_bat_offset_info_convertToJSON() failed [adj_period]");
        goto end;
    }
    }

    if (bat_offset_info->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_bat_offset_info_convertToJSON() failed [flows]");
        goto end;
    }
    OpenAPI_list_for_each(bat_offset_info->flows, node) {
        cJSON *itemLocal = OpenAPI_flows_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_bat_offset_info_convertToJSON() failed [flows]");
            goto end;
        }
        cJSON_AddItemToArray(flowsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_bat_offset_info_t *OpenAPI_bat_offset_info_parseFromJSON(cJSON *bat_offset_infoJSON)
{
    OpenAPI_bat_offset_info_t *bat_offset_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ran_bat_offset_notif = NULL;
    cJSON *adj_period = NULL;
    cJSON *flows = NULL;
    OpenAPI_list_t *flowsList = NULL;
    ran_bat_offset_notif = cJSON_GetObjectItemCaseSensitive(bat_offset_infoJSON, "ranBatOffsetNotif");
    if (!ran_bat_offset_notif) {
        ogs_error("OpenAPI_bat_offset_info_parseFromJSON() failed [ran_bat_offset_notif]");
        goto end;
    }
    if (!cJSON_IsNumber(ran_bat_offset_notif)) {
        ogs_error("OpenAPI_bat_offset_info_parseFromJSON() failed [ran_bat_offset_notif]");
        goto end;
    }

    adj_period = cJSON_GetObjectItemCaseSensitive(bat_offset_infoJSON, "adjPeriod");
    if (adj_period) {
    if (!cJSON_IsNumber(adj_period)) {
        ogs_error("OpenAPI_bat_offset_info_parseFromJSON() failed [adj_period]");
        goto end;
    }
    }

    flows = cJSON_GetObjectItemCaseSensitive(bat_offset_infoJSON, "flows");
    if (flows) {
        cJSON *flows_local = NULL;
        if (!cJSON_IsArray(flows)) {
            ogs_error("OpenAPI_bat_offset_info_parseFromJSON() failed [flows]");
            goto end;
        }

        flowsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flows_local, flows) {
            if (!cJSON_IsObject(flows_local)) {
                ogs_error("OpenAPI_bat_offset_info_parseFromJSON() failed [flows]");
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

    bat_offset_info_local_var = OpenAPI_bat_offset_info_create (
        
        ran_bat_offset_notif->valuedouble,
        adj_period ? true : false,
        adj_period ? adj_period->valuedouble : 0,
        flows ? flowsList : NULL
    );

    return bat_offset_info_local_var;
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

OpenAPI_bat_offset_info_t *OpenAPI_bat_offset_info_copy(OpenAPI_bat_offset_info_t *dst, OpenAPI_bat_offset_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bat_offset_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bat_offset_info_convertToJSON() failed");
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

    OpenAPI_bat_offset_info_free(dst);
    dst = OpenAPI_bat_offset_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


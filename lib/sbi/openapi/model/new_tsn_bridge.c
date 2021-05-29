
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "new_tsn_bridge.h"

OpenAPI_new_tsn_bridge_t *OpenAPI_new_tsn_bridge_create(
    OpenAPI_tsn_bridge_info_t *tsn_bridge_info,
    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt,
    OpenAPI_list_t *tsn_port_man_cont_nwtts
    )
{
    OpenAPI_new_tsn_bridge_t *new_tsn_bridge_local_var = OpenAPI_malloc(sizeof(OpenAPI_new_tsn_bridge_t));
    if (!new_tsn_bridge_local_var) {
        return NULL;
    }
    new_tsn_bridge_local_var->tsn_bridge_info = tsn_bridge_info;
    new_tsn_bridge_local_var->tsn_port_man_cont_dstt = tsn_port_man_cont_dstt;
    new_tsn_bridge_local_var->tsn_port_man_cont_nwtts = tsn_port_man_cont_nwtts;

    return new_tsn_bridge_local_var;
}

void OpenAPI_new_tsn_bridge_free(OpenAPI_new_tsn_bridge_t *new_tsn_bridge)
{
    if (NULL == new_tsn_bridge) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_tsn_bridge_info_free(new_tsn_bridge->tsn_bridge_info);
    OpenAPI_port_management_container_free(new_tsn_bridge->tsn_port_man_cont_dstt);
    OpenAPI_list_for_each(new_tsn_bridge->tsn_port_man_cont_nwtts, node) {
        OpenAPI_port_management_container_free(node->data);
    }
    OpenAPI_list_free(new_tsn_bridge->tsn_port_man_cont_nwtts);
    ogs_free(new_tsn_bridge);
}

cJSON *OpenAPI_new_tsn_bridge_convertToJSON(OpenAPI_new_tsn_bridge_t *new_tsn_bridge)
{
    cJSON *item = NULL;

    if (new_tsn_bridge == NULL) {
        ogs_error("OpenAPI_new_tsn_bridge_convertToJSON() failed [NewTsnBridge]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *tsn_bridge_info_local_JSON = OpenAPI_tsn_bridge_info_convertToJSON(new_tsn_bridge->tsn_bridge_info);
    if (tsn_bridge_info_local_JSON == NULL) {
        ogs_error("OpenAPI_new_tsn_bridge_convertToJSON() failed [tsn_bridge_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnBridgeInfo", tsn_bridge_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_new_tsn_bridge_convertToJSON() failed [tsn_bridge_info]");
        goto end;
    }

    if (new_tsn_bridge->tsn_port_man_cont_dstt) {
        cJSON *tsn_port_man_cont_dstt_local_JSON = OpenAPI_port_management_container_convertToJSON(new_tsn_bridge->tsn_port_man_cont_dstt);
        if (tsn_port_man_cont_dstt_local_JSON == NULL) {
            ogs_error("OpenAPI_new_tsn_bridge_convertToJSON() failed [tsn_port_man_cont_dstt]");
            goto end;
        }
        cJSON_AddItemToObject(item, "tsnPortManContDstt", tsn_port_man_cont_dstt_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_new_tsn_bridge_convertToJSON() failed [tsn_port_man_cont_dstt]");
            goto end;
        }
    }

    if (new_tsn_bridge->tsn_port_man_cont_nwtts) {
        cJSON *tsn_port_man_cont_nwttsList = cJSON_AddArrayToObject(item, "tsnPortManContNwtts");
        if (tsn_port_man_cont_nwttsList == NULL) {
            ogs_error("OpenAPI_new_tsn_bridge_convertToJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }

        OpenAPI_lnode_t *tsn_port_man_cont_nwtts_node;
        if (new_tsn_bridge->tsn_port_man_cont_nwtts) {
            OpenAPI_list_for_each(new_tsn_bridge->tsn_port_man_cont_nwtts, tsn_port_man_cont_nwtts_node) {
                cJSON *itemLocal = OpenAPI_port_management_container_convertToJSON(tsn_port_man_cont_nwtts_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_new_tsn_bridge_convertToJSON() failed [tsn_port_man_cont_nwtts]");
                    goto end;
                }
                cJSON_AddItemToArray(tsn_port_man_cont_nwttsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_new_tsn_bridge_t *OpenAPI_new_tsn_bridge_parseFromJSON(cJSON *new_tsn_bridgeJSON)
{
    OpenAPI_new_tsn_bridge_t *new_tsn_bridge_local_var = NULL;
    cJSON *tsn_bridge_info = cJSON_GetObjectItemCaseSensitive(new_tsn_bridgeJSON, "tsnBridgeInfo");
    if (!tsn_bridge_info) {
        ogs_error("OpenAPI_new_tsn_bridge_parseFromJSON() failed [tsn_bridge_info]");
        goto end;
    }

    OpenAPI_tsn_bridge_info_t *tsn_bridge_info_local_nonprim = NULL;

    tsn_bridge_info_local_nonprim = OpenAPI_tsn_bridge_info_parseFromJSON(tsn_bridge_info);

    cJSON *tsn_port_man_cont_dstt = cJSON_GetObjectItemCaseSensitive(new_tsn_bridgeJSON, "tsnPortManContDstt");

    OpenAPI_port_management_container_t *tsn_port_man_cont_dstt_local_nonprim = NULL;
    if (tsn_port_man_cont_dstt) {
        tsn_port_man_cont_dstt_local_nonprim = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_dstt);
    }

    cJSON *tsn_port_man_cont_nwtts = cJSON_GetObjectItemCaseSensitive(new_tsn_bridgeJSON, "tsnPortManContNwtts");

    OpenAPI_list_t *tsn_port_man_cont_nwttsList;
    if (tsn_port_man_cont_nwtts) {
        cJSON *tsn_port_man_cont_nwtts_local_nonprimitive;
        if (!cJSON_IsArray(tsn_port_man_cont_nwtts)) {
            ogs_error("OpenAPI_new_tsn_bridge_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
            goto end;
        }

        tsn_port_man_cont_nwttsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tsn_port_man_cont_nwtts_local_nonprimitive, tsn_port_man_cont_nwtts ) {
            if (!cJSON_IsObject(tsn_port_man_cont_nwtts_local_nonprimitive)) {
                ogs_error("OpenAPI_new_tsn_bridge_parseFromJSON() failed [tsn_port_man_cont_nwtts]");
                goto end;
            }
            OpenAPI_port_management_container_t *tsn_port_man_cont_nwttsItem = OpenAPI_port_management_container_parseFromJSON(tsn_port_man_cont_nwtts_local_nonprimitive);

            OpenAPI_list_add(tsn_port_man_cont_nwttsList, tsn_port_man_cont_nwttsItem);
        }
    }

    new_tsn_bridge_local_var = OpenAPI_new_tsn_bridge_create (
        tsn_bridge_info_local_nonprim,
        tsn_port_man_cont_dstt ? tsn_port_man_cont_dstt_local_nonprim : NULL,
        tsn_port_man_cont_nwtts ? tsn_port_man_cont_nwttsList : NULL
        );

    return new_tsn_bridge_local_var;
end:
    return NULL;
}

OpenAPI_new_tsn_bridge_t *OpenAPI_new_tsn_bridge_copy(OpenAPI_new_tsn_bridge_t *dst, OpenAPI_new_tsn_bridge_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_new_tsn_bridge_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_new_tsn_bridge_convertToJSON() failed");
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

    OpenAPI_new_tsn_bridge_free(dst);
    dst = OpenAPI_new_tsn_bridge_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


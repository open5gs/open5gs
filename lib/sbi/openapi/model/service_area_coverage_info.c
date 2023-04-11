
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_area_coverage_info.h"

OpenAPI_service_area_coverage_info_t *OpenAPI_service_area_coverage_info_create(
    OpenAPI_list_t *tac_list,
    OpenAPI_plmn_id_nid_1_t *serving_network
)
{
    OpenAPI_service_area_coverage_info_t *service_area_coverage_info_local_var = ogs_malloc(sizeof(OpenAPI_service_area_coverage_info_t));
    ogs_assert(service_area_coverage_info_local_var);

    service_area_coverage_info_local_var->tac_list = tac_list;
    service_area_coverage_info_local_var->serving_network = serving_network;

    return service_area_coverage_info_local_var;
}

void OpenAPI_service_area_coverage_info_free(OpenAPI_service_area_coverage_info_t *service_area_coverage_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_area_coverage_info) {
        return;
    }
    if (service_area_coverage_info->tac_list) {
        OpenAPI_list_for_each(service_area_coverage_info->tac_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_area_coverage_info->tac_list);
        service_area_coverage_info->tac_list = NULL;
    }
    if (service_area_coverage_info->serving_network) {
        OpenAPI_plmn_id_nid_1_free(service_area_coverage_info->serving_network);
        service_area_coverage_info->serving_network = NULL;
    }
    ogs_free(service_area_coverage_info);
}

cJSON *OpenAPI_service_area_coverage_info_convertToJSON(OpenAPI_service_area_coverage_info_t *service_area_coverage_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_area_coverage_info == NULL) {
        ogs_error("OpenAPI_service_area_coverage_info_convertToJSON() failed [ServiceAreaCoverageInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!service_area_coverage_info->tac_list) {
        ogs_error("OpenAPI_service_area_coverage_info_convertToJSON() failed [tac_list]");
        return NULL;
    }
    cJSON *tac_listList = cJSON_AddArrayToObject(item, "tacList");
    if (tac_listList == NULL) {
        ogs_error("OpenAPI_service_area_coverage_info_convertToJSON() failed [tac_list]");
        goto end;
    }
    OpenAPI_list_for_each(service_area_coverage_info->tac_list, node) {
        if (cJSON_AddStringToObject(tac_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_service_area_coverage_info_convertToJSON() failed [tac_list]");
            goto end;
        }
    }

    if (service_area_coverage_info->serving_network) {
    cJSON *serving_network_local_JSON = OpenAPI_plmn_id_nid_1_convertToJSON(service_area_coverage_info->serving_network);
    if (serving_network_local_JSON == NULL) {
        ogs_error("OpenAPI_service_area_coverage_info_convertToJSON() failed [serving_network]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingNetwork", serving_network_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_area_coverage_info_convertToJSON() failed [serving_network]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_service_area_coverage_info_t *OpenAPI_service_area_coverage_info_parseFromJSON(cJSON *service_area_coverage_infoJSON)
{
    OpenAPI_service_area_coverage_info_t *service_area_coverage_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tac_list = NULL;
    OpenAPI_list_t *tac_listList = NULL;
    cJSON *serving_network = NULL;
    OpenAPI_plmn_id_nid_1_t *serving_network_local_nonprim = NULL;
    tac_list = cJSON_GetObjectItemCaseSensitive(service_area_coverage_infoJSON, "tacList");
    if (!tac_list) {
        ogs_error("OpenAPI_service_area_coverage_info_parseFromJSON() failed [tac_list]");
        goto end;
    }
        cJSON *tac_list_local = NULL;
        if (!cJSON_IsArray(tac_list)) {
            ogs_error("OpenAPI_service_area_coverage_info_parseFromJSON() failed [tac_list]");
            goto end;
        }

        tac_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tac_list_local, tac_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(tac_list_local)) {
                ogs_error("OpenAPI_service_area_coverage_info_parseFromJSON() failed [tac_list]");
                goto end;
            }
            OpenAPI_list_add(tac_listList, ogs_strdup(tac_list_local->valuestring));
        }

    serving_network = cJSON_GetObjectItemCaseSensitive(service_area_coverage_infoJSON, "servingNetwork");
    if (serving_network) {
    serving_network_local_nonprim = OpenAPI_plmn_id_nid_1_parseFromJSON(serving_network);
    if (!serving_network_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_1_parseFromJSON failed [serving_network]");
        goto end;
    }
    }

    service_area_coverage_info_local_var = OpenAPI_service_area_coverage_info_create (
        tac_listList,
        serving_network ? serving_network_local_nonprim : NULL
    );

    return service_area_coverage_info_local_var;
end:
    if (tac_listList) {
        OpenAPI_list_for_each(tac_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(tac_listList);
        tac_listList = NULL;
    }
    if (serving_network_local_nonprim) {
        OpenAPI_plmn_id_nid_1_free(serving_network_local_nonprim);
        serving_network_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_service_area_coverage_info_t *OpenAPI_service_area_coverage_info_copy(OpenAPI_service_area_coverage_info_t *dst, OpenAPI_service_area_coverage_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_area_coverage_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_area_coverage_info_convertToJSON() failed");
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

    OpenAPI_service_area_coverage_info_free(dst);
    dst = OpenAPI_service_area_coverage_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "acceptable_service_info.h"

OpenAPI_acceptable_service_info_t *OpenAPI_acceptable_service_info_create(
    OpenAPI_list_t* acc_bw_med_comps,
    char *mar_bw_ul,
    char *mar_bw_dl
)
{
    OpenAPI_acceptable_service_info_t *acceptable_service_info_local_var = ogs_malloc(sizeof(OpenAPI_acceptable_service_info_t));
    ogs_assert(acceptable_service_info_local_var);

    acceptable_service_info_local_var->acc_bw_med_comps = acc_bw_med_comps;
    acceptable_service_info_local_var->mar_bw_ul = mar_bw_ul;
    acceptable_service_info_local_var->mar_bw_dl = mar_bw_dl;

    return acceptable_service_info_local_var;
}

void OpenAPI_acceptable_service_info_free(OpenAPI_acceptable_service_info_t *acceptable_service_info)
{
    if (NULL == acceptable_service_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(acceptable_service_info->acc_bw_med_comps, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_media_component_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(acceptable_service_info->acc_bw_med_comps);
    ogs_free(acceptable_service_info->mar_bw_ul);
    ogs_free(acceptable_service_info->mar_bw_dl);
    ogs_free(acceptable_service_info);
}

cJSON *OpenAPI_acceptable_service_info_convertToJSON(OpenAPI_acceptable_service_info_t *acceptable_service_info)
{
    cJSON *item = NULL;

    if (acceptable_service_info == NULL) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [AcceptableServiceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (acceptable_service_info->acc_bw_med_comps) {
    cJSON *acc_bw_med_comps = cJSON_AddObjectToObject(item, "accBwMedComps");
    if (acc_bw_med_comps == NULL) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [acc_bw_med_comps]");
        goto end;
    }
    cJSON *localMapObject = acc_bw_med_comps;
    OpenAPI_lnode_t *acc_bw_med_comps_node;
    if (acceptable_service_info->acc_bw_med_comps) {
        OpenAPI_list_for_each(acceptable_service_info->acc_bw_med_comps, acc_bw_med_comps_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)acc_bw_med_comps_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_media_component_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [acc_bw_med_comps]");
            goto end;
        }
        cJSON_AddItemToObject(acc_bw_med_comps, localKeyValue->key, itemLocal);
            }
        }
    }

    if (acceptable_service_info->mar_bw_ul) {
    if (cJSON_AddStringToObject(item, "marBwUl", acceptable_service_info->mar_bw_ul) == NULL) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    if (acceptable_service_info->mar_bw_dl) {
    if (cJSON_AddStringToObject(item, "marBwDl", acceptable_service_info->mar_bw_dl) == NULL) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_acceptable_service_info_t *OpenAPI_acceptable_service_info_parseFromJSON(cJSON *acceptable_service_infoJSON)
{
    OpenAPI_acceptable_service_info_t *acceptable_service_info_local_var = NULL;
    cJSON *acc_bw_med_comps = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "accBwMedComps");

    OpenAPI_list_t *acc_bw_med_compsList;
    if (acc_bw_med_comps) {
    cJSON *acc_bw_med_comps_local_map;
    if (!cJSON_IsObject(acc_bw_med_comps)) {
        ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [acc_bw_med_comps]");
        goto end;
    }
    acc_bw_med_compsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(acc_bw_med_comps_local_map, acc_bw_med_comps) {
        cJSON *localMapObject = acc_bw_med_comps_local_map;
        if (cJSON_IsObject(acc_bw_med_comps_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_media_component_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(acc_bw_med_comps_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [acc_bw_med_comps]");
            goto end;
        }
        OpenAPI_list_add(acc_bw_med_compsList , localMapKeyPair);
    }
    }

    cJSON *mar_bw_ul = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "marBwUl");

    if (mar_bw_ul) {
    if (!cJSON_IsString(mar_bw_ul)) {
        ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    cJSON *mar_bw_dl = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "marBwDl");

    if (mar_bw_dl) {
    if (!cJSON_IsString(mar_bw_dl)) {
        ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    acceptable_service_info_local_var = OpenAPI_acceptable_service_info_create (
        acc_bw_med_comps ? acc_bw_med_compsList : NULL,
        mar_bw_ul ? ogs_strdup(mar_bw_ul->valuestring) : NULL,
        mar_bw_dl ? ogs_strdup(mar_bw_dl->valuestring) : NULL
    );

    return acceptable_service_info_local_var;
end:
    return NULL;
}

OpenAPI_acceptable_service_info_t *OpenAPI_acceptable_service_info_copy(OpenAPI_acceptable_service_info_t *dst, OpenAPI_acceptable_service_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_acceptable_service_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed");
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

    OpenAPI_acceptable_service_info_free(dst);
    dst = OpenAPI_acceptable_service_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


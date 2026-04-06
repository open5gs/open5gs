
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "acceptable_service_info.h"

OpenAPI_acceptable_service_info_t *OpenAPI_acceptable_service_info_create(
    OpenAPI_list_t* acc_bw_med_comps,
    char *mar_bw_ul,
    char *mar_bw_dl,
    char *mir_bw_ul,
    char *mir_bw_dl,
    OpenAPI_tsn_qos_container_t *tsn_qos,
    OpenAPI_list_t *add_acc_qos_combs
)
{
    OpenAPI_acceptable_service_info_t *acceptable_service_info_local_var = ogs_malloc(sizeof(OpenAPI_acceptable_service_info_t));
    ogs_assert(acceptable_service_info_local_var);

    acceptable_service_info_local_var->acc_bw_med_comps = acc_bw_med_comps;
    acceptable_service_info_local_var->mar_bw_ul = mar_bw_ul;
    acceptable_service_info_local_var->mar_bw_dl = mar_bw_dl;
    acceptable_service_info_local_var->mir_bw_ul = mir_bw_ul;
    acceptable_service_info_local_var->mir_bw_dl = mir_bw_dl;
    acceptable_service_info_local_var->tsn_qos = tsn_qos;
    acceptable_service_info_local_var->add_acc_qos_combs = add_acc_qos_combs;

    return acceptable_service_info_local_var;
}

void OpenAPI_acceptable_service_info_free(OpenAPI_acceptable_service_info_t *acceptable_service_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == acceptable_service_info) {
        return;
    }
    if (acceptable_service_info->acc_bw_med_comps) {
        OpenAPI_list_for_each(acceptable_service_info->acc_bw_med_comps, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_media_component_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(acceptable_service_info->acc_bw_med_comps);
        acceptable_service_info->acc_bw_med_comps = NULL;
    }
    if (acceptable_service_info->mar_bw_ul) {
        ogs_free(acceptable_service_info->mar_bw_ul);
        acceptable_service_info->mar_bw_ul = NULL;
    }
    if (acceptable_service_info->mar_bw_dl) {
        ogs_free(acceptable_service_info->mar_bw_dl);
        acceptable_service_info->mar_bw_dl = NULL;
    }
    if (acceptable_service_info->mir_bw_ul) {
        ogs_free(acceptable_service_info->mir_bw_ul);
        acceptable_service_info->mir_bw_ul = NULL;
    }
    if (acceptable_service_info->mir_bw_dl) {
        ogs_free(acceptable_service_info->mir_bw_dl);
        acceptable_service_info->mir_bw_dl = NULL;
    }
    if (acceptable_service_info->tsn_qos) {
        OpenAPI_tsn_qos_container_free(acceptable_service_info->tsn_qos);
        acceptable_service_info->tsn_qos = NULL;
    }
    if (acceptable_service_info->add_acc_qos_combs) {
        OpenAPI_list_for_each(acceptable_service_info->add_acc_qos_combs, node) {
            OpenAPI_acceptable_service_info_free(node->data);
        }
        OpenAPI_list_free(acceptable_service_info->add_acc_qos_combs);
        acceptable_service_info->add_acc_qos_combs = NULL;
    }
    ogs_free(acceptable_service_info);
}

cJSON *OpenAPI_acceptable_service_info_convertToJSON(OpenAPI_acceptable_service_info_t *acceptable_service_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    if (acceptable_service_info->acc_bw_med_comps) {
        OpenAPI_list_for_each(acceptable_service_info->acc_bw_med_comps, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [acc_bw_med_comps]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [acc_bw_med_comps]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_media_component_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
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

    if (acceptable_service_info->mir_bw_ul) {
    if (cJSON_AddStringToObject(item, "mirBwUl", acceptable_service_info->mir_bw_ul) == NULL) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [mir_bw_ul]");
        goto end;
    }
    }

    if (acceptable_service_info->mir_bw_dl) {
    if (cJSON_AddStringToObject(item, "mirBwDl", acceptable_service_info->mir_bw_dl) == NULL) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [mir_bw_dl]");
        goto end;
    }
    }

    if (acceptable_service_info->tsn_qos) {
    cJSON *tsn_qos_local_JSON = OpenAPI_tsn_qos_container_convertToJSON(acceptable_service_info->tsn_qos);
    if (tsn_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [tsn_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tsnQos", tsn_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [tsn_qos]");
        goto end;
    }
    }

    if (acceptable_service_info->add_acc_qos_combs) {
    cJSON *add_acc_qos_combsList = cJSON_AddArrayToObject(item, "addAccQosCombs");
    if (add_acc_qos_combsList == NULL) {
        ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [add_acc_qos_combs]");
        goto end;
    }
    OpenAPI_list_for_each(acceptable_service_info->add_acc_qos_combs, node) {
        cJSON *itemLocal = OpenAPI_acceptable_service_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_acceptable_service_info_convertToJSON() failed [add_acc_qos_combs]");
            goto end;
        }
        cJSON_AddItemToArray(add_acc_qos_combsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_acceptable_service_info_t *OpenAPI_acceptable_service_info_parseFromJSON(cJSON *acceptable_service_infoJSON)
{
    OpenAPI_acceptable_service_info_t *acceptable_service_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *acc_bw_med_comps = NULL;
    OpenAPI_list_t *acc_bw_med_compsList = NULL;
    cJSON *mar_bw_ul = NULL;
    cJSON *mar_bw_dl = NULL;
    cJSON *mir_bw_ul = NULL;
    cJSON *mir_bw_dl = NULL;
    cJSON *tsn_qos = NULL;
    OpenAPI_tsn_qos_container_t *tsn_qos_local_nonprim = NULL;
    cJSON *add_acc_qos_combs = NULL;
    OpenAPI_list_t *add_acc_qos_combsList = NULL;
    acc_bw_med_comps = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "accBwMedComps");
    if (acc_bw_med_comps) {
        cJSON *acc_bw_med_comps_local_map = NULL;
        if (!cJSON_IsObject(acc_bw_med_comps) && !cJSON_IsNull(acc_bw_med_comps)) {
            ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [acc_bw_med_comps]");
            goto end;
        }
        if (cJSON_IsObject(acc_bw_med_comps)) {
            acc_bw_med_compsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(acc_bw_med_comps_local_map, acc_bw_med_comps) {
                cJSON *localMapObject = acc_bw_med_comps_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_media_component_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(acc_bw_med_compsList, localMapKeyPair);
            }
        }
    }

    mar_bw_ul = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "marBwUl");
    if (mar_bw_ul) {
    if (!cJSON_IsString(mar_bw_ul) && !cJSON_IsNull(mar_bw_ul)) {
        ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [mar_bw_ul]");
        goto end;
    }
    }

    mar_bw_dl = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "marBwDl");
    if (mar_bw_dl) {
    if (!cJSON_IsString(mar_bw_dl) && !cJSON_IsNull(mar_bw_dl)) {
        ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [mar_bw_dl]");
        goto end;
    }
    }

    mir_bw_ul = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "mirBwUl");
    if (mir_bw_ul) {
    if (!cJSON_IsString(mir_bw_ul) && !cJSON_IsNull(mir_bw_ul)) {
        ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [mir_bw_ul]");
        goto end;
    }
    }

    mir_bw_dl = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "mirBwDl");
    if (mir_bw_dl) {
    if (!cJSON_IsString(mir_bw_dl) && !cJSON_IsNull(mir_bw_dl)) {
        ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [mir_bw_dl]");
        goto end;
    }
    }

    tsn_qos = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "tsnQos");
    if (tsn_qos) {
    tsn_qos_local_nonprim = OpenAPI_tsn_qos_container_parseFromJSON(tsn_qos);
    if (!tsn_qos_local_nonprim) {
        ogs_error("OpenAPI_tsn_qos_container_parseFromJSON failed [tsn_qos]");
        goto end;
    }
    }

    add_acc_qos_combs = cJSON_GetObjectItemCaseSensitive(acceptable_service_infoJSON, "addAccQosCombs");
    if (add_acc_qos_combs) {
        cJSON *add_acc_qos_combs_local = NULL;
        if (!cJSON_IsArray(add_acc_qos_combs)) {
            ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [add_acc_qos_combs]");
            goto end;
        }

        add_acc_qos_combsList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_acc_qos_combs_local, add_acc_qos_combs) {
            if (!cJSON_IsObject(add_acc_qos_combs_local)) {
                ogs_error("OpenAPI_acceptable_service_info_parseFromJSON() failed [add_acc_qos_combs]");
                goto end;
            }
            OpenAPI_acceptable_service_info_t *add_acc_qos_combsItem = OpenAPI_acceptable_service_info_parseFromJSON(add_acc_qos_combs_local);
            if (!add_acc_qos_combsItem) {
                ogs_error("No add_acc_qos_combsItem");
                goto end;
            }
            OpenAPI_list_add(add_acc_qos_combsList, add_acc_qos_combsItem);
        }
    }

    acceptable_service_info_local_var = OpenAPI_acceptable_service_info_create (
        acc_bw_med_comps ? acc_bw_med_compsList : NULL,
        mar_bw_ul && !cJSON_IsNull(mar_bw_ul) ? ogs_strdup(mar_bw_ul->valuestring) : NULL,
        mar_bw_dl && !cJSON_IsNull(mar_bw_dl) ? ogs_strdup(mar_bw_dl->valuestring) : NULL,
        mir_bw_ul && !cJSON_IsNull(mir_bw_ul) ? ogs_strdup(mir_bw_ul->valuestring) : NULL,
        mir_bw_dl && !cJSON_IsNull(mir_bw_dl) ? ogs_strdup(mir_bw_dl->valuestring) : NULL,
        tsn_qos ? tsn_qos_local_nonprim : NULL,
        add_acc_qos_combs ? add_acc_qos_combsList : NULL
    );

    return acceptable_service_info_local_var;
end:
    if (acc_bw_med_compsList) {
        OpenAPI_list_for_each(acc_bw_med_compsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_media_component_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(acc_bw_med_compsList);
        acc_bw_med_compsList = NULL;
    }
    if (tsn_qos_local_nonprim) {
        OpenAPI_tsn_qos_container_free(tsn_qos_local_nonprim);
        tsn_qos_local_nonprim = NULL;
    }
    if (add_acc_qos_combsList) {
        OpenAPI_list_for_each(add_acc_qos_combsList, node) {
            OpenAPI_acceptable_service_info_free(node->data);
        }
        OpenAPI_list_free(add_acc_qos_combsList);
        add_acc_qos_combsList = NULL;
    }
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


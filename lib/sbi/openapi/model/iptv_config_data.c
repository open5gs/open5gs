
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "iptv_config_data.h"

OpenAPI_iptv_config_data_t *OpenAPI_iptv_config_data_create(
    char *supi,
    bool is_inter_group_id_null,
    OpenAPI_any_type_t *inter_group_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *af_app_id,
    OpenAPI_list_t* multi_acc_ctrls,
    char *supp_feat,
    char *res_uri,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_iptv_config_data_t *iptv_config_data_local_var = ogs_malloc(sizeof(OpenAPI_iptv_config_data_t));
    ogs_assert(iptv_config_data_local_var);

    iptv_config_data_local_var->supi = supi;
    iptv_config_data_local_var->is_inter_group_id_null = is_inter_group_id_null;
    iptv_config_data_local_var->inter_group_id = inter_group_id;
    iptv_config_data_local_var->dnn = dnn;
    iptv_config_data_local_var->snssai = snssai;
    iptv_config_data_local_var->af_app_id = af_app_id;
    iptv_config_data_local_var->multi_acc_ctrls = multi_acc_ctrls;
    iptv_config_data_local_var->supp_feat = supp_feat;
    iptv_config_data_local_var->res_uri = res_uri;
    iptv_config_data_local_var->reset_ids = reset_ids;

    return iptv_config_data_local_var;
}

void OpenAPI_iptv_config_data_free(OpenAPI_iptv_config_data_t *iptv_config_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == iptv_config_data) {
        return;
    }
    if (iptv_config_data->supi) {
        ogs_free(iptv_config_data->supi);
        iptv_config_data->supi = NULL;
    }
    if (iptv_config_data->inter_group_id) {
        OpenAPI_any_type_free(iptv_config_data->inter_group_id);
        iptv_config_data->inter_group_id = NULL;
    }
    if (iptv_config_data->dnn) {
        ogs_free(iptv_config_data->dnn);
        iptv_config_data->dnn = NULL;
    }
    if (iptv_config_data->snssai) {
        OpenAPI_snssai_free(iptv_config_data->snssai);
        iptv_config_data->snssai = NULL;
    }
    if (iptv_config_data->af_app_id) {
        ogs_free(iptv_config_data->af_app_id);
        iptv_config_data->af_app_id = NULL;
    }
    if (iptv_config_data->multi_acc_ctrls) {
        OpenAPI_list_for_each(iptv_config_data->multi_acc_ctrls, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_multicast_access_control_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(iptv_config_data->multi_acc_ctrls);
        iptv_config_data->multi_acc_ctrls = NULL;
    }
    if (iptv_config_data->supp_feat) {
        ogs_free(iptv_config_data->supp_feat);
        iptv_config_data->supp_feat = NULL;
    }
    if (iptv_config_data->res_uri) {
        ogs_free(iptv_config_data->res_uri);
        iptv_config_data->res_uri = NULL;
    }
    if (iptv_config_data->reset_ids) {
        OpenAPI_list_for_each(iptv_config_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(iptv_config_data->reset_ids);
        iptv_config_data->reset_ids = NULL;
    }
    ogs_free(iptv_config_data);
}

cJSON *OpenAPI_iptv_config_data_convertToJSON(OpenAPI_iptv_config_data_t *iptv_config_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (iptv_config_data == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [IptvConfigData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (iptv_config_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", iptv_config_data->supi) == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (iptv_config_data->inter_group_id) {
    cJSON *inter_group_id_object = OpenAPI_any_type_convertToJSON(iptv_config_data->inter_group_id);
    if (inter_group_id_object == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [inter_group_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "interGroupId", inter_group_id_object);
    if (item->child == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [inter_group_id]");
        goto end;
    }
    } else if (iptv_config_data->is_inter_group_id_null) {
        if (cJSON_AddNullToObject(item, "interGroupId") == NULL) {
            ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [inter_group_id]");
            goto end;
        }
    }

    if (iptv_config_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", iptv_config_data->dnn) == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (iptv_config_data->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(iptv_config_data->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (!iptv_config_data->af_app_id) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [af_app_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afAppId", iptv_config_data->af_app_id) == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [af_app_id]");
        goto end;
    }

    if (!iptv_config_data->multi_acc_ctrls) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [multi_acc_ctrls]");
        return NULL;
    }
    cJSON *multi_acc_ctrls = cJSON_AddObjectToObject(item, "multiAccCtrls");
    if (multi_acc_ctrls == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [multi_acc_ctrls]");
        goto end;
    }
    cJSON *localMapObject = multi_acc_ctrls;
    if (iptv_config_data->multi_acc_ctrls) {
        OpenAPI_list_for_each(iptv_config_data->multi_acc_ctrls, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [multi_acc_ctrls]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [multi_acc_ctrls]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_multicast_access_control_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

    if (iptv_config_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", iptv_config_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (iptv_config_data->res_uri) {
    if (cJSON_AddStringToObject(item, "resUri", iptv_config_data->res_uri) == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [res_uri]");
        goto end;
    }
    }

    if (iptv_config_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(iptv_config_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_iptv_config_data_t *OpenAPI_iptv_config_data_parseFromJSON(cJSON *iptv_config_dataJSON)
{
    OpenAPI_iptv_config_data_t *iptv_config_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *inter_group_id = NULL;
    OpenAPI_any_type_t *inter_group_id_local_object = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *af_app_id = NULL;
    cJSON *multi_acc_ctrls = NULL;
    OpenAPI_list_t *multi_acc_ctrlsList = NULL;
    cJSON *supp_feat = NULL;
    cJSON *res_uri = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    supi = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    inter_group_id = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "interGroupId");
    if (inter_group_id) {
    if (!cJSON_IsNull(inter_group_id)) {
    inter_group_id_local_object = OpenAPI_any_type_parseFromJSON(inter_group_id);
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    af_app_id = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "afAppId");
    if (!af_app_id) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [af_app_id]");
        goto end;
    }
    if (!cJSON_IsString(af_app_id)) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [af_app_id]");
        goto end;
    }

    multi_acc_ctrls = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "multiAccCtrls");
    if (!multi_acc_ctrls) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [multi_acc_ctrls]");
        goto end;
    }
        cJSON *multi_acc_ctrls_local_map = NULL;
        if (!cJSON_IsObject(multi_acc_ctrls) && !cJSON_IsNull(multi_acc_ctrls)) {
            ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [multi_acc_ctrls]");
            goto end;
        }
        if (cJSON_IsObject(multi_acc_ctrls)) {
            multi_acc_ctrlsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(multi_acc_ctrls_local_map, multi_acc_ctrls) {
                cJSON *localMapObject = multi_acc_ctrls_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_multicast_access_control_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(multi_acc_ctrlsList, localMapKeyPair);
            }
        }

    supp_feat = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    res_uri = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "resUri");
    if (res_uri) {
    if (!cJSON_IsString(res_uri) && !cJSON_IsNull(res_uri)) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [res_uri]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    iptv_config_data_local_var = OpenAPI_iptv_config_data_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        inter_group_id && cJSON_IsNull(inter_group_id) ? true : false,
        inter_group_id ? inter_group_id_local_object : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        ogs_strdup(af_app_id->valuestring),
        multi_acc_ctrlsList,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        res_uri && !cJSON_IsNull(res_uri) ? ogs_strdup(res_uri->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return iptv_config_data_local_var;
end:
    if (inter_group_id_local_object) {
        OpenAPI_any_type_free(inter_group_id_local_object);
        inter_group_id_local_object = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (multi_acc_ctrlsList) {
        OpenAPI_list_for_each(multi_acc_ctrlsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_multicast_access_control_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(multi_acc_ctrlsList);
        multi_acc_ctrlsList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    return NULL;
}

OpenAPI_iptv_config_data_t *OpenAPI_iptv_config_data_copy(OpenAPI_iptv_config_data_t *dst, OpenAPI_iptv_config_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_iptv_config_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed");
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

    OpenAPI_iptv_config_data_free(dst);
    dst = OpenAPI_iptv_config_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


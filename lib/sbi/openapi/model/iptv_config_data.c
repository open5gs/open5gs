
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "iptv_config_data.h"

OpenAPI_iptv_config_data_t *OpenAPI_iptv_config_data_create(
    char *supi,
    char *inter_group_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *af_app_id,
    OpenAPI_list_t* multi_acc_ctrls,
    char *supp_feat
    )
{
    OpenAPI_iptv_config_data_t *iptv_config_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_iptv_config_data_t));
    if (!iptv_config_data_local_var) {
        return NULL;
    }
    iptv_config_data_local_var->supi = supi;
    iptv_config_data_local_var->inter_group_id = inter_group_id;
    iptv_config_data_local_var->dnn = dnn;
    iptv_config_data_local_var->snssai = snssai;
    iptv_config_data_local_var->af_app_id = af_app_id;
    iptv_config_data_local_var->multi_acc_ctrls = multi_acc_ctrls;
    iptv_config_data_local_var->supp_feat = supp_feat;

    return iptv_config_data_local_var;
}

void OpenAPI_iptv_config_data_free(OpenAPI_iptv_config_data_t *iptv_config_data)
{
    if (NULL == iptv_config_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(iptv_config_data->supi);
    ogs_free(iptv_config_data->inter_group_id);
    ogs_free(iptv_config_data->dnn);
    OpenAPI_snssai_free(iptv_config_data->snssai);
    ogs_free(iptv_config_data->af_app_id);
    OpenAPI_list_for_each(iptv_config_data->multi_acc_ctrls, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_multicast_access_control_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(iptv_config_data->multi_acc_ctrls);
    ogs_free(iptv_config_data->supp_feat);
    ogs_free(iptv_config_data);
}

cJSON *OpenAPI_iptv_config_data_convertToJSON(OpenAPI_iptv_config_data_t *iptv_config_data)
{
    cJSON *item = NULL;

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
        if (cJSON_AddStringToObject(item, "interGroupId", iptv_config_data->inter_group_id) == NULL) {
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
        goto end;
    }
    if (cJSON_AddStringToObject(item, "afAppId", iptv_config_data->af_app_id) == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [af_app_id]");
        goto end;
    }

    if (!iptv_config_data->multi_acc_ctrls) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [multi_acc_ctrls]");
        goto end;
    }
    cJSON *multi_acc_ctrls = cJSON_AddObjectToObject(item, "multiAccCtrls");
    if (multi_acc_ctrls == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [multi_acc_ctrls]");
        goto end;
    }
    cJSON *localMapObject = multi_acc_ctrls;
    OpenAPI_lnode_t *multi_acc_ctrls_node;
    if (iptv_config_data->multi_acc_ctrls) {
        OpenAPI_list_for_each(iptv_config_data->multi_acc_ctrls, multi_acc_ctrls_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)multi_acc_ctrls_node->data;
            cJSON *itemLocal = OpenAPI_multicast_access_control_convertToJSON(localKeyValue->value);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [multi_acc_ctrls]");
                goto end;
            }
            cJSON_AddItemToObject(multi_acc_ctrls, localKeyValue->key, itemLocal);
        }
    }

    if (!iptv_config_data->supp_feat) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "suppFeat", iptv_config_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_iptv_config_data_convertToJSON() failed [supp_feat]");
        goto end;
    }

end:
    return item;
}

OpenAPI_iptv_config_data_t *OpenAPI_iptv_config_data_parseFromJSON(cJSON *iptv_config_dataJSON)
{
    OpenAPI_iptv_config_data_t *iptv_config_data_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "supi");

    if (supi) {
        if (!cJSON_IsString(supi)) {
            ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [supi]");
            goto end;
        }
    }

    cJSON *inter_group_id = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "interGroupId");

    if (inter_group_id) {
        if (!cJSON_IsString(inter_group_id)) {
            ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [inter_group_id]");
            goto end;
        }
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "dnn");

    if (dnn) {
        if (!cJSON_IsString(dnn)) {
            ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [dnn]");
            goto end;
        }
    }

    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "snssai");

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    if (snssai) {
        snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    }

    cJSON *af_app_id = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "afAppId");
    if (!af_app_id) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [af_app_id]");
        goto end;
    }


    if (!cJSON_IsString(af_app_id)) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [af_app_id]");
        goto end;
    }

    cJSON *multi_acc_ctrls = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "multiAccCtrls");
    if (!multi_acc_ctrls) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [multi_acc_ctrls]");
        goto end;
    }

    OpenAPI_list_t *multi_acc_ctrlsList;

    cJSON *multi_acc_ctrls_local_map;
    if (!cJSON_IsObject(multi_acc_ctrls)) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [multi_acc_ctrls]");
        goto end;
    }
    multi_acc_ctrlsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(multi_acc_ctrls_local_map, multi_acc_ctrls) {
        cJSON *localMapObject = multi_acc_ctrls_local_map;
        if (!cJSON_IsObject(multi_acc_ctrls_local_map)) {
            ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [multi_acc_ctrls]");
            goto end;
        }
        localMapKeyPair = OpenAPI_map_create(
            localMapObject->string, OpenAPI_multicast_access_control_parseFromJSON(localMapObject));
        OpenAPI_list_add(multi_acc_ctrlsList, localMapKeyPair);
    }

    cJSON *supp_feat = cJSON_GetObjectItemCaseSensitive(iptv_config_dataJSON, "suppFeat");
    if (!supp_feat) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }


    if (!cJSON_IsString(supp_feat)) {
        ogs_error("OpenAPI_iptv_config_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }

    iptv_config_data_local_var = OpenAPI_iptv_config_data_create (
        supi ? ogs_strdup(supi->valuestring) : NULL,
        inter_group_id ? ogs_strdup(inter_group_id->valuestring) : NULL,
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        ogs_strdup(af_app_id->valuestring),
        multi_acc_ctrlsList,
        ogs_strdup(supp_feat->valuestring)
        );

    return iptv_config_data_local_var;
end:
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


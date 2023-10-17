
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssai_1.h"

OpenAPI_nssai_1_t *OpenAPI_nssai_1_create(
    char *supported_features,
    OpenAPI_list_t *default_single_nssais,
    OpenAPI_list_t *single_nssais,
    char *provisioning_time,
    OpenAPI_list_t* additional_snssai_data,
    bool is_suppress_nssrg_ind,
    int suppress_nssrg_ind
)
{
    OpenAPI_nssai_1_t *nssai_1_local_var = ogs_malloc(sizeof(OpenAPI_nssai_1_t));
    ogs_assert(nssai_1_local_var);

    nssai_1_local_var->supported_features = supported_features;
    nssai_1_local_var->default_single_nssais = default_single_nssais;
    nssai_1_local_var->single_nssais = single_nssais;
    nssai_1_local_var->provisioning_time = provisioning_time;
    nssai_1_local_var->additional_snssai_data = additional_snssai_data;
    nssai_1_local_var->is_suppress_nssrg_ind = is_suppress_nssrg_ind;
    nssai_1_local_var->suppress_nssrg_ind = suppress_nssrg_ind;

    return nssai_1_local_var;
}

void OpenAPI_nssai_1_free(OpenAPI_nssai_1_t *nssai_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nssai_1) {
        return;
    }
    if (nssai_1->supported_features) {
        ogs_free(nssai_1->supported_features);
        nssai_1->supported_features = NULL;
    }
    if (nssai_1->default_single_nssais) {
        OpenAPI_list_for_each(nssai_1->default_single_nssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(nssai_1->default_single_nssais);
        nssai_1->default_single_nssais = NULL;
    }
    if (nssai_1->single_nssais) {
        OpenAPI_list_for_each(nssai_1->single_nssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(nssai_1->single_nssais);
        nssai_1->single_nssais = NULL;
    }
    if (nssai_1->provisioning_time) {
        ogs_free(nssai_1->provisioning_time);
        nssai_1->provisioning_time = NULL;
    }
    if (nssai_1->additional_snssai_data) {
        OpenAPI_list_for_each(nssai_1->additional_snssai_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_additional_snssai_data_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nssai_1->additional_snssai_data);
        nssai_1->additional_snssai_data = NULL;
    }
    ogs_free(nssai_1);
}

cJSON *OpenAPI_nssai_1_convertToJSON(OpenAPI_nssai_1_t *nssai_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nssai_1 == NULL) {
        ogs_error("OpenAPI_nssai_1_convertToJSON() failed [Nssai_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nssai_1->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", nssai_1->supported_features) == NULL) {
        ogs_error("OpenAPI_nssai_1_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (!nssai_1->default_single_nssais) {
        ogs_error("OpenAPI_nssai_1_convertToJSON() failed [default_single_nssais]");
        return NULL;
    }
    cJSON *default_single_nssaisList = cJSON_AddArrayToObject(item, "defaultSingleNssais");
    if (default_single_nssaisList == NULL) {
        ogs_error("OpenAPI_nssai_1_convertToJSON() failed [default_single_nssais]");
        goto end;
    }
    OpenAPI_list_for_each(nssai_1->default_single_nssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nssai_1_convertToJSON() failed [default_single_nssais]");
            goto end;
        }
        cJSON_AddItemToArray(default_single_nssaisList, itemLocal);
    }

    if (nssai_1->single_nssais) {
    cJSON *single_nssaisList = cJSON_AddArrayToObject(item, "singleNssais");
    if (single_nssaisList == NULL) {
        ogs_error("OpenAPI_nssai_1_convertToJSON() failed [single_nssais]");
        goto end;
    }
    OpenAPI_list_for_each(nssai_1->single_nssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nssai_1_convertToJSON() failed [single_nssais]");
            goto end;
        }
        cJSON_AddItemToArray(single_nssaisList, itemLocal);
    }
    }

    if (nssai_1->provisioning_time) {
    if (cJSON_AddStringToObject(item, "provisioningTime", nssai_1->provisioning_time) == NULL) {
        ogs_error("OpenAPI_nssai_1_convertToJSON() failed [provisioning_time]");
        goto end;
    }
    }

    if (nssai_1->additional_snssai_data) {
    cJSON *additional_snssai_data = cJSON_AddObjectToObject(item, "additionalSnssaiData");
    if (additional_snssai_data == NULL) {
        ogs_error("OpenAPI_nssai_1_convertToJSON() failed [additional_snssai_data]");
        goto end;
    }
    cJSON *localMapObject = additional_snssai_data;
    if (nssai_1->additional_snssai_data) {
        OpenAPI_list_for_each(nssai_1->additional_snssai_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nssai_1_convertToJSON() failed [additional_snssai_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nssai_1_convertToJSON() failed [additional_snssai_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_additional_snssai_data_1_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nssai_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nssai_1->is_suppress_nssrg_ind) {
    if (cJSON_AddBoolToObject(item, "suppressNssrgInd", nssai_1->suppress_nssrg_ind) == NULL) {
        ogs_error("OpenAPI_nssai_1_convertToJSON() failed [suppress_nssrg_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nssai_1_t *OpenAPI_nssai_1_parseFromJSON(cJSON *nssai_1JSON)
{
    OpenAPI_nssai_1_t *nssai_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supported_features = NULL;
    cJSON *default_single_nssais = NULL;
    OpenAPI_list_t *default_single_nssaisList = NULL;
    cJSON *single_nssais = NULL;
    OpenAPI_list_t *single_nssaisList = NULL;
    cJSON *provisioning_time = NULL;
    cJSON *additional_snssai_data = NULL;
    OpenAPI_list_t *additional_snssai_dataList = NULL;
    cJSON *suppress_nssrg_ind = NULL;
    supported_features = cJSON_GetObjectItemCaseSensitive(nssai_1JSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    default_single_nssais = cJSON_GetObjectItemCaseSensitive(nssai_1JSON, "defaultSingleNssais");
    if (!default_single_nssais) {
        ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [default_single_nssais]");
        goto end;
    }
        cJSON *default_single_nssais_local = NULL;
        if (!cJSON_IsArray(default_single_nssais)) {
            ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [default_single_nssais]");
            goto end;
        }

        default_single_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(default_single_nssais_local, default_single_nssais) {
            if (!cJSON_IsObject(default_single_nssais_local)) {
                ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [default_single_nssais]");
                goto end;
            }
            OpenAPI_snssai_t *default_single_nssaisItem = OpenAPI_snssai_parseFromJSON(default_single_nssais_local);
            if (!default_single_nssaisItem) {
                ogs_error("No default_single_nssaisItem");
                goto end;
            }
            OpenAPI_list_add(default_single_nssaisList, default_single_nssaisItem);
        }

    single_nssais = cJSON_GetObjectItemCaseSensitive(nssai_1JSON, "singleNssais");
    if (single_nssais) {
        cJSON *single_nssais_local = NULL;
        if (!cJSON_IsArray(single_nssais)) {
            ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [single_nssais]");
            goto end;
        }

        single_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(single_nssais_local, single_nssais) {
            if (!cJSON_IsObject(single_nssais_local)) {
                ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [single_nssais]");
                goto end;
            }
            OpenAPI_snssai_t *single_nssaisItem = OpenAPI_snssai_parseFromJSON(single_nssais_local);
            if (!single_nssaisItem) {
                ogs_error("No single_nssaisItem");
                goto end;
            }
            OpenAPI_list_add(single_nssaisList, single_nssaisItem);
        }
    }

    provisioning_time = cJSON_GetObjectItemCaseSensitive(nssai_1JSON, "provisioningTime");
    if (provisioning_time) {
    if (!cJSON_IsString(provisioning_time) && !cJSON_IsNull(provisioning_time)) {
        ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [provisioning_time]");
        goto end;
    }
    }

    additional_snssai_data = cJSON_GetObjectItemCaseSensitive(nssai_1JSON, "additionalSnssaiData");
    if (additional_snssai_data) {
        cJSON *additional_snssai_data_local_map = NULL;
        if (!cJSON_IsObject(additional_snssai_data) && !cJSON_IsNull(additional_snssai_data)) {
            ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [additional_snssai_data]");
            goto end;
        }
        if (cJSON_IsObject(additional_snssai_data)) {
            additional_snssai_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(additional_snssai_data_local_map, additional_snssai_data) {
                cJSON *localMapObject = additional_snssai_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_additional_snssai_data_1_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(additional_snssai_dataList, localMapKeyPair);
            }
        }
    }

    suppress_nssrg_ind = cJSON_GetObjectItemCaseSensitive(nssai_1JSON, "suppressNssrgInd");
    if (suppress_nssrg_ind) {
    if (!cJSON_IsBool(suppress_nssrg_ind)) {
        ogs_error("OpenAPI_nssai_1_parseFromJSON() failed [suppress_nssrg_ind]");
        goto end;
    }
    }

    nssai_1_local_var = OpenAPI_nssai_1_create (
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        default_single_nssaisList,
        single_nssais ? single_nssaisList : NULL,
        provisioning_time && !cJSON_IsNull(provisioning_time) ? ogs_strdup(provisioning_time->valuestring) : NULL,
        additional_snssai_data ? additional_snssai_dataList : NULL,
        suppress_nssrg_ind ? true : false,
        suppress_nssrg_ind ? suppress_nssrg_ind->valueint : 0
    );

    return nssai_1_local_var;
end:
    if (default_single_nssaisList) {
        OpenAPI_list_for_each(default_single_nssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(default_single_nssaisList);
        default_single_nssaisList = NULL;
    }
    if (single_nssaisList) {
        OpenAPI_list_for_each(single_nssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(single_nssaisList);
        single_nssaisList = NULL;
    }
    if (additional_snssai_dataList) {
        OpenAPI_list_for_each(additional_snssai_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_additional_snssai_data_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(additional_snssai_dataList);
        additional_snssai_dataList = NULL;
    }
    return NULL;
}

OpenAPI_nssai_1_t *OpenAPI_nssai_1_copy(OpenAPI_nssai_1_t *dst, OpenAPI_nssai_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nssai_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nssai_1_convertToJSON() failed");
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

    OpenAPI_nssai_1_free(dst);
    dst = OpenAPI_nssai_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nssai.h"

OpenAPI_nssai_t *OpenAPI_nssai_create(
    char *supported_features,
    OpenAPI_list_t *default_single_nssais,
    OpenAPI_list_t *single_nssais,
    char *provisioning_time,
    OpenAPI_list_t* additional_snssai_data
    )
{
    OpenAPI_nssai_t *nssai_local_var = OpenAPI_malloc(sizeof(OpenAPI_nssai_t));
    if (!nssai_local_var) {
        return NULL;
    }
    nssai_local_var->supported_features = supported_features;
    nssai_local_var->default_single_nssais = default_single_nssais;
    nssai_local_var->single_nssais = single_nssais;
    nssai_local_var->provisioning_time = provisioning_time;
    nssai_local_var->additional_snssai_data = additional_snssai_data;

    return nssai_local_var;
}

void OpenAPI_nssai_free(OpenAPI_nssai_t *nssai)
{
    if (NULL == nssai) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nssai->supported_features);
    OpenAPI_list_for_each(nssai->default_single_nssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(nssai->default_single_nssais);
    OpenAPI_list_for_each(nssai->single_nssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(nssai->single_nssais);
    ogs_free(nssai->provisioning_time);
    OpenAPI_list_for_each(nssai->additional_snssai_data, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_additional_snssai_data_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(nssai->additional_snssai_data);
    ogs_free(nssai);
}

cJSON *OpenAPI_nssai_convertToJSON(OpenAPI_nssai_t *nssai)
{
    cJSON *item = NULL;

    if (nssai == NULL) {
        ogs_error("OpenAPI_nssai_convertToJSON() failed [Nssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nssai->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", nssai->supported_features) == NULL) {
            ogs_error("OpenAPI_nssai_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

    if (!nssai->default_single_nssais) {
        ogs_error("OpenAPI_nssai_convertToJSON() failed [default_single_nssais]");
        goto end;
    }
    cJSON *default_single_nssaisList = cJSON_AddArrayToObject(item, "defaultSingleNssais");
    if (default_single_nssaisList == NULL) {
        ogs_error("OpenAPI_nssai_convertToJSON() failed [default_single_nssais]");
        goto end;
    }

    OpenAPI_lnode_t *default_single_nssais_node;
    if (nssai->default_single_nssais) {
        OpenAPI_list_for_each(nssai->default_single_nssais, default_single_nssais_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(default_single_nssais_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nssai_convertToJSON() failed [default_single_nssais]");
                goto end;
            }
            cJSON_AddItemToArray(default_single_nssaisList, itemLocal);
        }
    }

    if (nssai->single_nssais) {
        cJSON *single_nssaisList = cJSON_AddArrayToObject(item, "singleNssais");
        if (single_nssaisList == NULL) {
            ogs_error("OpenAPI_nssai_convertToJSON() failed [single_nssais]");
            goto end;
        }

        OpenAPI_lnode_t *single_nssais_node;
        if (nssai->single_nssais) {
            OpenAPI_list_for_each(nssai->single_nssais, single_nssais_node) {
                cJSON *itemLocal = OpenAPI_snssai_convertToJSON(single_nssais_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nssai_convertToJSON() failed [single_nssais]");
                    goto end;
                }
                cJSON_AddItemToArray(single_nssaisList, itemLocal);
            }
        }
    }

    if (nssai->provisioning_time) {
        if (cJSON_AddStringToObject(item, "provisioningTime", nssai->provisioning_time) == NULL) {
            ogs_error("OpenAPI_nssai_convertToJSON() failed [provisioning_time]");
            goto end;
        }
    }

    if (nssai->additional_snssai_data) {
        cJSON *additional_snssai_data = cJSON_AddObjectToObject(item, "additionalSnssaiData");
        if (additional_snssai_data == NULL) {
            ogs_error("OpenAPI_nssai_convertToJSON() failed [additional_snssai_data]");
            goto end;
        }
        cJSON *localMapObject = additional_snssai_data;
        OpenAPI_lnode_t *additional_snssai_data_node;
        if (nssai->additional_snssai_data) {
            OpenAPI_list_for_each(nssai->additional_snssai_data, additional_snssai_data_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)additional_snssai_data_node->data;
                cJSON *itemLocal = OpenAPI_additional_snssai_data_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nssai_convertToJSON() failed [additional_snssai_data]");
                    goto end;
                }
                cJSON_AddItemToObject(additional_snssai_data, localKeyValue->key, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_nssai_t *OpenAPI_nssai_parseFromJSON(cJSON *nssaiJSON)
{
    OpenAPI_nssai_t *nssai_local_var = NULL;
    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(nssaiJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_nssai_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *default_single_nssais = cJSON_GetObjectItemCaseSensitive(nssaiJSON, "defaultSingleNssais");
    if (!default_single_nssais) {
        ogs_error("OpenAPI_nssai_parseFromJSON() failed [default_single_nssais]");
        goto end;
    }

    OpenAPI_list_t *default_single_nssaisList;

    cJSON *default_single_nssais_local_nonprimitive;
    if (!cJSON_IsArray(default_single_nssais)) {
        ogs_error("OpenAPI_nssai_parseFromJSON() failed [default_single_nssais]");
        goto end;
    }

    default_single_nssaisList = OpenAPI_list_create();

    cJSON_ArrayForEach(default_single_nssais_local_nonprimitive, default_single_nssais ) {
        if (!cJSON_IsObject(default_single_nssais_local_nonprimitive)) {
            ogs_error("OpenAPI_nssai_parseFromJSON() failed [default_single_nssais]");
            goto end;
        }
        OpenAPI_snssai_t *default_single_nssaisItem = OpenAPI_snssai_parseFromJSON(default_single_nssais_local_nonprimitive);

        OpenAPI_list_add(default_single_nssaisList, default_single_nssaisItem);
    }

    cJSON *single_nssais = cJSON_GetObjectItemCaseSensitive(nssaiJSON, "singleNssais");

    OpenAPI_list_t *single_nssaisList;
    if (single_nssais) {
        cJSON *single_nssais_local_nonprimitive;
        if (!cJSON_IsArray(single_nssais)) {
            ogs_error("OpenAPI_nssai_parseFromJSON() failed [single_nssais]");
            goto end;
        }

        single_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(single_nssais_local_nonprimitive, single_nssais ) {
            if (!cJSON_IsObject(single_nssais_local_nonprimitive)) {
                ogs_error("OpenAPI_nssai_parseFromJSON() failed [single_nssais]");
                goto end;
            }
            OpenAPI_snssai_t *single_nssaisItem = OpenAPI_snssai_parseFromJSON(single_nssais_local_nonprimitive);

            OpenAPI_list_add(single_nssaisList, single_nssaisItem);
        }
    }

    cJSON *provisioning_time = cJSON_GetObjectItemCaseSensitive(nssaiJSON, "provisioningTime");

    if (provisioning_time) {
        if (!cJSON_IsString(provisioning_time)) {
            ogs_error("OpenAPI_nssai_parseFromJSON() failed [provisioning_time]");
            goto end;
        }
    }

    cJSON *additional_snssai_data = cJSON_GetObjectItemCaseSensitive(nssaiJSON, "additionalSnssaiData");

    OpenAPI_list_t *additional_snssai_dataList;
    if (additional_snssai_data) {
        cJSON *additional_snssai_data_local_map;
        if (!cJSON_IsObject(additional_snssai_data)) {
            ogs_error("OpenAPI_nssai_parseFromJSON() failed [additional_snssai_data]");
            goto end;
        }
        additional_snssai_dataList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(additional_snssai_data_local_map, additional_snssai_data) {
            cJSON *localMapObject = additional_snssai_data_local_map;
            if (!cJSON_IsObject(additional_snssai_data_local_map)) {
                ogs_error("OpenAPI_nssai_parseFromJSON() failed [additional_snssai_data]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_additional_snssai_data_parseFromJSON(localMapObject));
            OpenAPI_list_add(additional_snssai_dataList, localMapKeyPair);
        }
    }

    nssai_local_var = OpenAPI_nssai_create (
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        default_single_nssaisList,
        single_nssais ? single_nssaisList : NULL,
        provisioning_time ? ogs_strdup(provisioning_time->valuestring) : NULL,
        additional_snssai_data ? additional_snssai_dataList : NULL
        );

    return nssai_local_var;
end:
    return NULL;
}

OpenAPI_nssai_t *OpenAPI_nssai_copy(OpenAPI_nssai_t *dst, OpenAPI_nssai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nssai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nssai_convertToJSON() failed");
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

    OpenAPI_nssai_free(dst);
    dst = OpenAPI_nssai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


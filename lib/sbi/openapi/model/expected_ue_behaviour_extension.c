
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "expected_ue_behaviour_extension.h"

OpenAPI_expected_ue_behaviour_extension_t *OpenAPI_expected_ue_behaviour_extension_create(
    char *af_instance_id,
    int reference_id,
    OpenAPI_list_t* expected_ue_behaviour_data,
    char *mtc_provider_information
)
{
    OpenAPI_expected_ue_behaviour_extension_t *expected_ue_behaviour_extension_local_var = ogs_malloc(sizeof(OpenAPI_expected_ue_behaviour_extension_t));
    ogs_assert(expected_ue_behaviour_extension_local_var);

    expected_ue_behaviour_extension_local_var->af_instance_id = af_instance_id;
    expected_ue_behaviour_extension_local_var->reference_id = reference_id;
    expected_ue_behaviour_extension_local_var->expected_ue_behaviour_data = expected_ue_behaviour_data;
    expected_ue_behaviour_extension_local_var->mtc_provider_information = mtc_provider_information;

    return expected_ue_behaviour_extension_local_var;
}

void OpenAPI_expected_ue_behaviour_extension_free(OpenAPI_expected_ue_behaviour_extension_t *expected_ue_behaviour_extension)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == expected_ue_behaviour_extension) {
        return;
    }
    if (expected_ue_behaviour_extension->af_instance_id) {
        ogs_free(expected_ue_behaviour_extension->af_instance_id);
        expected_ue_behaviour_extension->af_instance_id = NULL;
    }
    if (expected_ue_behaviour_extension->expected_ue_behaviour_data) {
        OpenAPI_list_for_each(expected_ue_behaviour_extension->expected_ue_behaviour_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(expected_ue_behaviour_extension->expected_ue_behaviour_data);
        expected_ue_behaviour_extension->expected_ue_behaviour_data = NULL;
    }
    if (expected_ue_behaviour_extension->mtc_provider_information) {
        ogs_free(expected_ue_behaviour_extension->mtc_provider_information);
        expected_ue_behaviour_extension->mtc_provider_information = NULL;
    }
    ogs_free(expected_ue_behaviour_extension);
}

cJSON *OpenAPI_expected_ue_behaviour_extension_convertToJSON(OpenAPI_expected_ue_behaviour_extension_t *expected_ue_behaviour_extension)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (expected_ue_behaviour_extension == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed [ExpectedUeBehaviourExtension]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!expected_ue_behaviour_extension->af_instance_id) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed [af_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afInstanceId", expected_ue_behaviour_extension->af_instance_id) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed [af_instance_id]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "referenceId", expected_ue_behaviour_extension->reference_id) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed [reference_id]");
        goto end;
    }

    if (expected_ue_behaviour_extension->expected_ue_behaviour_data) {
    cJSON *expected_ue_behaviour_data = cJSON_AddObjectToObject(item, "expectedUeBehaviourData");
    if (expected_ue_behaviour_data == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed [expected_ue_behaviour_data]");
        goto end;
    }
    cJSON *localMapObject = expected_ue_behaviour_data;
    if (expected_ue_behaviour_extension->expected_ue_behaviour_data) {
        OpenAPI_list_for_each(expected_ue_behaviour_extension->expected_ue_behaviour_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed [expected_ue_behaviour_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed [expected_ue_behaviour_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_expected_ue_behaviour_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (expected_ue_behaviour_extension->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", expected_ue_behaviour_extension->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_expected_ue_behaviour_extension_t *OpenAPI_expected_ue_behaviour_extension_parseFromJSON(cJSON *expected_ue_behaviour_extensionJSON)
{
    OpenAPI_expected_ue_behaviour_extension_t *expected_ue_behaviour_extension_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *reference_id = NULL;
    cJSON *expected_ue_behaviour_data = NULL;
    OpenAPI_list_t *expected_ue_behaviour_dataList = NULL;
    cJSON *mtc_provider_information = NULL;
    af_instance_id = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_extensionJSON, "afInstanceId");
    if (!af_instance_id) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(af_instance_id)) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_parseFromJSON() failed [af_instance_id]");
        goto end;
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_extensionJSON, "referenceId");
    if (!reference_id) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_parseFromJSON() failed [reference_id]");
        goto end;
    }
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_parseFromJSON() failed [reference_id]");
        goto end;
    }

    expected_ue_behaviour_data = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_extensionJSON, "expectedUeBehaviourData");
    if (expected_ue_behaviour_data) {
        cJSON *expected_ue_behaviour_data_local_map = NULL;
        if (!cJSON_IsObject(expected_ue_behaviour_data) && !cJSON_IsNull(expected_ue_behaviour_data)) {
            ogs_error("OpenAPI_expected_ue_behaviour_extension_parseFromJSON() failed [expected_ue_behaviour_data]");
            goto end;
        }
        if (cJSON_IsObject(expected_ue_behaviour_data)) {
            expected_ue_behaviour_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(expected_ue_behaviour_data_local_map, expected_ue_behaviour_data) {
                cJSON *localMapObject = expected_ue_behaviour_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_expected_ue_behaviour_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_expected_ue_behaviour_extension_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(expected_ue_behaviour_dataList, localMapKeyPair);
            }
        }
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(expected_ue_behaviour_extensionJSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    expected_ue_behaviour_extension_local_var = OpenAPI_expected_ue_behaviour_extension_create (
        ogs_strdup(af_instance_id->valuestring),
        
        reference_id->valuedouble,
        expected_ue_behaviour_data ? expected_ue_behaviour_dataList : NULL,
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL
    );

    return expected_ue_behaviour_extension_local_var;
end:
    if (expected_ue_behaviour_dataList) {
        OpenAPI_list_for_each(expected_ue_behaviour_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(expected_ue_behaviour_dataList);
        expected_ue_behaviour_dataList = NULL;
    }
    return NULL;
}

OpenAPI_expected_ue_behaviour_extension_t *OpenAPI_expected_ue_behaviour_extension_copy(OpenAPI_expected_ue_behaviour_extension_t *dst, OpenAPI_expected_ue_behaviour_extension_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_expected_ue_behaviour_extension_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_convertToJSON() failed");
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

    OpenAPI_expected_ue_behaviour_extension_free(dst);
    dst = OpenAPI_expected_ue_behaviour_extension_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


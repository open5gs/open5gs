
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_data_for_individual_ue.h"

OpenAPI_policy_data_for_individual_ue_t *OpenAPI_policy_data_for_individual_ue_create(
    OpenAPI_ue_policy_set_t *ue_policy_data_set,
    OpenAPI_sm_policy_data_t *sm_policy_data_set,
    OpenAPI_am_policy_data_t *am_policy_data_set,
    OpenAPI_list_t* um_data,
    OpenAPI_list_t* operator_specific_data_set
)
{
    OpenAPI_policy_data_for_individual_ue_t *policy_data_for_individual_ue_local_var = ogs_malloc(sizeof(OpenAPI_policy_data_for_individual_ue_t));
    ogs_assert(policy_data_for_individual_ue_local_var);

    policy_data_for_individual_ue_local_var->ue_policy_data_set = ue_policy_data_set;
    policy_data_for_individual_ue_local_var->sm_policy_data_set = sm_policy_data_set;
    policy_data_for_individual_ue_local_var->am_policy_data_set = am_policy_data_set;
    policy_data_for_individual_ue_local_var->um_data = um_data;
    policy_data_for_individual_ue_local_var->operator_specific_data_set = operator_specific_data_set;

    return policy_data_for_individual_ue_local_var;
}

void OpenAPI_policy_data_for_individual_ue_free(OpenAPI_policy_data_for_individual_ue_t *policy_data_for_individual_ue)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == policy_data_for_individual_ue) {
        return;
    }
    if (policy_data_for_individual_ue->ue_policy_data_set) {
        OpenAPI_ue_policy_set_free(policy_data_for_individual_ue->ue_policy_data_set);
        policy_data_for_individual_ue->ue_policy_data_set = NULL;
    }
    if (policy_data_for_individual_ue->sm_policy_data_set) {
        OpenAPI_sm_policy_data_free(policy_data_for_individual_ue->sm_policy_data_set);
        policy_data_for_individual_ue->sm_policy_data_set = NULL;
    }
    if (policy_data_for_individual_ue->am_policy_data_set) {
        OpenAPI_am_policy_data_free(policy_data_for_individual_ue->am_policy_data_set);
        policy_data_for_individual_ue->am_policy_data_set = NULL;
    }
    if (policy_data_for_individual_ue->um_data) {
        OpenAPI_list_for_each(policy_data_for_individual_ue->um_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_usage_mon_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(policy_data_for_individual_ue->um_data);
        policy_data_for_individual_ue->um_data = NULL;
    }
    if (policy_data_for_individual_ue->operator_specific_data_set) {
        OpenAPI_list_for_each(policy_data_for_individual_ue->operator_specific_data_set, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_operator_specific_data_container_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(policy_data_for_individual_ue->operator_specific_data_set);
        policy_data_for_individual_ue->operator_specific_data_set = NULL;
    }
    ogs_free(policy_data_for_individual_ue);
}

cJSON *OpenAPI_policy_data_for_individual_ue_convertToJSON(OpenAPI_policy_data_for_individual_ue_t *policy_data_for_individual_ue)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (policy_data_for_individual_ue == NULL) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [PolicyDataForIndividualUe]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (policy_data_for_individual_ue->ue_policy_data_set) {
    cJSON *ue_policy_data_set_local_JSON = OpenAPI_ue_policy_set_convertToJSON(policy_data_for_individual_ue->ue_policy_data_set);
    if (ue_policy_data_set_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [ue_policy_data_set]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uePolicyDataSet", ue_policy_data_set_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [ue_policy_data_set]");
        goto end;
    }
    }

    if (policy_data_for_individual_ue->sm_policy_data_set) {
    cJSON *sm_policy_data_set_local_JSON = OpenAPI_sm_policy_data_convertToJSON(policy_data_for_individual_ue->sm_policy_data_set);
    if (sm_policy_data_set_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [sm_policy_data_set]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smPolicyDataSet", sm_policy_data_set_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [sm_policy_data_set]");
        goto end;
    }
    }

    if (policy_data_for_individual_ue->am_policy_data_set) {
    cJSON *am_policy_data_set_local_JSON = OpenAPI_am_policy_data_convertToJSON(policy_data_for_individual_ue->am_policy_data_set);
    if (am_policy_data_set_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [am_policy_data_set]");
        goto end;
    }
    cJSON_AddItemToObject(item, "amPolicyDataSet", am_policy_data_set_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [am_policy_data_set]");
        goto end;
    }
    }

    if (policy_data_for_individual_ue->um_data) {
    cJSON *um_data = cJSON_AddObjectToObject(item, "umData");
    if (um_data == NULL) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [um_data]");
        goto end;
    }
    cJSON *localMapObject = um_data;
    if (policy_data_for_individual_ue->um_data) {
        OpenAPI_list_for_each(policy_data_for_individual_ue->um_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [um_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [um_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_usage_mon_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (policy_data_for_individual_ue->operator_specific_data_set) {
    cJSON *operator_specific_data_set = cJSON_AddObjectToObject(item, "operatorSpecificDataSet");
    if (operator_specific_data_set == NULL) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [operator_specific_data_set]");
        goto end;
    }
    cJSON *localMapObject = operator_specific_data_set;
    if (policy_data_for_individual_ue->operator_specific_data_set) {
        OpenAPI_list_for_each(policy_data_for_individual_ue->operator_specific_data_set, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [operator_specific_data_set]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [operator_specific_data_set]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_operator_specific_data_container_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_policy_data_for_individual_ue_t *OpenAPI_policy_data_for_individual_ue_parseFromJSON(cJSON *policy_data_for_individual_ueJSON)
{
    OpenAPI_policy_data_for_individual_ue_t *policy_data_for_individual_ue_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_policy_data_set = NULL;
    OpenAPI_ue_policy_set_t *ue_policy_data_set_local_nonprim = NULL;
    cJSON *sm_policy_data_set = NULL;
    OpenAPI_sm_policy_data_t *sm_policy_data_set_local_nonprim = NULL;
    cJSON *am_policy_data_set = NULL;
    OpenAPI_am_policy_data_t *am_policy_data_set_local_nonprim = NULL;
    cJSON *um_data = NULL;
    OpenAPI_list_t *um_dataList = NULL;
    cJSON *operator_specific_data_set = NULL;
    OpenAPI_list_t *operator_specific_data_setList = NULL;
    ue_policy_data_set = cJSON_GetObjectItemCaseSensitive(policy_data_for_individual_ueJSON, "uePolicyDataSet");
    if (ue_policy_data_set) {
    ue_policy_data_set_local_nonprim = OpenAPI_ue_policy_set_parseFromJSON(ue_policy_data_set);
    if (!ue_policy_data_set_local_nonprim) {
        ogs_error("OpenAPI_ue_policy_set_parseFromJSON failed [ue_policy_data_set]");
        goto end;
    }
    }

    sm_policy_data_set = cJSON_GetObjectItemCaseSensitive(policy_data_for_individual_ueJSON, "smPolicyDataSet");
    if (sm_policy_data_set) {
    sm_policy_data_set_local_nonprim = OpenAPI_sm_policy_data_parseFromJSON(sm_policy_data_set);
    if (!sm_policy_data_set_local_nonprim) {
        ogs_error("OpenAPI_sm_policy_data_parseFromJSON failed [sm_policy_data_set]");
        goto end;
    }
    }

    am_policy_data_set = cJSON_GetObjectItemCaseSensitive(policy_data_for_individual_ueJSON, "amPolicyDataSet");
    if (am_policy_data_set) {
    am_policy_data_set_local_nonprim = OpenAPI_am_policy_data_parseFromJSON(am_policy_data_set);
    if (!am_policy_data_set_local_nonprim) {
        ogs_error("OpenAPI_am_policy_data_parseFromJSON failed [am_policy_data_set]");
        goto end;
    }
    }

    um_data = cJSON_GetObjectItemCaseSensitive(policy_data_for_individual_ueJSON, "umData");
    if (um_data) {
        cJSON *um_data_local_map = NULL;
        if (!cJSON_IsObject(um_data) && !cJSON_IsNull(um_data)) {
            ogs_error("OpenAPI_policy_data_for_individual_ue_parseFromJSON() failed [um_data]");
            goto end;
        }
        if (cJSON_IsObject(um_data)) {
            um_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(um_data_local_map, um_data) {
                cJSON *localMapObject = um_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_usage_mon_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_policy_data_for_individual_ue_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(um_dataList, localMapKeyPair);
            }
        }
    }

    operator_specific_data_set = cJSON_GetObjectItemCaseSensitive(policy_data_for_individual_ueJSON, "operatorSpecificDataSet");
    if (operator_specific_data_set) {
        cJSON *operator_specific_data_set_local_map = NULL;
        if (!cJSON_IsObject(operator_specific_data_set) && !cJSON_IsNull(operator_specific_data_set)) {
            ogs_error("OpenAPI_policy_data_for_individual_ue_parseFromJSON() failed [operator_specific_data_set]");
            goto end;
        }
        if (cJSON_IsObject(operator_specific_data_set)) {
            operator_specific_data_setList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(operator_specific_data_set_local_map, operator_specific_data_set) {
                cJSON *localMapObject = operator_specific_data_set_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_operator_specific_data_container_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_policy_data_for_individual_ue_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(operator_specific_data_setList, localMapKeyPair);
            }
        }
    }

    policy_data_for_individual_ue_local_var = OpenAPI_policy_data_for_individual_ue_create (
        ue_policy_data_set ? ue_policy_data_set_local_nonprim : NULL,
        sm_policy_data_set ? sm_policy_data_set_local_nonprim : NULL,
        am_policy_data_set ? am_policy_data_set_local_nonprim : NULL,
        um_data ? um_dataList : NULL,
        operator_specific_data_set ? operator_specific_data_setList : NULL
    );

    return policy_data_for_individual_ue_local_var;
end:
    if (ue_policy_data_set_local_nonprim) {
        OpenAPI_ue_policy_set_free(ue_policy_data_set_local_nonprim);
        ue_policy_data_set_local_nonprim = NULL;
    }
    if (sm_policy_data_set_local_nonprim) {
        OpenAPI_sm_policy_data_free(sm_policy_data_set_local_nonprim);
        sm_policy_data_set_local_nonprim = NULL;
    }
    if (am_policy_data_set_local_nonprim) {
        OpenAPI_am_policy_data_free(am_policy_data_set_local_nonprim);
        am_policy_data_set_local_nonprim = NULL;
    }
    if (um_dataList) {
        OpenAPI_list_for_each(um_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_usage_mon_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(um_dataList);
        um_dataList = NULL;
    }
    if (operator_specific_data_setList) {
        OpenAPI_list_for_each(operator_specific_data_setList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_operator_specific_data_container_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(operator_specific_data_setList);
        operator_specific_data_setList = NULL;
    }
    return NULL;
}

OpenAPI_policy_data_for_individual_ue_t *OpenAPI_policy_data_for_individual_ue_copy(OpenAPI_policy_data_for_individual_ue_t *dst, OpenAPI_policy_data_for_individual_ue_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_data_for_individual_ue_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_data_for_individual_ue_convertToJSON() failed");
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

    OpenAPI_policy_data_for_individual_ue_free(dst);
    dst = OpenAPI_policy_data_for_individual_ue_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dccf_cond.h"

char *OpenAPI_condition_typedccf_cond_ToString(OpenAPI_dccf_cond_condition_type_e condition_type)
{
    const char *condition_typeArray[] =  { "NULL", "DCCF_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    if (condition_type < sizeofArray)
        return (char *)condition_typeArray[condition_type];
    else
        return (char *)"Unknown";
}

OpenAPI_dccf_cond_condition_type_e OpenAPI_condition_typedccf_cond_FromString(char* condition_type)
{
    int stringToReturn = 0;
    const char *condition_typeArray[] =  { "NULL", "DCCF_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(condition_type, condition_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_dccf_cond_t *OpenAPI_dccf_cond_create(
    OpenAPI_dccf_cond_condition_type_e condition_type,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list
)
{
    OpenAPI_dccf_cond_t *dccf_cond_local_var = ogs_malloc(sizeof(OpenAPI_dccf_cond_t));
    ogs_assert(dccf_cond_local_var);

    dccf_cond_local_var->condition_type = condition_type;
    dccf_cond_local_var->tai_list = tai_list;
    dccf_cond_local_var->tai_range_list = tai_range_list;
    dccf_cond_local_var->serving_nf_type_list = serving_nf_type_list;
    dccf_cond_local_var->serving_nf_set_id_list = serving_nf_set_id_list;

    return dccf_cond_local_var;
}

void OpenAPI_dccf_cond_free(OpenAPI_dccf_cond_t *dccf_cond)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dccf_cond) {
        return;
    }
    if (dccf_cond->tai_list) {
        OpenAPI_list_for_each(dccf_cond->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(dccf_cond->tai_list);
        dccf_cond->tai_list = NULL;
    }
    if (dccf_cond->tai_range_list) {
        OpenAPI_list_for_each(dccf_cond->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(dccf_cond->tai_range_list);
        dccf_cond->tai_range_list = NULL;
    }
    if (dccf_cond->serving_nf_type_list) {
        OpenAPI_list_free(dccf_cond->serving_nf_type_list);
        dccf_cond->serving_nf_type_list = NULL;
    }
    if (dccf_cond->serving_nf_set_id_list) {
        OpenAPI_list_for_each(dccf_cond->serving_nf_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dccf_cond->serving_nf_set_id_list);
        dccf_cond->serving_nf_set_id_list = NULL;
    }
    ogs_free(dccf_cond);
}

cJSON *OpenAPI_dccf_cond_convertToJSON(OpenAPI_dccf_cond_t *dccf_cond)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dccf_cond == NULL) {
        ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [DccfCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (dccf_cond->condition_type == OpenAPI_dccf_cond_CONDITIONTYPE_NULL) {
        ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [condition_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "conditionType", OpenAPI_condition_typedccf_cond_ToString(dccf_cond->condition_type)) == NULL) {
        ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [condition_type]");
        goto end;
    }

    if (dccf_cond->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(dccf_cond->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (dccf_cond->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(dccf_cond->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (dccf_cond->serving_nf_type_list != OpenAPI_nf_type_NULL) {
    cJSON *serving_nf_type_listList = cJSON_AddArrayToObject(item, "servingNfTypeList");
    if (serving_nf_type_listList == NULL) {
        ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [serving_nf_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(dccf_cond->serving_nf_type_list, node) {
        if (cJSON_AddStringToObject(serving_nf_type_listList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [serving_nf_type_list]");
            goto end;
        }
    }
    }

    if (dccf_cond->serving_nf_set_id_list) {
    cJSON *serving_nf_set_id_listList = cJSON_AddArrayToObject(item, "servingNfSetIdList");
    if (serving_nf_set_id_listList == NULL) {
        ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [serving_nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(dccf_cond->serving_nf_set_id_list, node) {
        if (cJSON_AddStringToObject(serving_nf_set_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_dccf_cond_convertToJSON() failed [serving_nf_set_id_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_dccf_cond_t *OpenAPI_dccf_cond_parseFromJSON(cJSON *dccf_condJSON)
{
    OpenAPI_dccf_cond_t *dccf_cond_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *condition_type = NULL;
    OpenAPI_dccf_cond_condition_type_e condition_typeVariable = 0;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *serving_nf_type_list = NULL;
    OpenAPI_list_t *serving_nf_type_listList = NULL;
    cJSON *serving_nf_set_id_list = NULL;
    OpenAPI_list_t *serving_nf_set_id_listList = NULL;
    condition_type = cJSON_GetObjectItemCaseSensitive(dccf_condJSON, "conditionType");
    if (!condition_type) {
        ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }
    if (!cJSON_IsString(condition_type)) {
        ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }
    condition_typeVariable = OpenAPI_condition_typedccf_cond_FromString(condition_type->valuestring);

    tai_list = cJSON_GetObjectItemCaseSensitive(dccf_condJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(dccf_condJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    serving_nf_type_list = cJSON_GetObjectItemCaseSensitive(dccf_condJSON, "servingNfTypeList");
    if (serving_nf_type_list) {
        cJSON *serving_nf_type_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_type_list)) {
            ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [serving_nf_type_list]");
            goto end;
        }

        serving_nf_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_type_list_local, serving_nf_type_list) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(serving_nf_type_list_local)) {
                ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [serving_nf_type_list]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(serving_nf_type_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_nf_type_list\" is not supported. Ignoring it ...",
                         serving_nf_type_list_local->valuestring);
            } else {
                OpenAPI_list_add(serving_nf_type_listList, (void *)localEnum);
            }
        }
        if (serving_nf_type_listList->count == 0) {
            ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed: Expected serving_nf_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serving_nf_set_id_list = cJSON_GetObjectItemCaseSensitive(dccf_condJSON, "servingNfSetIdList");
    if (serving_nf_set_id_list) {
        cJSON *serving_nf_set_id_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_set_id_list)) {
            ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [serving_nf_set_id_list]");
            goto end;
        }

        serving_nf_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_set_id_list_local, serving_nf_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(serving_nf_set_id_list_local)) {
                ogs_error("OpenAPI_dccf_cond_parseFromJSON() failed [serving_nf_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(serving_nf_set_id_listList, ogs_strdup(serving_nf_set_id_list_local->valuestring));
        }
    }

    dccf_cond_local_var = OpenAPI_dccf_cond_create (
        condition_typeVariable,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        serving_nf_type_list ? serving_nf_type_listList : NULL,
        serving_nf_set_id_list ? serving_nf_set_id_listList : NULL
    );

    return dccf_cond_local_var;
end:
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    if (serving_nf_type_listList) {
        OpenAPI_list_free(serving_nf_type_listList);
        serving_nf_type_listList = NULL;
    }
    if (serving_nf_set_id_listList) {
        OpenAPI_list_for_each(serving_nf_set_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(serving_nf_set_id_listList);
        serving_nf_set_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_dccf_cond_t *OpenAPI_dccf_cond_copy(OpenAPI_dccf_cond_t *dst, OpenAPI_dccf_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dccf_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dccf_cond_convertToJSON() failed");
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

    OpenAPI_dccf_cond_free(dst);
    dst = OpenAPI_dccf_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


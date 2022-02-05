
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_cond.h"

char *OpenAPI_condition_typenwdaf_cond_ToString(OpenAPI_nwdaf_cond_condition_type_e condition_type)
{
    const char *condition_typeArray[] =  { "NULL", "NWDAF_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    if (condition_type < sizeofArray)
        return (char *)condition_typeArray[condition_type];
    else
        return (char *)"Unknown";
}

OpenAPI_nwdaf_cond_condition_type_e OpenAPI_condition_typenwdaf_cond_FromString(char* condition_type)
{
    int stringToReturn = 0;
    const char *condition_typeArray[] =  { "NULL", "NWDAF_COND" };
    size_t sizeofArray = sizeof(condition_typeArray) / sizeof(condition_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(condition_type, condition_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_create(
    OpenAPI_nwdaf_cond_condition_type_e condition_type,
    OpenAPI_list_t *analytics_ids,
    OpenAPI_list_t *snssai_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
)
{
    OpenAPI_nwdaf_cond_t *nwdaf_cond_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_cond_t));
    ogs_assert(nwdaf_cond_local_var);

    nwdaf_cond_local_var->condition_type = condition_type;
    nwdaf_cond_local_var->analytics_ids = analytics_ids;
    nwdaf_cond_local_var->snssai_list = snssai_list;
    nwdaf_cond_local_var->tai_list = tai_list;
    nwdaf_cond_local_var->tai_range_list = tai_range_list;

    return nwdaf_cond_local_var;
}

void OpenAPI_nwdaf_cond_free(OpenAPI_nwdaf_cond_t *nwdaf_cond)
{
    if (NULL == nwdaf_cond) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(nwdaf_cond->analytics_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(nwdaf_cond->analytics_ids);
    OpenAPI_list_for_each(nwdaf_cond->snssai_list, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(nwdaf_cond->snssai_list);
    OpenAPI_list_for_each(nwdaf_cond->tai_list, node) {
        OpenAPI_tai_free(node->data);
    }
    OpenAPI_list_free(nwdaf_cond->tai_list);
    OpenAPI_list_for_each(nwdaf_cond->tai_range_list, node) {
        OpenAPI_tai_range_free(node->data);
    }
    OpenAPI_list_free(nwdaf_cond->tai_range_list);
    ogs_free(nwdaf_cond);
}

cJSON *OpenAPI_nwdaf_cond_convertToJSON(OpenAPI_nwdaf_cond_t *nwdaf_cond)
{
    cJSON *item = NULL;

    if (nwdaf_cond == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [NwdafCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "conditionType", OpenAPI_condition_typenwdaf_cond_ToString(nwdaf_cond->condition_type)) == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [condition_type]");
        goto end;
    }

    if (nwdaf_cond->analytics_ids) {
    cJSON *analytics_ids = cJSON_AddArrayToObject(item, "analyticsIds");
    if (analytics_ids == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [analytics_ids]");
        goto end;
    }

    OpenAPI_lnode_t *analytics_ids_node;
    OpenAPI_list_for_each(nwdaf_cond->analytics_ids, analytics_ids_node)  {
    if (cJSON_AddStringToObject(analytics_ids, "", (char*)analytics_ids_node->data) == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [analytics_ids]");
        goto end;
    }
                    }
    }

    if (nwdaf_cond->snssai_list) {
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [snssai_list]");
        goto end;
    }

    OpenAPI_lnode_t *snssai_list_node;
    if (nwdaf_cond->snssai_list) {
        OpenAPI_list_for_each(nwdaf_cond->snssai_list, snssai_list_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(snssai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [snssai_list]");
                goto end;
            }
            cJSON_AddItemToArray(snssai_listList, itemLocal);
        }
    }
    }

    if (nwdaf_cond->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [tai_list]");
        goto end;
    }

    OpenAPI_lnode_t *tai_list_node;
    if (nwdaf_cond->tai_list) {
        OpenAPI_list_for_each(nwdaf_cond->tai_list, tai_list_node) {
            cJSON *itemLocal = OpenAPI_tai_convertToJSON(tai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [tai_list]");
                goto end;
            }
            cJSON_AddItemToArray(tai_listList, itemLocal);
        }
    }
    }

    if (nwdaf_cond->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [tai_range_list]");
        goto end;
    }

    OpenAPI_lnode_t *tai_range_list_node;
    if (nwdaf_cond->tai_range_list) {
        OpenAPI_list_for_each(nwdaf_cond->tai_range_list, tai_range_list_node) {
            cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(tai_range_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [tai_range_list]");
                goto end;
            }
            cJSON_AddItemToArray(tai_range_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_parseFromJSON(cJSON *nwdaf_condJSON)
{
    OpenAPI_nwdaf_cond_t *nwdaf_cond_local_var = NULL;
    cJSON *condition_type = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "conditionType");
    if (!condition_type) {
        ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }

    OpenAPI_nwdaf_cond_condition_type_e condition_typeVariable;
    if (!cJSON_IsString(condition_type)) {
        ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }
    condition_typeVariable = OpenAPI_condition_typenwdaf_cond_FromString(condition_type->valuestring);

    cJSON *analytics_ids = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "analyticsIds");

    OpenAPI_list_t *analytics_idsList;
    if (analytics_ids) {
    cJSON *analytics_ids_local;
    if (!cJSON_IsArray(analytics_ids)) {
        ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [analytics_ids]");
        goto end;
    }
    analytics_idsList = OpenAPI_list_create();

    cJSON_ArrayForEach(analytics_ids_local, analytics_ids) {
    if (!cJSON_IsString(analytics_ids_local)) {
        ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [analytics_ids]");
        goto end;
    }
    OpenAPI_list_add(analytics_idsList , ogs_strdup(analytics_ids_local->valuestring));
    }
    }

    cJSON *snssai_list = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "snssaiList");

    OpenAPI_list_t *snssai_listList;
    if (snssai_list) {
    cJSON *snssai_list_local_nonprimitive;
    if (!cJSON_IsArray(snssai_list)){
        ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [snssai_list]");
        goto end;
    }

    snssai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(snssai_list_local_nonprimitive, snssai_list ) {
        if (!cJSON_IsObject(snssai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [snssai_list]");
            goto end;
        }
        OpenAPI_snssai_t *snssai_listItem = OpenAPI_snssai_parseFromJSON(snssai_list_local_nonprimitive);

        if (!snssai_listItem) {
            ogs_error("No snssai_listItem");
            OpenAPI_list_free(snssai_listList);
            goto end;
        }

        OpenAPI_list_add(snssai_listList, snssai_listItem);
    }
    }

    cJSON *tai_list = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "taiList");

    OpenAPI_list_t *tai_listList;
    if (tai_list) {
    cJSON *tai_list_local_nonprimitive;
    if (!cJSON_IsArray(tai_list)){
        ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [tai_list]");
        goto end;
    }

    tai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(tai_list_local_nonprimitive, tai_list ) {
        if (!cJSON_IsObject(tai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [tai_list]");
            goto end;
        }
        OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local_nonprimitive);

        if (!tai_listItem) {
            ogs_error("No tai_listItem");
            OpenAPI_list_free(tai_listList);
            goto end;
        }

        OpenAPI_list_add(tai_listList, tai_listItem);
    }
    }

    cJSON *tai_range_list = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "taiRangeList");

    OpenAPI_list_t *tai_range_listList;
    if (tai_range_list) {
    cJSON *tai_range_list_local_nonprimitive;
    if (!cJSON_IsArray(tai_range_list)){
        ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [tai_range_list]");
        goto end;
    }

    tai_range_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(tai_range_list_local_nonprimitive, tai_range_list ) {
        if (!cJSON_IsObject(tai_range_list_local_nonprimitive)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [tai_range_list]");
            goto end;
        }
        OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local_nonprimitive);

        if (!tai_range_listItem) {
            ogs_error("No tai_range_listItem");
            OpenAPI_list_free(tai_range_listList);
            goto end;
        }

        OpenAPI_list_add(tai_range_listList, tai_range_listItem);
    }
    }

    nwdaf_cond_local_var = OpenAPI_nwdaf_cond_create (
        condition_typeVariable,
        analytics_ids ? analytics_idsList : NULL,
        snssai_list ? snssai_listList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL
    );

    return nwdaf_cond_local_var;
end:
    return NULL;
}

OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_copy(OpenAPI_nwdaf_cond_t *dst, OpenAPI_nwdaf_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nwdaf_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed");
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

    OpenAPI_nwdaf_cond_free(dst);
    dst = OpenAPI_nwdaf_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


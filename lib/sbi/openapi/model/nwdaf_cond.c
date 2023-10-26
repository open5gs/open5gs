
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
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *ml_analytics_list
)
{
    OpenAPI_nwdaf_cond_t *nwdaf_cond_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_cond_t));
    ogs_assert(nwdaf_cond_local_var);

    nwdaf_cond_local_var->condition_type = condition_type;
    nwdaf_cond_local_var->analytics_ids = analytics_ids;
    nwdaf_cond_local_var->snssai_list = snssai_list;
    nwdaf_cond_local_var->tai_list = tai_list;
    nwdaf_cond_local_var->tai_range_list = tai_range_list;
    nwdaf_cond_local_var->serving_nf_type_list = serving_nf_type_list;
    nwdaf_cond_local_var->serving_nf_set_id_list = serving_nf_set_id_list;
    nwdaf_cond_local_var->ml_analytics_list = ml_analytics_list;

    return nwdaf_cond_local_var;
}

void OpenAPI_nwdaf_cond_free(OpenAPI_nwdaf_cond_t *nwdaf_cond)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nwdaf_cond) {
        return;
    }
    if (nwdaf_cond->analytics_ids) {
        OpenAPI_list_for_each(nwdaf_cond->analytics_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nwdaf_cond->analytics_ids);
        nwdaf_cond->analytics_ids = NULL;
    }
    if (nwdaf_cond->snssai_list) {
        OpenAPI_list_for_each(nwdaf_cond->snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(nwdaf_cond->snssai_list);
        nwdaf_cond->snssai_list = NULL;
    }
    if (nwdaf_cond->tai_list) {
        OpenAPI_list_for_each(nwdaf_cond->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(nwdaf_cond->tai_list);
        nwdaf_cond->tai_list = NULL;
    }
    if (nwdaf_cond->tai_range_list) {
        OpenAPI_list_for_each(nwdaf_cond->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(nwdaf_cond->tai_range_list);
        nwdaf_cond->tai_range_list = NULL;
    }
    if (nwdaf_cond->serving_nf_type_list) {
        OpenAPI_list_free(nwdaf_cond->serving_nf_type_list);
        nwdaf_cond->serving_nf_type_list = NULL;
    }
    if (nwdaf_cond->serving_nf_set_id_list) {
        OpenAPI_list_for_each(nwdaf_cond->serving_nf_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nwdaf_cond->serving_nf_set_id_list);
        nwdaf_cond->serving_nf_set_id_list = NULL;
    }
    if (nwdaf_cond->ml_analytics_list) {
        OpenAPI_list_for_each(nwdaf_cond->ml_analytics_list, node) {
            OpenAPI_ml_analytics_info_free(node->data);
        }
        OpenAPI_list_free(nwdaf_cond->ml_analytics_list);
        nwdaf_cond->ml_analytics_list = NULL;
    }
    ogs_free(nwdaf_cond);
}

cJSON *OpenAPI_nwdaf_cond_convertToJSON(OpenAPI_nwdaf_cond_t *nwdaf_cond)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nwdaf_cond == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [NwdafCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nwdaf_cond->condition_type == OpenAPI_nwdaf_cond_CONDITIONTYPE_NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [condition_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "conditionType", OpenAPI_condition_typenwdaf_cond_ToString(nwdaf_cond->condition_type)) == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [condition_type]");
        goto end;
    }

    if (nwdaf_cond->analytics_ids) {
    cJSON *analytics_idsList = cJSON_AddArrayToObject(item, "analyticsIds");
    if (analytics_idsList == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [analytics_ids]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_cond->analytics_ids, node) {
        if (cJSON_AddStringToObject(analytics_idsList, "", (char*)node->data) == NULL) {
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
    OpenAPI_list_for_each(nwdaf_cond->snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(snssai_listList, itemLocal);
    }
    }

    if (nwdaf_cond->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_cond->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (nwdaf_cond->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_cond->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (nwdaf_cond->serving_nf_type_list != OpenAPI_nf_type_NULL) {
    cJSON *serving_nf_type_listList = cJSON_AddArrayToObject(item, "servingNfTypeList");
    if (serving_nf_type_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [serving_nf_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_cond->serving_nf_type_list, node) {
        if (cJSON_AddStringToObject(serving_nf_type_listList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [serving_nf_type_list]");
            goto end;
        }
    }
    }

    if (nwdaf_cond->serving_nf_set_id_list) {
    cJSON *serving_nf_set_id_listList = cJSON_AddArrayToObject(item, "servingNfSetIdList");
    if (serving_nf_set_id_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [serving_nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_cond->serving_nf_set_id_list, node) {
        if (cJSON_AddStringToObject(serving_nf_set_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [serving_nf_set_id_list]");
            goto end;
        }
    }
    }

    if (nwdaf_cond->ml_analytics_list) {
    cJSON *ml_analytics_listList = cJSON_AddArrayToObject(item, "mlAnalyticsList");
    if (ml_analytics_listList == NULL) {
        ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [ml_analytics_list]");
        goto end;
    }
    OpenAPI_list_for_each(nwdaf_cond->ml_analytics_list, node) {
        cJSON *itemLocal = OpenAPI_ml_analytics_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nwdaf_cond_convertToJSON() failed [ml_analytics_list]");
            goto end;
        }
        cJSON_AddItemToArray(ml_analytics_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_nwdaf_cond_t *OpenAPI_nwdaf_cond_parseFromJSON(cJSON *nwdaf_condJSON)
{
    OpenAPI_nwdaf_cond_t *nwdaf_cond_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *condition_type = NULL;
    OpenAPI_nwdaf_cond_condition_type_e condition_typeVariable = 0;
    cJSON *analytics_ids = NULL;
    OpenAPI_list_t *analytics_idsList = NULL;
    cJSON *snssai_list = NULL;
    OpenAPI_list_t *snssai_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *serving_nf_type_list = NULL;
    OpenAPI_list_t *serving_nf_type_listList = NULL;
    cJSON *serving_nf_set_id_list = NULL;
    OpenAPI_list_t *serving_nf_set_id_listList = NULL;
    cJSON *ml_analytics_list = NULL;
    OpenAPI_list_t *ml_analytics_listList = NULL;
    condition_type = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "conditionType");
    if (!condition_type) {
        ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }
    if (!cJSON_IsString(condition_type)) {
        ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [condition_type]");
        goto end;
    }
    condition_typeVariable = OpenAPI_condition_typenwdaf_cond_FromString(condition_type->valuestring);

    analytics_ids = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "analyticsIds");
    if (analytics_ids) {
        cJSON *analytics_ids_local = NULL;
        if (!cJSON_IsArray(analytics_ids)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [analytics_ids]");
            goto end;
        }

        analytics_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(analytics_ids_local, analytics_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(analytics_ids_local)) {
                ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [analytics_ids]");
                goto end;
            }
            OpenAPI_list_add(analytics_idsList, ogs_strdup(analytics_ids_local->valuestring));
        }
    }

    snssai_list = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "snssaiList");
    if (snssai_list) {
        cJSON *snssai_list_local = NULL;
        if (!cJSON_IsArray(snssai_list)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [snssai_list]");
            goto end;
        }

        snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssai_list_local, snssai_list) {
            if (!cJSON_IsObject(snssai_list_local)) {
                ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *snssai_listItem = OpenAPI_snssai_parseFromJSON(snssai_list_local);
            if (!snssai_listItem) {
                ogs_error("No snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(snssai_listList, snssai_listItem);
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [tai_range_list]");
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

    serving_nf_type_list = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "servingNfTypeList");
    if (serving_nf_type_list) {
        cJSON *serving_nf_type_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_type_list)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [serving_nf_type_list]");
            goto end;
        }

        serving_nf_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_type_list_local, serving_nf_type_list) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(serving_nf_type_list_local)) {
                ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [serving_nf_type_list]");
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
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed: Expected serving_nf_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serving_nf_set_id_list = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "servingNfSetIdList");
    if (serving_nf_set_id_list) {
        cJSON *serving_nf_set_id_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_set_id_list)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [serving_nf_set_id_list]");
            goto end;
        }

        serving_nf_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_set_id_list_local, serving_nf_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(serving_nf_set_id_list_local)) {
                ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [serving_nf_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(serving_nf_set_id_listList, ogs_strdup(serving_nf_set_id_list_local->valuestring));
        }
    }

    ml_analytics_list = cJSON_GetObjectItemCaseSensitive(nwdaf_condJSON, "mlAnalyticsList");
    if (ml_analytics_list) {
        cJSON *ml_analytics_list_local = NULL;
        if (!cJSON_IsArray(ml_analytics_list)) {
            ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [ml_analytics_list]");
            goto end;
        }

        ml_analytics_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ml_analytics_list_local, ml_analytics_list) {
            if (!cJSON_IsObject(ml_analytics_list_local)) {
                ogs_error("OpenAPI_nwdaf_cond_parseFromJSON() failed [ml_analytics_list]");
                goto end;
            }
            OpenAPI_ml_analytics_info_t *ml_analytics_listItem = OpenAPI_ml_analytics_info_parseFromJSON(ml_analytics_list_local);
            if (!ml_analytics_listItem) {
                ogs_error("No ml_analytics_listItem");
                goto end;
            }
            OpenAPI_list_add(ml_analytics_listList, ml_analytics_listItem);
        }
    }

    nwdaf_cond_local_var = OpenAPI_nwdaf_cond_create (
        condition_typeVariable,
        analytics_ids ? analytics_idsList : NULL,
        snssai_list ? snssai_listList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        serving_nf_type_list ? serving_nf_type_listList : NULL,
        serving_nf_set_id_list ? serving_nf_set_id_listList : NULL,
        ml_analytics_list ? ml_analytics_listList : NULL
    );

    return nwdaf_cond_local_var;
end:
    if (analytics_idsList) {
        OpenAPI_list_for_each(analytics_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(analytics_idsList);
        analytics_idsList = NULL;
    }
    if (snssai_listList) {
        OpenAPI_list_for_each(snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_listList);
        snssai_listList = NULL;
    }
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
    if (ml_analytics_listList) {
        OpenAPI_list_for_each(ml_analytics_listList, node) {
            OpenAPI_ml_analytics_info_free(node->data);
        }
        OpenAPI_list_free(ml_analytics_listList);
        ml_analytics_listList = NULL;
    }
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


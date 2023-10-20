
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mfaf_info.h"

OpenAPI_mfaf_info_t *OpenAPI_mfaf_info_create(
    OpenAPI_list_t *serving_nf_type_list,
    OpenAPI_list_t *serving_nf_set_id_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
)
{
    OpenAPI_mfaf_info_t *mfaf_info_local_var = ogs_malloc(sizeof(OpenAPI_mfaf_info_t));
    ogs_assert(mfaf_info_local_var);

    mfaf_info_local_var->serving_nf_type_list = serving_nf_type_list;
    mfaf_info_local_var->serving_nf_set_id_list = serving_nf_set_id_list;
    mfaf_info_local_var->tai_list = tai_list;
    mfaf_info_local_var->tai_range_list = tai_range_list;

    return mfaf_info_local_var;
}

void OpenAPI_mfaf_info_free(OpenAPI_mfaf_info_t *mfaf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mfaf_info) {
        return;
    }
    if (mfaf_info->serving_nf_type_list) {
        OpenAPI_list_free(mfaf_info->serving_nf_type_list);
        mfaf_info->serving_nf_type_list = NULL;
    }
    if (mfaf_info->serving_nf_set_id_list) {
        OpenAPI_list_for_each(mfaf_info->serving_nf_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mfaf_info->serving_nf_set_id_list);
        mfaf_info->serving_nf_set_id_list = NULL;
    }
    if (mfaf_info->tai_list) {
        OpenAPI_list_for_each(mfaf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(mfaf_info->tai_list);
        mfaf_info->tai_list = NULL;
    }
    if (mfaf_info->tai_range_list) {
        OpenAPI_list_for_each(mfaf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(mfaf_info->tai_range_list);
        mfaf_info->tai_range_list = NULL;
    }
    ogs_free(mfaf_info);
}

cJSON *OpenAPI_mfaf_info_convertToJSON(OpenAPI_mfaf_info_t *mfaf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mfaf_info == NULL) {
        ogs_error("OpenAPI_mfaf_info_convertToJSON() failed [MfafInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mfaf_info->serving_nf_type_list != OpenAPI_nf_type_NULL) {
    cJSON *serving_nf_type_listList = cJSON_AddArrayToObject(item, "servingNfTypeList");
    if (serving_nf_type_listList == NULL) {
        ogs_error("OpenAPI_mfaf_info_convertToJSON() failed [serving_nf_type_list]");
        goto end;
    }
    OpenAPI_list_for_each(mfaf_info->serving_nf_type_list, node) {
        if (cJSON_AddStringToObject(serving_nf_type_listList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_mfaf_info_convertToJSON() failed [serving_nf_type_list]");
            goto end;
        }
    }
    }

    if (mfaf_info->serving_nf_set_id_list) {
    cJSON *serving_nf_set_id_listList = cJSON_AddArrayToObject(item, "servingNfSetIdList");
    if (serving_nf_set_id_listList == NULL) {
        ogs_error("OpenAPI_mfaf_info_convertToJSON() failed [serving_nf_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(mfaf_info->serving_nf_set_id_list, node) {
        if (cJSON_AddStringToObject(serving_nf_set_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_mfaf_info_convertToJSON() failed [serving_nf_set_id_list]");
            goto end;
        }
    }
    }

    if (mfaf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_mfaf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(mfaf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mfaf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (mfaf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_mfaf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(mfaf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mfaf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mfaf_info_t *OpenAPI_mfaf_info_parseFromJSON(cJSON *mfaf_infoJSON)
{
    OpenAPI_mfaf_info_t *mfaf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serving_nf_type_list = NULL;
    OpenAPI_list_t *serving_nf_type_listList = NULL;
    cJSON *serving_nf_set_id_list = NULL;
    OpenAPI_list_t *serving_nf_set_id_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    serving_nf_type_list = cJSON_GetObjectItemCaseSensitive(mfaf_infoJSON, "servingNfTypeList");
    if (serving_nf_type_list) {
        cJSON *serving_nf_type_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_type_list)) {
            ogs_error("OpenAPI_mfaf_info_parseFromJSON() failed [serving_nf_type_list]");
            goto end;
        }

        serving_nf_type_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_type_list_local, serving_nf_type_list) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(serving_nf_type_list_local)) {
                ogs_error("OpenAPI_mfaf_info_parseFromJSON() failed [serving_nf_type_list]");
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
            ogs_error("OpenAPI_mfaf_info_parseFromJSON() failed: Expected serving_nf_type_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serving_nf_set_id_list = cJSON_GetObjectItemCaseSensitive(mfaf_infoJSON, "servingNfSetIdList");
    if (serving_nf_set_id_list) {
        cJSON *serving_nf_set_id_list_local = NULL;
        if (!cJSON_IsArray(serving_nf_set_id_list)) {
            ogs_error("OpenAPI_mfaf_info_parseFromJSON() failed [serving_nf_set_id_list]");
            goto end;
        }

        serving_nf_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_nf_set_id_list_local, serving_nf_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(serving_nf_set_id_list_local)) {
                ogs_error("OpenAPI_mfaf_info_parseFromJSON() failed [serving_nf_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(serving_nf_set_id_listList, ogs_strdup(serving_nf_set_id_list_local->valuestring));
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(mfaf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_mfaf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_mfaf_info_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(mfaf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_mfaf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_mfaf_info_parseFromJSON() failed [tai_range_list]");
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

    mfaf_info_local_var = OpenAPI_mfaf_info_create (
        serving_nf_type_list ? serving_nf_type_listList : NULL,
        serving_nf_set_id_list ? serving_nf_set_id_listList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL
    );

    return mfaf_info_local_var;
end:
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
    return NULL;
}

OpenAPI_mfaf_info_t *OpenAPI_mfaf_info_copy(OpenAPI_mfaf_info_t *dst, OpenAPI_mfaf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mfaf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mfaf_info_convertToJSON() failed");
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

    OpenAPI_mfaf_info_free(dst);
    dst = OpenAPI_mfaf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


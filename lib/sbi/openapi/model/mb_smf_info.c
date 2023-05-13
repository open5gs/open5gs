
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mb_smf_info.h"

OpenAPI_mb_smf_info_t *OpenAPI_mb_smf_info_create(
    OpenAPI_list_t* s_nssai_info_list,
    OpenAPI_list_t* tmgi_range_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t* mbs_session_list
)
{
    OpenAPI_mb_smf_info_t *mb_smf_info_local_var = ogs_malloc(sizeof(OpenAPI_mb_smf_info_t));
    ogs_assert(mb_smf_info_local_var);

    mb_smf_info_local_var->s_nssai_info_list = s_nssai_info_list;
    mb_smf_info_local_var->tmgi_range_list = tmgi_range_list;
    mb_smf_info_local_var->tai_list = tai_list;
    mb_smf_info_local_var->tai_range_list = tai_range_list;
    mb_smf_info_local_var->mbs_session_list = mbs_session_list;

    return mb_smf_info_local_var;
}

void OpenAPI_mb_smf_info_free(OpenAPI_mb_smf_info_t *mb_smf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mb_smf_info) {
        return;
    }
    if (mb_smf_info->s_nssai_info_list) {
        OpenAPI_list_for_each(mb_smf_info->s_nssai_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_mb_smf_info_item_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(mb_smf_info->s_nssai_info_list);
        mb_smf_info->s_nssai_info_list = NULL;
    }
    if (mb_smf_info->tmgi_range_list) {
        OpenAPI_list_for_each(mb_smf_info->tmgi_range_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_tmgi_range_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(mb_smf_info->tmgi_range_list);
        mb_smf_info->tmgi_range_list = NULL;
    }
    if (mb_smf_info->tai_list) {
        OpenAPI_list_for_each(mb_smf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(mb_smf_info->tai_list);
        mb_smf_info->tai_list = NULL;
    }
    if (mb_smf_info->tai_range_list) {
        OpenAPI_list_for_each(mb_smf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(mb_smf_info->tai_range_list);
        mb_smf_info->tai_range_list = NULL;
    }
    if (mb_smf_info->mbs_session_list) {
        OpenAPI_list_for_each(mb_smf_info->mbs_session_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_mbs_session_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(mb_smf_info->mbs_session_list);
        mb_smf_info->mbs_session_list = NULL;
    }
    ogs_free(mb_smf_info);
}

cJSON *OpenAPI_mb_smf_info_convertToJSON(OpenAPI_mb_smf_info_t *mb_smf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mb_smf_info == NULL) {
        ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [MbSmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mb_smf_info->s_nssai_info_list) {
    cJSON *s_nssai_info_list = cJSON_AddObjectToObject(item, "sNssaiInfoList");
    if (s_nssai_info_list == NULL) {
        ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [s_nssai_info_list]");
        goto end;
    }
    cJSON *localMapObject = s_nssai_info_list;
    if (mb_smf_info->s_nssai_info_list) {
        OpenAPI_list_for_each(mb_smf_info->s_nssai_info_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [s_nssai_info_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [s_nssai_info_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_snssai_mb_smf_info_item_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (mb_smf_info->tmgi_range_list) {
    cJSON *tmgi_range_list = cJSON_AddObjectToObject(item, "tmgiRangeList");
    if (tmgi_range_list == NULL) {
        ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [tmgi_range_list]");
        goto end;
    }
    cJSON *localMapObject = tmgi_range_list;
    if (mb_smf_info->tmgi_range_list) {
        OpenAPI_list_for_each(mb_smf_info->tmgi_range_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [tmgi_range_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [tmgi_range_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_tmgi_range_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (mb_smf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(mb_smf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (mb_smf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(mb_smf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (mb_smf_info->mbs_session_list) {
    cJSON *mbs_session_list = cJSON_AddObjectToObject(item, "mbsSessionList");
    if (mbs_session_list == NULL) {
        ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [mbs_session_list]");
        goto end;
    }
    cJSON *localMapObject = mbs_session_list;
    if (mb_smf_info->mbs_session_list) {
        OpenAPI_list_for_each(mb_smf_info->mbs_session_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [mbs_session_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [mbs_session_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_mbs_session_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_mb_smf_info_t *OpenAPI_mb_smf_info_parseFromJSON(cJSON *mb_smf_infoJSON)
{
    OpenAPI_mb_smf_info_t *mb_smf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai_info_list = NULL;
    OpenAPI_list_t *s_nssai_info_listList = NULL;
    cJSON *tmgi_range_list = NULL;
    OpenAPI_list_t *tmgi_range_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *mbs_session_list = NULL;
    OpenAPI_list_t *mbs_session_listList = NULL;
    s_nssai_info_list = cJSON_GetObjectItemCaseSensitive(mb_smf_infoJSON, "sNssaiInfoList");
    if (s_nssai_info_list) {
        cJSON *s_nssai_info_list_local_map = NULL;
        if (!cJSON_IsObject(s_nssai_info_list) && !cJSON_IsNull(s_nssai_info_list)) {
            ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [s_nssai_info_list]");
            goto end;
        }
        if (cJSON_IsObject(s_nssai_info_list)) {
            s_nssai_info_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(s_nssai_info_list_local_map, s_nssai_info_list) {
                cJSON *localMapObject = s_nssai_info_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_snssai_mb_smf_info_item_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(s_nssai_info_listList, localMapKeyPair);
            }
        }
    }

    tmgi_range_list = cJSON_GetObjectItemCaseSensitive(mb_smf_infoJSON, "tmgiRangeList");
    if (tmgi_range_list) {
        cJSON *tmgi_range_list_local_map = NULL;
        if (!cJSON_IsObject(tmgi_range_list) && !cJSON_IsNull(tmgi_range_list)) {
            ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [tmgi_range_list]");
            goto end;
        }
        if (cJSON_IsObject(tmgi_range_list)) {
            tmgi_range_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(tmgi_range_list_local_map, tmgi_range_list) {
                cJSON *localMapObject = tmgi_range_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_tmgi_range_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(tmgi_range_listList, localMapKeyPair);
            }
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(mb_smf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [tai_list]");
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

    tai_range_list = cJSON_GetObjectItemCaseSensitive(mb_smf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [tai_range_list]");
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

    mbs_session_list = cJSON_GetObjectItemCaseSensitive(mb_smf_infoJSON, "mbsSessionList");
    if (mbs_session_list) {
        cJSON *mbs_session_list_local_map = NULL;
        if (!cJSON_IsObject(mbs_session_list) && !cJSON_IsNull(mbs_session_list)) {
            ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [mbs_session_list]");
            goto end;
        }
        if (cJSON_IsObject(mbs_session_list)) {
            mbs_session_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(mbs_session_list_local_map, mbs_session_list) {
                cJSON *localMapObject = mbs_session_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_mbs_session_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_mb_smf_info_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(mbs_session_listList, localMapKeyPair);
            }
        }
    }

    mb_smf_info_local_var = OpenAPI_mb_smf_info_create (
        s_nssai_info_list ? s_nssai_info_listList : NULL,
        tmgi_range_list ? tmgi_range_listList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        mbs_session_list ? mbs_session_listList : NULL
    );

    return mb_smf_info_local_var;
end:
    if (s_nssai_info_listList) {
        OpenAPI_list_for_each(s_nssai_info_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_snssai_mb_smf_info_item_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(s_nssai_info_listList);
        s_nssai_info_listList = NULL;
    }
    if (tmgi_range_listList) {
        OpenAPI_list_for_each(tmgi_range_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_tmgi_range_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(tmgi_range_listList);
        tmgi_range_listList = NULL;
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
    if (mbs_session_listList) {
        OpenAPI_list_for_each(mbs_session_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_mbs_session_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(mbs_session_listList);
        mbs_session_listList = NULL;
    }
    return NULL;
}

OpenAPI_mb_smf_info_t *OpenAPI_mb_smf_info_copy(OpenAPI_mb_smf_info_t *dst, OpenAPI_mb_smf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mb_smf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mb_smf_info_convertToJSON() failed");
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

    OpenAPI_mb_smf_info_free(dst);
    dst = OpenAPI_mb_smf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


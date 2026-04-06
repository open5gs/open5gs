
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mf_info.h"

OpenAPI_mf_info_t *OpenAPI_mf_info_create(
    OpenAPI_list_t *media_capability_list
)
{
    OpenAPI_mf_info_t *mf_info_local_var = ogs_malloc(sizeof(OpenAPI_mf_info_t));
    ogs_assert(mf_info_local_var);

    mf_info_local_var->media_capability_list = media_capability_list;

    return mf_info_local_var;
}

void OpenAPI_mf_info_free(OpenAPI_mf_info_t *mf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mf_info) {
        return;
    }
    if (mf_info->media_capability_list) {
        OpenAPI_list_for_each(mf_info->media_capability_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mf_info->media_capability_list);
        mf_info->media_capability_list = NULL;
    }
    ogs_free(mf_info);
}

cJSON *OpenAPI_mf_info_convertToJSON(OpenAPI_mf_info_t *mf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mf_info == NULL) {
        ogs_error("OpenAPI_mf_info_convertToJSON() failed [MfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mf_info->media_capability_list) {
    cJSON *media_capability_listList = cJSON_AddArrayToObject(item, "mediaCapabilityList");
    if (media_capability_listList == NULL) {
        ogs_error("OpenAPI_mf_info_convertToJSON() failed [media_capability_list]");
        goto end;
    }
    OpenAPI_list_for_each(mf_info->media_capability_list, node) {
        if (cJSON_AddStringToObject(media_capability_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_mf_info_convertToJSON() failed [media_capability_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_mf_info_t *OpenAPI_mf_info_parseFromJSON(cJSON *mf_infoJSON)
{
    OpenAPI_mf_info_t *mf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *media_capability_list = NULL;
    OpenAPI_list_t *media_capability_listList = NULL;
    media_capability_list = cJSON_GetObjectItemCaseSensitive(mf_infoJSON, "mediaCapabilityList");
    if (media_capability_list) {
        cJSON *media_capability_list_local = NULL;
        if (!cJSON_IsArray(media_capability_list)) {
            ogs_error("OpenAPI_mf_info_parseFromJSON() failed [media_capability_list]");
            goto end;
        }

        media_capability_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(media_capability_list_local, media_capability_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(media_capability_list_local)) {
                ogs_error("OpenAPI_mf_info_parseFromJSON() failed [media_capability_list]");
                goto end;
            }
            OpenAPI_list_add(media_capability_listList, ogs_strdup(media_capability_list_local->valuestring));
        }
    }

    mf_info_local_var = OpenAPI_mf_info_create (
        media_capability_list ? media_capability_listList : NULL
    );

    return mf_info_local_var;
end:
    if (media_capability_listList) {
        OpenAPI_list_for_each(media_capability_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(media_capability_listList);
        media_capability_listList = NULL;
    }
    return NULL;
}

OpenAPI_mf_info_t *OpenAPI_mf_info_copy(OpenAPI_mf_info_t *dst, OpenAPI_mf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mf_info_convertToJSON() failed");
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

    OpenAPI_mf_info_free(dst);
    dst = OpenAPI_mf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


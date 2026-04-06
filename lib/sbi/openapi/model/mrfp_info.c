
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mrfp_info.h"

OpenAPI_mrfp_info_t *OpenAPI_mrfp_info_create(
    OpenAPI_list_t *media_capability_list
)
{
    OpenAPI_mrfp_info_t *mrfp_info_local_var = ogs_malloc(sizeof(OpenAPI_mrfp_info_t));
    ogs_assert(mrfp_info_local_var);

    mrfp_info_local_var->media_capability_list = media_capability_list;

    return mrfp_info_local_var;
}

void OpenAPI_mrfp_info_free(OpenAPI_mrfp_info_t *mrfp_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mrfp_info) {
        return;
    }
    if (mrfp_info->media_capability_list) {
        OpenAPI_list_for_each(mrfp_info->media_capability_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mrfp_info->media_capability_list);
        mrfp_info->media_capability_list = NULL;
    }
    ogs_free(mrfp_info);
}

cJSON *OpenAPI_mrfp_info_convertToJSON(OpenAPI_mrfp_info_t *mrfp_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mrfp_info == NULL) {
        ogs_error("OpenAPI_mrfp_info_convertToJSON() failed [MrfpInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mrfp_info->media_capability_list) {
    cJSON *media_capability_listList = cJSON_AddArrayToObject(item, "mediaCapabilityList");
    if (media_capability_listList == NULL) {
        ogs_error("OpenAPI_mrfp_info_convertToJSON() failed [media_capability_list]");
        goto end;
    }
    OpenAPI_list_for_each(mrfp_info->media_capability_list, node) {
        if (cJSON_AddStringToObject(media_capability_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_mrfp_info_convertToJSON() failed [media_capability_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_mrfp_info_t *OpenAPI_mrfp_info_parseFromJSON(cJSON *mrfp_infoJSON)
{
    OpenAPI_mrfp_info_t *mrfp_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *media_capability_list = NULL;
    OpenAPI_list_t *media_capability_listList = NULL;
    media_capability_list = cJSON_GetObjectItemCaseSensitive(mrfp_infoJSON, "mediaCapabilityList");
    if (media_capability_list) {
        cJSON *media_capability_list_local = NULL;
        if (!cJSON_IsArray(media_capability_list)) {
            ogs_error("OpenAPI_mrfp_info_parseFromJSON() failed [media_capability_list]");
            goto end;
        }

        media_capability_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(media_capability_list_local, media_capability_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(media_capability_list_local)) {
                ogs_error("OpenAPI_mrfp_info_parseFromJSON() failed [media_capability_list]");
                goto end;
            }
            OpenAPI_list_add(media_capability_listList, ogs_strdup(media_capability_list_local->valuestring));
        }
    }

    mrfp_info_local_var = OpenAPI_mrfp_info_create (
        media_capability_list ? media_capability_listList : NULL
    );

    return mrfp_info_local_var;
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

OpenAPI_mrfp_info_t *OpenAPI_mrfp_info_copy(OpenAPI_mrfp_info_t *dst, OpenAPI_mrfp_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mrfp_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mrfp_info_convertToJSON() failed");
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

    OpenAPI_mrfp_info_free(dst);
    dst = OpenAPI_mrfp_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


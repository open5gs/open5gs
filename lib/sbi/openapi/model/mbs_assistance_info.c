
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_assistance_info.h"

OpenAPI_mbs_assistance_info_t *OpenAPI_mbs_assistance_info_create(
    OpenAPI_mbs_session_id_t *mbs_session_id,
    OpenAPI_list_t *assistance_info
)
{
    OpenAPI_mbs_assistance_info_t *mbs_assistance_info_local_var = ogs_malloc(sizeof(OpenAPI_mbs_assistance_info_t));
    ogs_assert(mbs_assistance_info_local_var);

    mbs_assistance_info_local_var->mbs_session_id = mbs_session_id;
    mbs_assistance_info_local_var->assistance_info = assistance_info;

    return mbs_assistance_info_local_var;
}

void OpenAPI_mbs_assistance_info_free(OpenAPI_mbs_assistance_info_t *mbs_assistance_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_assistance_info) {
        return;
    }
    if (mbs_assistance_info->mbs_session_id) {
        OpenAPI_mbs_session_id_free(mbs_assistance_info->mbs_session_id);
        mbs_assistance_info->mbs_session_id = NULL;
    }
    if (mbs_assistance_info->assistance_info) {
        OpenAPI_list_for_each(mbs_assistance_info->assistance_info, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mbs_assistance_info->assistance_info);
        mbs_assistance_info->assistance_info = NULL;
    }
    ogs_free(mbs_assistance_info);
}

cJSON *OpenAPI_mbs_assistance_info_convertToJSON(OpenAPI_mbs_assistance_info_t *mbs_assistance_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_assistance_info == NULL) {
        ogs_error("OpenAPI_mbs_assistance_info_convertToJSON() failed [MbsAssistanceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!mbs_assistance_info->mbs_session_id) {
        ogs_error("OpenAPI_mbs_assistance_info_convertToJSON() failed [mbs_session_id]");
        return NULL;
    }
    cJSON *mbs_session_id_local_JSON = OpenAPI_mbs_session_id_convertToJSON(mbs_assistance_info->mbs_session_id);
    if (mbs_session_id_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_assistance_info_convertToJSON() failed [mbs_session_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mbsSessionId", mbs_session_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_assistance_info_convertToJSON() failed [mbs_session_id]");
        goto end;
    }

    if (mbs_assistance_info->assistance_info) {
    cJSON *assistance_infoList = cJSON_AddArrayToObject(item, "assistanceInfo");
    if (assistance_infoList == NULL) {
        ogs_error("OpenAPI_mbs_assistance_info_convertToJSON() failed [assistance_info]");
        goto end;
    }
    OpenAPI_list_for_each(mbs_assistance_info->assistance_info, node) {
        if (cJSON_AddStringToObject(assistance_infoList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_mbs_assistance_info_convertToJSON() failed [assistance_info]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_mbs_assistance_info_t *OpenAPI_mbs_assistance_info_parseFromJSON(cJSON *mbs_assistance_infoJSON)
{
    OpenAPI_mbs_assistance_info_t *mbs_assistance_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbs_session_id = NULL;
    OpenAPI_mbs_session_id_t *mbs_session_id_local_nonprim = NULL;
    cJSON *assistance_info = NULL;
    OpenAPI_list_t *assistance_infoList = NULL;
    mbs_session_id = cJSON_GetObjectItemCaseSensitive(mbs_assistance_infoJSON, "mbsSessionId");
    if (!mbs_session_id) {
        ogs_error("OpenAPI_mbs_assistance_info_parseFromJSON() failed [mbs_session_id]");
        goto end;
    }
    mbs_session_id_local_nonprim = OpenAPI_mbs_session_id_parseFromJSON(mbs_session_id);
    if (!mbs_session_id_local_nonprim) {
        ogs_error("OpenAPI_mbs_session_id_parseFromJSON failed [mbs_session_id]");
        goto end;
    }

    assistance_info = cJSON_GetObjectItemCaseSensitive(mbs_assistance_infoJSON, "assistanceInfo");
    if (assistance_info) {
        cJSON *assistance_info_local = NULL;
        if (!cJSON_IsArray(assistance_info)) {
            ogs_error("OpenAPI_mbs_assistance_info_parseFromJSON() failed [assistance_info]");
            goto end;
        }

        assistance_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(assistance_info_local, assistance_info) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(assistance_info_local)) {
                ogs_error("OpenAPI_mbs_assistance_info_parseFromJSON() failed [assistance_info]");
                goto end;
            }
            OpenAPI_list_add(assistance_infoList, ogs_strdup(assistance_info_local->valuestring));
        }
    }

    mbs_assistance_info_local_var = OpenAPI_mbs_assistance_info_create (
        mbs_session_id_local_nonprim,
        assistance_info ? assistance_infoList : NULL
    );

    return mbs_assistance_info_local_var;
end:
    if (mbs_session_id_local_nonprim) {
        OpenAPI_mbs_session_id_free(mbs_session_id_local_nonprim);
        mbs_session_id_local_nonprim = NULL;
    }
    if (assistance_infoList) {
        OpenAPI_list_for_each(assistance_infoList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(assistance_infoList);
        assistance_infoList = NULL;
    }
    return NULL;
}

OpenAPI_mbs_assistance_info_t *OpenAPI_mbs_assistance_info_copy(OpenAPI_mbs_assistance_info_t *dst, OpenAPI_mbs_assistance_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_assistance_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_assistance_info_convertToJSON() failed");
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

    OpenAPI_mbs_assistance_info_free(dst);
    dst = OpenAPI_mbs_assistance_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


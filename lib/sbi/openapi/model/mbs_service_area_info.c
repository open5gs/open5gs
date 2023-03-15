
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_service_area_info.h"

OpenAPI_mbs_service_area_info_t *OpenAPI_mbs_service_area_info_create(
    int area_session_id,
    OpenAPI_mbs_service_area_t *mbs_service_area
)
{
    OpenAPI_mbs_service_area_info_t *mbs_service_area_info_local_var = ogs_malloc(sizeof(OpenAPI_mbs_service_area_info_t));
    ogs_assert(mbs_service_area_info_local_var);

    mbs_service_area_info_local_var->area_session_id = area_session_id;
    mbs_service_area_info_local_var->mbs_service_area = mbs_service_area;

    return mbs_service_area_info_local_var;
}

void OpenAPI_mbs_service_area_info_free(OpenAPI_mbs_service_area_info_t *mbs_service_area_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_service_area_info) {
        return;
    }
    if (mbs_service_area_info->mbs_service_area) {
        OpenAPI_mbs_service_area_free(mbs_service_area_info->mbs_service_area);
        mbs_service_area_info->mbs_service_area = NULL;
    }
    ogs_free(mbs_service_area_info);
}

cJSON *OpenAPI_mbs_service_area_info_convertToJSON(OpenAPI_mbs_service_area_info_t *mbs_service_area_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_service_area_info == NULL) {
        ogs_error("OpenAPI_mbs_service_area_info_convertToJSON() failed [MbsServiceAreaInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "areaSessionId", mbs_service_area_info->area_session_id) == NULL) {
        ogs_error("OpenAPI_mbs_service_area_info_convertToJSON() failed [area_session_id]");
        goto end;
    }

    if (!mbs_service_area_info->mbs_service_area) {
        ogs_error("OpenAPI_mbs_service_area_info_convertToJSON() failed [mbs_service_area]");
        return NULL;
    }
    cJSON *mbs_service_area_local_JSON = OpenAPI_mbs_service_area_convertToJSON(mbs_service_area_info->mbs_service_area);
    if (mbs_service_area_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_service_area_info_convertToJSON() failed [mbs_service_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mbsServiceArea", mbs_service_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_service_area_info_convertToJSON() failed [mbs_service_area]");
        goto end;
    }

end:
    return item;
}

OpenAPI_mbs_service_area_info_t *OpenAPI_mbs_service_area_info_parseFromJSON(cJSON *mbs_service_area_infoJSON)
{
    OpenAPI_mbs_service_area_info_t *mbs_service_area_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *area_session_id = NULL;
    cJSON *mbs_service_area = NULL;
    OpenAPI_mbs_service_area_t *mbs_service_area_local_nonprim = NULL;
    area_session_id = cJSON_GetObjectItemCaseSensitive(mbs_service_area_infoJSON, "areaSessionId");
    if (!area_session_id) {
        ogs_error("OpenAPI_mbs_service_area_info_parseFromJSON() failed [area_session_id]");
        goto end;
    }
    if (!cJSON_IsNumber(area_session_id)) {
        ogs_error("OpenAPI_mbs_service_area_info_parseFromJSON() failed [area_session_id]");
        goto end;
    }

    mbs_service_area = cJSON_GetObjectItemCaseSensitive(mbs_service_area_infoJSON, "mbsServiceArea");
    if (!mbs_service_area) {
        ogs_error("OpenAPI_mbs_service_area_info_parseFromJSON() failed [mbs_service_area]");
        goto end;
    }
    mbs_service_area_local_nonprim = OpenAPI_mbs_service_area_parseFromJSON(mbs_service_area);
    if (!mbs_service_area_local_nonprim) {
        ogs_error("OpenAPI_mbs_service_area_parseFromJSON failed [mbs_service_area]");
        goto end;
    }

    mbs_service_area_info_local_var = OpenAPI_mbs_service_area_info_create (
        
        area_session_id->valuedouble,
        mbs_service_area_local_nonprim
    );

    return mbs_service_area_info_local_var;
end:
    if (mbs_service_area_local_nonprim) {
        OpenAPI_mbs_service_area_free(mbs_service_area_local_nonprim);
        mbs_service_area_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_mbs_service_area_info_t *OpenAPI_mbs_service_area_info_copy(OpenAPI_mbs_service_area_info_t *dst, OpenAPI_mbs_service_area_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_service_area_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_service_area_info_convertToJSON() failed");
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

    OpenAPI_mbs_service_area_info_free(dst);
    dst = OpenAPI_mbs_service_area_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


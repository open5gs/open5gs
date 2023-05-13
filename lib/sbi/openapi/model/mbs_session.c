
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_session.h"

OpenAPI_mbs_session_t *OpenAPI_mbs_session_create(
    OpenAPI_mbs_session_id_t *mbs_session_id,
    OpenAPI_list_t* mbs_area_sessions
)
{
    OpenAPI_mbs_session_t *mbs_session_local_var = ogs_malloc(sizeof(OpenAPI_mbs_session_t));
    ogs_assert(mbs_session_local_var);

    mbs_session_local_var->mbs_session_id = mbs_session_id;
    mbs_session_local_var->mbs_area_sessions = mbs_area_sessions;

    return mbs_session_local_var;
}

void OpenAPI_mbs_session_free(OpenAPI_mbs_session_t *mbs_session)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_session) {
        return;
    }
    if (mbs_session->mbs_session_id) {
        OpenAPI_mbs_session_id_free(mbs_session->mbs_session_id);
        mbs_session->mbs_session_id = NULL;
    }
    if (mbs_session->mbs_area_sessions) {
        OpenAPI_list_for_each(mbs_session->mbs_area_sessions, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_mbs_service_area_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(mbs_session->mbs_area_sessions);
        mbs_session->mbs_area_sessions = NULL;
    }
    ogs_free(mbs_session);
}

cJSON *OpenAPI_mbs_session_convertToJSON(OpenAPI_mbs_session_t *mbs_session)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_session == NULL) {
        ogs_error("OpenAPI_mbs_session_convertToJSON() failed [MbsSession]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!mbs_session->mbs_session_id) {
        ogs_error("OpenAPI_mbs_session_convertToJSON() failed [mbs_session_id]");
        return NULL;
    }
    cJSON *mbs_session_id_local_JSON = OpenAPI_mbs_session_id_convertToJSON(mbs_session->mbs_session_id);
    if (mbs_session_id_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_session_convertToJSON() failed [mbs_session_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mbsSessionId", mbs_session_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_session_convertToJSON() failed [mbs_session_id]");
        goto end;
    }

    if (mbs_session->mbs_area_sessions) {
    cJSON *mbs_area_sessions = cJSON_AddObjectToObject(item, "mbsAreaSessions");
    if (mbs_area_sessions == NULL) {
        ogs_error("OpenAPI_mbs_session_convertToJSON() failed [mbs_area_sessions]");
        goto end;
    }
    cJSON *localMapObject = mbs_area_sessions;
    if (mbs_session->mbs_area_sessions) {
        OpenAPI_list_for_each(mbs_session->mbs_area_sessions, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_mbs_session_convertToJSON() failed [mbs_area_sessions]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_mbs_session_convertToJSON() failed [mbs_area_sessions]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_mbs_service_area_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_mbs_session_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_mbs_session_t *OpenAPI_mbs_session_parseFromJSON(cJSON *mbs_sessionJSON)
{
    OpenAPI_mbs_session_t *mbs_session_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbs_session_id = NULL;
    OpenAPI_mbs_session_id_t *mbs_session_id_local_nonprim = NULL;
    cJSON *mbs_area_sessions = NULL;
    OpenAPI_list_t *mbs_area_sessionsList = NULL;
    mbs_session_id = cJSON_GetObjectItemCaseSensitive(mbs_sessionJSON, "mbsSessionId");
    if (!mbs_session_id) {
        ogs_error("OpenAPI_mbs_session_parseFromJSON() failed [mbs_session_id]");
        goto end;
    }
    mbs_session_id_local_nonprim = OpenAPI_mbs_session_id_parseFromJSON(mbs_session_id);
    if (!mbs_session_id_local_nonprim) {
        ogs_error("OpenAPI_mbs_session_id_parseFromJSON failed [mbs_session_id]");
        goto end;
    }

    mbs_area_sessions = cJSON_GetObjectItemCaseSensitive(mbs_sessionJSON, "mbsAreaSessions");
    if (mbs_area_sessions) {
        cJSON *mbs_area_sessions_local_map = NULL;
        if (!cJSON_IsObject(mbs_area_sessions) && !cJSON_IsNull(mbs_area_sessions)) {
            ogs_error("OpenAPI_mbs_session_parseFromJSON() failed [mbs_area_sessions]");
            goto end;
        }
        if (cJSON_IsObject(mbs_area_sessions)) {
            mbs_area_sessionsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(mbs_area_sessions_local_map, mbs_area_sessions) {
                cJSON *localMapObject = mbs_area_sessions_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_mbs_service_area_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_mbs_session_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(mbs_area_sessionsList, localMapKeyPair);
            }
        }
    }

    mbs_session_local_var = OpenAPI_mbs_session_create (
        mbs_session_id_local_nonprim,
        mbs_area_sessions ? mbs_area_sessionsList : NULL
    );

    return mbs_session_local_var;
end:
    if (mbs_session_id_local_nonprim) {
        OpenAPI_mbs_session_id_free(mbs_session_id_local_nonprim);
        mbs_session_id_local_nonprim = NULL;
    }
    if (mbs_area_sessionsList) {
        OpenAPI_list_for_each(mbs_area_sessionsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_mbs_service_area_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(mbs_area_sessionsList);
        mbs_area_sessionsList = NULL;
    }
    return NULL;
}

OpenAPI_mbs_session_t *OpenAPI_mbs_session_copy(OpenAPI_mbs_session_t *dst, OpenAPI_mbs_session_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_session_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_session_convertToJSON() failed");
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

    OpenAPI_mbs_session_free(dst);
    dst = OpenAPI_mbs_session_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


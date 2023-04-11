
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_5_mbs_authorization_info.h"

OpenAPI_model_5_mbs_authorization_info_t *OpenAPI_model_5_mbs_authorization_info_create(
    OpenAPI_list_t *_5mbs_session_ids
)
{
    OpenAPI_model_5_mbs_authorization_info_t *model_5_mbs_authorization_info_local_var = ogs_malloc(sizeof(OpenAPI_model_5_mbs_authorization_info_t));
    ogs_assert(model_5_mbs_authorization_info_local_var);

    model_5_mbs_authorization_info_local_var->_5mbs_session_ids = _5mbs_session_ids;

    return model_5_mbs_authorization_info_local_var;
}

void OpenAPI_model_5_mbs_authorization_info_free(OpenAPI_model_5_mbs_authorization_info_t *model_5_mbs_authorization_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_5_mbs_authorization_info) {
        return;
    }
    if (model_5_mbs_authorization_info->_5mbs_session_ids) {
        OpenAPI_list_for_each(model_5_mbs_authorization_info->_5mbs_session_ids, node) {
            OpenAPI_mbs_session_id_free(node->data);
        }
        OpenAPI_list_free(model_5_mbs_authorization_info->_5mbs_session_ids);
        model_5_mbs_authorization_info->_5mbs_session_ids = NULL;
    }
    ogs_free(model_5_mbs_authorization_info);
}

cJSON *OpenAPI_model_5_mbs_authorization_info_convertToJSON(OpenAPI_model_5_mbs_authorization_info_t *model_5_mbs_authorization_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_5_mbs_authorization_info == NULL) {
        ogs_error("OpenAPI_model_5_mbs_authorization_info_convertToJSON() failed [5MbsAuthorizationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (model_5_mbs_authorization_info->_5mbs_session_ids) {
    cJSON *_5mbs_session_idsList = cJSON_AddArrayToObject(item, "5mbsSessionIds");
    if (_5mbs_session_idsList == NULL) {
        ogs_error("OpenAPI_model_5_mbs_authorization_info_convertToJSON() failed [_5mbs_session_ids]");
        goto end;
    }
    OpenAPI_list_for_each(model_5_mbs_authorization_info->_5mbs_session_ids, node) {
        cJSON *itemLocal = OpenAPI_mbs_session_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_model_5_mbs_authorization_info_convertToJSON() failed [_5mbs_session_ids]");
            goto end;
        }
        cJSON_AddItemToArray(_5mbs_session_idsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_model_5_mbs_authorization_info_t *OpenAPI_model_5_mbs_authorization_info_parseFromJSON(cJSON *model_5_mbs_authorization_infoJSON)
{
    OpenAPI_model_5_mbs_authorization_info_t *model_5_mbs_authorization_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *_5mbs_session_ids = NULL;
    OpenAPI_list_t *_5mbs_session_idsList = NULL;
    _5mbs_session_ids = cJSON_GetObjectItemCaseSensitive(model_5_mbs_authorization_infoJSON, "5mbsSessionIds");
    if (_5mbs_session_ids) {
        cJSON *_5mbs_session_ids_local = NULL;
        if (!cJSON_IsArray(_5mbs_session_ids)) {
            ogs_error("OpenAPI_model_5_mbs_authorization_info_parseFromJSON() failed [_5mbs_session_ids]");
            goto end;
        }

        _5mbs_session_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(_5mbs_session_ids_local, _5mbs_session_ids) {
            if (!cJSON_IsObject(_5mbs_session_ids_local)) {
                ogs_error("OpenAPI_model_5_mbs_authorization_info_parseFromJSON() failed [_5mbs_session_ids]");
                goto end;
            }
            OpenAPI_mbs_session_id_t *_5mbs_session_idsItem = OpenAPI_mbs_session_id_parseFromJSON(_5mbs_session_ids_local);
            if (!_5mbs_session_idsItem) {
                ogs_error("No _5mbs_session_idsItem");
                goto end;
            }
            OpenAPI_list_add(_5mbs_session_idsList, _5mbs_session_idsItem);
        }
    }

    model_5_mbs_authorization_info_local_var = OpenAPI_model_5_mbs_authorization_info_create (
        _5mbs_session_ids ? _5mbs_session_idsList : NULL
    );

    return model_5_mbs_authorization_info_local_var;
end:
    if (_5mbs_session_idsList) {
        OpenAPI_list_for_each(_5mbs_session_idsList, node) {
            OpenAPI_mbs_session_id_free(node->data);
        }
        OpenAPI_list_free(_5mbs_session_idsList);
        _5mbs_session_idsList = NULL;
    }
    return NULL;
}

OpenAPI_model_5_mbs_authorization_info_t *OpenAPI_model_5_mbs_authorization_info_copy(OpenAPI_model_5_mbs_authorization_info_t *dst, OpenAPI_model_5_mbs_authorization_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_5_mbs_authorization_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_5_mbs_authorization_info_convertToJSON() failed");
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

    OpenAPI_model_5_mbs_authorization_info_free(dst);
    dst = OpenAPI_model_5_mbs_authorization_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


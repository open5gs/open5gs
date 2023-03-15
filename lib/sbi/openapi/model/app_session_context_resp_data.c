
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_session_context_resp_data.h"

OpenAPI_app_session_context_resp_data_t *OpenAPI_app_session_context_resp_data_create(
    OpenAPI_serv_auth_info_e serv_auth_info,
    OpenAPI_list_t *ue_ids,
    char *supp_feat
)
{
    OpenAPI_app_session_context_resp_data_t *app_session_context_resp_data_local_var = ogs_malloc(sizeof(OpenAPI_app_session_context_resp_data_t));
    ogs_assert(app_session_context_resp_data_local_var);

    app_session_context_resp_data_local_var->serv_auth_info = serv_auth_info;
    app_session_context_resp_data_local_var->ue_ids = ue_ids;
    app_session_context_resp_data_local_var->supp_feat = supp_feat;

    return app_session_context_resp_data_local_var;
}

void OpenAPI_app_session_context_resp_data_free(OpenAPI_app_session_context_resp_data_t *app_session_context_resp_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == app_session_context_resp_data) {
        return;
    }
    if (app_session_context_resp_data->ue_ids) {
        OpenAPI_list_for_each(app_session_context_resp_data->ue_ids, node) {
            OpenAPI_ue_identity_info_free(node->data);
        }
        OpenAPI_list_free(app_session_context_resp_data->ue_ids);
        app_session_context_resp_data->ue_ids = NULL;
    }
    if (app_session_context_resp_data->supp_feat) {
        ogs_free(app_session_context_resp_data->supp_feat);
        app_session_context_resp_data->supp_feat = NULL;
    }
    ogs_free(app_session_context_resp_data);
}

cJSON *OpenAPI_app_session_context_resp_data_convertToJSON(OpenAPI_app_session_context_resp_data_t *app_session_context_resp_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (app_session_context_resp_data == NULL) {
        ogs_error("OpenAPI_app_session_context_resp_data_convertToJSON() failed [AppSessionContextRespData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (app_session_context_resp_data->serv_auth_info != OpenAPI_serv_auth_info_NULL) {
    if (cJSON_AddStringToObject(item, "servAuthInfo", OpenAPI_serv_auth_info_ToString(app_session_context_resp_data->serv_auth_info)) == NULL) {
        ogs_error("OpenAPI_app_session_context_resp_data_convertToJSON() failed [serv_auth_info]");
        goto end;
    }
    }

    if (app_session_context_resp_data->ue_ids) {
    cJSON *ue_idsList = cJSON_AddArrayToObject(item, "ueIds");
    if (ue_idsList == NULL) {
        ogs_error("OpenAPI_app_session_context_resp_data_convertToJSON() failed [ue_ids]");
        goto end;
    }
    OpenAPI_list_for_each(app_session_context_resp_data->ue_ids, node) {
        cJSON *itemLocal = OpenAPI_ue_identity_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_app_session_context_resp_data_convertToJSON() failed [ue_ids]");
            goto end;
        }
        cJSON_AddItemToArray(ue_idsList, itemLocal);
    }
    }

    if (app_session_context_resp_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", app_session_context_resp_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_app_session_context_resp_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_app_session_context_resp_data_t *OpenAPI_app_session_context_resp_data_parseFromJSON(cJSON *app_session_context_resp_dataJSON)
{
    OpenAPI_app_session_context_resp_data_t *app_session_context_resp_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serv_auth_info = NULL;
    OpenAPI_serv_auth_info_e serv_auth_infoVariable = 0;
    cJSON *ue_ids = NULL;
    OpenAPI_list_t *ue_idsList = NULL;
    cJSON *supp_feat = NULL;
    serv_auth_info = cJSON_GetObjectItemCaseSensitive(app_session_context_resp_dataJSON, "servAuthInfo");
    if (serv_auth_info) {
    if (!cJSON_IsString(serv_auth_info)) {
        ogs_error("OpenAPI_app_session_context_resp_data_parseFromJSON() failed [serv_auth_info]");
        goto end;
    }
    serv_auth_infoVariable = OpenAPI_serv_auth_info_FromString(serv_auth_info->valuestring);
    }

    ue_ids = cJSON_GetObjectItemCaseSensitive(app_session_context_resp_dataJSON, "ueIds");
    if (ue_ids) {
        cJSON *ue_ids_local = NULL;
        if (!cJSON_IsArray(ue_ids)) {
            ogs_error("OpenAPI_app_session_context_resp_data_parseFromJSON() failed [ue_ids]");
            goto end;
        }

        ue_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_ids_local, ue_ids) {
            if (!cJSON_IsObject(ue_ids_local)) {
                ogs_error("OpenAPI_app_session_context_resp_data_parseFromJSON() failed [ue_ids]");
                goto end;
            }
            OpenAPI_ue_identity_info_t *ue_idsItem = OpenAPI_ue_identity_info_parseFromJSON(ue_ids_local);
            if (!ue_idsItem) {
                ogs_error("No ue_idsItem");
                goto end;
            }
            OpenAPI_list_add(ue_idsList, ue_idsItem);
        }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(app_session_context_resp_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_app_session_context_resp_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    app_session_context_resp_data_local_var = OpenAPI_app_session_context_resp_data_create (
        serv_auth_info ? serv_auth_infoVariable : 0,
        ue_ids ? ue_idsList : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return app_session_context_resp_data_local_var;
end:
    if (ue_idsList) {
        OpenAPI_list_for_each(ue_idsList, node) {
            OpenAPI_ue_identity_info_free(node->data);
        }
        OpenAPI_list_free(ue_idsList);
        ue_idsList = NULL;
    }
    return NULL;
}

OpenAPI_app_session_context_resp_data_t *OpenAPI_app_session_context_resp_data_copy(OpenAPI_app_session_context_resp_data_t *dst, OpenAPI_app_session_context_resp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_session_context_resp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_session_context_resp_data_convertToJSON() failed");
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

    OpenAPI_app_session_context_resp_data_free(dst);
    dst = OpenAPI_app_session_context_resp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


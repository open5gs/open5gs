
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_request_authorization.h"

OpenAPI_af_request_authorization_t *OpenAPI_af_request_authorization_create(
    OpenAPI_list_t *gptp_allowed_info_list,
    OpenAPI_asti_allowed_info_t *asti_allowed_info
)
{
    OpenAPI_af_request_authorization_t *af_request_authorization_local_var = ogs_malloc(sizeof(OpenAPI_af_request_authorization_t));
    ogs_assert(af_request_authorization_local_var);

    af_request_authorization_local_var->gptp_allowed_info_list = gptp_allowed_info_list;
    af_request_authorization_local_var->asti_allowed_info = asti_allowed_info;

    return af_request_authorization_local_var;
}

void OpenAPI_af_request_authorization_free(OpenAPI_af_request_authorization_t *af_request_authorization)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_request_authorization) {
        return;
    }
    if (af_request_authorization->gptp_allowed_info_list) {
        OpenAPI_list_for_each(af_request_authorization->gptp_allowed_info_list, node) {
            OpenAPI_gptp_allowed_info_free(node->data);
        }
        OpenAPI_list_free(af_request_authorization->gptp_allowed_info_list);
        af_request_authorization->gptp_allowed_info_list = NULL;
    }
    if (af_request_authorization->asti_allowed_info) {
        OpenAPI_asti_allowed_info_free(af_request_authorization->asti_allowed_info);
        af_request_authorization->asti_allowed_info = NULL;
    }
    ogs_free(af_request_authorization);
}

cJSON *OpenAPI_af_request_authorization_convertToJSON(OpenAPI_af_request_authorization_t *af_request_authorization)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_request_authorization == NULL) {
        ogs_error("OpenAPI_af_request_authorization_convertToJSON() failed [AfRequestAuthorization]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (af_request_authorization->gptp_allowed_info_list) {
    cJSON *gptp_allowed_info_listList = cJSON_AddArrayToObject(item, "gptpAllowedInfoList");
    if (gptp_allowed_info_listList == NULL) {
        ogs_error("OpenAPI_af_request_authorization_convertToJSON() failed [gptp_allowed_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(af_request_authorization->gptp_allowed_info_list, node) {
        cJSON *itemLocal = OpenAPI_gptp_allowed_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_af_request_authorization_convertToJSON() failed [gptp_allowed_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(gptp_allowed_info_listList, itemLocal);
    }
    }

    if (af_request_authorization->asti_allowed_info) {
    cJSON *asti_allowed_info_local_JSON = OpenAPI_asti_allowed_info_convertToJSON(af_request_authorization->asti_allowed_info);
    if (asti_allowed_info_local_JSON == NULL) {
        ogs_error("OpenAPI_af_request_authorization_convertToJSON() failed [asti_allowed_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "astiAllowedInfo", asti_allowed_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_request_authorization_convertToJSON() failed [asti_allowed_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_af_request_authorization_t *OpenAPI_af_request_authorization_parseFromJSON(cJSON *af_request_authorizationJSON)
{
    OpenAPI_af_request_authorization_t *af_request_authorization_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *gptp_allowed_info_list = NULL;
    OpenAPI_list_t *gptp_allowed_info_listList = NULL;
    cJSON *asti_allowed_info = NULL;
    OpenAPI_asti_allowed_info_t *asti_allowed_info_local_nonprim = NULL;
    gptp_allowed_info_list = cJSON_GetObjectItemCaseSensitive(af_request_authorizationJSON, "gptpAllowedInfoList");
    if (gptp_allowed_info_list) {
        cJSON *gptp_allowed_info_list_local = NULL;
        if (!cJSON_IsArray(gptp_allowed_info_list)) {
            ogs_error("OpenAPI_af_request_authorization_parseFromJSON() failed [gptp_allowed_info_list]");
            goto end;
        }

        gptp_allowed_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gptp_allowed_info_list_local, gptp_allowed_info_list) {
            if (!cJSON_IsObject(gptp_allowed_info_list_local)) {
                ogs_error("OpenAPI_af_request_authorization_parseFromJSON() failed [gptp_allowed_info_list]");
                goto end;
            }
            OpenAPI_gptp_allowed_info_t *gptp_allowed_info_listItem = OpenAPI_gptp_allowed_info_parseFromJSON(gptp_allowed_info_list_local);
            if (!gptp_allowed_info_listItem) {
                ogs_error("No gptp_allowed_info_listItem");
                goto end;
            }
            OpenAPI_list_add(gptp_allowed_info_listList, gptp_allowed_info_listItem);
        }
    }

    asti_allowed_info = cJSON_GetObjectItemCaseSensitive(af_request_authorizationJSON, "astiAllowedInfo");
    if (asti_allowed_info) {
    asti_allowed_info_local_nonprim = OpenAPI_asti_allowed_info_parseFromJSON(asti_allowed_info);
    if (!asti_allowed_info_local_nonprim) {
        ogs_error("OpenAPI_asti_allowed_info_parseFromJSON failed [asti_allowed_info]");
        goto end;
    }
    }

    af_request_authorization_local_var = OpenAPI_af_request_authorization_create (
        gptp_allowed_info_list ? gptp_allowed_info_listList : NULL,
        asti_allowed_info ? asti_allowed_info_local_nonprim : NULL
    );

    return af_request_authorization_local_var;
end:
    if (gptp_allowed_info_listList) {
        OpenAPI_list_for_each(gptp_allowed_info_listList, node) {
            OpenAPI_gptp_allowed_info_free(node->data);
        }
        OpenAPI_list_free(gptp_allowed_info_listList);
        gptp_allowed_info_listList = NULL;
    }
    if (asti_allowed_info_local_nonprim) {
        OpenAPI_asti_allowed_info_free(asti_allowed_info_local_nonprim);
        asti_allowed_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_af_request_authorization_t *OpenAPI_af_request_authorization_copy(OpenAPI_af_request_authorization_t *dst, OpenAPI_af_request_authorization_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_request_authorization_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_request_authorization_convertToJSON() failed");
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

    OpenAPI_af_request_authorization_free(dst);
    dst = OpenAPI_af_request_authorization_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


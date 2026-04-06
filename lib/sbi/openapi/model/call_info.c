
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "call_info.h"

OpenAPI_call_info_t *OpenAPI_call_info_create(
    OpenAPI_list_t *calling_party_addrs,
    bool is_callee_info_null,
    OpenAPI_callee_info_t *callee_info
)
{
    OpenAPI_call_info_t *call_info_local_var = ogs_malloc(sizeof(OpenAPI_call_info_t));
    ogs_assert(call_info_local_var);

    call_info_local_var->calling_party_addrs = calling_party_addrs;
    call_info_local_var->is_callee_info_null = is_callee_info_null;
    call_info_local_var->callee_info = callee_info;

    return call_info_local_var;
}

void OpenAPI_call_info_free(OpenAPI_call_info_t *call_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == call_info) {
        return;
    }
    if (call_info->calling_party_addrs) {
        OpenAPI_list_for_each(call_info->calling_party_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(call_info->calling_party_addrs);
        call_info->calling_party_addrs = NULL;
    }
    if (call_info->callee_info) {
        OpenAPI_callee_info_free(call_info->callee_info);
        call_info->callee_info = NULL;
    }
    ogs_free(call_info);
}

cJSON *OpenAPI_call_info_convertToJSON(OpenAPI_call_info_t *call_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (call_info == NULL) {
        ogs_error("OpenAPI_call_info_convertToJSON() failed [CallInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (call_info->calling_party_addrs) {
    cJSON *calling_party_addrsList = cJSON_AddArrayToObject(item, "callingPartyAddrs");
    if (calling_party_addrsList == NULL) {
        ogs_error("OpenAPI_call_info_convertToJSON() failed [calling_party_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(call_info->calling_party_addrs, node) {
        if (cJSON_AddStringToObject(calling_party_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_call_info_convertToJSON() failed [calling_party_addrs]");
            goto end;
        }
    }
    }

    if (call_info->callee_info) {
    cJSON *callee_info_local_JSON = OpenAPI_callee_info_convertToJSON(call_info->callee_info);
    if (callee_info_local_JSON == NULL) {
        ogs_error("OpenAPI_call_info_convertToJSON() failed [callee_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "calleeInfo", callee_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_call_info_convertToJSON() failed [callee_info]");
        goto end;
    }
    } else if (call_info->is_callee_info_null) {
        if (cJSON_AddNullToObject(item, "calleeInfo") == NULL) {
            ogs_error("OpenAPI_call_info_convertToJSON() failed [callee_info]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_call_info_t *OpenAPI_call_info_parseFromJSON(cJSON *call_infoJSON)
{
    OpenAPI_call_info_t *call_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *calling_party_addrs = NULL;
    OpenAPI_list_t *calling_party_addrsList = NULL;
    cJSON *callee_info = NULL;
    OpenAPI_callee_info_t *callee_info_local_nonprim = NULL;
    calling_party_addrs = cJSON_GetObjectItemCaseSensitive(call_infoJSON, "callingPartyAddrs");
    if (calling_party_addrs) {
        cJSON *calling_party_addrs_local = NULL;
        if (!cJSON_IsArray(calling_party_addrs)) {
            ogs_error("OpenAPI_call_info_parseFromJSON() failed [calling_party_addrs]");
            goto end;
        }

        calling_party_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(calling_party_addrs_local, calling_party_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(calling_party_addrs_local)) {
                ogs_error("OpenAPI_call_info_parseFromJSON() failed [calling_party_addrs]");
                goto end;
            }
            OpenAPI_list_add(calling_party_addrsList, ogs_strdup(calling_party_addrs_local->valuestring));
        }
    }

    callee_info = cJSON_GetObjectItemCaseSensitive(call_infoJSON, "calleeInfo");
    if (callee_info) {
    if (!cJSON_IsNull(callee_info)) {
    callee_info_local_nonprim = OpenAPI_callee_info_parseFromJSON(callee_info);
    if (!callee_info_local_nonprim) {
        ogs_error("OpenAPI_callee_info_parseFromJSON failed [callee_info]");
        goto end;
    }
    }
    }

    call_info_local_var = OpenAPI_call_info_create (
        calling_party_addrs ? calling_party_addrsList : NULL,
        callee_info && cJSON_IsNull(callee_info) ? true : false,
        callee_info ? callee_info_local_nonprim : NULL
    );

    return call_info_local_var;
end:
    if (calling_party_addrsList) {
        OpenAPI_list_for_each(calling_party_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(calling_party_addrsList);
        calling_party_addrsList = NULL;
    }
    if (callee_info_local_nonprim) {
        OpenAPI_callee_info_free(callee_info_local_nonprim);
        callee_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_call_info_t *OpenAPI_call_info_copy(OpenAPI_call_info_t *dst, OpenAPI_call_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_call_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_call_info_convertToJSON() failed");
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

    OpenAPI_call_info_free(dst);
    dst = OpenAPI_call_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


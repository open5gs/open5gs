
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "callee_info.h"

OpenAPI_callee_info_t *OpenAPI_callee_info_create(
    char *called_party_addr,
    OpenAPI_list_t *request_party_addrs,
    OpenAPI_list_t *called_assert_ids
)
{
    OpenAPI_callee_info_t *callee_info_local_var = ogs_malloc(sizeof(OpenAPI_callee_info_t));
    ogs_assert(callee_info_local_var);

    callee_info_local_var->called_party_addr = called_party_addr;
    callee_info_local_var->request_party_addrs = request_party_addrs;
    callee_info_local_var->called_assert_ids = called_assert_ids;

    return callee_info_local_var;
}

void OpenAPI_callee_info_free(OpenAPI_callee_info_t *callee_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == callee_info) {
        return;
    }
    if (callee_info->called_party_addr) {
        ogs_free(callee_info->called_party_addr);
        callee_info->called_party_addr = NULL;
    }
    if (callee_info->request_party_addrs) {
        OpenAPI_list_for_each(callee_info->request_party_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(callee_info->request_party_addrs);
        callee_info->request_party_addrs = NULL;
    }
    if (callee_info->called_assert_ids) {
        OpenAPI_list_for_each(callee_info->called_assert_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(callee_info->called_assert_ids);
        callee_info->called_assert_ids = NULL;
    }
    ogs_free(callee_info);
}

cJSON *OpenAPI_callee_info_convertToJSON(OpenAPI_callee_info_t *callee_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (callee_info == NULL) {
        ogs_error("OpenAPI_callee_info_convertToJSON() failed [CalleeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (callee_info->called_party_addr) {
    if (cJSON_AddStringToObject(item, "calledPartyAddr", callee_info->called_party_addr) == NULL) {
        ogs_error("OpenAPI_callee_info_convertToJSON() failed [called_party_addr]");
        goto end;
    }
    }

    if (callee_info->request_party_addrs) {
    cJSON *request_party_addrsList = cJSON_AddArrayToObject(item, "requestPartyAddrs");
    if (request_party_addrsList == NULL) {
        ogs_error("OpenAPI_callee_info_convertToJSON() failed [request_party_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(callee_info->request_party_addrs, node) {
        if (cJSON_AddStringToObject(request_party_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_callee_info_convertToJSON() failed [request_party_addrs]");
            goto end;
        }
    }
    }

    if (callee_info->called_assert_ids) {
    cJSON *called_assert_idsList = cJSON_AddArrayToObject(item, "calledAssertIds");
    if (called_assert_idsList == NULL) {
        ogs_error("OpenAPI_callee_info_convertToJSON() failed [called_assert_ids]");
        goto end;
    }
    OpenAPI_list_for_each(callee_info->called_assert_ids, node) {
        if (cJSON_AddStringToObject(called_assert_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_callee_info_convertToJSON() failed [called_assert_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_callee_info_t *OpenAPI_callee_info_parseFromJSON(cJSON *callee_infoJSON)
{
    OpenAPI_callee_info_t *callee_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *called_party_addr = NULL;
    cJSON *request_party_addrs = NULL;
    OpenAPI_list_t *request_party_addrsList = NULL;
    cJSON *called_assert_ids = NULL;
    OpenAPI_list_t *called_assert_idsList = NULL;
    called_party_addr = cJSON_GetObjectItemCaseSensitive(callee_infoJSON, "calledPartyAddr");
    if (called_party_addr) {
    if (!cJSON_IsString(called_party_addr) && !cJSON_IsNull(called_party_addr)) {
        ogs_error("OpenAPI_callee_info_parseFromJSON() failed [called_party_addr]");
        goto end;
    }
    }

    request_party_addrs = cJSON_GetObjectItemCaseSensitive(callee_infoJSON, "requestPartyAddrs");
    if (request_party_addrs) {
        cJSON *request_party_addrs_local = NULL;
        if (!cJSON_IsArray(request_party_addrs)) {
            ogs_error("OpenAPI_callee_info_parseFromJSON() failed [request_party_addrs]");
            goto end;
        }

        request_party_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(request_party_addrs_local, request_party_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(request_party_addrs_local)) {
                ogs_error("OpenAPI_callee_info_parseFromJSON() failed [request_party_addrs]");
                goto end;
            }
            OpenAPI_list_add(request_party_addrsList, ogs_strdup(request_party_addrs_local->valuestring));
        }
    }

    called_assert_ids = cJSON_GetObjectItemCaseSensitive(callee_infoJSON, "calledAssertIds");
    if (called_assert_ids) {
        cJSON *called_assert_ids_local = NULL;
        if (!cJSON_IsArray(called_assert_ids)) {
            ogs_error("OpenAPI_callee_info_parseFromJSON() failed [called_assert_ids]");
            goto end;
        }

        called_assert_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(called_assert_ids_local, called_assert_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(called_assert_ids_local)) {
                ogs_error("OpenAPI_callee_info_parseFromJSON() failed [called_assert_ids]");
                goto end;
            }
            OpenAPI_list_add(called_assert_idsList, ogs_strdup(called_assert_ids_local->valuestring));
        }
    }

    callee_info_local_var = OpenAPI_callee_info_create (
        called_party_addr && !cJSON_IsNull(called_party_addr) ? ogs_strdup(called_party_addr->valuestring) : NULL,
        request_party_addrs ? request_party_addrsList : NULL,
        called_assert_ids ? called_assert_idsList : NULL
    );

    return callee_info_local_var;
end:
    if (request_party_addrsList) {
        OpenAPI_list_for_each(request_party_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(request_party_addrsList);
        request_party_addrsList = NULL;
    }
    if (called_assert_idsList) {
        OpenAPI_list_for_each(called_assert_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(called_assert_idsList);
        called_assert_idsList = NULL;
    }
    return NULL;
}

OpenAPI_callee_info_t *OpenAPI_callee_info_copy(OpenAPI_callee_info_t *dst, OpenAPI_callee_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_callee_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_callee_info_convertToJSON() failed");
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

    OpenAPI_callee_info_free(dst);
    dst = OpenAPI_callee_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


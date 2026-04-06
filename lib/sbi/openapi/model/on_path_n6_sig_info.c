
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "on_path_n6_sig_info.h"

OpenAPI_on_path_n6_sig_info_t *OpenAPI_on_path_n6_sig_info_create(
    OpenAPI_on_path_n6_method_e on_path_n6_method,
    OpenAPI_ip_addr_t *as_proxy_addr
)
{
    OpenAPI_on_path_n6_sig_info_t *on_path_n6_sig_info_local_var = ogs_malloc(sizeof(OpenAPI_on_path_n6_sig_info_t));
    ogs_assert(on_path_n6_sig_info_local_var);

    on_path_n6_sig_info_local_var->on_path_n6_method = on_path_n6_method;
    on_path_n6_sig_info_local_var->as_proxy_addr = as_proxy_addr;

    return on_path_n6_sig_info_local_var;
}

void OpenAPI_on_path_n6_sig_info_free(OpenAPI_on_path_n6_sig_info_t *on_path_n6_sig_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == on_path_n6_sig_info) {
        return;
    }
    if (on_path_n6_sig_info->as_proxy_addr) {
        OpenAPI_ip_addr_free(on_path_n6_sig_info->as_proxy_addr);
        on_path_n6_sig_info->as_proxy_addr = NULL;
    }
    ogs_free(on_path_n6_sig_info);
}

cJSON *OpenAPI_on_path_n6_sig_info_convertToJSON(OpenAPI_on_path_n6_sig_info_t *on_path_n6_sig_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (on_path_n6_sig_info == NULL) {
        ogs_error("OpenAPI_on_path_n6_sig_info_convertToJSON() failed [OnPathN6SigInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (on_path_n6_sig_info->on_path_n6_method == OpenAPI_on_path_n6_method_NULL) {
        ogs_error("OpenAPI_on_path_n6_sig_info_convertToJSON() failed [on_path_n6_method]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "onPathN6Method", OpenAPI_on_path_n6_method_ToString(on_path_n6_sig_info->on_path_n6_method)) == NULL) {
        ogs_error("OpenAPI_on_path_n6_sig_info_convertToJSON() failed [on_path_n6_method]");
        goto end;
    }

    if (on_path_n6_sig_info->as_proxy_addr) {
    cJSON *as_proxy_addr_local_JSON = OpenAPI_ip_addr_convertToJSON(on_path_n6_sig_info->as_proxy_addr);
    if (as_proxy_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_on_path_n6_sig_info_convertToJSON() failed [as_proxy_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "asProxyAddr", as_proxy_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_on_path_n6_sig_info_convertToJSON() failed [as_proxy_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_on_path_n6_sig_info_t *OpenAPI_on_path_n6_sig_info_parseFromJSON(cJSON *on_path_n6_sig_infoJSON)
{
    OpenAPI_on_path_n6_sig_info_t *on_path_n6_sig_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *on_path_n6_method = NULL;
    OpenAPI_on_path_n6_method_e on_path_n6_methodVariable = 0;
    cJSON *as_proxy_addr = NULL;
    OpenAPI_ip_addr_t *as_proxy_addr_local_nonprim = NULL;
    on_path_n6_method = cJSON_GetObjectItemCaseSensitive(on_path_n6_sig_infoJSON, "onPathN6Method");
    if (!on_path_n6_method) {
        ogs_error("OpenAPI_on_path_n6_sig_info_parseFromJSON() failed [on_path_n6_method]");
        goto end;
    }
    if (!cJSON_IsString(on_path_n6_method)) {
        ogs_error("OpenAPI_on_path_n6_sig_info_parseFromJSON() failed [on_path_n6_method]");
        goto end;
    }
    on_path_n6_methodVariable = OpenAPI_on_path_n6_method_FromString(on_path_n6_method->valuestring);

    as_proxy_addr = cJSON_GetObjectItemCaseSensitive(on_path_n6_sig_infoJSON, "asProxyAddr");
    if (as_proxy_addr) {
    as_proxy_addr_local_nonprim = OpenAPI_ip_addr_parseFromJSON(as_proxy_addr);
    if (!as_proxy_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON failed [as_proxy_addr]");
        goto end;
    }
    }

    on_path_n6_sig_info_local_var = OpenAPI_on_path_n6_sig_info_create (
        on_path_n6_methodVariable,
        as_proxy_addr ? as_proxy_addr_local_nonprim : NULL
    );

    return on_path_n6_sig_info_local_var;
end:
    if (as_proxy_addr_local_nonprim) {
        OpenAPI_ip_addr_free(as_proxy_addr_local_nonprim);
        as_proxy_addr_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_on_path_n6_sig_info_t *OpenAPI_on_path_n6_sig_info_copy(OpenAPI_on_path_n6_sig_info_t *dst, OpenAPI_on_path_n6_sig_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_on_path_n6_sig_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_on_path_n6_sig_info_convertToJSON() failed");
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

    OpenAPI_on_path_n6_sig_info_free(dst);
    dst = OpenAPI_on_path_n6_sig_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


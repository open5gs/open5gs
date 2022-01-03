
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "acs_info_rm.h"

OpenAPI_acs_info_rm_t *OpenAPI_acs_info_rm_create(
    char *acs_url,
    char *acs_ipv4_addr,
    char *acs_ipv6_addr
)
{
    OpenAPI_acs_info_rm_t *acs_info_rm_local_var = ogs_malloc(sizeof(OpenAPI_acs_info_rm_t));
    ogs_assert(acs_info_rm_local_var);

    acs_info_rm_local_var->acs_url = acs_url;
    acs_info_rm_local_var->acs_ipv4_addr = acs_ipv4_addr;
    acs_info_rm_local_var->acs_ipv6_addr = acs_ipv6_addr;

    return acs_info_rm_local_var;
}

void OpenAPI_acs_info_rm_free(OpenAPI_acs_info_rm_t *acs_info_rm)
{
    if (NULL == acs_info_rm) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(acs_info_rm->acs_url);
    ogs_free(acs_info_rm->acs_ipv4_addr);
    ogs_free(acs_info_rm->acs_ipv6_addr);
    ogs_free(acs_info_rm);
}

cJSON *OpenAPI_acs_info_rm_convertToJSON(OpenAPI_acs_info_rm_t *acs_info_rm)
{
    cJSON *item = NULL;

    if (acs_info_rm == NULL) {
        ogs_error("OpenAPI_acs_info_rm_convertToJSON() failed [AcsInfoRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (acs_info_rm->acs_url) {
    if (cJSON_AddStringToObject(item, "acsUrl", acs_info_rm->acs_url) == NULL) {
        ogs_error("OpenAPI_acs_info_rm_convertToJSON() failed [acs_url]");
        goto end;
    }
    }

    if (acs_info_rm->acs_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "acsIpv4Addr", acs_info_rm->acs_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_acs_info_rm_convertToJSON() failed [acs_ipv4_addr]");
        goto end;
    }
    }

    if (acs_info_rm->acs_ipv6_addr) {
    if (cJSON_AddStringToObject(item, "acsIpv6Addr", acs_info_rm->acs_ipv6_addr) == NULL) {
        ogs_error("OpenAPI_acs_info_rm_convertToJSON() failed [acs_ipv6_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_acs_info_rm_t *OpenAPI_acs_info_rm_parseFromJSON(cJSON *acs_info_rmJSON)
{
    OpenAPI_acs_info_rm_t *acs_info_rm_local_var = NULL;
    cJSON *acs_url = cJSON_GetObjectItemCaseSensitive(acs_info_rmJSON, "acsUrl");

    if (acs_url) {
    if (!cJSON_IsString(acs_url)) {
        ogs_error("OpenAPI_acs_info_rm_parseFromJSON() failed [acs_url]");
        goto end;
    }
    }

    cJSON *acs_ipv4_addr = cJSON_GetObjectItemCaseSensitive(acs_info_rmJSON, "acsIpv4Addr");

    if (acs_ipv4_addr) {
    if (!cJSON_IsString(acs_ipv4_addr)) {
        ogs_error("OpenAPI_acs_info_rm_parseFromJSON() failed [acs_ipv4_addr]");
        goto end;
    }
    }

    cJSON *acs_ipv6_addr = cJSON_GetObjectItemCaseSensitive(acs_info_rmJSON, "acsIpv6Addr");

    if (acs_ipv6_addr) {
    if (!cJSON_IsString(acs_ipv6_addr)) {
        ogs_error("OpenAPI_acs_info_rm_parseFromJSON() failed [acs_ipv6_addr]");
        goto end;
    }
    }

    acs_info_rm_local_var = OpenAPI_acs_info_rm_create (
        acs_url ? ogs_strdup(acs_url->valuestring) : NULL,
        acs_ipv4_addr ? ogs_strdup(acs_ipv4_addr->valuestring) : NULL,
        acs_ipv6_addr ? ogs_strdup(acs_ipv6_addr->valuestring) : NULL
    );

    return acs_info_rm_local_var;
end:
    return NULL;
}

OpenAPI_acs_info_rm_t *OpenAPI_acs_info_rm_copy(OpenAPI_acs_info_rm_t *dst, OpenAPI_acs_info_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_acs_info_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_acs_info_rm_convertToJSON() failed");
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

    OpenAPI_acs_info_rm_free(dst);
    dst = OpenAPI_acs_info_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


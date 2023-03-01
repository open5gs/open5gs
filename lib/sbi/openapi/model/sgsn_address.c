
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sgsn_address.h"

OpenAPI_sgsn_address_t *OpenAPI_sgsn_address_create(
    char *sgsn_ipv4_addr,
    char *sgsn_ipv6_addr
)
{
    OpenAPI_sgsn_address_t *sgsn_address_local_var = ogs_malloc(sizeof(OpenAPI_sgsn_address_t));
    ogs_assert(sgsn_address_local_var);

    sgsn_address_local_var->sgsn_ipv4_addr = sgsn_ipv4_addr;
    sgsn_address_local_var->sgsn_ipv6_addr = sgsn_ipv6_addr;

    return sgsn_address_local_var;
}

void OpenAPI_sgsn_address_free(OpenAPI_sgsn_address_t *sgsn_address)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sgsn_address) {
        return;
    }
    if (sgsn_address->sgsn_ipv4_addr) {
        ogs_free(sgsn_address->sgsn_ipv4_addr);
        sgsn_address->sgsn_ipv4_addr = NULL;
    }
    if (sgsn_address->sgsn_ipv6_addr) {
        ogs_free(sgsn_address->sgsn_ipv6_addr);
        sgsn_address->sgsn_ipv6_addr = NULL;
    }
    ogs_free(sgsn_address);
}

cJSON *OpenAPI_sgsn_address_convertToJSON(OpenAPI_sgsn_address_t *sgsn_address)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sgsn_address == NULL) {
        ogs_error("OpenAPI_sgsn_address_convertToJSON() failed [SgsnAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sgsn_address->sgsn_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "sgsnIpv4Addr", sgsn_address->sgsn_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_sgsn_address_convertToJSON() failed [sgsn_ipv4_addr]");
        goto end;
    }
    }

    if (sgsn_address->sgsn_ipv6_addr) {
    if (cJSON_AddStringToObject(item, "sgsnIpv6Addr", sgsn_address->sgsn_ipv6_addr) == NULL) {
        ogs_error("OpenAPI_sgsn_address_convertToJSON() failed [sgsn_ipv6_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sgsn_address_t *OpenAPI_sgsn_address_parseFromJSON(cJSON *sgsn_addressJSON)
{
    OpenAPI_sgsn_address_t *sgsn_address_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sgsn_ipv4_addr = NULL;
    cJSON *sgsn_ipv6_addr = NULL;
    sgsn_ipv4_addr = cJSON_GetObjectItemCaseSensitive(sgsn_addressJSON, "sgsnIpv4Addr");
    if (sgsn_ipv4_addr) {
    if (!cJSON_IsString(sgsn_ipv4_addr) && !cJSON_IsNull(sgsn_ipv4_addr)) {
        ogs_error("OpenAPI_sgsn_address_parseFromJSON() failed [sgsn_ipv4_addr]");
        goto end;
    }
    }

    sgsn_ipv6_addr = cJSON_GetObjectItemCaseSensitive(sgsn_addressJSON, "sgsnIpv6Addr");
    if (sgsn_ipv6_addr) {
    if (!cJSON_IsString(sgsn_ipv6_addr) && !cJSON_IsNull(sgsn_ipv6_addr)) {
        ogs_error("OpenAPI_sgsn_address_parseFromJSON() failed [sgsn_ipv6_addr]");
        goto end;
    }
    }

    sgsn_address_local_var = OpenAPI_sgsn_address_create (
        sgsn_ipv4_addr && !cJSON_IsNull(sgsn_ipv4_addr) ? ogs_strdup(sgsn_ipv4_addr->valuestring) : NULL,
        sgsn_ipv6_addr && !cJSON_IsNull(sgsn_ipv6_addr) ? ogs_strdup(sgsn_ipv6_addr->valuestring) : NULL
    );

    return sgsn_address_local_var;
end:
    return NULL;
}

OpenAPI_sgsn_address_t *OpenAPI_sgsn_address_copy(OpenAPI_sgsn_address_t *dst, OpenAPI_sgsn_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sgsn_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sgsn_address_convertToJSON() failed");
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

    OpenAPI_sgsn_address_free(dst);
    dst = OpenAPI_sgsn_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


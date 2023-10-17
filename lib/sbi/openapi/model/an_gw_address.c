
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "an_gw_address.h"

OpenAPI_an_gw_address_t *OpenAPI_an_gw_address_create(
    char *an_gw_ipv4_addr,
    char *an_gw_ipv6_addr
)
{
    OpenAPI_an_gw_address_t *an_gw_address_local_var = ogs_malloc(sizeof(OpenAPI_an_gw_address_t));
    ogs_assert(an_gw_address_local_var);

    an_gw_address_local_var->an_gw_ipv4_addr = an_gw_ipv4_addr;
    an_gw_address_local_var->an_gw_ipv6_addr = an_gw_ipv6_addr;

    return an_gw_address_local_var;
}

void OpenAPI_an_gw_address_free(OpenAPI_an_gw_address_t *an_gw_address)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == an_gw_address) {
        return;
    }
    if (an_gw_address->an_gw_ipv4_addr) {
        ogs_free(an_gw_address->an_gw_ipv4_addr);
        an_gw_address->an_gw_ipv4_addr = NULL;
    }
    if (an_gw_address->an_gw_ipv6_addr) {
        ogs_free(an_gw_address->an_gw_ipv6_addr);
        an_gw_address->an_gw_ipv6_addr = NULL;
    }
    ogs_free(an_gw_address);
}

cJSON *OpenAPI_an_gw_address_convertToJSON(OpenAPI_an_gw_address_t *an_gw_address)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (an_gw_address == NULL) {
        ogs_error("OpenAPI_an_gw_address_convertToJSON() failed [AnGwAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (an_gw_address->an_gw_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "anGwIpv4Addr", an_gw_address->an_gw_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_an_gw_address_convertToJSON() failed [an_gw_ipv4_addr]");
        goto end;
    }
    }

    if (an_gw_address->an_gw_ipv6_addr) {
    if (cJSON_AddStringToObject(item, "anGwIpv6Addr", an_gw_address->an_gw_ipv6_addr) == NULL) {
        ogs_error("OpenAPI_an_gw_address_convertToJSON() failed [an_gw_ipv6_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_an_gw_address_t *OpenAPI_an_gw_address_parseFromJSON(cJSON *an_gw_addressJSON)
{
    OpenAPI_an_gw_address_t *an_gw_address_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *an_gw_ipv4_addr = NULL;
    cJSON *an_gw_ipv6_addr = NULL;
    an_gw_ipv4_addr = cJSON_GetObjectItemCaseSensitive(an_gw_addressJSON, "anGwIpv4Addr");
    if (an_gw_ipv4_addr) {
    if (!cJSON_IsString(an_gw_ipv4_addr) && !cJSON_IsNull(an_gw_ipv4_addr)) {
        ogs_error("OpenAPI_an_gw_address_parseFromJSON() failed [an_gw_ipv4_addr]");
        goto end;
    }
    }

    an_gw_ipv6_addr = cJSON_GetObjectItemCaseSensitive(an_gw_addressJSON, "anGwIpv6Addr");
    if (an_gw_ipv6_addr) {
    if (!cJSON_IsString(an_gw_ipv6_addr) && !cJSON_IsNull(an_gw_ipv6_addr)) {
        ogs_error("OpenAPI_an_gw_address_parseFromJSON() failed [an_gw_ipv6_addr]");
        goto end;
    }
    }

    an_gw_address_local_var = OpenAPI_an_gw_address_create (
        an_gw_ipv4_addr && !cJSON_IsNull(an_gw_ipv4_addr) ? ogs_strdup(an_gw_ipv4_addr->valuestring) : NULL,
        an_gw_ipv6_addr && !cJSON_IsNull(an_gw_ipv6_addr) ? ogs_strdup(an_gw_ipv6_addr->valuestring) : NULL
    );

    return an_gw_address_local_var;
end:
    return NULL;
}

OpenAPI_an_gw_address_t *OpenAPI_an_gw_address_copy(OpenAPI_an_gw_address_t *dst, OpenAPI_an_gw_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_an_gw_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_an_gw_address_convertToJSON() failed");
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

    OpenAPI_an_gw_address_free(dst);
    dst = OpenAPI_an_gw_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ims_address.h"

OpenAPI_ims_address_t *OpenAPI_ims_address_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *e164
)
{
    OpenAPI_ims_address_t *ims_address_local_var = ogs_malloc(sizeof(OpenAPI_ims_address_t));
    ogs_assert(ims_address_local_var);

    ims_address_local_var->ipv4_addr = ipv4_addr;
    ims_address_local_var->ipv6_addr = ipv6_addr;
    ims_address_local_var->e164 = e164;

    return ims_address_local_var;
}

void OpenAPI_ims_address_free(OpenAPI_ims_address_t *ims_address)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ims_address) {
        return;
    }
    if (ims_address->ipv4_addr) {
        ogs_free(ims_address->ipv4_addr);
        ims_address->ipv4_addr = NULL;
    }
    if (ims_address->ipv6_addr) {
        ogs_free(ims_address->ipv6_addr);
        ims_address->ipv6_addr = NULL;
    }
    if (ims_address->e164) {
        ogs_free(ims_address->e164);
        ims_address->e164 = NULL;
    }
    ogs_free(ims_address);
}

cJSON *OpenAPI_ims_address_convertToJSON(OpenAPI_ims_address_t *ims_address)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ims_address == NULL) {
        ogs_error("OpenAPI_ims_address_convertToJSON() failed [IMSAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ims_address->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", ims_address->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_ims_address_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (ims_address->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", ims_address->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_ims_address_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (ims_address->e164) {
    if (cJSON_AddStringToObject(item, "e164", ims_address->e164) == NULL) {
        ogs_error("OpenAPI_ims_address_convertToJSON() failed [e164]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ims_address_t *OpenAPI_ims_address_parseFromJSON(cJSON *ims_addressJSON)
{
    OpenAPI_ims_address_t *ims_address_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_addr = NULL;
    cJSON *e164 = NULL;
    ipv4_addr = cJSON_GetObjectItemCaseSensitive(ims_addressJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_ims_address_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_addr = cJSON_GetObjectItemCaseSensitive(ims_addressJSON, "ipv6Addr");
    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr) && !cJSON_IsNull(ipv6_addr)) {
        ogs_error("OpenAPI_ims_address_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    e164 = cJSON_GetObjectItemCaseSensitive(ims_addressJSON, "e164");
    if (e164) {
    if (!cJSON_IsString(e164) && !cJSON_IsNull(e164)) {
        ogs_error("OpenAPI_ims_address_parseFromJSON() failed [e164]");
        goto end;
    }
    }

    ims_address_local_var = OpenAPI_ims_address_create (
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr && !cJSON_IsNull(ipv6_addr) ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        e164 && !cJSON_IsNull(e164) ? ogs_strdup(e164->valuestring) : NULL
    );

    return ims_address_local_var;
end:
    return NULL;
}

OpenAPI_ims_address_t *OpenAPI_ims_address_copy(OpenAPI_ims_address_t *dst, OpenAPI_ims_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ims_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ims_address_convertToJSON() failed");
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

    OpenAPI_ims_address_free(dst);
    dst = OpenAPI_ims_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


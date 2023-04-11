
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "addr_fqdn.h"

OpenAPI_addr_fqdn_t *OpenAPI_addr_fqdn_create(
    OpenAPI_ip_addr_t *ip_addr,
    char *fqdn
)
{
    OpenAPI_addr_fqdn_t *addr_fqdn_local_var = ogs_malloc(sizeof(OpenAPI_addr_fqdn_t));
    ogs_assert(addr_fqdn_local_var);

    addr_fqdn_local_var->ip_addr = ip_addr;
    addr_fqdn_local_var->fqdn = fqdn;

    return addr_fqdn_local_var;
}

void OpenAPI_addr_fqdn_free(OpenAPI_addr_fqdn_t *addr_fqdn)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == addr_fqdn) {
        return;
    }
    if (addr_fqdn->ip_addr) {
        OpenAPI_ip_addr_free(addr_fqdn->ip_addr);
        addr_fqdn->ip_addr = NULL;
    }
    if (addr_fqdn->fqdn) {
        ogs_free(addr_fqdn->fqdn);
        addr_fqdn->fqdn = NULL;
    }
    ogs_free(addr_fqdn);
}

cJSON *OpenAPI_addr_fqdn_convertToJSON(OpenAPI_addr_fqdn_t *addr_fqdn)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (addr_fqdn == NULL) {
        ogs_error("OpenAPI_addr_fqdn_convertToJSON() failed [AddrFqdn]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (addr_fqdn->ip_addr) {
    cJSON *ip_addr_local_JSON = OpenAPI_ip_addr_convertToJSON(addr_fqdn->ip_addr);
    if (ip_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_addr_fqdn_convertToJSON() failed [ip_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ipAddr", ip_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_addr_fqdn_convertToJSON() failed [ip_addr]");
        goto end;
    }
    }

    if (addr_fqdn->fqdn) {
    if (cJSON_AddStringToObject(item, "fqdn", addr_fqdn->fqdn) == NULL) {
        ogs_error("OpenAPI_addr_fqdn_convertToJSON() failed [fqdn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_addr_fqdn_t *OpenAPI_addr_fqdn_parseFromJSON(cJSON *addr_fqdnJSON)
{
    OpenAPI_addr_fqdn_t *addr_fqdn_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ip_addr = NULL;
    OpenAPI_ip_addr_t *ip_addr_local_nonprim = NULL;
    cJSON *fqdn = NULL;
    ip_addr = cJSON_GetObjectItemCaseSensitive(addr_fqdnJSON, "ipAddr");
    if (ip_addr) {
    ip_addr_local_nonprim = OpenAPI_ip_addr_parseFromJSON(ip_addr);
    if (!ip_addr_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON failed [ip_addr]");
        goto end;
    }
    }

    fqdn = cJSON_GetObjectItemCaseSensitive(addr_fqdnJSON, "fqdn");
    if (fqdn) {
    if (!cJSON_IsString(fqdn) && !cJSON_IsNull(fqdn)) {
        ogs_error("OpenAPI_addr_fqdn_parseFromJSON() failed [fqdn]");
        goto end;
    }
    }

    addr_fqdn_local_var = OpenAPI_addr_fqdn_create (
        ip_addr ? ip_addr_local_nonprim : NULL,
        fqdn && !cJSON_IsNull(fqdn) ? ogs_strdup(fqdn->valuestring) : NULL
    );

    return addr_fqdn_local_var;
end:
    if (ip_addr_local_nonprim) {
        OpenAPI_ip_addr_free(ip_addr_local_nonprim);
        ip_addr_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_addr_fqdn_t *OpenAPI_addr_fqdn_copy(OpenAPI_addr_fqdn_t *dst, OpenAPI_addr_fqdn_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_addr_fqdn_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_addr_fqdn_convertToJSON() failed");
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

    OpenAPI_addr_fqdn_free(dst);
    dst = OpenAPI_addr_fqdn_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


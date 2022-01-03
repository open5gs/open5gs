
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vgmlc_address.h"

OpenAPI_vgmlc_address_t *OpenAPI_vgmlc_address_create(
    char *vgmlc_address_ipv4,
    char *vgmlc_address_ipv6,
    char *vgmlc_fqdn
)
{
    OpenAPI_vgmlc_address_t *vgmlc_address_local_var = ogs_malloc(sizeof(OpenAPI_vgmlc_address_t));
    ogs_assert(vgmlc_address_local_var);

    vgmlc_address_local_var->vgmlc_address_ipv4 = vgmlc_address_ipv4;
    vgmlc_address_local_var->vgmlc_address_ipv6 = vgmlc_address_ipv6;
    vgmlc_address_local_var->vgmlc_fqdn = vgmlc_fqdn;

    return vgmlc_address_local_var;
}

void OpenAPI_vgmlc_address_free(OpenAPI_vgmlc_address_t *vgmlc_address)
{
    if (NULL == vgmlc_address) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(vgmlc_address->vgmlc_address_ipv4);
    ogs_free(vgmlc_address->vgmlc_address_ipv6);
    ogs_free(vgmlc_address->vgmlc_fqdn);
    ogs_free(vgmlc_address);
}

cJSON *OpenAPI_vgmlc_address_convertToJSON(OpenAPI_vgmlc_address_t *vgmlc_address)
{
    cJSON *item = NULL;

    if (vgmlc_address == NULL) {
        ogs_error("OpenAPI_vgmlc_address_convertToJSON() failed [VgmlcAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (vgmlc_address->vgmlc_address_ipv4) {
    if (cJSON_AddStringToObject(item, "vgmlcAddressIpv4", vgmlc_address->vgmlc_address_ipv4) == NULL) {
        ogs_error("OpenAPI_vgmlc_address_convertToJSON() failed [vgmlc_address_ipv4]");
        goto end;
    }
    }

    if (vgmlc_address->vgmlc_address_ipv6) {
    if (cJSON_AddStringToObject(item, "vgmlcAddressIpv6", vgmlc_address->vgmlc_address_ipv6) == NULL) {
        ogs_error("OpenAPI_vgmlc_address_convertToJSON() failed [vgmlc_address_ipv6]");
        goto end;
    }
    }

    if (vgmlc_address->vgmlc_fqdn) {
    if (cJSON_AddStringToObject(item, "vgmlcFqdn", vgmlc_address->vgmlc_fqdn) == NULL) {
        ogs_error("OpenAPI_vgmlc_address_convertToJSON() failed [vgmlc_fqdn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_vgmlc_address_t *OpenAPI_vgmlc_address_parseFromJSON(cJSON *vgmlc_addressJSON)
{
    OpenAPI_vgmlc_address_t *vgmlc_address_local_var = NULL;
    cJSON *vgmlc_address_ipv4 = cJSON_GetObjectItemCaseSensitive(vgmlc_addressJSON, "vgmlcAddressIpv4");

    if (vgmlc_address_ipv4) {
    if (!cJSON_IsString(vgmlc_address_ipv4)) {
        ogs_error("OpenAPI_vgmlc_address_parseFromJSON() failed [vgmlc_address_ipv4]");
        goto end;
    }
    }

    cJSON *vgmlc_address_ipv6 = cJSON_GetObjectItemCaseSensitive(vgmlc_addressJSON, "vgmlcAddressIpv6");

    if (vgmlc_address_ipv6) {
    if (!cJSON_IsString(vgmlc_address_ipv6)) {
        ogs_error("OpenAPI_vgmlc_address_parseFromJSON() failed [vgmlc_address_ipv6]");
        goto end;
    }
    }

    cJSON *vgmlc_fqdn = cJSON_GetObjectItemCaseSensitive(vgmlc_addressJSON, "vgmlcFqdn");

    if (vgmlc_fqdn) {
    if (!cJSON_IsString(vgmlc_fqdn)) {
        ogs_error("OpenAPI_vgmlc_address_parseFromJSON() failed [vgmlc_fqdn]");
        goto end;
    }
    }

    vgmlc_address_local_var = OpenAPI_vgmlc_address_create (
        vgmlc_address_ipv4 ? ogs_strdup(vgmlc_address_ipv4->valuestring) : NULL,
        vgmlc_address_ipv6 ? ogs_strdup(vgmlc_address_ipv6->valuestring) : NULL,
        vgmlc_fqdn ? ogs_strdup(vgmlc_fqdn->valuestring) : NULL
    );

    return vgmlc_address_local_var;
end:
    return NULL;
}

OpenAPI_vgmlc_address_t *OpenAPI_vgmlc_address_copy(OpenAPI_vgmlc_address_t *dst, OpenAPI_vgmlc_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vgmlc_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vgmlc_address_convertToJSON() failed");
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

    OpenAPI_vgmlc_address_free(dst);
    dst = OpenAPI_vgmlc_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


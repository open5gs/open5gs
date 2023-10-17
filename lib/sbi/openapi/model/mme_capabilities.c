
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mme_capabilities.h"

OpenAPI_mme_capabilities_t *OpenAPI_mme_capabilities_create(
    bool is_non_ip_supported,
    int non_ip_supported,
    bool is_ethernet_supported,
    int ethernet_supported,
    bool is_upip_supported,
    int upip_supported
)
{
    OpenAPI_mme_capabilities_t *mme_capabilities_local_var = ogs_malloc(sizeof(OpenAPI_mme_capabilities_t));
    ogs_assert(mme_capabilities_local_var);

    mme_capabilities_local_var->is_non_ip_supported = is_non_ip_supported;
    mme_capabilities_local_var->non_ip_supported = non_ip_supported;
    mme_capabilities_local_var->is_ethernet_supported = is_ethernet_supported;
    mme_capabilities_local_var->ethernet_supported = ethernet_supported;
    mme_capabilities_local_var->is_upip_supported = is_upip_supported;
    mme_capabilities_local_var->upip_supported = upip_supported;

    return mme_capabilities_local_var;
}

void OpenAPI_mme_capabilities_free(OpenAPI_mme_capabilities_t *mme_capabilities)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mme_capabilities) {
        return;
    }
    ogs_free(mme_capabilities);
}

cJSON *OpenAPI_mme_capabilities_convertToJSON(OpenAPI_mme_capabilities_t *mme_capabilities)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mme_capabilities == NULL) {
        ogs_error("OpenAPI_mme_capabilities_convertToJSON() failed [MmeCapabilities]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mme_capabilities->is_non_ip_supported) {
    if (cJSON_AddBoolToObject(item, "nonIpSupported", mme_capabilities->non_ip_supported) == NULL) {
        ogs_error("OpenAPI_mme_capabilities_convertToJSON() failed [non_ip_supported]");
        goto end;
    }
    }

    if (mme_capabilities->is_ethernet_supported) {
    if (cJSON_AddBoolToObject(item, "ethernetSupported", mme_capabilities->ethernet_supported) == NULL) {
        ogs_error("OpenAPI_mme_capabilities_convertToJSON() failed [ethernet_supported]");
        goto end;
    }
    }

    if (mme_capabilities->is_upip_supported) {
    if (cJSON_AddBoolToObject(item, "upipSupported", mme_capabilities->upip_supported) == NULL) {
        ogs_error("OpenAPI_mme_capabilities_convertToJSON() failed [upip_supported]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mme_capabilities_t *OpenAPI_mme_capabilities_parseFromJSON(cJSON *mme_capabilitiesJSON)
{
    OpenAPI_mme_capabilities_t *mme_capabilities_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *non_ip_supported = NULL;
    cJSON *ethernet_supported = NULL;
    cJSON *upip_supported = NULL;
    non_ip_supported = cJSON_GetObjectItemCaseSensitive(mme_capabilitiesJSON, "nonIpSupported");
    if (non_ip_supported) {
    if (!cJSON_IsBool(non_ip_supported)) {
        ogs_error("OpenAPI_mme_capabilities_parseFromJSON() failed [non_ip_supported]");
        goto end;
    }
    }

    ethernet_supported = cJSON_GetObjectItemCaseSensitive(mme_capabilitiesJSON, "ethernetSupported");
    if (ethernet_supported) {
    if (!cJSON_IsBool(ethernet_supported)) {
        ogs_error("OpenAPI_mme_capabilities_parseFromJSON() failed [ethernet_supported]");
        goto end;
    }
    }

    upip_supported = cJSON_GetObjectItemCaseSensitive(mme_capabilitiesJSON, "upipSupported");
    if (upip_supported) {
    if (!cJSON_IsBool(upip_supported)) {
        ogs_error("OpenAPI_mme_capabilities_parseFromJSON() failed [upip_supported]");
        goto end;
    }
    }

    mme_capabilities_local_var = OpenAPI_mme_capabilities_create (
        non_ip_supported ? true : false,
        non_ip_supported ? non_ip_supported->valueint : 0,
        ethernet_supported ? true : false,
        ethernet_supported ? ethernet_supported->valueint : 0,
        upip_supported ? true : false,
        upip_supported ? upip_supported->valueint : 0
    );

    return mme_capabilities_local_var;
end:
    return NULL;
}

OpenAPI_mme_capabilities_t *OpenAPI_mme_capabilities_copy(OpenAPI_mme_capabilities_t *dst, OpenAPI_mme_capabilities_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mme_capabilities_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mme_capabilities_convertToJSON() failed");
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

    OpenAPI_mme_capabilities_free(dst);
    dst = OpenAPI_mme_capabilities_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


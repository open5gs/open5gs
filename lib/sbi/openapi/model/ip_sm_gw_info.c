
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_sm_gw_info.h"

OpenAPI_ip_sm_gw_info_t *OpenAPI_ip_sm_gw_info_create(
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration,
    OpenAPI_ip_sm_gw_guidance_t *ip_sm_gw_guidance
)
{
    OpenAPI_ip_sm_gw_info_t *ip_sm_gw_info_local_var = ogs_malloc(sizeof(OpenAPI_ip_sm_gw_info_t));
    ogs_assert(ip_sm_gw_info_local_var);

    ip_sm_gw_info_local_var->ip_sm_gw_registration = ip_sm_gw_registration;
    ip_sm_gw_info_local_var->ip_sm_gw_guidance = ip_sm_gw_guidance;

    return ip_sm_gw_info_local_var;
}

void OpenAPI_ip_sm_gw_info_free(OpenAPI_ip_sm_gw_info_t *ip_sm_gw_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ip_sm_gw_info) {
        return;
    }
    if (ip_sm_gw_info->ip_sm_gw_registration) {
        OpenAPI_ip_sm_gw_registration_free(ip_sm_gw_info->ip_sm_gw_registration);
        ip_sm_gw_info->ip_sm_gw_registration = NULL;
    }
    if (ip_sm_gw_info->ip_sm_gw_guidance) {
        OpenAPI_ip_sm_gw_guidance_free(ip_sm_gw_info->ip_sm_gw_guidance);
        ip_sm_gw_info->ip_sm_gw_guidance = NULL;
    }
    ogs_free(ip_sm_gw_info);
}

cJSON *OpenAPI_ip_sm_gw_info_convertToJSON(OpenAPI_ip_sm_gw_info_t *ip_sm_gw_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ip_sm_gw_info == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_info_convertToJSON() failed [IpSmGwInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ip_sm_gw_info->ip_sm_gw_registration) {
    cJSON *ip_sm_gw_registration_local_JSON = OpenAPI_ip_sm_gw_registration_convertToJSON(ip_sm_gw_info->ip_sm_gw_registration);
    if (ip_sm_gw_registration_local_JSON == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_info_convertToJSON() failed [ip_sm_gw_registration]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ipSmGwRegistration", ip_sm_gw_registration_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_info_convertToJSON() failed [ip_sm_gw_registration]");
        goto end;
    }
    }

    if (ip_sm_gw_info->ip_sm_gw_guidance) {
    cJSON *ip_sm_gw_guidance_local_JSON = OpenAPI_ip_sm_gw_guidance_convertToJSON(ip_sm_gw_info->ip_sm_gw_guidance);
    if (ip_sm_gw_guidance_local_JSON == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_info_convertToJSON() failed [ip_sm_gw_guidance]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ipSmGwGuidance", ip_sm_gw_guidance_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ip_sm_gw_info_convertToJSON() failed [ip_sm_gw_guidance]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ip_sm_gw_info_t *OpenAPI_ip_sm_gw_info_parseFromJSON(cJSON *ip_sm_gw_infoJSON)
{
    OpenAPI_ip_sm_gw_info_t *ip_sm_gw_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ip_sm_gw_registration = NULL;
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_registration_local_nonprim = NULL;
    cJSON *ip_sm_gw_guidance = NULL;
    OpenAPI_ip_sm_gw_guidance_t *ip_sm_gw_guidance_local_nonprim = NULL;
    ip_sm_gw_registration = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_infoJSON, "ipSmGwRegistration");
    if (ip_sm_gw_registration) {
    ip_sm_gw_registration_local_nonprim = OpenAPI_ip_sm_gw_registration_parseFromJSON(ip_sm_gw_registration);
    if (!ip_sm_gw_registration_local_nonprim) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON failed [ip_sm_gw_registration]");
        goto end;
    }
    }

    ip_sm_gw_guidance = cJSON_GetObjectItemCaseSensitive(ip_sm_gw_infoJSON, "ipSmGwGuidance");
    if (ip_sm_gw_guidance) {
    ip_sm_gw_guidance_local_nonprim = OpenAPI_ip_sm_gw_guidance_parseFromJSON(ip_sm_gw_guidance);
    if (!ip_sm_gw_guidance_local_nonprim) {
        ogs_error("OpenAPI_ip_sm_gw_guidance_parseFromJSON failed [ip_sm_gw_guidance]");
        goto end;
    }
    }

    ip_sm_gw_info_local_var = OpenAPI_ip_sm_gw_info_create (
        ip_sm_gw_registration ? ip_sm_gw_registration_local_nonprim : NULL,
        ip_sm_gw_guidance ? ip_sm_gw_guidance_local_nonprim : NULL
    );

    return ip_sm_gw_info_local_var;
end:
    if (ip_sm_gw_registration_local_nonprim) {
        OpenAPI_ip_sm_gw_registration_free(ip_sm_gw_registration_local_nonprim);
        ip_sm_gw_registration_local_nonprim = NULL;
    }
    if (ip_sm_gw_guidance_local_nonprim) {
        OpenAPI_ip_sm_gw_guidance_free(ip_sm_gw_guidance_local_nonprim);
        ip_sm_gw_guidance_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ip_sm_gw_info_t *OpenAPI_ip_sm_gw_info_copy(OpenAPI_ip_sm_gw_info_t *dst, OpenAPI_ip_sm_gw_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ip_sm_gw_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ip_sm_gw_info_convertToJSON() failed");
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

    OpenAPI_ip_sm_gw_info_free(dst);
    dst = OpenAPI_ip_sm_gw_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redirect_response_add_info.h"

OpenAPI_redirect_response_add_info_t *OpenAPI_redirect_response_add_info_create(
    char *sepp_fqdn_for_discovery
)
{
    OpenAPI_redirect_response_add_info_t *redirect_response_add_info_local_var = ogs_malloc(sizeof(OpenAPI_redirect_response_add_info_t));
    ogs_assert(redirect_response_add_info_local_var);

    redirect_response_add_info_local_var->sepp_fqdn_for_discovery = sepp_fqdn_for_discovery;

    return redirect_response_add_info_local_var;
}

void OpenAPI_redirect_response_add_info_free(OpenAPI_redirect_response_add_info_t *redirect_response_add_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == redirect_response_add_info) {
        return;
    }
    if (redirect_response_add_info->sepp_fqdn_for_discovery) {
        ogs_free(redirect_response_add_info->sepp_fqdn_for_discovery);
        redirect_response_add_info->sepp_fqdn_for_discovery = NULL;
    }
    ogs_free(redirect_response_add_info);
}

cJSON *OpenAPI_redirect_response_add_info_convertToJSON(OpenAPI_redirect_response_add_info_t *redirect_response_add_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (redirect_response_add_info == NULL) {
        ogs_error("OpenAPI_redirect_response_add_info_convertToJSON() failed [RedirectResponseAddInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (redirect_response_add_info->sepp_fqdn_for_discovery) {
    if (cJSON_AddStringToObject(item, "seppFqdnForDiscovery", redirect_response_add_info->sepp_fqdn_for_discovery) == NULL) {
        ogs_error("OpenAPI_redirect_response_add_info_convertToJSON() failed [sepp_fqdn_for_discovery]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_redirect_response_add_info_t *OpenAPI_redirect_response_add_info_parseFromJSON(cJSON *redirect_response_add_infoJSON)
{
    OpenAPI_redirect_response_add_info_t *redirect_response_add_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sepp_fqdn_for_discovery = NULL;
    sepp_fqdn_for_discovery = cJSON_GetObjectItemCaseSensitive(redirect_response_add_infoJSON, "seppFqdnForDiscovery");
    if (sepp_fqdn_for_discovery) {
    if (!cJSON_IsString(sepp_fqdn_for_discovery) && !cJSON_IsNull(sepp_fqdn_for_discovery)) {
        ogs_error("OpenAPI_redirect_response_add_info_parseFromJSON() failed [sepp_fqdn_for_discovery]");
        goto end;
    }
    }

    redirect_response_add_info_local_var = OpenAPI_redirect_response_add_info_create (
        sepp_fqdn_for_discovery && !cJSON_IsNull(sepp_fqdn_for_discovery) ? ogs_strdup(sepp_fqdn_for_discovery->valuestring) : NULL
    );

    return redirect_response_add_info_local_var;
end:
    return NULL;
}

OpenAPI_redirect_response_add_info_t *OpenAPI_redirect_response_add_info_copy(OpenAPI_redirect_response_add_info_t *dst, OpenAPI_redirect_response_add_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_redirect_response_add_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_redirect_response_add_info_convertToJSON() failed");
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

    OpenAPI_redirect_response_add_info_free(dst);
    dst = OpenAPI_redirect_response_add_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


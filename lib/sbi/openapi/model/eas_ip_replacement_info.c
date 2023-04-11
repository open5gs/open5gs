
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eas_ip_replacement_info.h"

OpenAPI_eas_ip_replacement_info_t *OpenAPI_eas_ip_replacement_info_create(
    OpenAPI_eas_server_address_t *source,
    OpenAPI_eas_server_address_t *target
)
{
    OpenAPI_eas_ip_replacement_info_t *eas_ip_replacement_info_local_var = ogs_malloc(sizeof(OpenAPI_eas_ip_replacement_info_t));
    ogs_assert(eas_ip_replacement_info_local_var);

    eas_ip_replacement_info_local_var->source = source;
    eas_ip_replacement_info_local_var->target = target;

    return eas_ip_replacement_info_local_var;
}

void OpenAPI_eas_ip_replacement_info_free(OpenAPI_eas_ip_replacement_info_t *eas_ip_replacement_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eas_ip_replacement_info) {
        return;
    }
    if (eas_ip_replacement_info->source) {
        OpenAPI_eas_server_address_free(eas_ip_replacement_info->source);
        eas_ip_replacement_info->source = NULL;
    }
    if (eas_ip_replacement_info->target) {
        OpenAPI_eas_server_address_free(eas_ip_replacement_info->target);
        eas_ip_replacement_info->target = NULL;
    }
    ogs_free(eas_ip_replacement_info);
}

cJSON *OpenAPI_eas_ip_replacement_info_convertToJSON(OpenAPI_eas_ip_replacement_info_t *eas_ip_replacement_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eas_ip_replacement_info == NULL) {
        ogs_error("OpenAPI_eas_ip_replacement_info_convertToJSON() failed [EasIpReplacementInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!eas_ip_replacement_info->source) {
        ogs_error("OpenAPI_eas_ip_replacement_info_convertToJSON() failed [source]");
        return NULL;
    }
    cJSON *source_local_JSON = OpenAPI_eas_server_address_convertToJSON(eas_ip_replacement_info->source);
    if (source_local_JSON == NULL) {
        ogs_error("OpenAPI_eas_ip_replacement_info_convertToJSON() failed [source]");
        goto end;
    }
    cJSON_AddItemToObject(item, "source", source_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eas_ip_replacement_info_convertToJSON() failed [source]");
        goto end;
    }

    if (!eas_ip_replacement_info->target) {
        ogs_error("OpenAPI_eas_ip_replacement_info_convertToJSON() failed [target]");
        return NULL;
    }
    cJSON *target_local_JSON = OpenAPI_eas_server_address_convertToJSON(eas_ip_replacement_info->target);
    if (target_local_JSON == NULL) {
        ogs_error("OpenAPI_eas_ip_replacement_info_convertToJSON() failed [target]");
        goto end;
    }
    cJSON_AddItemToObject(item, "target", target_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eas_ip_replacement_info_convertToJSON() failed [target]");
        goto end;
    }

end:
    return item;
}

OpenAPI_eas_ip_replacement_info_t *OpenAPI_eas_ip_replacement_info_parseFromJSON(cJSON *eas_ip_replacement_infoJSON)
{
    OpenAPI_eas_ip_replacement_info_t *eas_ip_replacement_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *source = NULL;
    OpenAPI_eas_server_address_t *source_local_nonprim = NULL;
    cJSON *target = NULL;
    OpenAPI_eas_server_address_t *target_local_nonprim = NULL;
    source = cJSON_GetObjectItemCaseSensitive(eas_ip_replacement_infoJSON, "source");
    if (!source) {
        ogs_error("OpenAPI_eas_ip_replacement_info_parseFromJSON() failed [source]");
        goto end;
    }
    source_local_nonprim = OpenAPI_eas_server_address_parseFromJSON(source);
    if (!source_local_nonprim) {
        ogs_error("OpenAPI_eas_server_address_parseFromJSON failed [source]");
        goto end;
    }

    target = cJSON_GetObjectItemCaseSensitive(eas_ip_replacement_infoJSON, "target");
    if (!target) {
        ogs_error("OpenAPI_eas_ip_replacement_info_parseFromJSON() failed [target]");
        goto end;
    }
    target_local_nonprim = OpenAPI_eas_server_address_parseFromJSON(target);
    if (!target_local_nonprim) {
        ogs_error("OpenAPI_eas_server_address_parseFromJSON failed [target]");
        goto end;
    }

    eas_ip_replacement_info_local_var = OpenAPI_eas_ip_replacement_info_create (
        source_local_nonprim,
        target_local_nonprim
    );

    return eas_ip_replacement_info_local_var;
end:
    if (source_local_nonprim) {
        OpenAPI_eas_server_address_free(source_local_nonprim);
        source_local_nonprim = NULL;
    }
    if (target_local_nonprim) {
        OpenAPI_eas_server_address_free(target_local_nonprim);
        target_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_eas_ip_replacement_info_t *OpenAPI_eas_ip_replacement_info_copy(OpenAPI_eas_ip_replacement_info_t *dst, OpenAPI_eas_ip_replacement_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eas_ip_replacement_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eas_ip_replacement_info_convertToJSON() failed");
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

    OpenAPI_eas_ip_replacement_info_free(dst);
    dst = OpenAPI_eas_ip_replacement_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


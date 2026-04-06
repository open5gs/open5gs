
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "last_known_aiotf_info.h"

OpenAPI_last_known_aiotf_info_t *OpenAPI_last_known_aiotf_info_create(
    int last_known_aiotf_info_ind,
    char *last_known_aiotf_id,
    OpenAPI_ip_addr_1_t *last_known_aiotf_address,
    char *last_known_aiotf_fqdn
)
{
    OpenAPI_last_known_aiotf_info_t *last_known_aiotf_info_local_var = ogs_malloc(sizeof(OpenAPI_last_known_aiotf_info_t));
    ogs_assert(last_known_aiotf_info_local_var);

    last_known_aiotf_info_local_var->last_known_aiotf_info_ind = last_known_aiotf_info_ind;
    last_known_aiotf_info_local_var->last_known_aiotf_id = last_known_aiotf_id;
    last_known_aiotf_info_local_var->last_known_aiotf_address = last_known_aiotf_address;
    last_known_aiotf_info_local_var->last_known_aiotf_fqdn = last_known_aiotf_fqdn;

    return last_known_aiotf_info_local_var;
}

void OpenAPI_last_known_aiotf_info_free(OpenAPI_last_known_aiotf_info_t *last_known_aiotf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == last_known_aiotf_info) {
        return;
    }
    if (last_known_aiotf_info->last_known_aiotf_id) {
        ogs_free(last_known_aiotf_info->last_known_aiotf_id);
        last_known_aiotf_info->last_known_aiotf_id = NULL;
    }
    if (last_known_aiotf_info->last_known_aiotf_address) {
        OpenAPI_ip_addr_1_free(last_known_aiotf_info->last_known_aiotf_address);
        last_known_aiotf_info->last_known_aiotf_address = NULL;
    }
    if (last_known_aiotf_info->last_known_aiotf_fqdn) {
        ogs_free(last_known_aiotf_info->last_known_aiotf_fqdn);
        last_known_aiotf_info->last_known_aiotf_fqdn = NULL;
    }
    ogs_free(last_known_aiotf_info);
}

cJSON *OpenAPI_last_known_aiotf_info_convertToJSON(OpenAPI_last_known_aiotf_info_t *last_known_aiotf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (last_known_aiotf_info == NULL) {
        ogs_error("OpenAPI_last_known_aiotf_info_convertToJSON() failed [LastKnownAiotfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddBoolToObject(item, "lastKnownAiotfInfoInd", last_known_aiotf_info->last_known_aiotf_info_ind) == NULL) {
        ogs_error("OpenAPI_last_known_aiotf_info_convertToJSON() failed [last_known_aiotf_info_ind]");
        goto end;
    }

    if (last_known_aiotf_info->last_known_aiotf_id) {
    if (cJSON_AddStringToObject(item, "lastKnownAiotfId", last_known_aiotf_info->last_known_aiotf_id) == NULL) {
        ogs_error("OpenAPI_last_known_aiotf_info_convertToJSON() failed [last_known_aiotf_id]");
        goto end;
    }
    }

    if (last_known_aiotf_info->last_known_aiotf_address) {
    cJSON *last_known_aiotf_address_local_JSON = OpenAPI_ip_addr_1_convertToJSON(last_known_aiotf_info->last_known_aiotf_address);
    if (last_known_aiotf_address_local_JSON == NULL) {
        ogs_error("OpenAPI_last_known_aiotf_info_convertToJSON() failed [last_known_aiotf_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lastKnownAiotfAddress", last_known_aiotf_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_last_known_aiotf_info_convertToJSON() failed [last_known_aiotf_address]");
        goto end;
    }
    }

    if (last_known_aiotf_info->last_known_aiotf_fqdn) {
    if (cJSON_AddStringToObject(item, "lastKnownAiotfFqdn", last_known_aiotf_info->last_known_aiotf_fqdn) == NULL) {
        ogs_error("OpenAPI_last_known_aiotf_info_convertToJSON() failed [last_known_aiotf_fqdn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_last_known_aiotf_info_t *OpenAPI_last_known_aiotf_info_parseFromJSON(cJSON *last_known_aiotf_infoJSON)
{
    OpenAPI_last_known_aiotf_info_t *last_known_aiotf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *last_known_aiotf_info_ind = NULL;
    cJSON *last_known_aiotf_id = NULL;
    cJSON *last_known_aiotf_address = NULL;
    OpenAPI_ip_addr_1_t *last_known_aiotf_address_local_nonprim = NULL;
    cJSON *last_known_aiotf_fqdn = NULL;
    last_known_aiotf_info_ind = cJSON_GetObjectItemCaseSensitive(last_known_aiotf_infoJSON, "lastKnownAiotfInfoInd");
    if (!last_known_aiotf_info_ind) {
        ogs_error("OpenAPI_last_known_aiotf_info_parseFromJSON() failed [last_known_aiotf_info_ind]");
        goto end;
    }
    if (!cJSON_IsBool(last_known_aiotf_info_ind)) {
        ogs_error("OpenAPI_last_known_aiotf_info_parseFromJSON() failed [last_known_aiotf_info_ind]");
        goto end;
    }

    last_known_aiotf_id = cJSON_GetObjectItemCaseSensitive(last_known_aiotf_infoJSON, "lastKnownAiotfId");
    if (last_known_aiotf_id) {
    if (!cJSON_IsString(last_known_aiotf_id) && !cJSON_IsNull(last_known_aiotf_id)) {
        ogs_error("OpenAPI_last_known_aiotf_info_parseFromJSON() failed [last_known_aiotf_id]");
        goto end;
    }
    }

    last_known_aiotf_address = cJSON_GetObjectItemCaseSensitive(last_known_aiotf_infoJSON, "lastKnownAiotfAddress");
    if (last_known_aiotf_address) {
    last_known_aiotf_address_local_nonprim = OpenAPI_ip_addr_1_parseFromJSON(last_known_aiotf_address);
    if (!last_known_aiotf_address_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_1_parseFromJSON failed [last_known_aiotf_address]");
        goto end;
    }
    }

    last_known_aiotf_fqdn = cJSON_GetObjectItemCaseSensitive(last_known_aiotf_infoJSON, "lastKnownAiotfFqdn");
    if (last_known_aiotf_fqdn) {
    if (!cJSON_IsString(last_known_aiotf_fqdn) && !cJSON_IsNull(last_known_aiotf_fqdn)) {
        ogs_error("OpenAPI_last_known_aiotf_info_parseFromJSON() failed [last_known_aiotf_fqdn]");
        goto end;
    }
    }

    last_known_aiotf_info_local_var = OpenAPI_last_known_aiotf_info_create (
        
        last_known_aiotf_info_ind->valueint,
        last_known_aiotf_id && !cJSON_IsNull(last_known_aiotf_id) ? ogs_strdup(last_known_aiotf_id->valuestring) : NULL,
        last_known_aiotf_address ? last_known_aiotf_address_local_nonprim : NULL,
        last_known_aiotf_fqdn && !cJSON_IsNull(last_known_aiotf_fqdn) ? ogs_strdup(last_known_aiotf_fqdn->valuestring) : NULL
    );

    return last_known_aiotf_info_local_var;
end:
    if (last_known_aiotf_address_local_nonprim) {
        OpenAPI_ip_addr_1_free(last_known_aiotf_address_local_nonprim);
        last_known_aiotf_address_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_last_known_aiotf_info_t *OpenAPI_last_known_aiotf_info_copy(OpenAPI_last_known_aiotf_info_t *dst, OpenAPI_last_known_aiotf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_last_known_aiotf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_last_known_aiotf_info_convertToJSON() failed");
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

    OpenAPI_last_known_aiotf_info_free(dst);
    dst = OpenAPI_last_known_aiotf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


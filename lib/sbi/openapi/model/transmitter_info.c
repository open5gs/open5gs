
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transmitter_info.h"

OpenAPI_transmitter_info_t *OpenAPI_transmitter_info_create(
    OpenAPI_ip_addr_t *prose_source_ip_address,
    char *prose_source_l2_id
)
{
    OpenAPI_transmitter_info_t *transmitter_info_local_var = ogs_malloc(sizeof(OpenAPI_transmitter_info_t));
    ogs_assert(transmitter_info_local_var);

    transmitter_info_local_var->prose_source_ip_address = prose_source_ip_address;
    transmitter_info_local_var->prose_source_l2_id = prose_source_l2_id;

    return transmitter_info_local_var;
}

void OpenAPI_transmitter_info_free(OpenAPI_transmitter_info_t *transmitter_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == transmitter_info) {
        return;
    }
    if (transmitter_info->prose_source_ip_address) {
        OpenAPI_ip_addr_free(transmitter_info->prose_source_ip_address);
        transmitter_info->prose_source_ip_address = NULL;
    }
    if (transmitter_info->prose_source_l2_id) {
        ogs_free(transmitter_info->prose_source_l2_id);
        transmitter_info->prose_source_l2_id = NULL;
    }
    ogs_free(transmitter_info);
}

cJSON *OpenAPI_transmitter_info_convertToJSON(OpenAPI_transmitter_info_t *transmitter_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (transmitter_info == NULL) {
        ogs_error("OpenAPI_transmitter_info_convertToJSON() failed [TransmitterInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (transmitter_info->prose_source_ip_address) {
    cJSON *prose_source_ip_address_local_JSON = OpenAPI_ip_addr_convertToJSON(transmitter_info->prose_source_ip_address);
    if (prose_source_ip_address_local_JSON == NULL) {
        ogs_error("OpenAPI_transmitter_info_convertToJSON() failed [prose_source_ip_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proseSourceIPAddress", prose_source_ip_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_transmitter_info_convertToJSON() failed [prose_source_ip_address]");
        goto end;
    }
    }

    if (transmitter_info->prose_source_l2_id) {
    if (cJSON_AddStringToObject(item, "proseSourceL2Id", transmitter_info->prose_source_l2_id) == NULL) {
        ogs_error("OpenAPI_transmitter_info_convertToJSON() failed [prose_source_l2_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_transmitter_info_t *OpenAPI_transmitter_info_parseFromJSON(cJSON *transmitter_infoJSON)
{
    OpenAPI_transmitter_info_t *transmitter_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *prose_source_ip_address = NULL;
    OpenAPI_ip_addr_t *prose_source_ip_address_local_nonprim = NULL;
    cJSON *prose_source_l2_id = NULL;
    prose_source_ip_address = cJSON_GetObjectItemCaseSensitive(transmitter_infoJSON, "proseSourceIPAddress");
    if (prose_source_ip_address) {
    prose_source_ip_address_local_nonprim = OpenAPI_ip_addr_parseFromJSON(prose_source_ip_address);
    if (!prose_source_ip_address_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON failed [prose_source_ip_address]");
        goto end;
    }
    }

    prose_source_l2_id = cJSON_GetObjectItemCaseSensitive(transmitter_infoJSON, "proseSourceL2Id");
    if (prose_source_l2_id) {
    if (!cJSON_IsString(prose_source_l2_id) && !cJSON_IsNull(prose_source_l2_id)) {
        ogs_error("OpenAPI_transmitter_info_parseFromJSON() failed [prose_source_l2_id]");
        goto end;
    }
    }

    transmitter_info_local_var = OpenAPI_transmitter_info_create (
        prose_source_ip_address ? prose_source_ip_address_local_nonprim : NULL,
        prose_source_l2_id && !cJSON_IsNull(prose_source_l2_id) ? ogs_strdup(prose_source_l2_id->valuestring) : NULL
    );

    return transmitter_info_local_var;
end:
    if (prose_source_ip_address_local_nonprim) {
        OpenAPI_ip_addr_free(prose_source_ip_address_local_nonprim);
        prose_source_ip_address_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_transmitter_info_t *OpenAPI_transmitter_info_copy(OpenAPI_transmitter_info_t *dst, OpenAPI_transmitter_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_transmitter_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_transmitter_info_convertToJSON() failed");
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

    OpenAPI_transmitter_info_free(dst);
    dst = OpenAPI_transmitter_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "non3gpp_device_info.h"

OpenAPI_non3gpp_device_info_t *OpenAPI_non3gpp_device_info_create(
    char *n3g_dev_id,
    OpenAPI_user_plane_address_t *n3g_dev_addr
)
{
    OpenAPI_non3gpp_device_info_t *non3gpp_device_info_local_var = ogs_malloc(sizeof(OpenAPI_non3gpp_device_info_t));
    ogs_assert(non3gpp_device_info_local_var);

    non3gpp_device_info_local_var->n3g_dev_id = n3g_dev_id;
    non3gpp_device_info_local_var->n3g_dev_addr = n3g_dev_addr;

    return non3gpp_device_info_local_var;
}

void OpenAPI_non3gpp_device_info_free(OpenAPI_non3gpp_device_info_t *non3gpp_device_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == non3gpp_device_info) {
        return;
    }
    if (non3gpp_device_info->n3g_dev_id) {
        ogs_free(non3gpp_device_info->n3g_dev_id);
        non3gpp_device_info->n3g_dev_id = NULL;
    }
    if (non3gpp_device_info->n3g_dev_addr) {
        OpenAPI_user_plane_address_free(non3gpp_device_info->n3g_dev_addr);
        non3gpp_device_info->n3g_dev_addr = NULL;
    }
    ogs_free(non3gpp_device_info);
}

cJSON *OpenAPI_non3gpp_device_info_convertToJSON(OpenAPI_non3gpp_device_info_t *non3gpp_device_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (non3gpp_device_info == NULL) {
        ogs_error("OpenAPI_non3gpp_device_info_convertToJSON() failed [Non3gppDeviceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!non3gpp_device_info->n3g_dev_id) {
        ogs_error("OpenAPI_non3gpp_device_info_convertToJSON() failed [n3g_dev_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "n3gDevId", non3gpp_device_info->n3g_dev_id) == NULL) {
        ogs_error("OpenAPI_non3gpp_device_info_convertToJSON() failed [n3g_dev_id]");
        goto end;
    }

    if (!non3gpp_device_info->n3g_dev_addr) {
        ogs_error("OpenAPI_non3gpp_device_info_convertToJSON() failed [n3g_dev_addr]");
        return NULL;
    }
    cJSON *n3g_dev_addr_local_JSON = OpenAPI_user_plane_address_convertToJSON(non3gpp_device_info->n3g_dev_addr);
    if (n3g_dev_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_non3gpp_device_info_convertToJSON() failed [n3g_dev_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n3gDevAddr", n3g_dev_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_non3gpp_device_info_convertToJSON() failed [n3g_dev_addr]");
        goto end;
    }

end:
    return item;
}

OpenAPI_non3gpp_device_info_t *OpenAPI_non3gpp_device_info_parseFromJSON(cJSON *non3gpp_device_infoJSON)
{
    OpenAPI_non3gpp_device_info_t *non3gpp_device_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n3g_dev_id = NULL;
    cJSON *n3g_dev_addr = NULL;
    OpenAPI_user_plane_address_t *n3g_dev_addr_local_nonprim = NULL;
    n3g_dev_id = cJSON_GetObjectItemCaseSensitive(non3gpp_device_infoJSON, "n3gDevId");
    if (!n3g_dev_id) {
        ogs_error("OpenAPI_non3gpp_device_info_parseFromJSON() failed [n3g_dev_id]");
        goto end;
    }
    if (!cJSON_IsString(n3g_dev_id)) {
        ogs_error("OpenAPI_non3gpp_device_info_parseFromJSON() failed [n3g_dev_id]");
        goto end;
    }

    n3g_dev_addr = cJSON_GetObjectItemCaseSensitive(non3gpp_device_infoJSON, "n3gDevAddr");
    if (!n3g_dev_addr) {
        ogs_error("OpenAPI_non3gpp_device_info_parseFromJSON() failed [n3g_dev_addr]");
        goto end;
    }
    n3g_dev_addr_local_nonprim = OpenAPI_user_plane_address_parseFromJSON(n3g_dev_addr);
    if (!n3g_dev_addr_local_nonprim) {
        ogs_error("OpenAPI_user_plane_address_parseFromJSON failed [n3g_dev_addr]");
        goto end;
    }

    non3gpp_device_info_local_var = OpenAPI_non3gpp_device_info_create (
        ogs_strdup(n3g_dev_id->valuestring),
        n3g_dev_addr_local_nonprim
    );

    return non3gpp_device_info_local_var;
end:
    if (n3g_dev_addr_local_nonprim) {
        OpenAPI_user_plane_address_free(n3g_dev_addr_local_nonprim);
        n3g_dev_addr_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_non3gpp_device_info_t *OpenAPI_non3gpp_device_info_copy(OpenAPI_non3gpp_device_info_t *dst, OpenAPI_non3gpp_device_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_non3gpp_device_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_non3gpp_device_info_convertToJSON() failed");
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

    OpenAPI_non3gpp_device_info_free(dst);
    dst = OpenAPI_non3gpp_device_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


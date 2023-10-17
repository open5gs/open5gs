
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "upf_information.h"

OpenAPI_upf_information_t *OpenAPI_upf_information_create(
    char *upf_id,
    OpenAPI_addr_fqdn_t *upf_addr
)
{
    OpenAPI_upf_information_t *upf_information_local_var = ogs_malloc(sizeof(OpenAPI_upf_information_t));
    ogs_assert(upf_information_local_var);

    upf_information_local_var->upf_id = upf_id;
    upf_information_local_var->upf_addr = upf_addr;

    return upf_information_local_var;
}

void OpenAPI_upf_information_free(OpenAPI_upf_information_t *upf_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == upf_information) {
        return;
    }
    if (upf_information->upf_id) {
        ogs_free(upf_information->upf_id);
        upf_information->upf_id = NULL;
    }
    if (upf_information->upf_addr) {
        OpenAPI_addr_fqdn_free(upf_information->upf_addr);
        upf_information->upf_addr = NULL;
    }
    ogs_free(upf_information);
}

cJSON *OpenAPI_upf_information_convertToJSON(OpenAPI_upf_information_t *upf_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (upf_information == NULL) {
        ogs_error("OpenAPI_upf_information_convertToJSON() failed [UpfInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (upf_information->upf_id) {
    if (cJSON_AddStringToObject(item, "upfId", upf_information->upf_id) == NULL) {
        ogs_error("OpenAPI_upf_information_convertToJSON() failed [upf_id]");
        goto end;
    }
    }

    if (upf_information->upf_addr) {
    cJSON *upf_addr_local_JSON = OpenAPI_addr_fqdn_convertToJSON(upf_information->upf_addr);
    if (upf_addr_local_JSON == NULL) {
        ogs_error("OpenAPI_upf_information_convertToJSON() failed [upf_addr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "upfAddr", upf_addr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_upf_information_convertToJSON() failed [upf_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_upf_information_t *OpenAPI_upf_information_parseFromJSON(cJSON *upf_informationJSON)
{
    OpenAPI_upf_information_t *upf_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *upf_id = NULL;
    cJSON *upf_addr = NULL;
    OpenAPI_addr_fqdn_t *upf_addr_local_nonprim = NULL;
    upf_id = cJSON_GetObjectItemCaseSensitive(upf_informationJSON, "upfId");
    if (upf_id) {
    if (!cJSON_IsString(upf_id) && !cJSON_IsNull(upf_id)) {
        ogs_error("OpenAPI_upf_information_parseFromJSON() failed [upf_id]");
        goto end;
    }
    }

    upf_addr = cJSON_GetObjectItemCaseSensitive(upf_informationJSON, "upfAddr");
    if (upf_addr) {
    upf_addr_local_nonprim = OpenAPI_addr_fqdn_parseFromJSON(upf_addr);
    if (!upf_addr_local_nonprim) {
        ogs_error("OpenAPI_addr_fqdn_parseFromJSON failed [upf_addr]");
        goto end;
    }
    }

    upf_information_local_var = OpenAPI_upf_information_create (
        upf_id && !cJSON_IsNull(upf_id) ? ogs_strdup(upf_id->valuestring) : NULL,
        upf_addr ? upf_addr_local_nonprim : NULL
    );

    return upf_information_local_var;
end:
    if (upf_addr_local_nonprim) {
        OpenAPI_addr_fqdn_free(upf_addr_local_nonprim);
        upf_addr_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_upf_information_t *OpenAPI_upf_information_copy(OpenAPI_upf_information_t *dst, OpenAPI_upf_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_upf_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_upf_information_convertToJSON() failed");
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

    OpenAPI_upf_information_free(dst);
    dst = OpenAPI_upf_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


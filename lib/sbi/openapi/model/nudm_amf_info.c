
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nudm_amf_info.h"

OpenAPI_nudm_amf_info_t *OpenAPI_nudm_amf_info_create(
    char *amf_instance_id,
    OpenAPI_guami_t *guami,
    OpenAPI_access_type_e access_type
)
{
    OpenAPI_nudm_amf_info_t *nudm_amf_info_local_var = ogs_malloc(sizeof(OpenAPI_nudm_amf_info_t));
    ogs_assert(nudm_amf_info_local_var);

    nudm_amf_info_local_var->amf_instance_id = amf_instance_id;
    nudm_amf_info_local_var->guami = guami;
    nudm_amf_info_local_var->access_type = access_type;

    return nudm_amf_info_local_var;
}

void OpenAPI_nudm_amf_info_free(OpenAPI_nudm_amf_info_t *nudm_amf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nudm_amf_info) {
        return;
    }
    if (nudm_amf_info->amf_instance_id) {
        ogs_free(nudm_amf_info->amf_instance_id);
        nudm_amf_info->amf_instance_id = NULL;
    }
    if (nudm_amf_info->guami) {
        OpenAPI_guami_free(nudm_amf_info->guami);
        nudm_amf_info->guami = NULL;
    }
    ogs_free(nudm_amf_info);
}

cJSON *OpenAPI_nudm_amf_info_convertToJSON(OpenAPI_nudm_amf_info_t *nudm_amf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nudm_amf_info == NULL) {
        ogs_error("OpenAPI_nudm_amf_info_convertToJSON() failed [NudmAmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nudm_amf_info->amf_instance_id) {
        ogs_error("OpenAPI_nudm_amf_info_convertToJSON() failed [amf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfInstanceId", nudm_amf_info->amf_instance_id) == NULL) {
        ogs_error("OpenAPI_nudm_amf_info_convertToJSON() failed [amf_instance_id]");
        goto end;
    }

    if (!nudm_amf_info->guami) {
        ogs_error("OpenAPI_nudm_amf_info_convertToJSON() failed [guami]");
        return NULL;
    }
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(nudm_amf_info->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_nudm_amf_info_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nudm_amf_info_convertToJSON() failed [guami]");
        goto end;
    }

    if (nudm_amf_info->access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(nudm_amf_info->access_type)) == NULL) {
        ogs_error("OpenAPI_nudm_amf_info_convertToJSON() failed [access_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nudm_amf_info_t *OpenAPI_nudm_amf_info_parseFromJSON(cJSON *nudm_amf_infoJSON)
{
    OpenAPI_nudm_amf_info_t *nudm_amf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *amf_instance_id = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    amf_instance_id = cJSON_GetObjectItemCaseSensitive(nudm_amf_infoJSON, "amfInstanceId");
    if (!amf_instance_id) {
        ogs_error("OpenAPI_nudm_amf_info_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_instance_id)) {
        ogs_error("OpenAPI_nudm_amf_info_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }

    guami = cJSON_GetObjectItemCaseSensitive(nudm_amf_infoJSON, "guami");
    if (!guami) {
        ogs_error("OpenAPI_nudm_amf_info_parseFromJSON() failed [guami]");
        goto end;
    }
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }

    access_type = cJSON_GetObjectItemCaseSensitive(nudm_amf_infoJSON, "accessType");
    if (access_type) {
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_nudm_amf_info_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);
    }

    nudm_amf_info_local_var = OpenAPI_nudm_amf_info_create (
        ogs_strdup(amf_instance_id->valuestring),
        guami_local_nonprim,
        access_type ? access_typeVariable : 0
    );

    return nudm_amf_info_local_var;
end:
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nudm_amf_info_t *OpenAPI_nudm_amf_info_copy(OpenAPI_nudm_amf_info_t *dst, OpenAPI_nudm_amf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nudm_amf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nudm_amf_info_convertToJSON() failed");
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

    OpenAPI_nudm_amf_info_free(dst);
    dst = OpenAPI_nudm_amf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


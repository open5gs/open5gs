
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "inter_chf_information.h"

OpenAPI_inter_chf_information_t *OpenAPI_inter_chf_information_create(
    char *remote_chf_resource,
    OpenAPI_nf_identification_t *original_nf_consumer_id
)
{
    OpenAPI_inter_chf_information_t *inter_chf_information_local_var = ogs_malloc(sizeof(OpenAPI_inter_chf_information_t));
    ogs_assert(inter_chf_information_local_var);

    inter_chf_information_local_var->remote_chf_resource = remote_chf_resource;
    inter_chf_information_local_var->original_nf_consumer_id = original_nf_consumer_id;

    return inter_chf_information_local_var;
}

void OpenAPI_inter_chf_information_free(OpenAPI_inter_chf_information_t *inter_chf_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == inter_chf_information) {
        return;
    }
    if (inter_chf_information->remote_chf_resource) {
        ogs_free(inter_chf_information->remote_chf_resource);
        inter_chf_information->remote_chf_resource = NULL;
    }
    if (inter_chf_information->original_nf_consumer_id) {
        OpenAPI_nf_identification_free(inter_chf_information->original_nf_consumer_id);
        inter_chf_information->original_nf_consumer_id = NULL;
    }
    ogs_free(inter_chf_information);
}

cJSON *OpenAPI_inter_chf_information_convertToJSON(OpenAPI_inter_chf_information_t *inter_chf_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (inter_chf_information == NULL) {
        ogs_error("OpenAPI_inter_chf_information_convertToJSON() failed [InterCHFInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (inter_chf_information->remote_chf_resource) {
    if (cJSON_AddStringToObject(item, "remoteCHFResource", inter_chf_information->remote_chf_resource) == NULL) {
        ogs_error("OpenAPI_inter_chf_information_convertToJSON() failed [remote_chf_resource]");
        goto end;
    }
    }

    if (inter_chf_information->original_nf_consumer_id) {
    cJSON *original_nf_consumer_id_local_JSON = OpenAPI_nf_identification_convertToJSON(inter_chf_information->original_nf_consumer_id);
    if (original_nf_consumer_id_local_JSON == NULL) {
        ogs_error("OpenAPI_inter_chf_information_convertToJSON() failed [original_nf_consumer_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "originalNFConsumerId", original_nf_consumer_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_inter_chf_information_convertToJSON() failed [original_nf_consumer_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_inter_chf_information_t *OpenAPI_inter_chf_information_parseFromJSON(cJSON *inter_chf_informationJSON)
{
    OpenAPI_inter_chf_information_t *inter_chf_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *remote_chf_resource = NULL;
    cJSON *original_nf_consumer_id = NULL;
    OpenAPI_nf_identification_t *original_nf_consumer_id_local_nonprim = NULL;
    remote_chf_resource = cJSON_GetObjectItemCaseSensitive(inter_chf_informationJSON, "remoteCHFResource");
    if (remote_chf_resource) {
    if (!cJSON_IsString(remote_chf_resource) && !cJSON_IsNull(remote_chf_resource)) {
        ogs_error("OpenAPI_inter_chf_information_parseFromJSON() failed [remote_chf_resource]");
        goto end;
    }
    }

    original_nf_consumer_id = cJSON_GetObjectItemCaseSensitive(inter_chf_informationJSON, "originalNFConsumerId");
    if (original_nf_consumer_id) {
    original_nf_consumer_id_local_nonprim = OpenAPI_nf_identification_parseFromJSON(original_nf_consumer_id);
    if (!original_nf_consumer_id_local_nonprim) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON failed [original_nf_consumer_id]");
        goto end;
    }
    }

    inter_chf_information_local_var = OpenAPI_inter_chf_information_create (
        remote_chf_resource && !cJSON_IsNull(remote_chf_resource) ? ogs_strdup(remote_chf_resource->valuestring) : NULL,
        original_nf_consumer_id ? original_nf_consumer_id_local_nonprim : NULL
    );

    return inter_chf_information_local_var;
end:
    if (original_nf_consumer_id_local_nonprim) {
        OpenAPI_nf_identification_free(original_nf_consumer_id_local_nonprim);
        original_nf_consumer_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_inter_chf_information_t *OpenAPI_inter_chf_information_copy(OpenAPI_inter_chf_information_t *dst, OpenAPI_inter_chf_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_inter_chf_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_inter_chf_information_convertToJSON() failed");
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

    OpenAPI_inter_chf_information_free(dst);
    dst = OpenAPI_inter_chf_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


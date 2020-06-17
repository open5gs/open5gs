
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_message_container.h"

OpenAPI_n1_message_container_t *OpenAPI_n1_message_container_create(
    OpenAPI_n1_message_class_e n1_message_class,
    OpenAPI_ref_to_binary_data_t *n1_message_content,
    char *nf_id,
    char *service_instance_id
    )
{
    OpenAPI_n1_message_container_t *n1_message_container_local_var = OpenAPI_malloc(sizeof(OpenAPI_n1_message_container_t));
    if (!n1_message_container_local_var) {
        return NULL;
    }
    n1_message_container_local_var->n1_message_class = n1_message_class;
    n1_message_container_local_var->n1_message_content = n1_message_content;
    n1_message_container_local_var->nf_id = nf_id;
    n1_message_container_local_var->service_instance_id = service_instance_id;

    return n1_message_container_local_var;
}

void OpenAPI_n1_message_container_free(OpenAPI_n1_message_container_t *n1_message_container)
{
    if (NULL == n1_message_container) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ref_to_binary_data_free(n1_message_container->n1_message_content);
    ogs_free(n1_message_container->nf_id);
    ogs_free(n1_message_container->service_instance_id);
    ogs_free(n1_message_container);
}

cJSON *OpenAPI_n1_message_container_convertToJSON(OpenAPI_n1_message_container_t *n1_message_container)
{
    cJSON *item = NULL;

    if (n1_message_container == NULL) {
        ogs_error("OpenAPI_n1_message_container_convertToJSON() failed [N1MessageContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n1_message_container->n1_message_class) {
        ogs_error("OpenAPI_n1_message_container_convertToJSON() failed [n1_message_class]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "n1MessageClass", OpenAPI_n1_message_class_ToString(n1_message_container->n1_message_class)) == NULL) {
        ogs_error("OpenAPI_n1_message_container_convertToJSON() failed [n1_message_class]");
        goto end;
    }

    if (!n1_message_container->n1_message_content) {
        ogs_error("OpenAPI_n1_message_container_convertToJSON() failed [n1_message_content]");
        goto end;
    }
    cJSON *n1_message_content_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(n1_message_container->n1_message_content);
    if (n1_message_content_local_JSON == NULL) {
        ogs_error("OpenAPI_n1_message_container_convertToJSON() failed [n1_message_content]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1MessageContent", n1_message_content_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n1_message_container_convertToJSON() failed [n1_message_content]");
        goto end;
    }

    if (n1_message_container->nf_id) {
        if (cJSON_AddStringToObject(item, "nfId", n1_message_container->nf_id) == NULL) {
            ogs_error("OpenAPI_n1_message_container_convertToJSON() failed [nf_id]");
            goto end;
        }
    }

    if (n1_message_container->service_instance_id) {
        if (cJSON_AddStringToObject(item, "serviceInstanceId", n1_message_container->service_instance_id) == NULL) {
            ogs_error("OpenAPI_n1_message_container_convertToJSON() failed [service_instance_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_n1_message_container_t *OpenAPI_n1_message_container_parseFromJSON(cJSON *n1_message_containerJSON)
{
    OpenAPI_n1_message_container_t *n1_message_container_local_var = NULL;
    cJSON *n1_message_class = cJSON_GetObjectItemCaseSensitive(n1_message_containerJSON, "n1MessageClass");
    if (!n1_message_class) {
        ogs_error("OpenAPI_n1_message_container_parseFromJSON() failed [n1_message_class]");
        goto end;
    }

    OpenAPI_n1_message_class_e n1_message_classVariable;

    if (!cJSON_IsString(n1_message_class)) {
        ogs_error("OpenAPI_n1_message_container_parseFromJSON() failed [n1_message_class]");
        goto end;
    }
    n1_message_classVariable = OpenAPI_n1_message_class_FromString(n1_message_class->valuestring);

    cJSON *n1_message_content = cJSON_GetObjectItemCaseSensitive(n1_message_containerJSON, "n1MessageContent");
    if (!n1_message_content) {
        ogs_error("OpenAPI_n1_message_container_parseFromJSON() failed [n1_message_content]");
        goto end;
    }

    OpenAPI_ref_to_binary_data_t *n1_message_content_local_nonprim = NULL;

    n1_message_content_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_message_content);

    cJSON *nf_id = cJSON_GetObjectItemCaseSensitive(n1_message_containerJSON, "nfId");

    if (nf_id) {
        if (!cJSON_IsString(nf_id)) {
            ogs_error("OpenAPI_n1_message_container_parseFromJSON() failed [nf_id]");
            goto end;
        }
    }

    cJSON *service_instance_id = cJSON_GetObjectItemCaseSensitive(n1_message_containerJSON, "serviceInstanceId");

    if (service_instance_id) {
        if (!cJSON_IsString(service_instance_id)) {
            ogs_error("OpenAPI_n1_message_container_parseFromJSON() failed [service_instance_id]");
            goto end;
        }
    }

    n1_message_container_local_var = OpenAPI_n1_message_container_create (
        n1_message_classVariable,
        n1_message_content_local_nonprim,
        nf_id ? ogs_strdup(nf_id->valuestring) : NULL,
        service_instance_id ? ogs_strdup(service_instance_id->valuestring) : NULL
        );

    return n1_message_container_local_var;
end:
    return NULL;
}

OpenAPI_n1_message_container_t *OpenAPI_n1_message_container_copy(OpenAPI_n1_message_container_t *dst, OpenAPI_n1_message_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n1_message_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n1_message_container_convertToJSON() failed");
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

    OpenAPI_n1_message_container_free(dst);
    dst = OpenAPI_n1_message_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


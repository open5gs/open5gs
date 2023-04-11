
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_info_content.h"

OpenAPI_n2_info_content_t *OpenAPI_n2_info_content_create(
    bool is_ngap_message_type,
    int ngap_message_type,
    OpenAPI_ngap_ie_type_e ngap_ie_type,
    OpenAPI_ref_to_binary_data_t *ngap_data
)
{
    OpenAPI_n2_info_content_t *n2_info_content_local_var = ogs_malloc(sizeof(OpenAPI_n2_info_content_t));
    ogs_assert(n2_info_content_local_var);

    n2_info_content_local_var->is_ngap_message_type = is_ngap_message_type;
    n2_info_content_local_var->ngap_message_type = ngap_message_type;
    n2_info_content_local_var->ngap_ie_type = ngap_ie_type;
    n2_info_content_local_var->ngap_data = ngap_data;

    return n2_info_content_local_var;
}

void OpenAPI_n2_info_content_free(OpenAPI_n2_info_content_t *n2_info_content)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_info_content) {
        return;
    }
    if (n2_info_content->ngap_data) {
        OpenAPI_ref_to_binary_data_free(n2_info_content->ngap_data);
        n2_info_content->ngap_data = NULL;
    }
    ogs_free(n2_info_content);
}

cJSON *OpenAPI_n2_info_content_convertToJSON(OpenAPI_n2_info_content_t *n2_info_content)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_info_content == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [N2InfoContent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n2_info_content->is_ngap_message_type) {
    if (cJSON_AddNumberToObject(item, "ngapMessageType", n2_info_content->ngap_message_type) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_message_type]");
        goto end;
    }
    }

    if (n2_info_content->ngap_ie_type != OpenAPI_ngap_ie_type_NULL) {
    if (cJSON_AddStringToObject(item, "ngapIeType", OpenAPI_ngap_ie_type_ToString(n2_info_content->ngap_ie_type)) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_ie_type]");
        goto end;
    }
    }

    if (!n2_info_content->ngap_data) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_data]");
        return NULL;
    }
    cJSON *ngap_data_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(n2_info_content->ngap_data);
    if (ngap_data_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngapData", ngap_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_data]");
        goto end;
    }

end:
    return item;
}

OpenAPI_n2_info_content_t *OpenAPI_n2_info_content_parseFromJSON(cJSON *n2_info_contentJSON)
{
    OpenAPI_n2_info_content_t *n2_info_content_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ngap_message_type = NULL;
    cJSON *ngap_ie_type = NULL;
    OpenAPI_ngap_ie_type_e ngap_ie_typeVariable = 0;
    cJSON *ngap_data = NULL;
    OpenAPI_ref_to_binary_data_t *ngap_data_local_nonprim = NULL;
    ngap_message_type = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "ngapMessageType");
    if (ngap_message_type) {
    if (!cJSON_IsNumber(ngap_message_type)) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON() failed [ngap_message_type]");
        goto end;
    }
    }

    ngap_ie_type = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "ngapIeType");
    if (ngap_ie_type) {
    if (!cJSON_IsString(ngap_ie_type)) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON() failed [ngap_ie_type]");
        goto end;
    }
    ngap_ie_typeVariable = OpenAPI_ngap_ie_type_FromString(ngap_ie_type->valuestring);
    }

    ngap_data = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "ngapData");
    if (!ngap_data) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON() failed [ngap_data]");
        goto end;
    }
    ngap_data_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(ngap_data);
    if (!ngap_data_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [ngap_data]");
        goto end;
    }

    n2_info_content_local_var = OpenAPI_n2_info_content_create (
        ngap_message_type ? true : false,
        ngap_message_type ? ngap_message_type->valuedouble : 0,
        ngap_ie_type ? ngap_ie_typeVariable : 0,
        ngap_data_local_nonprim
    );

    return n2_info_content_local_var;
end:
    if (ngap_data_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(ngap_data_local_nonprim);
        ngap_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_n2_info_content_t *OpenAPI_n2_info_content_copy(OpenAPI_n2_info_content_t *dst, OpenAPI_n2_info_content_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_info_content_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed");
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

    OpenAPI_n2_info_content_free(dst);
    dst = OpenAPI_n2_info_content_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


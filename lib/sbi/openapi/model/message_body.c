
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "message_body.h"

OpenAPI_message_body_t *OpenAPI_message_body_create(
    char *content_type,
    int content_length,
    char *content_disposition,
    OpenAPI_originator_party_type_e originator
)
{
    OpenAPI_message_body_t *message_body_local_var = ogs_malloc(sizeof(OpenAPI_message_body_t));
    ogs_assert(message_body_local_var);

    message_body_local_var->content_type = content_type;
    message_body_local_var->content_length = content_length;
    message_body_local_var->content_disposition = content_disposition;
    message_body_local_var->originator = originator;

    return message_body_local_var;
}

void OpenAPI_message_body_free(OpenAPI_message_body_t *message_body)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == message_body) {
        return;
    }
    if (message_body->content_type) {
        ogs_free(message_body->content_type);
        message_body->content_type = NULL;
    }
    if (message_body->content_disposition) {
        ogs_free(message_body->content_disposition);
        message_body->content_disposition = NULL;
    }
    ogs_free(message_body);
}

cJSON *OpenAPI_message_body_convertToJSON(OpenAPI_message_body_t *message_body)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (message_body == NULL) {
        ogs_error("OpenAPI_message_body_convertToJSON() failed [MessageBody]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!message_body->content_type) {
        ogs_error("OpenAPI_message_body_convertToJSON() failed [content_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "contentType", message_body->content_type) == NULL) {
        ogs_error("OpenAPI_message_body_convertToJSON() failed [content_type]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "contentLength", message_body->content_length) == NULL) {
        ogs_error("OpenAPI_message_body_convertToJSON() failed [content_length]");
        goto end;
    }

    if (message_body->content_disposition) {
    if (cJSON_AddStringToObject(item, "contentDisposition", message_body->content_disposition) == NULL) {
        ogs_error("OpenAPI_message_body_convertToJSON() failed [content_disposition]");
        goto end;
    }
    }

    if (message_body->originator != OpenAPI_originator_party_type_NULL) {
    if (cJSON_AddStringToObject(item, "originator", OpenAPI_originator_party_type_ToString(message_body->originator)) == NULL) {
        ogs_error("OpenAPI_message_body_convertToJSON() failed [originator]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_message_body_t *OpenAPI_message_body_parseFromJSON(cJSON *message_bodyJSON)
{
    OpenAPI_message_body_t *message_body_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *content_type = NULL;
    cJSON *content_length = NULL;
    cJSON *content_disposition = NULL;
    cJSON *originator = NULL;
    OpenAPI_originator_party_type_e originatorVariable = 0;
    content_type = cJSON_GetObjectItemCaseSensitive(message_bodyJSON, "contentType");
    if (!content_type) {
        ogs_error("OpenAPI_message_body_parseFromJSON() failed [content_type]");
        goto end;
    }
    if (!cJSON_IsString(content_type)) {
        ogs_error("OpenAPI_message_body_parseFromJSON() failed [content_type]");
        goto end;
    }

    content_length = cJSON_GetObjectItemCaseSensitive(message_bodyJSON, "contentLength");
    if (!content_length) {
        ogs_error("OpenAPI_message_body_parseFromJSON() failed [content_length]");
        goto end;
    }
    if (!cJSON_IsNumber(content_length)) {
        ogs_error("OpenAPI_message_body_parseFromJSON() failed [content_length]");
        goto end;
    }

    content_disposition = cJSON_GetObjectItemCaseSensitive(message_bodyJSON, "contentDisposition");
    if (content_disposition) {
    if (!cJSON_IsString(content_disposition) && !cJSON_IsNull(content_disposition)) {
        ogs_error("OpenAPI_message_body_parseFromJSON() failed [content_disposition]");
        goto end;
    }
    }

    originator = cJSON_GetObjectItemCaseSensitive(message_bodyJSON, "originator");
    if (originator) {
    if (!cJSON_IsString(originator)) {
        ogs_error("OpenAPI_message_body_parseFromJSON() failed [originator]");
        goto end;
    }
    originatorVariable = OpenAPI_originator_party_type_FromString(originator->valuestring);
    }

    message_body_local_var = OpenAPI_message_body_create (
        ogs_strdup(content_type->valuestring),
        
        content_length->valuedouble,
        content_disposition && !cJSON_IsNull(content_disposition) ? ogs_strdup(content_disposition->valuestring) : NULL,
        originator ? originatorVariable : 0
    );

    return message_body_local_var;
end:
    return NULL;
}

OpenAPI_message_body_t *OpenAPI_message_body_copy(OpenAPI_message_body_t *dst, OpenAPI_message_body_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_message_body_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_message_body_convertToJSON() failed");
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

    OpenAPI_message_body_free(dst);
    dst = OpenAPI_message_body_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


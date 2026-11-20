
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "message_class.h"

OpenAPI_message_class_t *OpenAPI_message_class_create(
    OpenAPI_class_identifier_e class_identifier,
    char *token_text
)
{
    OpenAPI_message_class_t *message_class_local_var = ogs_malloc(sizeof(OpenAPI_message_class_t));
    ogs_assert(message_class_local_var);

    message_class_local_var->class_identifier = class_identifier;
    message_class_local_var->token_text = token_text;

    return message_class_local_var;
}

void OpenAPI_message_class_free(OpenAPI_message_class_t *message_class)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == message_class) {
        return;
    }
    if (message_class->token_text) {
        ogs_free(message_class->token_text);
        message_class->token_text = NULL;
    }
    ogs_free(message_class);
}

cJSON *OpenAPI_message_class_convertToJSON(OpenAPI_message_class_t *message_class)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (message_class == NULL) {
        ogs_error("OpenAPI_message_class_convertToJSON() failed [MessageClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (message_class->class_identifier != OpenAPI_class_identifier_NULL) {
    if (cJSON_AddStringToObject(item, "classIdentifier", OpenAPI_class_identifier_ToString(message_class->class_identifier)) == NULL) {
        ogs_error("OpenAPI_message_class_convertToJSON() failed [class_identifier]");
        goto end;
    }
    }

    if (message_class->token_text) {
    if (cJSON_AddStringToObject(item, "tokenText", message_class->token_text) == NULL) {
        ogs_error("OpenAPI_message_class_convertToJSON() failed [token_text]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_message_class_t *OpenAPI_message_class_parseFromJSON(cJSON *message_classJSON)
{
    OpenAPI_message_class_t *message_class_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *class_identifier = NULL;
    OpenAPI_class_identifier_e class_identifierVariable = 0;
    cJSON *token_text = NULL;
    class_identifier = cJSON_GetObjectItemCaseSensitive(message_classJSON, "classIdentifier");
    if (class_identifier) {
    if (!cJSON_IsString(class_identifier)) {
        ogs_error("OpenAPI_message_class_parseFromJSON() failed [class_identifier]");
        goto end;
    }
    class_identifierVariable = OpenAPI_class_identifier_FromString(class_identifier->valuestring);
    }

    token_text = cJSON_GetObjectItemCaseSensitive(message_classJSON, "tokenText");
    if (token_text) {
    if (!cJSON_IsString(token_text) && !cJSON_IsNull(token_text)) {
        ogs_error("OpenAPI_message_class_parseFromJSON() failed [token_text]");
        goto end;
    }
    }

    message_class_local_var = OpenAPI_message_class_create (
        class_identifier ? class_identifierVariable : 0,
        token_text && !cJSON_IsNull(token_text) ? ogs_strdup(token_text->valuestring) : NULL
    );

    return message_class_local_var;
end:
    return NULL;
}

OpenAPI_message_class_t *OpenAPI_message_class_copy(OpenAPI_message_class_t *dst, OpenAPI_message_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_message_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_message_class_convertToJSON() failed");
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

    OpenAPI_message_class_free(dst);
    dst = OpenAPI_message_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


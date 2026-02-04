
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_report_message.h"

OpenAPI_event_report_message_t *OpenAPI_event_report_message_create(
    OpenAPI_event_class_t *event_class,
    OpenAPI_ref_to_binary_data_t *event_content
)
{
    OpenAPI_event_report_message_t *event_report_message_local_var = ogs_malloc(sizeof(OpenAPI_event_report_message_t));
    ogs_assert(event_report_message_local_var);

    event_report_message_local_var->event_class = event_class;
    event_report_message_local_var->event_content = event_content;

    return event_report_message_local_var;
}

void OpenAPI_event_report_message_free(OpenAPI_event_report_message_t *event_report_message)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == event_report_message) {
        return;
    }
    if (event_report_message->event_class) {
        OpenAPI_event_class_free(event_report_message->event_class);
        event_report_message->event_class = NULL;
    }
    if (event_report_message->event_content) {
        OpenAPI_ref_to_binary_data_free(event_report_message->event_content);
        event_report_message->event_content = NULL;
    }
    ogs_free(event_report_message);
}

cJSON *OpenAPI_event_report_message_convertToJSON(OpenAPI_event_report_message_t *event_report_message)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (event_report_message == NULL) {
        ogs_error("OpenAPI_event_report_message_convertToJSON() failed [EventReportMessage]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!event_report_message->event_class) {
        ogs_error("OpenAPI_event_report_message_convertToJSON() failed [event_class]");
        return NULL;
    }
    cJSON *event_class_local_JSON = OpenAPI_event_class_convertToJSON(event_report_message->event_class);
    if (event_class_local_JSON == NULL) {
        ogs_error("OpenAPI_event_report_message_convertToJSON() failed [event_class]");
        goto end;
    }
    cJSON_AddItemToObject(item, "eventClass", event_class_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_report_message_convertToJSON() failed [event_class]");
        goto end;
    }

    if (!event_report_message->event_content) {
        ogs_error("OpenAPI_event_report_message_convertToJSON() failed [event_content]");
        return NULL;
    }
    cJSON *event_content_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(event_report_message->event_content);
    if (event_content_local_JSON == NULL) {
        ogs_error("OpenAPI_event_report_message_convertToJSON() failed [event_content]");
        goto end;
    }
    cJSON_AddItemToObject(item, "eventContent", event_content_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_report_message_convertToJSON() failed [event_content]");
        goto end;
    }

end:
    return item;
}

OpenAPI_event_report_message_t *OpenAPI_event_report_message_parseFromJSON(cJSON *event_report_messageJSON)
{
    OpenAPI_event_report_message_t *event_report_message_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_class = NULL;
    OpenAPI_event_class_t *event_class_local_nonprim = NULL;
    cJSON *event_content = NULL;
    OpenAPI_ref_to_binary_data_t *event_content_local_nonprim = NULL;
    event_class = cJSON_GetObjectItemCaseSensitive(event_report_messageJSON, "eventClass");
    if (!event_class) {
        ogs_error("OpenAPI_event_report_message_parseFromJSON() failed [event_class]");
        goto end;
    }
    event_class_local_nonprim = OpenAPI_event_class_parseFromJSON(event_class);
    if (!event_class_local_nonprim) {
        ogs_error("OpenAPI_event_class_parseFromJSON failed [event_class]");
        goto end;
    }

    event_content = cJSON_GetObjectItemCaseSensitive(event_report_messageJSON, "eventContent");
    if (!event_content) {
        ogs_error("OpenAPI_event_report_message_parseFromJSON() failed [event_content]");
        goto end;
    }
    event_content_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(event_content);
    if (!event_content_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [event_content]");
        goto end;
    }

    event_report_message_local_var = OpenAPI_event_report_message_create (
        event_class_local_nonprim,
        event_content_local_nonprim
    );

    return event_report_message_local_var;
end:
    if (event_class_local_nonprim) {
        OpenAPI_event_class_free(event_class_local_nonprim);
        event_class_local_nonprim = NULL;
    }
    if (event_content_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(event_content_local_nonprim);
        event_content_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_event_report_message_t *OpenAPI_event_report_message_copy(OpenAPI_event_report_message_t *dst, OpenAPI_event_report_message_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_event_report_message_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_event_report_message_convertToJSON() failed");
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

    OpenAPI_event_report_message_free(dst);
    dst = OpenAPI_event_report_message_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_info_notification_rsp_data.h"

OpenAPI_n2_info_notification_rsp_data_t *OpenAPI_n2_info_notification_rsp_data_create(
    OpenAPI_n2_info_content_t *n2_info_content
)
{
    OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data_local_var = ogs_malloc(sizeof(OpenAPI_n2_info_notification_rsp_data_t));
    ogs_assert(n2_info_notification_rsp_data_local_var);

    n2_info_notification_rsp_data_local_var->n2_info_content = n2_info_content;

    return n2_info_notification_rsp_data_local_var;
}

void OpenAPI_n2_info_notification_rsp_data_free(OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data)
{
    if (NULL == n2_info_notification_rsp_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_n2_info_content_free(n2_info_notification_rsp_data->n2_info_content);
    ogs_free(n2_info_notification_rsp_data);
}

cJSON *OpenAPI_n2_info_notification_rsp_data_convertToJSON(OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data)
{
    cJSON *item = NULL;

    if (n2_info_notification_rsp_data == NULL) {
        ogs_error("OpenAPI_n2_info_notification_rsp_data_convertToJSON() failed [N2InfoNotificationRspData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n2_info_notification_rsp_data->n2_info_content) {
    cJSON *n2_info_content_local_JSON = OpenAPI_n2_info_content_convertToJSON(n2_info_notification_rsp_data->n2_info_content);
    if (n2_info_content_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_notification_rsp_data_convertToJSON() failed [n2_info_content]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2InfoContent", n2_info_content_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_notification_rsp_data_convertToJSON() failed [n2_info_content]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n2_info_notification_rsp_data_t *OpenAPI_n2_info_notification_rsp_data_parseFromJSON(cJSON *n2_info_notification_rsp_dataJSON)
{
    OpenAPI_n2_info_notification_rsp_data_t *n2_info_notification_rsp_data_local_var = NULL;
    cJSON *n2_info_content = cJSON_GetObjectItemCaseSensitive(n2_info_notification_rsp_dataJSON, "n2InfoContent");

    OpenAPI_n2_info_content_t *n2_info_content_local_nonprim = NULL;
    if (n2_info_content) {
    n2_info_content_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(n2_info_content);
    }

    n2_info_notification_rsp_data_local_var = OpenAPI_n2_info_notification_rsp_data_create (
        n2_info_content ? n2_info_content_local_nonprim : NULL
    );

    return n2_info_notification_rsp_data_local_var;
end:
    return NULL;
}

OpenAPI_n2_info_notification_rsp_data_t *OpenAPI_n2_info_notification_rsp_data_copy(OpenAPI_n2_info_notification_rsp_data_t *dst, OpenAPI_n2_info_notification_rsp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_info_notification_rsp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_info_notification_rsp_data_convertToJSON() failed");
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

    OpenAPI_n2_info_notification_rsp_data_free(dst);
    dst = OpenAPI_n2_info_notification_rsp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


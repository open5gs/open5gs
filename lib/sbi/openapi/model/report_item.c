
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "report_item.h"

OpenAPI_report_item_t *OpenAPI_report_item_create(
    char *path,
    char *reason
)
{
    OpenAPI_report_item_t *report_item_local_var = ogs_malloc(sizeof(OpenAPI_report_item_t));
    ogs_assert(report_item_local_var);

    report_item_local_var->path = path;
    report_item_local_var->reason = reason;

    return report_item_local_var;
}

void OpenAPI_report_item_free(OpenAPI_report_item_t *report_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == report_item) {
        return;
    }
    if (report_item->path) {
        ogs_free(report_item->path);
        report_item->path = NULL;
    }
    if (report_item->reason) {
        ogs_free(report_item->reason);
        report_item->reason = NULL;
    }
    ogs_free(report_item);
}

cJSON *OpenAPI_report_item_convertToJSON(OpenAPI_report_item_t *report_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (report_item == NULL) {
        ogs_error("OpenAPI_report_item_convertToJSON() failed [ReportItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!report_item->path) {
        ogs_error("OpenAPI_report_item_convertToJSON() failed [path]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "path", report_item->path) == NULL) {
        ogs_error("OpenAPI_report_item_convertToJSON() failed [path]");
        goto end;
    }

    if (report_item->reason) {
    if (cJSON_AddStringToObject(item, "reason", report_item->reason) == NULL) {
        ogs_error("OpenAPI_report_item_convertToJSON() failed [reason]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_report_item_t *OpenAPI_report_item_parseFromJSON(cJSON *report_itemJSON)
{
    OpenAPI_report_item_t *report_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *path = NULL;
    cJSON *reason = NULL;
    path = cJSON_GetObjectItemCaseSensitive(report_itemJSON, "path");
    if (!path) {
        ogs_error("OpenAPI_report_item_parseFromJSON() failed [path]");
        goto end;
    }
    if (!cJSON_IsString(path)) {
        ogs_error("OpenAPI_report_item_parseFromJSON() failed [path]");
        goto end;
    }

    reason = cJSON_GetObjectItemCaseSensitive(report_itemJSON, "reason");
    if (reason) {
    if (!cJSON_IsString(reason) && !cJSON_IsNull(reason)) {
        ogs_error("OpenAPI_report_item_parseFromJSON() failed [reason]");
        goto end;
    }
    }

    report_item_local_var = OpenAPI_report_item_create (
        ogs_strdup(path->valuestring),
        reason && !cJSON_IsNull(reason) ? ogs_strdup(reason->valuestring) : NULL
    );

    return report_item_local_var;
end:
    return NULL;
}

OpenAPI_report_item_t *OpenAPI_report_item_copy(OpenAPI_report_item_t *dst, OpenAPI_report_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_report_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_report_item_convertToJSON() failed");
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

    OpenAPI_report_item_free(dst);
    dst = OpenAPI_report_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


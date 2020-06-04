
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "report_item.h"

OpenAPI_report_item_t *OpenAPI_report_item_create(
    char *path
    )
{
    OpenAPI_report_item_t *report_item_local_var = OpenAPI_malloc(sizeof(OpenAPI_report_item_t));
    if (!report_item_local_var) {
        return NULL;
    }
    report_item_local_var->path = path;

    return report_item_local_var;
}

void OpenAPI_report_item_free(OpenAPI_report_item_t *report_item)
{
    if (NULL == report_item) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(report_item->path);
    ogs_free(report_item);
}

cJSON *OpenAPI_report_item_convertToJSON(OpenAPI_report_item_t *report_item)
{
    cJSON *item = NULL;

    if (report_item == NULL) {
        ogs_error("OpenAPI_report_item_convertToJSON() failed [ReportItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!report_item->path) {
        ogs_error("OpenAPI_report_item_convertToJSON() failed [path]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "path", report_item->path) == NULL) {
        ogs_error("OpenAPI_report_item_convertToJSON() failed [path]");
        goto end;
    }

end:
    return item;
}

OpenAPI_report_item_t *OpenAPI_report_item_parseFromJSON(cJSON *report_itemJSON)
{
    OpenAPI_report_item_t *report_item_local_var = NULL;
    cJSON *path = cJSON_GetObjectItemCaseSensitive(report_itemJSON, "path");
    if (!path) {
        ogs_error("OpenAPI_report_item_parseFromJSON() failed [path]");
        goto end;
    }


    if (!cJSON_IsString(path)) {
        ogs_error("OpenAPI_report_item_parseFromJSON() failed [path]");
        goto end;
    }

    report_item_local_var = OpenAPI_report_item_create (
        ogs_strdup(path->valuestring)
        );

    return report_item_local_var;
end:
    return NULL;
}


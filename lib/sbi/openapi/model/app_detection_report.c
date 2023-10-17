
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_detection_report.h"

OpenAPI_app_detection_report_t *OpenAPI_app_detection_report_create(
    OpenAPI_app_detection_notif_type_e ad_notif_type,
    char *af_app_id
)
{
    OpenAPI_app_detection_report_t *app_detection_report_local_var = ogs_malloc(sizeof(OpenAPI_app_detection_report_t));
    ogs_assert(app_detection_report_local_var);

    app_detection_report_local_var->ad_notif_type = ad_notif_type;
    app_detection_report_local_var->af_app_id = af_app_id;

    return app_detection_report_local_var;
}

void OpenAPI_app_detection_report_free(OpenAPI_app_detection_report_t *app_detection_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == app_detection_report) {
        return;
    }
    if (app_detection_report->af_app_id) {
        ogs_free(app_detection_report->af_app_id);
        app_detection_report->af_app_id = NULL;
    }
    ogs_free(app_detection_report);
}

cJSON *OpenAPI_app_detection_report_convertToJSON(OpenAPI_app_detection_report_t *app_detection_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (app_detection_report == NULL) {
        ogs_error("OpenAPI_app_detection_report_convertToJSON() failed [AppDetectionReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (app_detection_report->ad_notif_type == OpenAPI_app_detection_notif_type_NULL) {
        ogs_error("OpenAPI_app_detection_report_convertToJSON() failed [ad_notif_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "adNotifType", OpenAPI_app_detection_notif_type_ToString(app_detection_report->ad_notif_type)) == NULL) {
        ogs_error("OpenAPI_app_detection_report_convertToJSON() failed [ad_notif_type]");
        goto end;
    }

    if (!app_detection_report->af_app_id) {
        ogs_error("OpenAPI_app_detection_report_convertToJSON() failed [af_app_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afAppId", app_detection_report->af_app_id) == NULL) {
        ogs_error("OpenAPI_app_detection_report_convertToJSON() failed [af_app_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_app_detection_report_t *OpenAPI_app_detection_report_parseFromJSON(cJSON *app_detection_reportJSON)
{
    OpenAPI_app_detection_report_t *app_detection_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ad_notif_type = NULL;
    OpenAPI_app_detection_notif_type_e ad_notif_typeVariable = 0;
    cJSON *af_app_id = NULL;
    ad_notif_type = cJSON_GetObjectItemCaseSensitive(app_detection_reportJSON, "adNotifType");
    if (!ad_notif_type) {
        ogs_error("OpenAPI_app_detection_report_parseFromJSON() failed [ad_notif_type]");
        goto end;
    }
    if (!cJSON_IsString(ad_notif_type)) {
        ogs_error("OpenAPI_app_detection_report_parseFromJSON() failed [ad_notif_type]");
        goto end;
    }
    ad_notif_typeVariable = OpenAPI_app_detection_notif_type_FromString(ad_notif_type->valuestring);

    af_app_id = cJSON_GetObjectItemCaseSensitive(app_detection_reportJSON, "afAppId");
    if (!af_app_id) {
        ogs_error("OpenAPI_app_detection_report_parseFromJSON() failed [af_app_id]");
        goto end;
    }
    if (!cJSON_IsString(af_app_id)) {
        ogs_error("OpenAPI_app_detection_report_parseFromJSON() failed [af_app_id]");
        goto end;
    }

    app_detection_report_local_var = OpenAPI_app_detection_report_create (
        ad_notif_typeVariable,
        ogs_strdup(af_app_id->valuestring)
    );

    return app_detection_report_local_var;
end:
    return NULL;
}

OpenAPI_app_detection_report_t *OpenAPI_app_detection_report_copy(OpenAPI_app_detection_report_t *dst, OpenAPI_app_detection_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_detection_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_detection_report_convertToJSON() failed");
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

    OpenAPI_app_detection_report_free(dst);
    dst = OpenAPI_app_detection_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


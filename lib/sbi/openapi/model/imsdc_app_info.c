
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "imsdc_app_info.h"

OpenAPI_imsdc_app_info_t *OpenAPI_imsdc_app_info_create(
    char *application_id,
    OpenAPI_replace_http_url_t *http_url
)
{
    OpenAPI_imsdc_app_info_t *imsdc_app_info_local_var = ogs_malloc(sizeof(OpenAPI_imsdc_app_info_t));
    ogs_assert(imsdc_app_info_local_var);

    imsdc_app_info_local_var->application_id = application_id;
    imsdc_app_info_local_var->http_url = http_url;

    return imsdc_app_info_local_var;
}

void OpenAPI_imsdc_app_info_free(OpenAPI_imsdc_app_info_t *imsdc_app_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == imsdc_app_info) {
        return;
    }
    if (imsdc_app_info->application_id) {
        ogs_free(imsdc_app_info->application_id);
        imsdc_app_info->application_id = NULL;
    }
    if (imsdc_app_info->http_url) {
        OpenAPI_replace_http_url_free(imsdc_app_info->http_url);
        imsdc_app_info->http_url = NULL;
    }
    ogs_free(imsdc_app_info);
}

cJSON *OpenAPI_imsdc_app_info_convertToJSON(OpenAPI_imsdc_app_info_t *imsdc_app_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (imsdc_app_info == NULL) {
        ogs_error("OpenAPI_imsdc_app_info_convertToJSON() failed [IMSDCAppInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (imsdc_app_info->application_id) {
    if (cJSON_AddStringToObject(item, "applicationId", imsdc_app_info->application_id) == NULL) {
        ogs_error("OpenAPI_imsdc_app_info_convertToJSON() failed [application_id]");
        goto end;
    }
    }

    if (imsdc_app_info->http_url) {
    cJSON *http_url_local_JSON = OpenAPI_replace_http_url_convertToJSON(imsdc_app_info->http_url);
    if (http_url_local_JSON == NULL) {
        ogs_error("OpenAPI_imsdc_app_info_convertToJSON() failed [http_url]");
        goto end;
    }
    cJSON_AddItemToObject(item, "httpUrl", http_url_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_imsdc_app_info_convertToJSON() failed [http_url]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_imsdc_app_info_t *OpenAPI_imsdc_app_info_parseFromJSON(cJSON *imsdc_app_infoJSON)
{
    OpenAPI_imsdc_app_info_t *imsdc_app_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *application_id = NULL;
    cJSON *http_url = NULL;
    OpenAPI_replace_http_url_t *http_url_local_nonprim = NULL;
    application_id = cJSON_GetObjectItemCaseSensitive(imsdc_app_infoJSON, "applicationId");
    if (application_id) {
    if (!cJSON_IsString(application_id) && !cJSON_IsNull(application_id)) {
        ogs_error("OpenAPI_imsdc_app_info_parseFromJSON() failed [application_id]");
        goto end;
    }
    }

    http_url = cJSON_GetObjectItemCaseSensitive(imsdc_app_infoJSON, "httpUrl");
    if (http_url) {
    http_url_local_nonprim = OpenAPI_replace_http_url_parseFromJSON(http_url);
    if (!http_url_local_nonprim) {
        ogs_error("OpenAPI_replace_http_url_parseFromJSON failed [http_url]");
        goto end;
    }
    }

    imsdc_app_info_local_var = OpenAPI_imsdc_app_info_create (
        application_id && !cJSON_IsNull(application_id) ? ogs_strdup(application_id->valuestring) : NULL,
        http_url ? http_url_local_nonprim : NULL
    );

    return imsdc_app_info_local_var;
end:
    if (http_url_local_nonprim) {
        OpenAPI_replace_http_url_free(http_url_local_nonprim);
        http_url_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_imsdc_app_info_t *OpenAPI_imsdc_app_info_copy(OpenAPI_imsdc_app_info_t *dst, OpenAPI_imsdc_app_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_imsdc_app_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_imsdc_app_info_convertToJSON() failed");
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

    OpenAPI_imsdc_app_info_free(dst);
    dst = OpenAPI_imsdc_app_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


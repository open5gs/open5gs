
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pfd_data_for_app.h"

OpenAPI_pfd_data_for_app_t *OpenAPI_pfd_data_for_app_create(
    char *application_id,
    OpenAPI_list_t *pfds,
    char *caching_time
)
{
    OpenAPI_pfd_data_for_app_t *pfd_data_for_app_local_var = ogs_malloc(sizeof(OpenAPI_pfd_data_for_app_t));
    ogs_assert(pfd_data_for_app_local_var);

    pfd_data_for_app_local_var->application_id = application_id;
    pfd_data_for_app_local_var->pfds = pfds;
    pfd_data_for_app_local_var->caching_time = caching_time;

    return pfd_data_for_app_local_var;
}

void OpenAPI_pfd_data_for_app_free(OpenAPI_pfd_data_for_app_t *pfd_data_for_app)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pfd_data_for_app) {
        return;
    }
    if (pfd_data_for_app->application_id) {
        ogs_free(pfd_data_for_app->application_id);
        pfd_data_for_app->application_id = NULL;
    }
    if (pfd_data_for_app->pfds) {
        OpenAPI_list_for_each(pfd_data_for_app->pfds, node) {
            OpenAPI_pfd_content_free(node->data);
        }
        OpenAPI_list_free(pfd_data_for_app->pfds);
        pfd_data_for_app->pfds = NULL;
    }
    if (pfd_data_for_app->caching_time) {
        ogs_free(pfd_data_for_app->caching_time);
        pfd_data_for_app->caching_time = NULL;
    }
    ogs_free(pfd_data_for_app);
}

cJSON *OpenAPI_pfd_data_for_app_convertToJSON(OpenAPI_pfd_data_for_app_t *pfd_data_for_app)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pfd_data_for_app == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_convertToJSON() failed [PfdDataForApp]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pfd_data_for_app->application_id) {
        ogs_error("OpenAPI_pfd_data_for_app_convertToJSON() failed [application_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "applicationId", pfd_data_for_app->application_id) == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_convertToJSON() failed [application_id]");
        goto end;
    }

    if (!pfd_data_for_app->pfds) {
        ogs_error("OpenAPI_pfd_data_for_app_convertToJSON() failed [pfds]");
        return NULL;
    }
    cJSON *pfdsList = cJSON_AddArrayToObject(item, "pfds");
    if (pfdsList == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_convertToJSON() failed [pfds]");
        goto end;
    }
    OpenAPI_list_for_each(pfd_data_for_app->pfds, node) {
        cJSON *itemLocal = OpenAPI_pfd_content_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pfd_data_for_app_convertToJSON() failed [pfds]");
            goto end;
        }
        cJSON_AddItemToArray(pfdsList, itemLocal);
    }

    if (pfd_data_for_app->caching_time) {
    if (cJSON_AddStringToObject(item, "cachingTime", pfd_data_for_app->caching_time) == NULL) {
        ogs_error("OpenAPI_pfd_data_for_app_convertToJSON() failed [caching_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pfd_data_for_app_t *OpenAPI_pfd_data_for_app_parseFromJSON(cJSON *pfd_data_for_appJSON)
{
    OpenAPI_pfd_data_for_app_t *pfd_data_for_app_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *application_id = NULL;
    cJSON *pfds = NULL;
    OpenAPI_list_t *pfdsList = NULL;
    cJSON *caching_time = NULL;
    application_id = cJSON_GetObjectItemCaseSensitive(pfd_data_for_appJSON, "applicationId");
    if (!application_id) {
        ogs_error("OpenAPI_pfd_data_for_app_parseFromJSON() failed [application_id]");
        goto end;
    }
    if (!cJSON_IsString(application_id)) {
        ogs_error("OpenAPI_pfd_data_for_app_parseFromJSON() failed [application_id]");
        goto end;
    }

    pfds = cJSON_GetObjectItemCaseSensitive(pfd_data_for_appJSON, "pfds");
    if (!pfds) {
        ogs_error("OpenAPI_pfd_data_for_app_parseFromJSON() failed [pfds]");
        goto end;
    }
        cJSON *pfds_local = NULL;
        if (!cJSON_IsArray(pfds)) {
            ogs_error("OpenAPI_pfd_data_for_app_parseFromJSON() failed [pfds]");
            goto end;
        }

        pfdsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pfds_local, pfds) {
            if (!cJSON_IsObject(pfds_local)) {
                ogs_error("OpenAPI_pfd_data_for_app_parseFromJSON() failed [pfds]");
                goto end;
            }
            OpenAPI_pfd_content_t *pfdsItem = OpenAPI_pfd_content_parseFromJSON(pfds_local);
            if (!pfdsItem) {
                ogs_error("No pfdsItem");
                OpenAPI_list_free(pfdsList);
                goto end;
            }
            OpenAPI_list_add(pfdsList, pfdsItem);
        }

    caching_time = cJSON_GetObjectItemCaseSensitive(pfd_data_for_appJSON, "cachingTime");
    if (caching_time) {
    if (!cJSON_IsString(caching_time) && !cJSON_IsNull(caching_time)) {
        ogs_error("OpenAPI_pfd_data_for_app_parseFromJSON() failed [caching_time]");
        goto end;
    }
    }

    pfd_data_for_app_local_var = OpenAPI_pfd_data_for_app_create (
        ogs_strdup(application_id->valuestring),
        pfdsList,
        caching_time && !cJSON_IsNull(caching_time) ? ogs_strdup(caching_time->valuestring) : NULL
    );

    return pfd_data_for_app_local_var;
end:
    if (pfdsList) {
        OpenAPI_list_for_each(pfdsList, node) {
            OpenAPI_pfd_content_free(node->data);
        }
        OpenAPI_list_free(pfdsList);
        pfdsList = NULL;
    }
    return NULL;
}

OpenAPI_pfd_data_for_app_t *OpenAPI_pfd_data_for_app_copy(OpenAPI_pfd_data_for_app_t *dst, OpenAPI_pfd_data_for_app_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pfd_data_for_app_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pfd_data_for_app_convertToJSON() failed");
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

    OpenAPI_pfd_data_for_app_free(dst);
    dst = OpenAPI_pfd_data_for_app_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


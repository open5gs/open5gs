
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "application_data_subs.h"

OpenAPI_application_data_subs_t *OpenAPI_application_data_subs_create(
    char *notification_uri,
    OpenAPI_list_t *data_filters,
    char *expiry,
    char *supported_features
    )
{
    OpenAPI_application_data_subs_t *application_data_subs_local_var = OpenAPI_malloc(sizeof(OpenAPI_application_data_subs_t));
    if (!application_data_subs_local_var) {
        return NULL;
    }
    application_data_subs_local_var->notification_uri = notification_uri;
    application_data_subs_local_var->data_filters = data_filters;
    application_data_subs_local_var->expiry = expiry;
    application_data_subs_local_var->supported_features = supported_features;

    return application_data_subs_local_var;
}

void OpenAPI_application_data_subs_free(OpenAPI_application_data_subs_t *application_data_subs)
{
    if (NULL == application_data_subs) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(application_data_subs->notification_uri);
    OpenAPI_list_for_each(application_data_subs->data_filters, node) {
        OpenAPI_data_filter_free(node->data);
    }
    OpenAPI_list_free(application_data_subs->data_filters);
    ogs_free(application_data_subs->expiry);
    ogs_free(application_data_subs->supported_features);
    ogs_free(application_data_subs);
}

cJSON *OpenAPI_application_data_subs_convertToJSON(OpenAPI_application_data_subs_t *application_data_subs)
{
    cJSON *item = NULL;

    if (application_data_subs == NULL) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [ApplicationDataSubs]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!application_data_subs->notification_uri) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [notification_uri]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "notificationUri", application_data_subs->notification_uri) == NULL) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (application_data_subs->data_filters) {
        cJSON *data_filtersList = cJSON_AddArrayToObject(item, "dataFilters");
        if (data_filtersList == NULL) {
            ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [data_filters]");
            goto end;
        }

        OpenAPI_lnode_t *data_filters_node;
        if (application_data_subs->data_filters) {
            OpenAPI_list_for_each(application_data_subs->data_filters, data_filters_node) {
                cJSON *itemLocal = OpenAPI_data_filter_convertToJSON(data_filters_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [data_filters]");
                    goto end;
                }
                cJSON_AddItemToArray(data_filtersList, itemLocal);
            }
        }
    }

    if (application_data_subs->expiry) {
        if (cJSON_AddStringToObject(item, "expiry", application_data_subs->expiry) == NULL) {
            ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [expiry]");
            goto end;
        }
    }

    if (application_data_subs->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", application_data_subs->supported_features) == NULL) {
            ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_application_data_subs_t *OpenAPI_application_data_subs_parseFromJSON(cJSON *application_data_subsJSON)
{
    OpenAPI_application_data_subs_t *application_data_subs_local_var = NULL;
    cJSON *notification_uri = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [notification_uri]");
        goto end;
    }


    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    cJSON *data_filters = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "dataFilters");

    OpenAPI_list_t *data_filtersList;
    if (data_filters) {
        cJSON *data_filters_local_nonprimitive;
        if (!cJSON_IsArray(data_filters)) {
            ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [data_filters]");
            goto end;
        }

        data_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(data_filters_local_nonprimitive, data_filters ) {
            if (!cJSON_IsObject(data_filters_local_nonprimitive)) {
                ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [data_filters]");
                goto end;
            }
            OpenAPI_data_filter_t *data_filtersItem = OpenAPI_data_filter_parseFromJSON(data_filters_local_nonprimitive);

            OpenAPI_list_add(data_filtersList, data_filtersItem);
        }
    }

    cJSON *expiry = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "expiry");

    if (expiry) {
        if (!cJSON_IsString(expiry)) {
            ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [expiry]");
            goto end;
        }
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    application_data_subs_local_var = OpenAPI_application_data_subs_create (
        ogs_strdup(notification_uri->valuestring),
        data_filters ? data_filtersList : NULL,
        expiry ? ogs_strdup(expiry->valuestring) : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
        );

    return application_data_subs_local_var;
end:
    return NULL;
}

OpenAPI_application_data_subs_t *OpenAPI_application_data_subs_copy(OpenAPI_application_data_subs_t *dst, OpenAPI_application_data_subs_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_application_data_subs_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed");
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

    OpenAPI_application_data_subs_free(dst);
    dst = OpenAPI_application_data_subs_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "application_data_subs.h"

OpenAPI_application_data_subs_t *OpenAPI_application_data_subs_create(
    char *notification_uri,
    OpenAPI_list_t *data_filters,
    char *expiry,
    bool is_imm_rep,
    int imm_rep,
    OpenAPI_list_t *am_influ_entries,
    char *supported_features,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_application_data_subs_t *application_data_subs_local_var = ogs_malloc(sizeof(OpenAPI_application_data_subs_t));
    ogs_assert(application_data_subs_local_var);

    application_data_subs_local_var->notification_uri = notification_uri;
    application_data_subs_local_var->data_filters = data_filters;
    application_data_subs_local_var->expiry = expiry;
    application_data_subs_local_var->is_imm_rep = is_imm_rep;
    application_data_subs_local_var->imm_rep = imm_rep;
    application_data_subs_local_var->am_influ_entries = am_influ_entries;
    application_data_subs_local_var->supported_features = supported_features;
    application_data_subs_local_var->reset_ids = reset_ids;

    return application_data_subs_local_var;
}

void OpenAPI_application_data_subs_free(OpenAPI_application_data_subs_t *application_data_subs)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == application_data_subs) {
        return;
    }
    if (application_data_subs->notification_uri) {
        ogs_free(application_data_subs->notification_uri);
        application_data_subs->notification_uri = NULL;
    }
    if (application_data_subs->data_filters) {
        OpenAPI_list_for_each(application_data_subs->data_filters, node) {
            OpenAPI_data_filter_free(node->data);
        }
        OpenAPI_list_free(application_data_subs->data_filters);
        application_data_subs->data_filters = NULL;
    }
    if (application_data_subs->expiry) {
        ogs_free(application_data_subs->expiry);
        application_data_subs->expiry = NULL;
    }
    if (application_data_subs->am_influ_entries) {
        OpenAPI_list_for_each(application_data_subs->am_influ_entries, node) {
            OpenAPI_am_influ_data_free(node->data);
        }
        OpenAPI_list_free(application_data_subs->am_influ_entries);
        application_data_subs->am_influ_entries = NULL;
    }
    if (application_data_subs->supported_features) {
        ogs_free(application_data_subs->supported_features);
        application_data_subs->supported_features = NULL;
    }
    if (application_data_subs->reset_ids) {
        OpenAPI_list_for_each(application_data_subs->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(application_data_subs->reset_ids);
        application_data_subs->reset_ids = NULL;
    }
    ogs_free(application_data_subs);
}

cJSON *OpenAPI_application_data_subs_convertToJSON(OpenAPI_application_data_subs_t *application_data_subs)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (application_data_subs == NULL) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [ApplicationDataSubs]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!application_data_subs->notification_uri) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [notification_uri]");
        return NULL;
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
    OpenAPI_list_for_each(application_data_subs->data_filters, node) {
        cJSON *itemLocal = OpenAPI_data_filter_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [data_filters]");
            goto end;
        }
        cJSON_AddItemToArray(data_filtersList, itemLocal);
    }
    }

    if (application_data_subs->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", application_data_subs->expiry) == NULL) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (application_data_subs->is_imm_rep) {
    if (cJSON_AddBoolToObject(item, "immRep", application_data_subs->imm_rep) == NULL) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [imm_rep]");
        goto end;
    }
    }

    if (application_data_subs->am_influ_entries) {
    cJSON *am_influ_entriesList = cJSON_AddArrayToObject(item, "amInfluEntries");
    if (am_influ_entriesList == NULL) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [am_influ_entries]");
        goto end;
    }
    OpenAPI_list_for_each(application_data_subs->am_influ_entries, node) {
        cJSON *itemLocal = OpenAPI_am_influ_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [am_influ_entries]");
            goto end;
        }
        cJSON_AddItemToArray(am_influ_entriesList, itemLocal);
    }
    }

    if (application_data_subs->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", application_data_subs->supported_features) == NULL) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (application_data_subs->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(application_data_subs->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_application_data_subs_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_application_data_subs_t *OpenAPI_application_data_subs_parseFromJSON(cJSON *application_data_subsJSON)
{
    OpenAPI_application_data_subs_t *application_data_subs_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notification_uri = NULL;
    cJSON *data_filters = NULL;
    OpenAPI_list_t *data_filtersList = NULL;
    cJSON *expiry = NULL;
    cJSON *imm_rep = NULL;
    cJSON *am_influ_entries = NULL;
    OpenAPI_list_t *am_influ_entriesList = NULL;
    cJSON *supported_features = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    notification_uri = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    data_filters = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "dataFilters");
    if (data_filters) {
        cJSON *data_filters_local = NULL;
        if (!cJSON_IsArray(data_filters)) {
            ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [data_filters]");
            goto end;
        }

        data_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(data_filters_local, data_filters) {
            if (!cJSON_IsObject(data_filters_local)) {
                ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [data_filters]");
                goto end;
            }
            OpenAPI_data_filter_t *data_filtersItem = OpenAPI_data_filter_parseFromJSON(data_filters_local);
            if (!data_filtersItem) {
                ogs_error("No data_filtersItem");
                goto end;
            }
            OpenAPI_list_add(data_filtersList, data_filtersItem);
        }
    }

    expiry = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    imm_rep = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "immRep");
    if (imm_rep) {
    if (!cJSON_IsBool(imm_rep)) {
        ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [imm_rep]");
        goto end;
    }
    }

    am_influ_entries = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "amInfluEntries");
    if (am_influ_entries) {
        cJSON *am_influ_entries_local = NULL;
        if (!cJSON_IsArray(am_influ_entries)) {
            ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [am_influ_entries]");
            goto end;
        }

        am_influ_entriesList = OpenAPI_list_create();

        cJSON_ArrayForEach(am_influ_entries_local, am_influ_entries) {
            if (!cJSON_IsObject(am_influ_entries_local)) {
                ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [am_influ_entries]");
                goto end;
            }
            OpenAPI_am_influ_data_t *am_influ_entriesItem = OpenAPI_am_influ_data_parseFromJSON(am_influ_entries_local);
            if (!am_influ_entriesItem) {
                ogs_error("No am_influ_entriesItem");
                goto end;
            }
            OpenAPI_list_add(am_influ_entriesList, am_influ_entriesItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(application_data_subsJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_application_data_subs_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    application_data_subs_local_var = OpenAPI_application_data_subs_create (
        ogs_strdup(notification_uri->valuestring),
        data_filters ? data_filtersList : NULL,
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL,
        imm_rep ? true : false,
        imm_rep ? imm_rep->valueint : 0,
        am_influ_entries ? am_influ_entriesList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return application_data_subs_local_var;
end:
    if (data_filtersList) {
        OpenAPI_list_for_each(data_filtersList, node) {
            OpenAPI_data_filter_free(node->data);
        }
        OpenAPI_list_free(data_filtersList);
        data_filtersList = NULL;
    }
    if (am_influ_entriesList) {
        OpenAPI_list_for_each(am_influ_entriesList, node) {
            OpenAPI_am_influ_data_free(node->data);
        }
        OpenAPI_list_free(am_influ_entriesList);
        am_influ_entriesList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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


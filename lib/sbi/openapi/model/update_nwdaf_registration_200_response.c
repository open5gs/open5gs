
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "update_nwdaf_registration_200_response.h"

OpenAPI_update_nwdaf_registration_200_response_t *OpenAPI_update_nwdaf_registration_200_response_create(
    char *nwdaf_instance_id,
    OpenAPI_list_t *analytics_ids,
    char *nwdaf_set_id,
    char *registration_time,
    OpenAPI_context_info_t *context_info,
    char *supported_features,
    OpenAPI_list_t *reset_ids,
    OpenAPI_list_t *report
)
{
    OpenAPI_update_nwdaf_registration_200_response_t *update_nwdaf_registration_200_response_local_var = ogs_malloc(sizeof(OpenAPI_update_nwdaf_registration_200_response_t));
    ogs_assert(update_nwdaf_registration_200_response_local_var);

    update_nwdaf_registration_200_response_local_var->nwdaf_instance_id = nwdaf_instance_id;
    update_nwdaf_registration_200_response_local_var->analytics_ids = analytics_ids;
    update_nwdaf_registration_200_response_local_var->nwdaf_set_id = nwdaf_set_id;
    update_nwdaf_registration_200_response_local_var->registration_time = registration_time;
    update_nwdaf_registration_200_response_local_var->context_info = context_info;
    update_nwdaf_registration_200_response_local_var->supported_features = supported_features;
    update_nwdaf_registration_200_response_local_var->reset_ids = reset_ids;
    update_nwdaf_registration_200_response_local_var->report = report;

    return update_nwdaf_registration_200_response_local_var;
}

void OpenAPI_update_nwdaf_registration_200_response_free(OpenAPI_update_nwdaf_registration_200_response_t *update_nwdaf_registration_200_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == update_nwdaf_registration_200_response) {
        return;
    }
    if (update_nwdaf_registration_200_response->nwdaf_instance_id) {
        ogs_free(update_nwdaf_registration_200_response->nwdaf_instance_id);
        update_nwdaf_registration_200_response->nwdaf_instance_id = NULL;
    }
    if (update_nwdaf_registration_200_response->analytics_ids) {
        OpenAPI_list_for_each(update_nwdaf_registration_200_response->analytics_ids, node) {
            OpenAPI_event_id_free(node->data);
        }
        OpenAPI_list_free(update_nwdaf_registration_200_response->analytics_ids);
        update_nwdaf_registration_200_response->analytics_ids = NULL;
    }
    if (update_nwdaf_registration_200_response->nwdaf_set_id) {
        ogs_free(update_nwdaf_registration_200_response->nwdaf_set_id);
        update_nwdaf_registration_200_response->nwdaf_set_id = NULL;
    }
    if (update_nwdaf_registration_200_response->registration_time) {
        ogs_free(update_nwdaf_registration_200_response->registration_time);
        update_nwdaf_registration_200_response->registration_time = NULL;
    }
    if (update_nwdaf_registration_200_response->context_info) {
        OpenAPI_context_info_free(update_nwdaf_registration_200_response->context_info);
        update_nwdaf_registration_200_response->context_info = NULL;
    }
    if (update_nwdaf_registration_200_response->supported_features) {
        ogs_free(update_nwdaf_registration_200_response->supported_features);
        update_nwdaf_registration_200_response->supported_features = NULL;
    }
    if (update_nwdaf_registration_200_response->reset_ids) {
        OpenAPI_list_for_each(update_nwdaf_registration_200_response->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(update_nwdaf_registration_200_response->reset_ids);
        update_nwdaf_registration_200_response->reset_ids = NULL;
    }
    if (update_nwdaf_registration_200_response->report) {
        OpenAPI_list_for_each(update_nwdaf_registration_200_response->report, node) {
            OpenAPI_report_item_free(node->data);
        }
        OpenAPI_list_free(update_nwdaf_registration_200_response->report);
        update_nwdaf_registration_200_response->report = NULL;
    }
    ogs_free(update_nwdaf_registration_200_response);
}

cJSON *OpenAPI_update_nwdaf_registration_200_response_convertToJSON(OpenAPI_update_nwdaf_registration_200_response_t *update_nwdaf_registration_200_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (update_nwdaf_registration_200_response == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [UpdateNwdafRegistration_200_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!update_nwdaf_registration_200_response->nwdaf_instance_id) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [nwdaf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nwdafInstanceId", update_nwdaf_registration_200_response->nwdaf_instance_id) == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [nwdaf_instance_id]");
        goto end;
    }

    if (!update_nwdaf_registration_200_response->analytics_ids) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [analytics_ids]");
        return NULL;
    }
    cJSON *analytics_idsList = cJSON_AddArrayToObject(item, "analyticsIds");
    if (analytics_idsList == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [analytics_ids]");
        goto end;
    }
    OpenAPI_list_for_each(update_nwdaf_registration_200_response->analytics_ids, node) {
        cJSON *itemLocal = OpenAPI_event_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [analytics_ids]");
            goto end;
        }
        cJSON_AddItemToArray(analytics_idsList, itemLocal);
    }

    if (update_nwdaf_registration_200_response->nwdaf_set_id) {
    if (cJSON_AddStringToObject(item, "nwdafSetId", update_nwdaf_registration_200_response->nwdaf_set_id) == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [nwdaf_set_id]");
        goto end;
    }
    }

    if (update_nwdaf_registration_200_response->registration_time) {
    if (cJSON_AddStringToObject(item, "registrationTime", update_nwdaf_registration_200_response->registration_time) == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [registration_time]");
        goto end;
    }
    }

    if (update_nwdaf_registration_200_response->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(update_nwdaf_registration_200_response->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [context_info]");
        goto end;
    }
    }

    if (update_nwdaf_registration_200_response->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", update_nwdaf_registration_200_response->supported_features) == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (update_nwdaf_registration_200_response->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(update_nwdaf_registration_200_response->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

    if (!update_nwdaf_registration_200_response->report) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [report]");
        return NULL;
    }
    cJSON *reportList = cJSON_AddArrayToObject(item, "report");
    if (reportList == NULL) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [report]");
        goto end;
    }
    OpenAPI_list_for_each(update_nwdaf_registration_200_response->report, node) {
        cJSON *itemLocal = OpenAPI_report_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed [report]");
            goto end;
        }
        cJSON_AddItemToArray(reportList, itemLocal);
    }

end:
    return item;
}

OpenAPI_update_nwdaf_registration_200_response_t *OpenAPI_update_nwdaf_registration_200_response_parseFromJSON(cJSON *update_nwdaf_registration_200_responseJSON)
{
    OpenAPI_update_nwdaf_registration_200_response_t *update_nwdaf_registration_200_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nwdaf_instance_id = NULL;
    cJSON *analytics_ids = NULL;
    OpenAPI_list_t *analytics_idsList = NULL;
    cJSON *nwdaf_set_id = NULL;
    cJSON *registration_time = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    cJSON *report = NULL;
    OpenAPI_list_t *reportList = NULL;
    nwdaf_instance_id = cJSON_GetObjectItemCaseSensitive(update_nwdaf_registration_200_responseJSON, "nwdafInstanceId");
    if (!nwdaf_instance_id) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [nwdaf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(nwdaf_instance_id)) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [nwdaf_instance_id]");
        goto end;
    }

    analytics_ids = cJSON_GetObjectItemCaseSensitive(update_nwdaf_registration_200_responseJSON, "analyticsIds");
    if (!analytics_ids) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [analytics_ids]");
        goto end;
    }
        cJSON *analytics_ids_local = NULL;
        if (!cJSON_IsArray(analytics_ids)) {
            ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [analytics_ids]");
            goto end;
        }

        analytics_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(analytics_ids_local, analytics_ids) {
            if (!cJSON_IsObject(analytics_ids_local)) {
                ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [analytics_ids]");
                goto end;
            }
            OpenAPI_event_id_t *analytics_idsItem = OpenAPI_event_id_parseFromJSON(analytics_ids_local);
            if (!analytics_idsItem) {
                ogs_error("No analytics_idsItem");
                goto end;
            }
            OpenAPI_list_add(analytics_idsList, analytics_idsItem);
        }

    nwdaf_set_id = cJSON_GetObjectItemCaseSensitive(update_nwdaf_registration_200_responseJSON, "nwdafSetId");
    if (nwdaf_set_id) {
    if (!cJSON_IsString(nwdaf_set_id) && !cJSON_IsNull(nwdaf_set_id)) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [nwdaf_set_id]");
        goto end;
    }
    }

    registration_time = cJSON_GetObjectItemCaseSensitive(update_nwdaf_registration_200_responseJSON, "registrationTime");
    if (registration_time) {
    if (!cJSON_IsString(registration_time) && !cJSON_IsNull(registration_time)) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [registration_time]");
        goto end;
    }
    }

    context_info = cJSON_GetObjectItemCaseSensitive(update_nwdaf_registration_200_responseJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    if (!context_info_local_nonprim) {
        ogs_error("OpenAPI_context_info_parseFromJSON failed [context_info]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(update_nwdaf_registration_200_responseJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(update_nwdaf_registration_200_responseJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    report = cJSON_GetObjectItemCaseSensitive(update_nwdaf_registration_200_responseJSON, "report");
    if (!report) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [report]");
        goto end;
    }
        cJSON *report_local = NULL;
        if (!cJSON_IsArray(report)) {
            ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [report]");
            goto end;
        }

        reportList = OpenAPI_list_create();

        cJSON_ArrayForEach(report_local, report) {
            if (!cJSON_IsObject(report_local)) {
                ogs_error("OpenAPI_update_nwdaf_registration_200_response_parseFromJSON() failed [report]");
                goto end;
            }
            OpenAPI_report_item_t *reportItem = OpenAPI_report_item_parseFromJSON(report_local);
            if (!reportItem) {
                ogs_error("No reportItem");
                goto end;
            }
            OpenAPI_list_add(reportList, reportItem);
        }

    update_nwdaf_registration_200_response_local_var = OpenAPI_update_nwdaf_registration_200_response_create (
        ogs_strdup(nwdaf_instance_id->valuestring),
        analytics_idsList,
        nwdaf_set_id && !cJSON_IsNull(nwdaf_set_id) ? ogs_strdup(nwdaf_set_id->valuestring) : NULL,
        registration_time && !cJSON_IsNull(registration_time) ? ogs_strdup(registration_time->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL,
        reportList
    );

    return update_nwdaf_registration_200_response_local_var;
end:
    if (analytics_idsList) {
        OpenAPI_list_for_each(analytics_idsList, node) {
            OpenAPI_event_id_free(node->data);
        }
        OpenAPI_list_free(analytics_idsList);
        analytics_idsList = NULL;
    }
    if (context_info_local_nonprim) {
        OpenAPI_context_info_free(context_info_local_nonprim);
        context_info_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    if (reportList) {
        OpenAPI_list_for_each(reportList, node) {
            OpenAPI_report_item_free(node->data);
        }
        OpenAPI_list_free(reportList);
        reportList = NULL;
    }
    return NULL;
}

OpenAPI_update_nwdaf_registration_200_response_t *OpenAPI_update_nwdaf_registration_200_response_copy(OpenAPI_update_nwdaf_registration_200_response_t *dst, OpenAPI_update_nwdaf_registration_200_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_update_nwdaf_registration_200_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_update_nwdaf_registration_200_response_convertToJSON() failed");
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

    OpenAPI_update_nwdaf_registration_200_response_free(dst);
    dst = OpenAPI_update_nwdaf_registration_200_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


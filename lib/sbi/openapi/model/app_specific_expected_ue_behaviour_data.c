
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_specific_expected_ue_behaviour_data.h"

OpenAPI_app_specific_expected_ue_behaviour_data_t *OpenAPI_app_specific_expected_ue_behaviour_data_create(
    char *app_id,
    OpenAPI_list_t *traffic_filters,
    bool is_expected_inactivity_time,
    int expected_inactivity_time,
    char *validity_time,
    char *confidence_level,
    char *accuracy_level
)
{
    OpenAPI_app_specific_expected_ue_behaviour_data_t *app_specific_expected_ue_behaviour_data_local_var = ogs_malloc(sizeof(OpenAPI_app_specific_expected_ue_behaviour_data_t));
    ogs_assert(app_specific_expected_ue_behaviour_data_local_var);

    app_specific_expected_ue_behaviour_data_local_var->app_id = app_id;
    app_specific_expected_ue_behaviour_data_local_var->traffic_filters = traffic_filters;
    app_specific_expected_ue_behaviour_data_local_var->is_expected_inactivity_time = is_expected_inactivity_time;
    app_specific_expected_ue_behaviour_data_local_var->expected_inactivity_time = expected_inactivity_time;
    app_specific_expected_ue_behaviour_data_local_var->validity_time = validity_time;
    app_specific_expected_ue_behaviour_data_local_var->confidence_level = confidence_level;
    app_specific_expected_ue_behaviour_data_local_var->accuracy_level = accuracy_level;

    return app_specific_expected_ue_behaviour_data_local_var;
}

void OpenAPI_app_specific_expected_ue_behaviour_data_free(OpenAPI_app_specific_expected_ue_behaviour_data_t *app_specific_expected_ue_behaviour_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == app_specific_expected_ue_behaviour_data) {
        return;
    }
    if (app_specific_expected_ue_behaviour_data->app_id) {
        ogs_free(app_specific_expected_ue_behaviour_data->app_id);
        app_specific_expected_ue_behaviour_data->app_id = NULL;
    }
    if (app_specific_expected_ue_behaviour_data->traffic_filters) {
        OpenAPI_list_for_each(app_specific_expected_ue_behaviour_data->traffic_filters, node) {
            OpenAPI_flow_info_free(node->data);
        }
        OpenAPI_list_free(app_specific_expected_ue_behaviour_data->traffic_filters);
        app_specific_expected_ue_behaviour_data->traffic_filters = NULL;
    }
    if (app_specific_expected_ue_behaviour_data->validity_time) {
        ogs_free(app_specific_expected_ue_behaviour_data->validity_time);
        app_specific_expected_ue_behaviour_data->validity_time = NULL;
    }
    if (app_specific_expected_ue_behaviour_data->confidence_level) {
        ogs_free(app_specific_expected_ue_behaviour_data->confidence_level);
        app_specific_expected_ue_behaviour_data->confidence_level = NULL;
    }
    if (app_specific_expected_ue_behaviour_data->accuracy_level) {
        ogs_free(app_specific_expected_ue_behaviour_data->accuracy_level);
        app_specific_expected_ue_behaviour_data->accuracy_level = NULL;
    }
    ogs_free(app_specific_expected_ue_behaviour_data);
}

cJSON *OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON(OpenAPI_app_specific_expected_ue_behaviour_data_t *app_specific_expected_ue_behaviour_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (app_specific_expected_ue_behaviour_data == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON() failed [AppSpecificExpectedUeBehaviourData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (app_specific_expected_ue_behaviour_data->app_id) {
    if (cJSON_AddStringToObject(item, "appId", app_specific_expected_ue_behaviour_data->app_id) == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (app_specific_expected_ue_behaviour_data->traffic_filters) {
    cJSON *traffic_filtersList = cJSON_AddArrayToObject(item, "trafficFilters");
    if (traffic_filtersList == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON() failed [traffic_filters]");
        goto end;
    }
    OpenAPI_list_for_each(app_specific_expected_ue_behaviour_data->traffic_filters, node) {
        cJSON *itemLocal = OpenAPI_flow_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON() failed [traffic_filters]");
            goto end;
        }
        cJSON_AddItemToArray(traffic_filtersList, itemLocal);
    }
    }

    if (app_specific_expected_ue_behaviour_data->is_expected_inactivity_time) {
    if (cJSON_AddNumberToObject(item, "expectedInactivityTime", app_specific_expected_ue_behaviour_data->expected_inactivity_time) == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON() failed [expected_inactivity_time]");
        goto end;
    }
    }

    if (app_specific_expected_ue_behaviour_data->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", app_specific_expected_ue_behaviour_data->validity_time) == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (app_specific_expected_ue_behaviour_data->confidence_level) {
    if (cJSON_AddStringToObject(item, "confidenceLevel", app_specific_expected_ue_behaviour_data->confidence_level) == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON() failed [confidence_level]");
        goto end;
    }
    }

    if (app_specific_expected_ue_behaviour_data->accuracy_level) {
    if (cJSON_AddStringToObject(item, "accuracyLevel", app_specific_expected_ue_behaviour_data->accuracy_level) == NULL) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON() failed [accuracy_level]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_app_specific_expected_ue_behaviour_data_t *OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON(cJSON *app_specific_expected_ue_behaviour_dataJSON)
{
    OpenAPI_app_specific_expected_ue_behaviour_data_t *app_specific_expected_ue_behaviour_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_id = NULL;
    cJSON *traffic_filters = NULL;
    OpenAPI_list_t *traffic_filtersList = NULL;
    cJSON *expected_inactivity_time = NULL;
    cJSON *validity_time = NULL;
    cJSON *confidence_level = NULL;
    cJSON *accuracy_level = NULL;
    app_id = cJSON_GetObjectItemCaseSensitive(app_specific_expected_ue_behaviour_dataJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    traffic_filters = cJSON_GetObjectItemCaseSensitive(app_specific_expected_ue_behaviour_dataJSON, "trafficFilters");
    if (traffic_filters) {
        cJSON *traffic_filters_local = NULL;
        if (!cJSON_IsArray(traffic_filters)) {
            ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON() failed [traffic_filters]");
            goto end;
        }

        traffic_filtersList = OpenAPI_list_create();

        cJSON_ArrayForEach(traffic_filters_local, traffic_filters) {
            if (!cJSON_IsObject(traffic_filters_local)) {
                ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON() failed [traffic_filters]");
                goto end;
            }
            OpenAPI_flow_info_t *traffic_filtersItem = OpenAPI_flow_info_parseFromJSON(traffic_filters_local);
            if (!traffic_filtersItem) {
                ogs_error("No traffic_filtersItem");
                goto end;
            }
            OpenAPI_list_add(traffic_filtersList, traffic_filtersItem);
        }
    }

    expected_inactivity_time = cJSON_GetObjectItemCaseSensitive(app_specific_expected_ue_behaviour_dataJSON, "expectedInactivityTime");
    if (expected_inactivity_time) {
    if (!cJSON_IsNumber(expected_inactivity_time)) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON() failed [expected_inactivity_time]");
        goto end;
    }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(app_specific_expected_ue_behaviour_dataJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    confidence_level = cJSON_GetObjectItemCaseSensitive(app_specific_expected_ue_behaviour_dataJSON, "confidenceLevel");
    if (confidence_level) {
    if (!cJSON_IsString(confidence_level) && !cJSON_IsNull(confidence_level)) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON() failed [confidence_level]");
        goto end;
    }
    }

    accuracy_level = cJSON_GetObjectItemCaseSensitive(app_specific_expected_ue_behaviour_dataJSON, "accuracyLevel");
    if (accuracy_level) {
    if (!cJSON_IsString(accuracy_level) && !cJSON_IsNull(accuracy_level)) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON() failed [accuracy_level]");
        goto end;
    }
    }

    app_specific_expected_ue_behaviour_data_local_var = OpenAPI_app_specific_expected_ue_behaviour_data_create (
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        traffic_filters ? traffic_filtersList : NULL,
        expected_inactivity_time ? true : false,
        expected_inactivity_time ? expected_inactivity_time->valuedouble : 0,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL,
        confidence_level && !cJSON_IsNull(confidence_level) ? ogs_strdup(confidence_level->valuestring) : NULL,
        accuracy_level && !cJSON_IsNull(accuracy_level) ? ogs_strdup(accuracy_level->valuestring) : NULL
    );

    return app_specific_expected_ue_behaviour_data_local_var;
end:
    if (traffic_filtersList) {
        OpenAPI_list_for_each(traffic_filtersList, node) {
            OpenAPI_flow_info_free(node->data);
        }
        OpenAPI_list_free(traffic_filtersList);
        traffic_filtersList = NULL;
    }
    return NULL;
}

OpenAPI_app_specific_expected_ue_behaviour_data_t *OpenAPI_app_specific_expected_ue_behaviour_data_copy(OpenAPI_app_specific_expected_ue_behaviour_data_t *dst, OpenAPI_app_specific_expected_ue_behaviour_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON() failed");
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

    OpenAPI_app_specific_expected_ue_behaviour_data_free(dst);
    dst = OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


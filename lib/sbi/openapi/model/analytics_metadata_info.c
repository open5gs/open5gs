
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_metadata_info.h"

OpenAPI_analytics_metadata_info_t *OpenAPI_analytics_metadata_info_create(
    bool is_num_samples,
    int num_samples,
    OpenAPI_time_window_t *data_window,
    OpenAPI_list_t *data_stat_props,
    OpenAPI_output_strategy_t *strategy,
    OpenAPI_accuracy_t *accuracy
)
{
    OpenAPI_analytics_metadata_info_t *analytics_metadata_info_local_var = ogs_malloc(sizeof(OpenAPI_analytics_metadata_info_t));
    ogs_assert(analytics_metadata_info_local_var);

    analytics_metadata_info_local_var->is_num_samples = is_num_samples;
    analytics_metadata_info_local_var->num_samples = num_samples;
    analytics_metadata_info_local_var->data_window = data_window;
    analytics_metadata_info_local_var->data_stat_props = data_stat_props;
    analytics_metadata_info_local_var->strategy = strategy;
    analytics_metadata_info_local_var->accuracy = accuracy;

    return analytics_metadata_info_local_var;
}

void OpenAPI_analytics_metadata_info_free(OpenAPI_analytics_metadata_info_t *analytics_metadata_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == analytics_metadata_info) {
        return;
    }
    if (analytics_metadata_info->data_window) {
        OpenAPI_time_window_free(analytics_metadata_info->data_window);
        analytics_metadata_info->data_window = NULL;
    }
    if (analytics_metadata_info->data_stat_props) {
        OpenAPI_list_for_each(analytics_metadata_info->data_stat_props, node) {
            OpenAPI_dataset_statistical_property_free(node->data);
        }
        OpenAPI_list_free(analytics_metadata_info->data_stat_props);
        analytics_metadata_info->data_stat_props = NULL;
    }
    if (analytics_metadata_info->strategy) {
        OpenAPI_output_strategy_free(analytics_metadata_info->strategy);
        analytics_metadata_info->strategy = NULL;
    }
    if (analytics_metadata_info->accuracy) {
        OpenAPI_accuracy_free(analytics_metadata_info->accuracy);
        analytics_metadata_info->accuracy = NULL;
    }
    ogs_free(analytics_metadata_info);
}

cJSON *OpenAPI_analytics_metadata_info_convertToJSON(OpenAPI_analytics_metadata_info_t *analytics_metadata_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (analytics_metadata_info == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [AnalyticsMetadataInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (analytics_metadata_info->is_num_samples) {
    if (cJSON_AddNumberToObject(item, "numSamples", analytics_metadata_info->num_samples) == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [num_samples]");
        goto end;
    }
    }

    if (analytics_metadata_info->data_window) {
    cJSON *data_window_local_JSON = OpenAPI_time_window_convertToJSON(analytics_metadata_info->data_window);
    if (data_window_local_JSON == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [data_window]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dataWindow", data_window_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [data_window]");
        goto end;
    }
    }

    if (analytics_metadata_info->data_stat_props) {
    cJSON *data_stat_propsList = cJSON_AddArrayToObject(item, "dataStatProps");
    if (data_stat_propsList == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [data_stat_props]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_metadata_info->data_stat_props, node) {
        cJSON *itemLocal = OpenAPI_dataset_statistical_property_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [data_stat_props]");
            goto end;
        }
        cJSON_AddItemToArray(data_stat_propsList, itemLocal);
    }
    }

    if (analytics_metadata_info->strategy) {
    cJSON *strategy_local_JSON = OpenAPI_output_strategy_convertToJSON(analytics_metadata_info->strategy);
    if (strategy_local_JSON == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [strategy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "strategy", strategy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [strategy]");
        goto end;
    }
    }

    if (analytics_metadata_info->accuracy) {
    cJSON *accuracy_local_JSON = OpenAPI_accuracy_convertToJSON(analytics_metadata_info->accuracy);
    if (accuracy_local_JSON == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [accuracy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accuracy", accuracy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [accuracy]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_analytics_metadata_info_t *OpenAPI_analytics_metadata_info_parseFromJSON(cJSON *analytics_metadata_infoJSON)
{
    OpenAPI_analytics_metadata_info_t *analytics_metadata_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *num_samples = NULL;
    cJSON *data_window = NULL;
    OpenAPI_time_window_t *data_window_local_nonprim = NULL;
    cJSON *data_stat_props = NULL;
    OpenAPI_list_t *data_stat_propsList = NULL;
    cJSON *strategy = NULL;
    OpenAPI_output_strategy_t *strategy_local_nonprim = NULL;
    cJSON *accuracy = NULL;
    OpenAPI_accuracy_t *accuracy_local_nonprim = NULL;
    num_samples = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "numSamples");
    if (num_samples) {
    if (!cJSON_IsNumber(num_samples)) {
        ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [num_samples]");
        goto end;
    }
    }

    data_window = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "dataWindow");
    if (data_window) {
    data_window_local_nonprim = OpenAPI_time_window_parseFromJSON(data_window);
    if (!data_window_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [data_window]");
        goto end;
    }
    }

    data_stat_props = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "dataStatProps");
    if (data_stat_props) {
        cJSON *data_stat_props_local = NULL;
        if (!cJSON_IsArray(data_stat_props)) {
            ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [data_stat_props]");
            goto end;
        }

        data_stat_propsList = OpenAPI_list_create();

        cJSON_ArrayForEach(data_stat_props_local, data_stat_props) {
            if (!cJSON_IsObject(data_stat_props_local)) {
                ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [data_stat_props]");
                goto end;
            }
            OpenAPI_dataset_statistical_property_t *data_stat_propsItem = OpenAPI_dataset_statistical_property_parseFromJSON(data_stat_props_local);
            if (!data_stat_propsItem) {
                ogs_error("No data_stat_propsItem");
                goto end;
            }
            OpenAPI_list_add(data_stat_propsList, data_stat_propsItem);
        }
    }

    strategy = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "strategy");
    if (strategy) {
    strategy_local_nonprim = OpenAPI_output_strategy_parseFromJSON(strategy);
    if (!strategy_local_nonprim) {
        ogs_error("OpenAPI_output_strategy_parseFromJSON failed [strategy]");
        goto end;
    }
    }

    accuracy = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "accuracy");
    if (accuracy) {
    accuracy_local_nonprim = OpenAPI_accuracy_parseFromJSON(accuracy);
    if (!accuracy_local_nonprim) {
        ogs_error("OpenAPI_accuracy_parseFromJSON failed [accuracy]");
        goto end;
    }
    }

    analytics_metadata_info_local_var = OpenAPI_analytics_metadata_info_create (
        num_samples ? true : false,
        num_samples ? num_samples->valuedouble : 0,
        data_window ? data_window_local_nonprim : NULL,
        data_stat_props ? data_stat_propsList : NULL,
        strategy ? strategy_local_nonprim : NULL,
        accuracy ? accuracy_local_nonprim : NULL
    );

    return analytics_metadata_info_local_var;
end:
    if (data_window_local_nonprim) {
        OpenAPI_time_window_free(data_window_local_nonprim);
        data_window_local_nonprim = NULL;
    }
    if (data_stat_propsList) {
        OpenAPI_list_for_each(data_stat_propsList, node) {
            OpenAPI_dataset_statistical_property_free(node->data);
        }
        OpenAPI_list_free(data_stat_propsList);
        data_stat_propsList = NULL;
    }
    if (strategy_local_nonprim) {
        OpenAPI_output_strategy_free(strategy_local_nonprim);
        strategy_local_nonprim = NULL;
    }
    if (accuracy_local_nonprim) {
        OpenAPI_accuracy_free(accuracy_local_nonprim);
        accuracy_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_analytics_metadata_info_t *OpenAPI_analytics_metadata_info_copy(OpenAPI_analytics_metadata_info_t *dst, OpenAPI_analytics_metadata_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_analytics_metadata_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed");
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

    OpenAPI_analytics_metadata_info_free(dst);
    dst = OpenAPI_analytics_metadata_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


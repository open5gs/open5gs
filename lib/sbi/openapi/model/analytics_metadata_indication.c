
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_metadata_indication.h"

OpenAPI_analytics_metadata_indication_t *OpenAPI_analytics_metadata_indication_create(
    OpenAPI_time_window_t *data_window,
    OpenAPI_list_t *data_stat_props,
    OpenAPI_output_strategy_t *strategy,
    OpenAPI_list_t *aggr_nwdaf_ids
)
{
    OpenAPI_analytics_metadata_indication_t *analytics_metadata_indication_local_var = ogs_malloc(sizeof(OpenAPI_analytics_metadata_indication_t));
    ogs_assert(analytics_metadata_indication_local_var);

    analytics_metadata_indication_local_var->data_window = data_window;
    analytics_metadata_indication_local_var->data_stat_props = data_stat_props;
    analytics_metadata_indication_local_var->strategy = strategy;
    analytics_metadata_indication_local_var->aggr_nwdaf_ids = aggr_nwdaf_ids;

    return analytics_metadata_indication_local_var;
}

void OpenAPI_analytics_metadata_indication_free(OpenAPI_analytics_metadata_indication_t *analytics_metadata_indication)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == analytics_metadata_indication) {
        return;
    }
    if (analytics_metadata_indication->data_window) {
        OpenAPI_time_window_free(analytics_metadata_indication->data_window);
        analytics_metadata_indication->data_window = NULL;
    }
    if (analytics_metadata_indication->data_stat_props) {
        OpenAPI_list_for_each(analytics_metadata_indication->data_stat_props, node) {
            OpenAPI_dataset_statistical_property_free(node->data);
        }
        OpenAPI_list_free(analytics_metadata_indication->data_stat_props);
        analytics_metadata_indication->data_stat_props = NULL;
    }
    if (analytics_metadata_indication->strategy) {
        OpenAPI_output_strategy_free(analytics_metadata_indication->strategy);
        analytics_metadata_indication->strategy = NULL;
    }
    if (analytics_metadata_indication->aggr_nwdaf_ids) {
        OpenAPI_list_for_each(analytics_metadata_indication->aggr_nwdaf_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(analytics_metadata_indication->aggr_nwdaf_ids);
        analytics_metadata_indication->aggr_nwdaf_ids = NULL;
    }
    ogs_free(analytics_metadata_indication);
}

cJSON *OpenAPI_analytics_metadata_indication_convertToJSON(OpenAPI_analytics_metadata_indication_t *analytics_metadata_indication)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (analytics_metadata_indication == NULL) {
        ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed [AnalyticsMetadataIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (analytics_metadata_indication->data_window) {
    cJSON *data_window_local_JSON = OpenAPI_time_window_convertToJSON(analytics_metadata_indication->data_window);
    if (data_window_local_JSON == NULL) {
        ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed [data_window]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dataWindow", data_window_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed [data_window]");
        goto end;
    }
    }

    if (analytics_metadata_indication->data_stat_props) {
    cJSON *data_stat_propsList = cJSON_AddArrayToObject(item, "dataStatProps");
    if (data_stat_propsList == NULL) {
        ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed [data_stat_props]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_metadata_indication->data_stat_props, node) {
        cJSON *itemLocal = OpenAPI_dataset_statistical_property_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed [data_stat_props]");
            goto end;
        }
        cJSON_AddItemToArray(data_stat_propsList, itemLocal);
    }
    }

    if (analytics_metadata_indication->strategy) {
    cJSON *strategy_local_JSON = OpenAPI_output_strategy_convertToJSON(analytics_metadata_indication->strategy);
    if (strategy_local_JSON == NULL) {
        ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed [strategy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "strategy", strategy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed [strategy]");
        goto end;
    }
    }

    if (analytics_metadata_indication->aggr_nwdaf_ids) {
    cJSON *aggr_nwdaf_idsList = cJSON_AddArrayToObject(item, "aggrNwdafIds");
    if (aggr_nwdaf_idsList == NULL) {
        ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed [aggr_nwdaf_ids]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_metadata_indication->aggr_nwdaf_ids, node) {
        if (cJSON_AddStringToObject(aggr_nwdaf_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed [aggr_nwdaf_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_analytics_metadata_indication_t *OpenAPI_analytics_metadata_indication_parseFromJSON(cJSON *analytics_metadata_indicationJSON)
{
    OpenAPI_analytics_metadata_indication_t *analytics_metadata_indication_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *data_window = NULL;
    OpenAPI_time_window_t *data_window_local_nonprim = NULL;
    cJSON *data_stat_props = NULL;
    OpenAPI_list_t *data_stat_propsList = NULL;
    cJSON *strategy = NULL;
    OpenAPI_output_strategy_t *strategy_local_nonprim = NULL;
    cJSON *aggr_nwdaf_ids = NULL;
    OpenAPI_list_t *aggr_nwdaf_idsList = NULL;
    data_window = cJSON_GetObjectItemCaseSensitive(analytics_metadata_indicationJSON, "dataWindow");
    if (data_window) {
    data_window_local_nonprim = OpenAPI_time_window_parseFromJSON(data_window);
    if (!data_window_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [data_window]");
        goto end;
    }
    }

    data_stat_props = cJSON_GetObjectItemCaseSensitive(analytics_metadata_indicationJSON, "dataStatProps");
    if (data_stat_props) {
        cJSON *data_stat_props_local = NULL;
        if (!cJSON_IsArray(data_stat_props)) {
            ogs_error("OpenAPI_analytics_metadata_indication_parseFromJSON() failed [data_stat_props]");
            goto end;
        }

        data_stat_propsList = OpenAPI_list_create();

        cJSON_ArrayForEach(data_stat_props_local, data_stat_props) {
            if (!cJSON_IsObject(data_stat_props_local)) {
                ogs_error("OpenAPI_analytics_metadata_indication_parseFromJSON() failed [data_stat_props]");
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

    strategy = cJSON_GetObjectItemCaseSensitive(analytics_metadata_indicationJSON, "strategy");
    if (strategy) {
    strategy_local_nonprim = OpenAPI_output_strategy_parseFromJSON(strategy);
    if (!strategy_local_nonprim) {
        ogs_error("OpenAPI_output_strategy_parseFromJSON failed [strategy]");
        goto end;
    }
    }

    aggr_nwdaf_ids = cJSON_GetObjectItemCaseSensitive(analytics_metadata_indicationJSON, "aggrNwdafIds");
    if (aggr_nwdaf_ids) {
        cJSON *aggr_nwdaf_ids_local = NULL;
        if (!cJSON_IsArray(aggr_nwdaf_ids)) {
            ogs_error("OpenAPI_analytics_metadata_indication_parseFromJSON() failed [aggr_nwdaf_ids]");
            goto end;
        }

        aggr_nwdaf_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(aggr_nwdaf_ids_local, aggr_nwdaf_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(aggr_nwdaf_ids_local)) {
                ogs_error("OpenAPI_analytics_metadata_indication_parseFromJSON() failed [aggr_nwdaf_ids]");
                goto end;
            }
            OpenAPI_list_add(aggr_nwdaf_idsList, ogs_strdup(aggr_nwdaf_ids_local->valuestring));
        }
    }

    analytics_metadata_indication_local_var = OpenAPI_analytics_metadata_indication_create (
        data_window ? data_window_local_nonprim : NULL,
        data_stat_props ? data_stat_propsList : NULL,
        strategy ? strategy_local_nonprim : NULL,
        aggr_nwdaf_ids ? aggr_nwdaf_idsList : NULL
    );

    return analytics_metadata_indication_local_var;
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
    if (aggr_nwdaf_idsList) {
        OpenAPI_list_for_each(aggr_nwdaf_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(aggr_nwdaf_idsList);
        aggr_nwdaf_idsList = NULL;
    }
    return NULL;
}

OpenAPI_analytics_metadata_indication_t *OpenAPI_analytics_metadata_indication_copy(OpenAPI_analytics_metadata_indication_t *dst, OpenAPI_analytics_metadata_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_analytics_metadata_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_analytics_metadata_indication_convertToJSON() failed");
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

    OpenAPI_analytics_metadata_indication_free(dst);
    dst = OpenAPI_analytics_metadata_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


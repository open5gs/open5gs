
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "analytics_metadata_info.h"

OpenAPI_analytics_metadata_info_t *OpenAPI_analytics_metadata_info_create(
    bool is_num_samples,
    int num_samples,
    OpenAPI_time_window_t *data_window,
    OpenAPI_list_t *data_stat_props,
    OpenAPI_output_strategy_e strategy,
    OpenAPI_accuracy_e accuracy,
    OpenAPI_list_t *nf_ids,
    OpenAPI_list_t *nf_set_ids,
    OpenAPI_list_t *proc_instructs
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
    analytics_metadata_info_local_var->nf_ids = nf_ids;
    analytics_metadata_info_local_var->nf_set_ids = nf_set_ids;
    analytics_metadata_info_local_var->proc_instructs = proc_instructs;

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
        OpenAPI_list_free(analytics_metadata_info->data_stat_props);
        analytics_metadata_info->data_stat_props = NULL;
    }
    if (analytics_metadata_info->nf_ids) {
        OpenAPI_list_for_each(analytics_metadata_info->nf_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(analytics_metadata_info->nf_ids);
        analytics_metadata_info->nf_ids = NULL;
    }
    if (analytics_metadata_info->nf_set_ids) {
        OpenAPI_list_for_each(analytics_metadata_info->nf_set_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(analytics_metadata_info->nf_set_ids);
        analytics_metadata_info->nf_set_ids = NULL;
    }
    if (analytics_metadata_info->proc_instructs) {
        OpenAPI_list_for_each(analytics_metadata_info->proc_instructs, node) {
            OpenAPI_processing_instruction_free(node->data);
        }
        OpenAPI_list_free(analytics_metadata_info->proc_instructs);
        analytics_metadata_info->proc_instructs = NULL;
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

    if (analytics_metadata_info->data_stat_props != OpenAPI_dataset_statistical_property_NULL) {
    cJSON *data_stat_propsList = cJSON_AddArrayToObject(item, "dataStatProps");
    if (data_stat_propsList == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [data_stat_props]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_metadata_info->data_stat_props, node) {
        if (cJSON_AddStringToObject(data_stat_propsList, "", OpenAPI_dataset_statistical_property_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [data_stat_props]");
            goto end;
        }
    }
    }

    if (analytics_metadata_info->strategy != OpenAPI_output_strategy_NULL) {
    if (cJSON_AddStringToObject(item, "strategy", OpenAPI_output_strategy_ToString(analytics_metadata_info->strategy)) == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [strategy]");
        goto end;
    }
    }

    if (analytics_metadata_info->accuracy != OpenAPI_accuracy_NULL) {
    if (cJSON_AddStringToObject(item, "accuracy", OpenAPI_accuracy_ToString(analytics_metadata_info->accuracy)) == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [accuracy]");
        goto end;
    }
    }

    if (analytics_metadata_info->nf_ids) {
    cJSON *nf_idsList = cJSON_AddArrayToObject(item, "nfIds");
    if (nf_idsList == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [nf_ids]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_metadata_info->nf_ids, node) {
        if (cJSON_AddStringToObject(nf_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [nf_ids]");
            goto end;
        }
    }
    }

    if (analytics_metadata_info->nf_set_ids) {
    cJSON *nf_set_idsList = cJSON_AddArrayToObject(item, "nfSetIds");
    if (nf_set_idsList == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [nf_set_ids]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_metadata_info->nf_set_ids, node) {
        if (cJSON_AddStringToObject(nf_set_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [nf_set_ids]");
            goto end;
        }
    }
    }

    if (analytics_metadata_info->proc_instructs) {
    cJSON *proc_instructsList = cJSON_AddArrayToObject(item, "procInstructs");
    if (proc_instructsList == NULL) {
        ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [proc_instructs]");
        goto end;
    }
    OpenAPI_list_for_each(analytics_metadata_info->proc_instructs, node) {
        cJSON *itemLocal = OpenAPI_processing_instruction_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_analytics_metadata_info_convertToJSON() failed [proc_instructs]");
            goto end;
        }
        cJSON_AddItemToArray(proc_instructsList, itemLocal);
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
    OpenAPI_output_strategy_e strategyVariable = 0;
    cJSON *accuracy = NULL;
    OpenAPI_accuracy_e accuracyVariable = 0;
    cJSON *nf_ids = NULL;
    OpenAPI_list_t *nf_idsList = NULL;
    cJSON *nf_set_ids = NULL;
    OpenAPI_list_t *nf_set_idsList = NULL;
    cJSON *proc_instructs = NULL;
    OpenAPI_list_t *proc_instructsList = NULL;
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
            OpenAPI_dataset_statistical_property_e localEnum = OpenAPI_dataset_statistical_property_NULL;
            if (!cJSON_IsString(data_stat_props_local)) {
                ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [data_stat_props]");
                goto end;
            }
            localEnum = OpenAPI_dataset_statistical_property_FromString(data_stat_props_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"data_stat_props\" is not supported. Ignoring it ...",
                         data_stat_props_local->valuestring);
            } else {
                OpenAPI_list_add(data_stat_propsList, (void *)localEnum);
            }
        }
        if (data_stat_propsList->count == 0) {
            ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed: Expected data_stat_propsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    strategy = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "strategy");
    if (strategy) {
    if (!cJSON_IsString(strategy)) {
        ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [strategy]");
        goto end;
    }
    strategyVariable = OpenAPI_output_strategy_FromString(strategy->valuestring);
    }

    accuracy = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "accuracy");
    if (accuracy) {
    if (!cJSON_IsString(accuracy)) {
        ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [accuracy]");
        goto end;
    }
    accuracyVariable = OpenAPI_accuracy_FromString(accuracy->valuestring);
    }

    nf_ids = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "nfIds");
    if (nf_ids) {
        cJSON *nf_ids_local = NULL;
        if (!cJSON_IsArray(nf_ids)) {
            ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [nf_ids]");
            goto end;
        }

        nf_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_ids_local, nf_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_ids_local)) {
                ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [nf_ids]");
                goto end;
            }
            OpenAPI_list_add(nf_idsList, ogs_strdup(nf_ids_local->valuestring));
        }
    }

    nf_set_ids = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "nfSetIds");
    if (nf_set_ids) {
        cJSON *nf_set_ids_local = NULL;
        if (!cJSON_IsArray(nf_set_ids)) {
            ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [nf_set_ids]");
            goto end;
        }

        nf_set_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_set_ids_local, nf_set_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_set_ids_local)) {
                ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [nf_set_ids]");
                goto end;
            }
            OpenAPI_list_add(nf_set_idsList, ogs_strdup(nf_set_ids_local->valuestring));
        }
    }

    proc_instructs = cJSON_GetObjectItemCaseSensitive(analytics_metadata_infoJSON, "procInstructs");
    if (proc_instructs) {
        cJSON *proc_instructs_local = NULL;
        if (!cJSON_IsArray(proc_instructs)) {
            ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [proc_instructs]");
            goto end;
        }

        proc_instructsList = OpenAPI_list_create();

        cJSON_ArrayForEach(proc_instructs_local, proc_instructs) {
            if (!cJSON_IsObject(proc_instructs_local)) {
                ogs_error("OpenAPI_analytics_metadata_info_parseFromJSON() failed [proc_instructs]");
                goto end;
            }
            OpenAPI_processing_instruction_t *proc_instructsItem = OpenAPI_processing_instruction_parseFromJSON(proc_instructs_local);
            if (!proc_instructsItem) {
                ogs_error("No proc_instructsItem");
                goto end;
            }
            OpenAPI_list_add(proc_instructsList, proc_instructsItem);
        }
    }

    analytics_metadata_info_local_var = OpenAPI_analytics_metadata_info_create (
        num_samples ? true : false,
        num_samples ? num_samples->valuedouble : 0,
        data_window ? data_window_local_nonprim : NULL,
        data_stat_props ? data_stat_propsList : NULL,
        strategy ? strategyVariable : 0,
        accuracy ? accuracyVariable : 0,
        nf_ids ? nf_idsList : NULL,
        nf_set_ids ? nf_set_idsList : NULL,
        proc_instructs ? proc_instructsList : NULL
    );

    return analytics_metadata_info_local_var;
end:
    if (data_window_local_nonprim) {
        OpenAPI_time_window_free(data_window_local_nonprim);
        data_window_local_nonprim = NULL;
    }
    if (data_stat_propsList) {
        OpenAPI_list_free(data_stat_propsList);
        data_stat_propsList = NULL;
    }
    if (nf_idsList) {
        OpenAPI_list_for_each(nf_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_idsList);
        nf_idsList = NULL;
    }
    if (nf_set_idsList) {
        OpenAPI_list_for_each(nf_set_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_set_idsList);
        nf_set_idsList = NULL;
    }
    if (proc_instructsList) {
        OpenAPI_list_for_each(proc_instructsList, node) {
            OpenAPI_processing_instruction_free(node->data);
        }
        OpenAPI_list_free(proc_instructsList);
        proc_instructsList = NULL;
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


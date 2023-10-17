
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_reporting_requirement.h"

OpenAPI_event_reporting_requirement_t *OpenAPI_event_reporting_requirement_create(
    OpenAPI_accuracy_t *accuracy,
    OpenAPI_list_t *acc_per_subset,
    char *start_ts,
    char *end_ts,
    bool is_offset_period,
    int offset_period,
    bool is_samp_ratio,
    int samp_ratio,
    bool is_max_object_nbr,
    int max_object_nbr,
    bool is_max_supi_nbr,
    int max_supi_nbr,
    char *time_ana_needed,
    OpenAPI_list_t *ana_meta,
    OpenAPI_analytics_metadata_indication_t *ana_meta_ind
)
{
    OpenAPI_event_reporting_requirement_t *event_reporting_requirement_local_var = ogs_malloc(sizeof(OpenAPI_event_reporting_requirement_t));
    ogs_assert(event_reporting_requirement_local_var);

    event_reporting_requirement_local_var->accuracy = accuracy;
    event_reporting_requirement_local_var->acc_per_subset = acc_per_subset;
    event_reporting_requirement_local_var->start_ts = start_ts;
    event_reporting_requirement_local_var->end_ts = end_ts;
    event_reporting_requirement_local_var->is_offset_period = is_offset_period;
    event_reporting_requirement_local_var->offset_period = offset_period;
    event_reporting_requirement_local_var->is_samp_ratio = is_samp_ratio;
    event_reporting_requirement_local_var->samp_ratio = samp_ratio;
    event_reporting_requirement_local_var->is_max_object_nbr = is_max_object_nbr;
    event_reporting_requirement_local_var->max_object_nbr = max_object_nbr;
    event_reporting_requirement_local_var->is_max_supi_nbr = is_max_supi_nbr;
    event_reporting_requirement_local_var->max_supi_nbr = max_supi_nbr;
    event_reporting_requirement_local_var->time_ana_needed = time_ana_needed;
    event_reporting_requirement_local_var->ana_meta = ana_meta;
    event_reporting_requirement_local_var->ana_meta_ind = ana_meta_ind;

    return event_reporting_requirement_local_var;
}

void OpenAPI_event_reporting_requirement_free(OpenAPI_event_reporting_requirement_t *event_reporting_requirement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == event_reporting_requirement) {
        return;
    }
    if (event_reporting_requirement->accuracy) {
        OpenAPI_accuracy_free(event_reporting_requirement->accuracy);
        event_reporting_requirement->accuracy = NULL;
    }
    if (event_reporting_requirement->acc_per_subset) {
        OpenAPI_list_for_each(event_reporting_requirement->acc_per_subset, node) {
            OpenAPI_accuracy_free(node->data);
        }
        OpenAPI_list_free(event_reporting_requirement->acc_per_subset);
        event_reporting_requirement->acc_per_subset = NULL;
    }
    if (event_reporting_requirement->start_ts) {
        ogs_free(event_reporting_requirement->start_ts);
        event_reporting_requirement->start_ts = NULL;
    }
    if (event_reporting_requirement->end_ts) {
        ogs_free(event_reporting_requirement->end_ts);
        event_reporting_requirement->end_ts = NULL;
    }
    if (event_reporting_requirement->time_ana_needed) {
        ogs_free(event_reporting_requirement->time_ana_needed);
        event_reporting_requirement->time_ana_needed = NULL;
    }
    if (event_reporting_requirement->ana_meta) {
        OpenAPI_list_for_each(event_reporting_requirement->ana_meta, node) {
            OpenAPI_analytics_metadata_free(node->data);
        }
        OpenAPI_list_free(event_reporting_requirement->ana_meta);
        event_reporting_requirement->ana_meta = NULL;
    }
    if (event_reporting_requirement->ana_meta_ind) {
        OpenAPI_analytics_metadata_indication_free(event_reporting_requirement->ana_meta_ind);
        event_reporting_requirement->ana_meta_ind = NULL;
    }
    ogs_free(event_reporting_requirement);
}

cJSON *OpenAPI_event_reporting_requirement_convertToJSON(OpenAPI_event_reporting_requirement_t *event_reporting_requirement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (event_reporting_requirement == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [EventReportingRequirement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (event_reporting_requirement->accuracy) {
    cJSON *accuracy_local_JSON = OpenAPI_accuracy_convertToJSON(event_reporting_requirement->accuracy);
    if (accuracy_local_JSON == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [accuracy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accuracy", accuracy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [accuracy]");
        goto end;
    }
    }

    if (event_reporting_requirement->acc_per_subset) {
    cJSON *acc_per_subsetList = cJSON_AddArrayToObject(item, "accPerSubset");
    if (acc_per_subsetList == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [acc_per_subset]");
        goto end;
    }
    OpenAPI_list_for_each(event_reporting_requirement->acc_per_subset, node) {
        cJSON *itemLocal = OpenAPI_accuracy_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [acc_per_subset]");
            goto end;
        }
        cJSON_AddItemToArray(acc_per_subsetList, itemLocal);
    }
    }

    if (event_reporting_requirement->start_ts) {
    if (cJSON_AddStringToObject(item, "startTs", event_reporting_requirement->start_ts) == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [start_ts]");
        goto end;
    }
    }

    if (event_reporting_requirement->end_ts) {
    if (cJSON_AddStringToObject(item, "endTs", event_reporting_requirement->end_ts) == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [end_ts]");
        goto end;
    }
    }

    if (event_reporting_requirement->is_offset_period) {
    if (cJSON_AddNumberToObject(item, "offsetPeriod", event_reporting_requirement->offset_period) == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [offset_period]");
        goto end;
    }
    }

    if (event_reporting_requirement->is_samp_ratio) {
    if (cJSON_AddNumberToObject(item, "sampRatio", event_reporting_requirement->samp_ratio) == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [samp_ratio]");
        goto end;
    }
    }

    if (event_reporting_requirement->is_max_object_nbr) {
    if (cJSON_AddNumberToObject(item, "maxObjectNbr", event_reporting_requirement->max_object_nbr) == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [max_object_nbr]");
        goto end;
    }
    }

    if (event_reporting_requirement->is_max_supi_nbr) {
    if (cJSON_AddNumberToObject(item, "maxSupiNbr", event_reporting_requirement->max_supi_nbr) == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [max_supi_nbr]");
        goto end;
    }
    }

    if (event_reporting_requirement->time_ana_needed) {
    if (cJSON_AddStringToObject(item, "timeAnaNeeded", event_reporting_requirement->time_ana_needed) == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [time_ana_needed]");
        goto end;
    }
    }

    if (event_reporting_requirement->ana_meta) {
    cJSON *ana_metaList = cJSON_AddArrayToObject(item, "anaMeta");
    if (ana_metaList == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [ana_meta]");
        goto end;
    }
    OpenAPI_list_for_each(event_reporting_requirement->ana_meta, node) {
        cJSON *itemLocal = OpenAPI_analytics_metadata_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [ana_meta]");
            goto end;
        }
        cJSON_AddItemToArray(ana_metaList, itemLocal);
    }
    }

    if (event_reporting_requirement->ana_meta_ind) {
    cJSON *ana_meta_ind_local_JSON = OpenAPI_analytics_metadata_indication_convertToJSON(event_reporting_requirement->ana_meta_ind);
    if (ana_meta_ind_local_JSON == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [ana_meta_ind]");
        goto end;
    }
    cJSON_AddItemToObject(item, "anaMetaInd", ana_meta_ind_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed [ana_meta_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_event_reporting_requirement_t *OpenAPI_event_reporting_requirement_parseFromJSON(cJSON *event_reporting_requirementJSON)
{
    OpenAPI_event_reporting_requirement_t *event_reporting_requirement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *accuracy = NULL;
    OpenAPI_accuracy_t *accuracy_local_nonprim = NULL;
    cJSON *acc_per_subset = NULL;
    OpenAPI_list_t *acc_per_subsetList = NULL;
    cJSON *start_ts = NULL;
    cJSON *end_ts = NULL;
    cJSON *offset_period = NULL;
    cJSON *samp_ratio = NULL;
    cJSON *max_object_nbr = NULL;
    cJSON *max_supi_nbr = NULL;
    cJSON *time_ana_needed = NULL;
    cJSON *ana_meta = NULL;
    OpenAPI_list_t *ana_metaList = NULL;
    cJSON *ana_meta_ind = NULL;
    OpenAPI_analytics_metadata_indication_t *ana_meta_ind_local_nonprim = NULL;
    accuracy = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "accuracy");
    if (accuracy) {
    accuracy_local_nonprim = OpenAPI_accuracy_parseFromJSON(accuracy);
    if (!accuracy_local_nonprim) {
        ogs_error("OpenAPI_accuracy_parseFromJSON failed [accuracy]");
        goto end;
    }
    }

    acc_per_subset = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "accPerSubset");
    if (acc_per_subset) {
        cJSON *acc_per_subset_local = NULL;
        if (!cJSON_IsArray(acc_per_subset)) {
            ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [acc_per_subset]");
            goto end;
        }

        acc_per_subsetList = OpenAPI_list_create();

        cJSON_ArrayForEach(acc_per_subset_local, acc_per_subset) {
            if (!cJSON_IsObject(acc_per_subset_local)) {
                ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [acc_per_subset]");
                goto end;
            }
            OpenAPI_accuracy_t *acc_per_subsetItem = OpenAPI_accuracy_parseFromJSON(acc_per_subset_local);
            if (!acc_per_subsetItem) {
                ogs_error("No acc_per_subsetItem");
                goto end;
            }
            OpenAPI_list_add(acc_per_subsetList, acc_per_subsetItem);
        }
    }

    start_ts = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "startTs");
    if (start_ts) {
    if (!cJSON_IsString(start_ts) && !cJSON_IsNull(start_ts)) {
        ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [start_ts]");
        goto end;
    }
    }

    end_ts = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "endTs");
    if (end_ts) {
    if (!cJSON_IsString(end_ts) && !cJSON_IsNull(end_ts)) {
        ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [end_ts]");
        goto end;
    }
    }

    offset_period = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "offsetPeriod");
    if (offset_period) {
    if (!cJSON_IsNumber(offset_period)) {
        ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [offset_period]");
        goto end;
    }
    }

    samp_ratio = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "sampRatio");
    if (samp_ratio) {
    if (!cJSON_IsNumber(samp_ratio)) {
        ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [samp_ratio]");
        goto end;
    }
    }

    max_object_nbr = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "maxObjectNbr");
    if (max_object_nbr) {
    if (!cJSON_IsNumber(max_object_nbr)) {
        ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [max_object_nbr]");
        goto end;
    }
    }

    max_supi_nbr = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "maxSupiNbr");
    if (max_supi_nbr) {
    if (!cJSON_IsNumber(max_supi_nbr)) {
        ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [max_supi_nbr]");
        goto end;
    }
    }

    time_ana_needed = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "timeAnaNeeded");
    if (time_ana_needed) {
    if (!cJSON_IsString(time_ana_needed) && !cJSON_IsNull(time_ana_needed)) {
        ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [time_ana_needed]");
        goto end;
    }
    }

    ana_meta = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "anaMeta");
    if (ana_meta) {
        cJSON *ana_meta_local = NULL;
        if (!cJSON_IsArray(ana_meta)) {
            ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [ana_meta]");
            goto end;
        }

        ana_metaList = OpenAPI_list_create();

        cJSON_ArrayForEach(ana_meta_local, ana_meta) {
            if (!cJSON_IsObject(ana_meta_local)) {
                ogs_error("OpenAPI_event_reporting_requirement_parseFromJSON() failed [ana_meta]");
                goto end;
            }
            OpenAPI_analytics_metadata_t *ana_metaItem = OpenAPI_analytics_metadata_parseFromJSON(ana_meta_local);
            if (!ana_metaItem) {
                ogs_error("No ana_metaItem");
                goto end;
            }
            OpenAPI_list_add(ana_metaList, ana_metaItem);
        }
    }

    ana_meta_ind = cJSON_GetObjectItemCaseSensitive(event_reporting_requirementJSON, "anaMetaInd");
    if (ana_meta_ind) {
    ana_meta_ind_local_nonprim = OpenAPI_analytics_metadata_indication_parseFromJSON(ana_meta_ind);
    if (!ana_meta_ind_local_nonprim) {
        ogs_error("OpenAPI_analytics_metadata_indication_parseFromJSON failed [ana_meta_ind]");
        goto end;
    }
    }

    event_reporting_requirement_local_var = OpenAPI_event_reporting_requirement_create (
        accuracy ? accuracy_local_nonprim : NULL,
        acc_per_subset ? acc_per_subsetList : NULL,
        start_ts && !cJSON_IsNull(start_ts) ? ogs_strdup(start_ts->valuestring) : NULL,
        end_ts && !cJSON_IsNull(end_ts) ? ogs_strdup(end_ts->valuestring) : NULL,
        offset_period ? true : false,
        offset_period ? offset_period->valuedouble : 0,
        samp_ratio ? true : false,
        samp_ratio ? samp_ratio->valuedouble : 0,
        max_object_nbr ? true : false,
        max_object_nbr ? max_object_nbr->valuedouble : 0,
        max_supi_nbr ? true : false,
        max_supi_nbr ? max_supi_nbr->valuedouble : 0,
        time_ana_needed && !cJSON_IsNull(time_ana_needed) ? ogs_strdup(time_ana_needed->valuestring) : NULL,
        ana_meta ? ana_metaList : NULL,
        ana_meta_ind ? ana_meta_ind_local_nonprim : NULL
    );

    return event_reporting_requirement_local_var;
end:
    if (accuracy_local_nonprim) {
        OpenAPI_accuracy_free(accuracy_local_nonprim);
        accuracy_local_nonprim = NULL;
    }
    if (acc_per_subsetList) {
        OpenAPI_list_for_each(acc_per_subsetList, node) {
            OpenAPI_accuracy_free(node->data);
        }
        OpenAPI_list_free(acc_per_subsetList);
        acc_per_subsetList = NULL;
    }
    if (ana_metaList) {
        OpenAPI_list_for_each(ana_metaList, node) {
            OpenAPI_analytics_metadata_free(node->data);
        }
        OpenAPI_list_free(ana_metaList);
        ana_metaList = NULL;
    }
    if (ana_meta_ind_local_nonprim) {
        OpenAPI_analytics_metadata_indication_free(ana_meta_ind_local_nonprim);
        ana_meta_ind_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_event_reporting_requirement_t *OpenAPI_event_reporting_requirement_copy(OpenAPI_event_reporting_requirement_t *dst, OpenAPI_event_reporting_requirement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_event_reporting_requirement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_event_reporting_requirement_convertToJSON() failed");
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

    OpenAPI_event_reporting_requirement_free(dst);
    dst = OpenAPI_event_reporting_requirement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


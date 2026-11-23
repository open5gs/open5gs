
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_synchronization_information.h"

OpenAPI_time_synchronization_information_t *OpenAPI_time_synchronization_information_create(
    OpenAPI_time_distribution_method_e distribution_method,
    bool is_t_s_ntime_domain_number,
    int t_s_ntime_domain_number,
    bool is_temporal_validity_information,
    int temporal_validity_information,
    OpenAPI_list_t *spatial_validity_information,
    bool is_time_synchronization_error_budget,
    int time_synchronization_error_budget,
    OpenAPI_synchronization_state_e synchronization_state,
    OpenAPI_clock_quality_t *clock_quality,
    OpenAPI_time_source_e parent_time_source
)
{
    OpenAPI_time_synchronization_information_t *time_synchronization_information_local_var = ogs_malloc(sizeof(OpenAPI_time_synchronization_information_t));
    ogs_assert(time_synchronization_information_local_var);

    time_synchronization_information_local_var->distribution_method = distribution_method;
    time_synchronization_information_local_var->is_t_s_ntime_domain_number = is_t_s_ntime_domain_number;
    time_synchronization_information_local_var->t_s_ntime_domain_number = t_s_ntime_domain_number;
    time_synchronization_information_local_var->is_temporal_validity_information = is_temporal_validity_information;
    time_synchronization_information_local_var->temporal_validity_information = temporal_validity_information;
    time_synchronization_information_local_var->spatial_validity_information = spatial_validity_information;
    time_synchronization_information_local_var->is_time_synchronization_error_budget = is_time_synchronization_error_budget;
    time_synchronization_information_local_var->time_synchronization_error_budget = time_synchronization_error_budget;
    time_synchronization_information_local_var->synchronization_state = synchronization_state;
    time_synchronization_information_local_var->clock_quality = clock_quality;
    time_synchronization_information_local_var->parent_time_source = parent_time_source;

    return time_synchronization_information_local_var;
}

void OpenAPI_time_synchronization_information_free(OpenAPI_time_synchronization_information_t *time_synchronization_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == time_synchronization_information) {
        return;
    }
    if (time_synchronization_information->spatial_validity_information) {
        OpenAPI_list_for_each(time_synchronization_information->spatial_validity_information, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(time_synchronization_information->spatial_validity_information);
        time_synchronization_information->spatial_validity_information = NULL;
    }
    if (time_synchronization_information->clock_quality) {
        OpenAPI_clock_quality_free(time_synchronization_information->clock_quality);
        time_synchronization_information->clock_quality = NULL;
    }
    ogs_free(time_synchronization_information);
}

cJSON *OpenAPI_time_synchronization_information_convertToJSON(OpenAPI_time_synchronization_information_t *time_synchronization_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (time_synchronization_information == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [TimeSynchronizationInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (time_synchronization_information->distribution_method != OpenAPI_time_distribution_method_NULL) {
    if (cJSON_AddStringToObject(item, "distributionMethod", OpenAPI_time_distribution_method_ToString(time_synchronization_information->distribution_method)) == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [distribution_method]");
        goto end;
    }
    }

    if (time_synchronization_information->is_t_s_ntime_domain_number) {
    if (cJSON_AddNumberToObject(item, "tSNtimeDomainNumber", time_synchronization_information->t_s_ntime_domain_number) == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [t_s_ntime_domain_number]");
        goto end;
    }
    }

    if (time_synchronization_information->is_temporal_validity_information) {
    if (cJSON_AddNumberToObject(item, "temporalValidityInformation", time_synchronization_information->temporal_validity_information) == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [temporal_validity_information]");
        goto end;
    }
    }

    if (time_synchronization_information->spatial_validity_information) {
    cJSON *spatial_validity_informationList = cJSON_AddArrayToObject(item, "spatialValidityInformation");
    if (spatial_validity_informationList == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [spatial_validity_information]");
        goto end;
    }
    OpenAPI_list_for_each(time_synchronization_information->spatial_validity_information, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [spatial_validity_information]");
            goto end;
        }
        cJSON_AddItemToArray(spatial_validity_informationList, itemLocal);
    }
    }

    if (time_synchronization_information->is_time_synchronization_error_budget) {
    if (cJSON_AddNumberToObject(item, "timeSynchronizationErrorBudget", time_synchronization_information->time_synchronization_error_budget) == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [time_synchronization_error_budget]");
        goto end;
    }
    }

    if (time_synchronization_information->synchronization_state != OpenAPI_synchronization_state_NULL) {
    if (cJSON_AddStringToObject(item, "synchronizationState", OpenAPI_synchronization_state_ToString(time_synchronization_information->synchronization_state)) == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [synchronization_state]");
        goto end;
    }
    }

    if (time_synchronization_information->clock_quality) {
    cJSON *clock_quality_local_JSON = OpenAPI_clock_quality_convertToJSON(time_synchronization_information->clock_quality);
    if (clock_quality_local_JSON == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [clock_quality]");
        goto end;
    }
    cJSON_AddItemToObject(item, "clockQuality", clock_quality_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [clock_quality]");
        goto end;
    }
    }

    if (time_synchronization_information->parent_time_source != OpenAPI_time_source_NULL) {
    if (cJSON_AddStringToObject(item, "parentTimeSource", OpenAPI_time_source_ToString(time_synchronization_information->parent_time_source)) == NULL) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed [parent_time_source]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_time_synchronization_information_t *OpenAPI_time_synchronization_information_parseFromJSON(cJSON *time_synchronization_informationJSON)
{
    OpenAPI_time_synchronization_information_t *time_synchronization_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *distribution_method = NULL;
    OpenAPI_time_distribution_method_e distribution_methodVariable = 0;
    cJSON *t_s_ntime_domain_number = NULL;
    cJSON *temporal_validity_information = NULL;
    cJSON *spatial_validity_information = NULL;
    OpenAPI_list_t *spatial_validity_informationList = NULL;
    cJSON *time_synchronization_error_budget = NULL;
    cJSON *synchronization_state = NULL;
    OpenAPI_synchronization_state_e synchronization_stateVariable = 0;
    cJSON *clock_quality = NULL;
    OpenAPI_clock_quality_t *clock_quality_local_nonprim = NULL;
    cJSON *parent_time_source = NULL;
    OpenAPI_time_source_e parent_time_sourceVariable = 0;
    distribution_method = cJSON_GetObjectItemCaseSensitive(time_synchronization_informationJSON, "distributionMethod");
    if (distribution_method) {
    if (!cJSON_IsString(distribution_method)) {
        ogs_error("OpenAPI_time_synchronization_information_parseFromJSON() failed [distribution_method]");
        goto end;
    }
    distribution_methodVariable = OpenAPI_time_distribution_method_FromString(distribution_method->valuestring);
    }

    t_s_ntime_domain_number = cJSON_GetObjectItemCaseSensitive(time_synchronization_informationJSON, "tSNtimeDomainNumber");
    if (t_s_ntime_domain_number) {
    if (!cJSON_IsNumber(t_s_ntime_domain_number)) {
        ogs_error("OpenAPI_time_synchronization_information_parseFromJSON() failed [t_s_ntime_domain_number]");
        goto end;
    }
    }

    temporal_validity_information = cJSON_GetObjectItemCaseSensitive(time_synchronization_informationJSON, "temporalValidityInformation");
    if (temporal_validity_information) {
    if (!cJSON_IsNumber(temporal_validity_information)) {
        ogs_error("OpenAPI_time_synchronization_information_parseFromJSON() failed [temporal_validity_information]");
        goto end;
    }
    }

    spatial_validity_information = cJSON_GetObjectItemCaseSensitive(time_synchronization_informationJSON, "spatialValidityInformation");
    if (spatial_validity_information) {
        cJSON *spatial_validity_information_local = NULL;
        if (!cJSON_IsArray(spatial_validity_information)) {
            ogs_error("OpenAPI_time_synchronization_information_parseFromJSON() failed [spatial_validity_information]");
            goto end;
        }

        spatial_validity_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(spatial_validity_information_local, spatial_validity_information) {
            if (!cJSON_IsObject(spatial_validity_information_local)) {
                ogs_error("OpenAPI_time_synchronization_information_parseFromJSON() failed [spatial_validity_information]");
                goto end;
            }
            OpenAPI_tai_t *spatial_validity_informationItem = OpenAPI_tai_parseFromJSON(spatial_validity_information_local);
            if (!spatial_validity_informationItem) {
                ogs_error("No spatial_validity_informationItem");
                goto end;
            }
            OpenAPI_list_add(spatial_validity_informationList, spatial_validity_informationItem);
        }
    }

    time_synchronization_error_budget = cJSON_GetObjectItemCaseSensitive(time_synchronization_informationJSON, "timeSynchronizationErrorBudget");
    if (time_synchronization_error_budget) {
    if (!cJSON_IsNumber(time_synchronization_error_budget)) {
        ogs_error("OpenAPI_time_synchronization_information_parseFromJSON() failed [time_synchronization_error_budget]");
        goto end;
    }
    }

    synchronization_state = cJSON_GetObjectItemCaseSensitive(time_synchronization_informationJSON, "synchronizationState");
    if (synchronization_state) {
    if (!cJSON_IsString(synchronization_state)) {
        ogs_error("OpenAPI_time_synchronization_information_parseFromJSON() failed [synchronization_state]");
        goto end;
    }
    synchronization_stateVariable = OpenAPI_synchronization_state_FromString(synchronization_state->valuestring);
    }

    clock_quality = cJSON_GetObjectItemCaseSensitive(time_synchronization_informationJSON, "clockQuality");
    if (clock_quality) {
    clock_quality_local_nonprim = OpenAPI_clock_quality_parseFromJSON(clock_quality);
    if (!clock_quality_local_nonprim) {
        ogs_error("OpenAPI_clock_quality_parseFromJSON failed [clock_quality]");
        goto end;
    }
    }

    parent_time_source = cJSON_GetObjectItemCaseSensitive(time_synchronization_informationJSON, "parentTimeSource");
    if (parent_time_source) {
    if (!cJSON_IsString(parent_time_source)) {
        ogs_error("OpenAPI_time_synchronization_information_parseFromJSON() failed [parent_time_source]");
        goto end;
    }
    parent_time_sourceVariable = OpenAPI_time_source_FromString(parent_time_source->valuestring);
    }

    time_synchronization_information_local_var = OpenAPI_time_synchronization_information_create (
        distribution_method ? distribution_methodVariable : 0,
        t_s_ntime_domain_number ? true : false,
        t_s_ntime_domain_number ? t_s_ntime_domain_number->valuedouble : 0,
        temporal_validity_information ? true : false,
        temporal_validity_information ? temporal_validity_information->valuedouble : 0,
        spatial_validity_information ? spatial_validity_informationList : NULL,
        time_synchronization_error_budget ? true : false,
        time_synchronization_error_budget ? time_synchronization_error_budget->valuedouble : 0,
        synchronization_state ? synchronization_stateVariable : 0,
        clock_quality ? clock_quality_local_nonprim : NULL,
        parent_time_source ? parent_time_sourceVariable : 0
    );

    return time_synchronization_information_local_var;
end:
    if (spatial_validity_informationList) {
        OpenAPI_list_for_each(spatial_validity_informationList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(spatial_validity_informationList);
        spatial_validity_informationList = NULL;
    }
    if (clock_quality_local_nonprim) {
        OpenAPI_clock_quality_free(clock_quality_local_nonprim);
        clock_quality_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_time_synchronization_information_t *OpenAPI_time_synchronization_information_copy(OpenAPI_time_synchronization_information_t *dst, OpenAPI_time_synchronization_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_time_synchronization_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_time_synchronization_information_convertToJSON() failed");
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

    OpenAPI_time_synchronization_information_free(dst);
    dst = OpenAPI_time_synchronization_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


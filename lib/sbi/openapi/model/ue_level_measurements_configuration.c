
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_level_measurements_configuration.h"

OpenAPI_ue_level_measurements_configuration_t *OpenAPI_ue_level_measurements_configuration_create(
    OpenAPI_job_type_e job_type,
    OpenAPI_list_t *ue_level_measurements_list,
    bool is_granularity_period,
    int granularity_period
)
{
    OpenAPI_ue_level_measurements_configuration_t *ue_level_measurements_configuration_local_var = ogs_malloc(sizeof(OpenAPI_ue_level_measurements_configuration_t));
    ogs_assert(ue_level_measurements_configuration_local_var);

    ue_level_measurements_configuration_local_var->job_type = job_type;
    ue_level_measurements_configuration_local_var->ue_level_measurements_list = ue_level_measurements_list;
    ue_level_measurements_configuration_local_var->is_granularity_period = is_granularity_period;
    ue_level_measurements_configuration_local_var->granularity_period = granularity_period;

    return ue_level_measurements_configuration_local_var;
}

void OpenAPI_ue_level_measurements_configuration_free(OpenAPI_ue_level_measurements_configuration_t *ue_level_measurements_configuration)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_level_measurements_configuration) {
        return;
    }
    if (ue_level_measurements_configuration->ue_level_measurements_list) {
        OpenAPI_list_free(ue_level_measurements_configuration->ue_level_measurements_list);
        ue_level_measurements_configuration->ue_level_measurements_list = NULL;
    }
    ogs_free(ue_level_measurements_configuration);
}

cJSON *OpenAPI_ue_level_measurements_configuration_convertToJSON(OpenAPI_ue_level_measurements_configuration_t *ue_level_measurements_configuration)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_level_measurements_configuration == NULL) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_convertToJSON() failed [UeLevelMeasurementsConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_level_measurements_configuration->job_type == OpenAPI_job_type_NULL) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_convertToJSON() failed [job_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "jobType", OpenAPI_job_type_ToString(ue_level_measurements_configuration->job_type)) == NULL) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_convertToJSON() failed [job_type]");
        goto end;
    }

    if (ue_level_measurements_configuration->ue_level_measurements_list == OpenAPI_measurement_type_NULL) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_convertToJSON() failed [ue_level_measurements_list]");
        return NULL;
    }
    cJSON *ue_level_measurements_listList = cJSON_AddArrayToObject(item, "ueLevelMeasurementsList");
    if (ue_level_measurements_listList == NULL) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_convertToJSON() failed [ue_level_measurements_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_level_measurements_configuration->ue_level_measurements_list, node) {
        if (cJSON_AddStringToObject(ue_level_measurements_listList, "", OpenAPI_measurement_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_ue_level_measurements_configuration_convertToJSON() failed [ue_level_measurements_list]");
            goto end;
        }
    }

    if (ue_level_measurements_configuration->is_granularity_period) {
    if (cJSON_AddNumberToObject(item, "granularityPeriod", ue_level_measurements_configuration->granularity_period) == NULL) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_convertToJSON() failed [granularity_period]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_level_measurements_configuration_t *OpenAPI_ue_level_measurements_configuration_parseFromJSON(cJSON *ue_level_measurements_configurationJSON)
{
    OpenAPI_ue_level_measurements_configuration_t *ue_level_measurements_configuration_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *job_type = NULL;
    OpenAPI_job_type_e job_typeVariable = 0;
    cJSON *ue_level_measurements_list = NULL;
    OpenAPI_list_t *ue_level_measurements_listList = NULL;
    cJSON *granularity_period = NULL;
    job_type = cJSON_GetObjectItemCaseSensitive(ue_level_measurements_configurationJSON, "jobType");
    if (!job_type) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_parseFromJSON() failed [job_type]");
        goto end;
    }
    if (!cJSON_IsString(job_type)) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_parseFromJSON() failed [job_type]");
        goto end;
    }
    job_typeVariable = OpenAPI_job_type_FromString(job_type->valuestring);

    ue_level_measurements_list = cJSON_GetObjectItemCaseSensitive(ue_level_measurements_configurationJSON, "ueLevelMeasurementsList");
    if (!ue_level_measurements_list) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_parseFromJSON() failed [ue_level_measurements_list]");
        goto end;
    }
        cJSON *ue_level_measurements_list_local = NULL;
        if (!cJSON_IsArray(ue_level_measurements_list)) {
            ogs_error("OpenAPI_ue_level_measurements_configuration_parseFromJSON() failed [ue_level_measurements_list]");
            goto end;
        }

        ue_level_measurements_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_level_measurements_list_local, ue_level_measurements_list) {
            OpenAPI_measurement_type_e localEnum = OpenAPI_measurement_type_NULL;
            if (!cJSON_IsString(ue_level_measurements_list_local)) {
                ogs_error("OpenAPI_ue_level_measurements_configuration_parseFromJSON() failed [ue_level_measurements_list]");
                goto end;
            }
            localEnum = OpenAPI_measurement_type_FromString(ue_level_measurements_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"ue_level_measurements_list\" is not supported. Ignoring it ...",
                         ue_level_measurements_list_local->valuestring);
            } else {
                OpenAPI_list_add(ue_level_measurements_listList, (void *)localEnum);
            }
        }
        if (ue_level_measurements_listList->count == 0) {
            ogs_error("OpenAPI_ue_level_measurements_configuration_parseFromJSON() failed: Expected ue_level_measurements_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    granularity_period = cJSON_GetObjectItemCaseSensitive(ue_level_measurements_configurationJSON, "granularityPeriod");
    if (granularity_period) {
    if (!cJSON_IsNumber(granularity_period)) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_parseFromJSON() failed [granularity_period]");
        goto end;
    }
    }

    ue_level_measurements_configuration_local_var = OpenAPI_ue_level_measurements_configuration_create (
        job_typeVariable,
        ue_level_measurements_listList,
        granularity_period ? true : false,
        granularity_period ? granularity_period->valuedouble : 0
    );

    return ue_level_measurements_configuration_local_var;
end:
    if (ue_level_measurements_listList) {
        OpenAPI_list_free(ue_level_measurements_listList);
        ue_level_measurements_listList = NULL;
    }
    return NULL;
}

OpenAPI_ue_level_measurements_configuration_t *OpenAPI_ue_level_measurements_configuration_copy(OpenAPI_ue_level_measurements_configuration_t *dst, OpenAPI_ue_level_measurements_configuration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_level_measurements_configuration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_convertToJSON() failed");
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

    OpenAPI_ue_level_measurements_configuration_free(dst);
    dst = OpenAPI_ue_level_measurements_configuration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


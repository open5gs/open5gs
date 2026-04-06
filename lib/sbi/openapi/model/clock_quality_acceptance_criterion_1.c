
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "clock_quality_acceptance_criterion_1.h"

OpenAPI_clock_quality_acceptance_criterion_1_t *OpenAPI_clock_quality_acceptance_criterion_1_create(
    OpenAPI_list_t *synchronization_state,
    OpenAPI_clock_quality_1_t *clock_quality,
    OpenAPI_list_t *parent_time_source
)
{
    OpenAPI_clock_quality_acceptance_criterion_1_t *clock_quality_acceptance_criterion_1_local_var = ogs_malloc(sizeof(OpenAPI_clock_quality_acceptance_criterion_1_t));
    ogs_assert(clock_quality_acceptance_criterion_1_local_var);

    clock_quality_acceptance_criterion_1_local_var->synchronization_state = synchronization_state;
    clock_quality_acceptance_criterion_1_local_var->clock_quality = clock_quality;
    clock_quality_acceptance_criterion_1_local_var->parent_time_source = parent_time_source;

    return clock_quality_acceptance_criterion_1_local_var;
}

void OpenAPI_clock_quality_acceptance_criterion_1_free(OpenAPI_clock_quality_acceptance_criterion_1_t *clock_quality_acceptance_criterion_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == clock_quality_acceptance_criterion_1) {
        return;
    }
    if (clock_quality_acceptance_criterion_1->synchronization_state) {
        OpenAPI_list_free(clock_quality_acceptance_criterion_1->synchronization_state);
        clock_quality_acceptance_criterion_1->synchronization_state = NULL;
    }
    if (clock_quality_acceptance_criterion_1->clock_quality) {
        OpenAPI_clock_quality_1_free(clock_quality_acceptance_criterion_1->clock_quality);
        clock_quality_acceptance_criterion_1->clock_quality = NULL;
    }
    if (clock_quality_acceptance_criterion_1->parent_time_source) {
        OpenAPI_list_free(clock_quality_acceptance_criterion_1->parent_time_source);
        clock_quality_acceptance_criterion_1->parent_time_source = NULL;
    }
    ogs_free(clock_quality_acceptance_criterion_1);
}

cJSON *OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON(OpenAPI_clock_quality_acceptance_criterion_1_t *clock_quality_acceptance_criterion_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (clock_quality_acceptance_criterion_1 == NULL) {
        ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON() failed [ClockQualityAcceptanceCriterion_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (clock_quality_acceptance_criterion_1->synchronization_state != OpenAPI_synchronization_state_NULL) {
    cJSON *synchronization_stateList = cJSON_AddArrayToObject(item, "synchronizationState");
    if (synchronization_stateList == NULL) {
        ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON() failed [synchronization_state]");
        goto end;
    }
    OpenAPI_list_for_each(clock_quality_acceptance_criterion_1->synchronization_state, node) {
        if (cJSON_AddStringToObject(synchronization_stateList, "", OpenAPI_synchronization_state_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON() failed [synchronization_state]");
            goto end;
        }
    }
    }

    if (clock_quality_acceptance_criterion_1->clock_quality) {
    cJSON *clock_quality_local_JSON = OpenAPI_clock_quality_1_convertToJSON(clock_quality_acceptance_criterion_1->clock_quality);
    if (clock_quality_local_JSON == NULL) {
        ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON() failed [clock_quality]");
        goto end;
    }
    cJSON_AddItemToObject(item, "clockQuality", clock_quality_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON() failed [clock_quality]");
        goto end;
    }
    }

    if (clock_quality_acceptance_criterion_1->parent_time_source != OpenAPI_time_source_NULL) {
    cJSON *parent_time_sourceList = cJSON_AddArrayToObject(item, "parentTimeSource");
    if (parent_time_sourceList == NULL) {
        ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON() failed [parent_time_source]");
        goto end;
    }
    OpenAPI_list_for_each(clock_quality_acceptance_criterion_1->parent_time_source, node) {
        if (cJSON_AddStringToObject(parent_time_sourceList, "", OpenAPI_time_source_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON() failed [parent_time_source]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_clock_quality_acceptance_criterion_1_t *OpenAPI_clock_quality_acceptance_criterion_1_parseFromJSON(cJSON *clock_quality_acceptance_criterion_1JSON)
{
    OpenAPI_clock_quality_acceptance_criterion_1_t *clock_quality_acceptance_criterion_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *synchronization_state = NULL;
    OpenAPI_list_t *synchronization_stateList = NULL;
    cJSON *clock_quality = NULL;
    OpenAPI_clock_quality_1_t *clock_quality_local_nonprim = NULL;
    cJSON *parent_time_source = NULL;
    OpenAPI_list_t *parent_time_sourceList = NULL;
    synchronization_state = cJSON_GetObjectItemCaseSensitive(clock_quality_acceptance_criterion_1JSON, "synchronizationState");
    if (synchronization_state) {
        cJSON *synchronization_state_local = NULL;
        if (!cJSON_IsArray(synchronization_state)) {
            ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_parseFromJSON() failed [synchronization_state]");
            goto end;
        }

        synchronization_stateList = OpenAPI_list_create();

        cJSON_ArrayForEach(synchronization_state_local, synchronization_state) {
            OpenAPI_synchronization_state_e localEnum = OpenAPI_synchronization_state_NULL;
            if (!cJSON_IsString(synchronization_state_local)) {
                ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_parseFromJSON() failed [synchronization_state]");
                goto end;
            }
            localEnum = OpenAPI_synchronization_state_FromString(synchronization_state_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"synchronization_state\" is not supported. Ignoring it ...",
                         synchronization_state_local->valuestring);
            } else {
                OpenAPI_list_add(synchronization_stateList, (void *)localEnum);
            }
        }
        if (synchronization_stateList->count == 0) {
            ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_parseFromJSON() failed: Expected synchronization_stateList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    clock_quality = cJSON_GetObjectItemCaseSensitive(clock_quality_acceptance_criterion_1JSON, "clockQuality");
    if (clock_quality) {
    clock_quality_local_nonprim = OpenAPI_clock_quality_1_parseFromJSON(clock_quality);
    if (!clock_quality_local_nonprim) {
        ogs_error("OpenAPI_clock_quality_1_parseFromJSON failed [clock_quality]");
        goto end;
    }
    }

    parent_time_source = cJSON_GetObjectItemCaseSensitive(clock_quality_acceptance_criterion_1JSON, "parentTimeSource");
    if (parent_time_source) {
        cJSON *parent_time_source_local = NULL;
        if (!cJSON_IsArray(parent_time_source)) {
            ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_parseFromJSON() failed [parent_time_source]");
            goto end;
        }

        parent_time_sourceList = OpenAPI_list_create();

        cJSON_ArrayForEach(parent_time_source_local, parent_time_source) {
            OpenAPI_time_source_e localEnum = OpenAPI_time_source_NULL;
            if (!cJSON_IsString(parent_time_source_local)) {
                ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_parseFromJSON() failed [parent_time_source]");
                goto end;
            }
            localEnum = OpenAPI_time_source_FromString(parent_time_source_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"parent_time_source\" is not supported. Ignoring it ...",
                         parent_time_source_local->valuestring);
            } else {
                OpenAPI_list_add(parent_time_sourceList, (void *)localEnum);
            }
        }
        if (parent_time_sourceList->count == 0) {
            ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_parseFromJSON() failed: Expected parent_time_sourceList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    clock_quality_acceptance_criterion_1_local_var = OpenAPI_clock_quality_acceptance_criterion_1_create (
        synchronization_state ? synchronization_stateList : NULL,
        clock_quality ? clock_quality_local_nonprim : NULL,
        parent_time_source ? parent_time_sourceList : NULL
    );

    return clock_quality_acceptance_criterion_1_local_var;
end:
    if (synchronization_stateList) {
        OpenAPI_list_free(synchronization_stateList);
        synchronization_stateList = NULL;
    }
    if (clock_quality_local_nonprim) {
        OpenAPI_clock_quality_1_free(clock_quality_local_nonprim);
        clock_quality_local_nonprim = NULL;
    }
    if (parent_time_sourceList) {
        OpenAPI_list_free(parent_time_sourceList);
        parent_time_sourceList = NULL;
    }
    return NULL;
}

OpenAPI_clock_quality_acceptance_criterion_1_t *OpenAPI_clock_quality_acceptance_criterion_1_copy(OpenAPI_clock_quality_acceptance_criterion_1_t *dst, OpenAPI_clock_quality_acceptance_criterion_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_clock_quality_acceptance_criterion_1_convertToJSON() failed");
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

    OpenAPI_clock_quality_acceptance_criterion_1_free(dst);
    dst = OpenAPI_clock_quality_acceptance_criterion_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


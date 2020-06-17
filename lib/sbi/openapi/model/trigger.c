
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trigger.h"

OpenAPI_trigger_t *OpenAPI_trigger_create(
    OpenAPI_trigger_type_t *trigger_type,
    OpenAPI_trigger_category_t *trigger_category,
    int time_limit,
    int volume_limit,
    long volume_limit64,
    int max_number_ofccc
    )
{
    OpenAPI_trigger_t *trigger_local_var = OpenAPI_malloc(sizeof(OpenAPI_trigger_t));
    if (!trigger_local_var) {
        return NULL;
    }
    trigger_local_var->trigger_type = trigger_type;
    trigger_local_var->trigger_category = trigger_category;
    trigger_local_var->time_limit = time_limit;
    trigger_local_var->volume_limit = volume_limit;
    trigger_local_var->volume_limit64 = volume_limit64;
    trigger_local_var->max_number_ofccc = max_number_ofccc;

    return trigger_local_var;
}

void OpenAPI_trigger_free(OpenAPI_trigger_t *trigger)
{
    if (NULL == trigger) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_trigger_type_free(trigger->trigger_type);
    OpenAPI_trigger_category_free(trigger->trigger_category);
    ogs_free(trigger);
}

cJSON *OpenAPI_trigger_convertToJSON(OpenAPI_trigger_t *trigger)
{
    cJSON *item = NULL;

    if (trigger == NULL) {
        ogs_error("OpenAPI_trigger_convertToJSON() failed [Trigger]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!trigger->trigger_type) {
        ogs_error("OpenAPI_trigger_convertToJSON() failed [trigger_type]");
        goto end;
    }
    cJSON *trigger_type_local_JSON = OpenAPI_trigger_type_convertToJSON(trigger->trigger_type);
    if (trigger_type_local_JSON == NULL) {
        ogs_error("OpenAPI_trigger_convertToJSON() failed [trigger_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "triggerType", trigger_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_trigger_convertToJSON() failed [trigger_type]");
        goto end;
    }

    if (!trigger->trigger_category) {
        ogs_error("OpenAPI_trigger_convertToJSON() failed [trigger_category]");
        goto end;
    }
    cJSON *trigger_category_local_JSON = OpenAPI_trigger_category_convertToJSON(trigger->trigger_category);
    if (trigger_category_local_JSON == NULL) {
        ogs_error("OpenAPI_trigger_convertToJSON() failed [trigger_category]");
        goto end;
    }
    cJSON_AddItemToObject(item, "triggerCategory", trigger_category_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_trigger_convertToJSON() failed [trigger_category]");
        goto end;
    }

    if (trigger->time_limit) {
        if (cJSON_AddNumberToObject(item, "timeLimit", trigger->time_limit) == NULL) {
            ogs_error("OpenAPI_trigger_convertToJSON() failed [time_limit]");
            goto end;
        }
    }

    if (trigger->volume_limit) {
        if (cJSON_AddNumberToObject(item, "volumeLimit", trigger->volume_limit) == NULL) {
            ogs_error("OpenAPI_trigger_convertToJSON() failed [volume_limit]");
            goto end;
        }
    }

    if (trigger->volume_limit64) {
        if (cJSON_AddNumberToObject(item, "volumeLimit64", trigger->volume_limit64) == NULL) {
            ogs_error("OpenAPI_trigger_convertToJSON() failed [volume_limit64]");
            goto end;
        }
    }

    if (trigger->max_number_ofccc) {
        if (cJSON_AddNumberToObject(item, "maxNumberOfccc", trigger->max_number_ofccc) == NULL) {
            ogs_error("OpenAPI_trigger_convertToJSON() failed [max_number_ofccc]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_trigger_t *OpenAPI_trigger_parseFromJSON(cJSON *triggerJSON)
{
    OpenAPI_trigger_t *trigger_local_var = NULL;
    cJSON *trigger_type = cJSON_GetObjectItemCaseSensitive(triggerJSON, "triggerType");
    if (!trigger_type) {
        ogs_error("OpenAPI_trigger_parseFromJSON() failed [trigger_type]");
        goto end;
    }

    OpenAPI_trigger_type_t *trigger_type_local_nonprim = NULL;

    trigger_type_local_nonprim = OpenAPI_trigger_type_parseFromJSON(trigger_type);

    cJSON *trigger_category = cJSON_GetObjectItemCaseSensitive(triggerJSON, "triggerCategory");
    if (!trigger_category) {
        ogs_error("OpenAPI_trigger_parseFromJSON() failed [trigger_category]");
        goto end;
    }

    OpenAPI_trigger_category_t *trigger_category_local_nonprim = NULL;

    trigger_category_local_nonprim = OpenAPI_trigger_category_parseFromJSON(trigger_category);

    cJSON *time_limit = cJSON_GetObjectItemCaseSensitive(triggerJSON, "timeLimit");

    if (time_limit) {
        if (!cJSON_IsNumber(time_limit)) {
            ogs_error("OpenAPI_trigger_parseFromJSON() failed [time_limit]");
            goto end;
        }
    }

    cJSON *volume_limit = cJSON_GetObjectItemCaseSensitive(triggerJSON, "volumeLimit");

    if (volume_limit) {
        if (!cJSON_IsNumber(volume_limit)) {
            ogs_error("OpenAPI_trigger_parseFromJSON() failed [volume_limit]");
            goto end;
        }
    }

    cJSON *volume_limit64 = cJSON_GetObjectItemCaseSensitive(triggerJSON, "volumeLimit64");

    if (volume_limit64) {
        if (!cJSON_IsNumber(volume_limit64)) {
            ogs_error("OpenAPI_trigger_parseFromJSON() failed [volume_limit64]");
            goto end;
        }
    }

    cJSON *max_number_ofccc = cJSON_GetObjectItemCaseSensitive(triggerJSON, "maxNumberOfccc");

    if (max_number_ofccc) {
        if (!cJSON_IsNumber(max_number_ofccc)) {
            ogs_error("OpenAPI_trigger_parseFromJSON() failed [max_number_ofccc]");
            goto end;
        }
    }

    trigger_local_var = OpenAPI_trigger_create (
        trigger_type_local_nonprim,
        trigger_category_local_nonprim,
        time_limit ? time_limit->valuedouble : 0,
        volume_limit ? volume_limit->valuedouble : 0,
        volume_limit64 ? volume_limit64->valuedouble : 0,
        max_number_ofccc ? max_number_ofccc->valuedouble : 0
        );

    return trigger_local_var;
end:
    return NULL;
}

OpenAPI_trigger_t *OpenAPI_trigger_copy(OpenAPI_trigger_t *dst, OpenAPI_trigger_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trigger_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trigger_convertToJSON() failed");
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

    OpenAPI_trigger_free(dst);
    dst = OpenAPI_trigger_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


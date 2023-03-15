
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "roaming_charging_profile.h"

OpenAPI_roaming_charging_profile_t *OpenAPI_roaming_charging_profile_create(
    OpenAPI_list_t *triggers,
    OpenAPI_partial_record_method_t *partial_record_method
)
{
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_var = ogs_malloc(sizeof(OpenAPI_roaming_charging_profile_t));
    ogs_assert(roaming_charging_profile_local_var);

    roaming_charging_profile_local_var->triggers = triggers;
    roaming_charging_profile_local_var->partial_record_method = partial_record_method;

    return roaming_charging_profile_local_var;
}

void OpenAPI_roaming_charging_profile_free(OpenAPI_roaming_charging_profile_t *roaming_charging_profile)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == roaming_charging_profile) {
        return;
    }
    if (roaming_charging_profile->triggers) {
        OpenAPI_list_for_each(roaming_charging_profile->triggers, node) {
            OpenAPI_trigger_free(node->data);
        }
        OpenAPI_list_free(roaming_charging_profile->triggers);
        roaming_charging_profile->triggers = NULL;
    }
    if (roaming_charging_profile->partial_record_method) {
        OpenAPI_partial_record_method_free(roaming_charging_profile->partial_record_method);
        roaming_charging_profile->partial_record_method = NULL;
    }
    ogs_free(roaming_charging_profile);
}

cJSON *OpenAPI_roaming_charging_profile_convertToJSON(OpenAPI_roaming_charging_profile_t *roaming_charging_profile)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (roaming_charging_profile == NULL) {
        ogs_error("OpenAPI_roaming_charging_profile_convertToJSON() failed [RoamingChargingProfile]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (roaming_charging_profile->triggers) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_roaming_charging_profile_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(roaming_charging_profile->triggers, node) {
        cJSON *itemLocal = OpenAPI_trigger_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_roaming_charging_profile_convertToJSON() failed [triggers]");
            goto end;
        }
        cJSON_AddItemToArray(triggersList, itemLocal);
    }
    }

    if (roaming_charging_profile->partial_record_method) {
    cJSON *partial_record_method_local_JSON = OpenAPI_partial_record_method_convertToJSON(roaming_charging_profile->partial_record_method);
    if (partial_record_method_local_JSON == NULL) {
        ogs_error("OpenAPI_roaming_charging_profile_convertToJSON() failed [partial_record_method]");
        goto end;
    }
    cJSON_AddItemToObject(item, "partialRecordMethod", partial_record_method_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_roaming_charging_profile_convertToJSON() failed [partial_record_method]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_roaming_charging_profile_t *OpenAPI_roaming_charging_profile_parseFromJSON(cJSON *roaming_charging_profileJSON)
{
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *triggers = NULL;
    OpenAPI_list_t *triggersList = NULL;
    cJSON *partial_record_method = NULL;
    OpenAPI_partial_record_method_t *partial_record_method_local_nonprim = NULL;
    triggers = cJSON_GetObjectItemCaseSensitive(roaming_charging_profileJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_roaming_charging_profile_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            if (!cJSON_IsObject(triggers_local)) {
                ogs_error("OpenAPI_roaming_charging_profile_parseFromJSON() failed [triggers]");
                goto end;
            }
            OpenAPI_trigger_t *triggersItem = OpenAPI_trigger_parseFromJSON(triggers_local);
            if (!triggersItem) {
                ogs_error("No triggersItem");
                goto end;
            }
            OpenAPI_list_add(triggersList, triggersItem);
        }
    }

    partial_record_method = cJSON_GetObjectItemCaseSensitive(roaming_charging_profileJSON, "partialRecordMethod");
    if (partial_record_method) {
    partial_record_method_local_nonprim = OpenAPI_partial_record_method_parseFromJSON(partial_record_method);
    if (!partial_record_method_local_nonprim) {
        ogs_error("OpenAPI_partial_record_method_parseFromJSON failed [partial_record_method]");
        goto end;
    }
    }

    roaming_charging_profile_local_var = OpenAPI_roaming_charging_profile_create (
        triggers ? triggersList : NULL,
        partial_record_method ? partial_record_method_local_nonprim : NULL
    );

    return roaming_charging_profile_local_var;
end:
    if (triggersList) {
        OpenAPI_list_for_each(triggersList, node) {
            OpenAPI_trigger_free(node->data);
        }
        OpenAPI_list_free(triggersList);
        triggersList = NULL;
    }
    if (partial_record_method_local_nonprim) {
        OpenAPI_partial_record_method_free(partial_record_method_local_nonprim);
        partial_record_method_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_roaming_charging_profile_t *OpenAPI_roaming_charging_profile_copy(OpenAPI_roaming_charging_profile_t *dst, OpenAPI_roaming_charging_profile_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_roaming_charging_profile_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_roaming_charging_profile_convertToJSON() failed");
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

    OpenAPI_roaming_charging_profile_free(dst);
    dst = OpenAPI_roaming_charging_profile_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


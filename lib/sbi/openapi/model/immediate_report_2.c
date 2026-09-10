
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "immediate_report_2.h"

OpenAPI_immediate_report_2_t *OpenAPI_immediate_report_2_create(
    OpenAPI_subscription_data_sets_1_t *subscription_data_sets,
    OpenAPI_list_t *shared_data_list
)
{
    OpenAPI_immediate_report_2_t *immediate_report_2_local_var = ogs_malloc(sizeof(OpenAPI_immediate_report_2_t));
    ogs_assert(immediate_report_2_local_var);

    immediate_report_2_local_var->subscription_data_sets = subscription_data_sets;
    immediate_report_2_local_var->shared_data_list = shared_data_list;

    return immediate_report_2_local_var;
}

void OpenAPI_immediate_report_2_free(OpenAPI_immediate_report_2_t *immediate_report_2)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == immediate_report_2) {
        return;
    }
    if (immediate_report_2->subscription_data_sets) {
        OpenAPI_subscription_data_sets_1_free(immediate_report_2->subscription_data_sets);
        immediate_report_2->subscription_data_sets = NULL;
    }
    if (immediate_report_2->shared_data_list) {
        OpenAPI_list_for_each(immediate_report_2->shared_data_list, node) {
            OpenAPI_shared_data_1_free(node->data);
        }
        OpenAPI_list_free(immediate_report_2->shared_data_list);
        immediate_report_2->shared_data_list = NULL;
    }
    ogs_free(immediate_report_2);
}

/*
 * oneOf union (x-open5gs-union): the members are the alternatives of the
 * original oneOf. Exactly one is set, and it is the JSON value itself
 * rather than a property of a wrapper object.
 */
cJSON *OpenAPI_immediate_report_2_convertToJSON(OpenAPI_immediate_report_2_t *immediate_report_2)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (immediate_report_2 == NULL) {
        ogs_error("OpenAPI_immediate_report_2_convertToJSON() failed [ImmediateReport_2]");
        return NULL;
    }

    if ((immediate_report_2->subscription_data_sets != NULL) +
            (immediate_report_2->shared_data_list != NULL) != 1) {
        ogs_error("OpenAPI_immediate_report_2_convertToJSON() failed [ImmediateReport_2]: exactly one alternative must be set");
        return NULL;
    }

    if (immediate_report_2->subscription_data_sets) {
        return OpenAPI_subscription_data_sets_1_convertToJSON(immediate_report_2->subscription_data_sets);
    }
    if (immediate_report_2->shared_data_list) {
        item = cJSON_CreateArray();
        if (item == NULL) {
            ogs_error("OpenAPI_immediate_report_2_convertToJSON() failed [shared_data_list]");
            return NULL;
        }
        OpenAPI_list_for_each(immediate_report_2->shared_data_list, node) {
            cJSON *itemLocal = OpenAPI_shared_data_1_convertToJSON(node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_immediate_report_2_convertToJSON() failed [shared_data_list]");
                cJSON_Delete(item);
                return NULL;
            }
            cJSON_AddItemToArray(item, itemLocal);
        }
        return item;
    }

    ogs_error("OpenAPI_immediate_report_2_convertToJSON() failed [ImmediateReport_2]");
    return NULL;
}

OpenAPI_immediate_report_2_t *OpenAPI_immediate_report_2_parseFromJSON(cJSON *immediate_report_2JSON)
{
    OpenAPI_lnode_t *node = NULL;
    OpenAPI_subscription_data_sets_1_t *subscription_data_sets = NULL;
    OpenAPI_list_t *shared_data_list = NULL;

    if (cJSON_IsObject(immediate_report_2JSON)) {
        subscription_data_sets = OpenAPI_subscription_data_sets_1_parseFromJSON(immediate_report_2JSON);
        if (!subscription_data_sets) {
            ogs_error("OpenAPI_subscription_data_sets_1_parseFromJSON failed [subscription_data_sets]");
            goto end;
        }
    } else if (cJSON_IsArray(immediate_report_2JSON)) {
        cJSON *shared_data_list_local = NULL;

        shared_data_list = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_data_list_local, immediate_report_2JSON) {
            if (!cJSON_IsObject(shared_data_list_local)) {
                ogs_error("OpenAPI_immediate_report_2_parseFromJSON() failed [shared_data_list]");
                goto end;
            }
            OpenAPI_shared_data_1_t *shared_data_listItem = OpenAPI_shared_data_1_parseFromJSON(shared_data_list_local);
            if (!shared_data_listItem) {
                ogs_error("No shared_data_listItem");
                goto end;
            }
            OpenAPI_list_add(shared_data_list, shared_data_listItem);
        }
    } else {
        ogs_error("OpenAPI_immediate_report_2_parseFromJSON() failed [ImmediateReport_2]");
        goto end;
    }

    return OpenAPI_immediate_report_2_create(subscription_data_sets, shared_data_list);
end:
    if (subscription_data_sets) {
        OpenAPI_subscription_data_sets_1_free(subscription_data_sets);
        subscription_data_sets = NULL;
    }
    if (shared_data_list) {
        OpenAPI_list_for_each(shared_data_list, node) {
            OpenAPI_shared_data_1_free(node->data);
        }
        OpenAPI_list_free(shared_data_list);
        shared_data_list = NULL;
    }
    return NULL;
}

OpenAPI_immediate_report_2_t *OpenAPI_immediate_report_2_copy(OpenAPI_immediate_report_2_t *dst, OpenAPI_immediate_report_2_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_immediate_report_2_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_immediate_report_2_convertToJSON() failed");
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

    OpenAPI_immediate_report_2_free(dst);
    dst = OpenAPI_immediate_report_2_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


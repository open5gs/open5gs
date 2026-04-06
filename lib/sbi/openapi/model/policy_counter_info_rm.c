
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_counter_info_rm.h"

OpenAPI_policy_counter_info_rm_t *OpenAPI_policy_counter_info_rm_create(
    bool is_current_status_null,
    char *current_status,
    bool is_pen_pol_counter_statuses_null,
    OpenAPI_list_t *pen_pol_counter_statuses
)
{
    OpenAPI_policy_counter_info_rm_t *policy_counter_info_rm_local_var = ogs_malloc(sizeof(OpenAPI_policy_counter_info_rm_t));
    ogs_assert(policy_counter_info_rm_local_var);

    policy_counter_info_rm_local_var->is_current_status_null = is_current_status_null;
    policy_counter_info_rm_local_var->current_status = current_status;
    policy_counter_info_rm_local_var->is_pen_pol_counter_statuses_null = is_pen_pol_counter_statuses_null;
    policy_counter_info_rm_local_var->pen_pol_counter_statuses = pen_pol_counter_statuses;

    return policy_counter_info_rm_local_var;
}

void OpenAPI_policy_counter_info_rm_free(OpenAPI_policy_counter_info_rm_t *policy_counter_info_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == policy_counter_info_rm) {
        return;
    }
    if (policy_counter_info_rm->current_status) {
        ogs_free(policy_counter_info_rm->current_status);
        policy_counter_info_rm->current_status = NULL;
    }
    if (policy_counter_info_rm->pen_pol_counter_statuses) {
        OpenAPI_list_for_each(policy_counter_info_rm->pen_pol_counter_statuses, node) {
            OpenAPI_pending_policy_counter_status_free(node->data);
        }
        OpenAPI_list_free(policy_counter_info_rm->pen_pol_counter_statuses);
        policy_counter_info_rm->pen_pol_counter_statuses = NULL;
    }
    ogs_free(policy_counter_info_rm);
}

cJSON *OpenAPI_policy_counter_info_rm_convertToJSON(OpenAPI_policy_counter_info_rm_t *policy_counter_info_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (policy_counter_info_rm == NULL) {
        ogs_error("OpenAPI_policy_counter_info_rm_convertToJSON() failed [PolicyCounterInfoRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (policy_counter_info_rm->current_status) {
    if (cJSON_AddStringToObject(item, "currentStatus", policy_counter_info_rm->current_status) == NULL) {
        ogs_error("OpenAPI_policy_counter_info_rm_convertToJSON() failed [current_status]");
        goto end;
    }
    } else if (policy_counter_info_rm->is_current_status_null) {
        if (cJSON_AddNullToObject(item, "currentStatus") == NULL) {
            ogs_error("OpenAPI_policy_counter_info_rm_convertToJSON() failed [current_status]");
            goto end;
        }
    }

    if (policy_counter_info_rm->pen_pol_counter_statuses) {
    cJSON *pen_pol_counter_statusesList = cJSON_AddArrayToObject(item, "penPolCounterStatuses");
    if (pen_pol_counter_statusesList == NULL) {
        ogs_error("OpenAPI_policy_counter_info_rm_convertToJSON() failed [pen_pol_counter_statuses]");
        goto end;
    }
    OpenAPI_list_for_each(policy_counter_info_rm->pen_pol_counter_statuses, node) {
        cJSON *itemLocal = OpenAPI_pending_policy_counter_status_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_counter_info_rm_convertToJSON() failed [pen_pol_counter_statuses]");
            goto end;
        }
        cJSON_AddItemToArray(pen_pol_counter_statusesList, itemLocal);
    }
    } else if (policy_counter_info_rm->is_pen_pol_counter_statuses_null) {
        if (cJSON_AddNullToObject(item, "penPolCounterStatuses") == NULL) {
            ogs_error("OpenAPI_policy_counter_info_rm_convertToJSON() failed [pen_pol_counter_statuses]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_policy_counter_info_rm_t *OpenAPI_policy_counter_info_rm_parseFromJSON(cJSON *policy_counter_info_rmJSON)
{
    OpenAPI_policy_counter_info_rm_t *policy_counter_info_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *current_status = NULL;
    cJSON *pen_pol_counter_statuses = NULL;
    OpenAPI_list_t *pen_pol_counter_statusesList = NULL;
    current_status = cJSON_GetObjectItemCaseSensitive(policy_counter_info_rmJSON, "currentStatus");
    if (current_status) {
    if (!cJSON_IsNull(current_status)) {
    if (!cJSON_IsString(current_status) && !cJSON_IsNull(current_status)) {
        ogs_error("OpenAPI_policy_counter_info_rm_parseFromJSON() failed [current_status]");
        goto end;
    }
    }
    }

    pen_pol_counter_statuses = cJSON_GetObjectItemCaseSensitive(policy_counter_info_rmJSON, "penPolCounterStatuses");
    if (pen_pol_counter_statuses) {
    if (!cJSON_IsNull(pen_pol_counter_statuses)) {
        cJSON *pen_pol_counter_statuses_local = NULL;
        if (!cJSON_IsArray(pen_pol_counter_statuses)) {
            ogs_error("OpenAPI_policy_counter_info_rm_parseFromJSON() failed [pen_pol_counter_statuses]");
            goto end;
        }

        pen_pol_counter_statusesList = OpenAPI_list_create();

        cJSON_ArrayForEach(pen_pol_counter_statuses_local, pen_pol_counter_statuses) {
            if (!cJSON_IsObject(pen_pol_counter_statuses_local)) {
                ogs_error("OpenAPI_policy_counter_info_rm_parseFromJSON() failed [pen_pol_counter_statuses]");
                goto end;
            }
            OpenAPI_pending_policy_counter_status_t *pen_pol_counter_statusesItem = OpenAPI_pending_policy_counter_status_parseFromJSON(pen_pol_counter_statuses_local);
            if (!pen_pol_counter_statusesItem) {
                ogs_error("No pen_pol_counter_statusesItem");
                goto end;
            }
            OpenAPI_list_add(pen_pol_counter_statusesList, pen_pol_counter_statusesItem);
        }
    }
    }

    policy_counter_info_rm_local_var = OpenAPI_policy_counter_info_rm_create (
        current_status && cJSON_IsNull(current_status) ? true : false,
        current_status && !cJSON_IsNull(current_status) ? ogs_strdup(current_status->valuestring) : NULL,
        pen_pol_counter_statuses && cJSON_IsNull(pen_pol_counter_statuses) ? true : false,
        pen_pol_counter_statuses ? pen_pol_counter_statusesList : NULL
    );

    return policy_counter_info_rm_local_var;
end:
    if (pen_pol_counter_statusesList) {
        OpenAPI_list_for_each(pen_pol_counter_statusesList, node) {
            OpenAPI_pending_policy_counter_status_free(node->data);
        }
        OpenAPI_list_free(pen_pol_counter_statusesList);
        pen_pol_counter_statusesList = NULL;
    }
    return NULL;
}

OpenAPI_policy_counter_info_rm_t *OpenAPI_policy_counter_info_rm_copy(OpenAPI_policy_counter_info_rm_t *dst, OpenAPI_policy_counter_info_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_counter_info_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_counter_info_rm_convertToJSON() failed");
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

    OpenAPI_policy_counter_info_rm_free(dst);
    dst = OpenAPI_policy_counter_info_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_policy_assist_info.h"

OpenAPI_qos_policy_assist_info_t *OpenAPI_qos_policy_assist_info_create(
    OpenAPI_list_t *qos_pol_assist_info
)
{
    OpenAPI_qos_policy_assist_info_t *qos_policy_assist_info_local_var = ogs_malloc(sizeof(OpenAPI_qos_policy_assist_info_t));
    ogs_assert(qos_policy_assist_info_local_var);

    qos_policy_assist_info_local_var->qos_pol_assist_info = qos_pol_assist_info;

    return qos_policy_assist_info_local_var;
}

void OpenAPI_qos_policy_assist_info_free(OpenAPI_qos_policy_assist_info_t *qos_policy_assist_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_policy_assist_info) {
        return;
    }
    if (qos_policy_assist_info->qos_pol_assist_info) {
        OpenAPI_list_for_each(qos_policy_assist_info->qos_pol_assist_info, node) {
            OpenAPI_qos_policy_assist_sets_per_ts_free(node->data);
        }
        OpenAPI_list_free(qos_policy_assist_info->qos_pol_assist_info);
        qos_policy_assist_info->qos_pol_assist_info = NULL;
    }
    ogs_free(qos_policy_assist_info);
}

cJSON *OpenAPI_qos_policy_assist_info_convertToJSON(OpenAPI_qos_policy_assist_info_t *qos_policy_assist_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_policy_assist_info == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_info_convertToJSON() failed [QosPolicyAssistInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_policy_assist_info->qos_pol_assist_info) {
    cJSON *qos_pol_assist_infoList = cJSON_AddArrayToObject(item, "qosPolAssistInfo");
    if (qos_pol_assist_infoList == NULL) {
        ogs_error("OpenAPI_qos_policy_assist_info_convertToJSON() failed [qos_pol_assist_info]");
        goto end;
    }
    OpenAPI_list_for_each(qos_policy_assist_info->qos_pol_assist_info, node) {
        cJSON *itemLocal = OpenAPI_qos_policy_assist_sets_per_ts_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qos_policy_assist_info_convertToJSON() failed [qos_pol_assist_info]");
            goto end;
        }
        cJSON_AddItemToArray(qos_pol_assist_infoList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_qos_policy_assist_info_t *OpenAPI_qos_policy_assist_info_parseFromJSON(cJSON *qos_policy_assist_infoJSON)
{
    OpenAPI_qos_policy_assist_info_t *qos_policy_assist_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qos_pol_assist_info = NULL;
    OpenAPI_list_t *qos_pol_assist_infoList = NULL;
    qos_pol_assist_info = cJSON_GetObjectItemCaseSensitive(qos_policy_assist_infoJSON, "qosPolAssistInfo");
    if (qos_pol_assist_info) {
        cJSON *qos_pol_assist_info_local = NULL;
        if (!cJSON_IsArray(qos_pol_assist_info)) {
            ogs_error("OpenAPI_qos_policy_assist_info_parseFromJSON() failed [qos_pol_assist_info]");
            goto end;
        }

        qos_pol_assist_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(qos_pol_assist_info_local, qos_pol_assist_info) {
            if (!cJSON_IsObject(qos_pol_assist_info_local)) {
                ogs_error("OpenAPI_qos_policy_assist_info_parseFromJSON() failed [qos_pol_assist_info]");
                goto end;
            }
            OpenAPI_qos_policy_assist_sets_per_ts_t *qos_pol_assist_infoItem = OpenAPI_qos_policy_assist_sets_per_ts_parseFromJSON(qos_pol_assist_info_local);
            if (!qos_pol_assist_infoItem) {
                ogs_error("No qos_pol_assist_infoItem");
                goto end;
            }
            OpenAPI_list_add(qos_pol_assist_infoList, qos_pol_assist_infoItem);
        }
    }

    qos_policy_assist_info_local_var = OpenAPI_qos_policy_assist_info_create (
        qos_pol_assist_info ? qos_pol_assist_infoList : NULL
    );

    return qos_policy_assist_info_local_var;
end:
    if (qos_pol_assist_infoList) {
        OpenAPI_list_for_each(qos_pol_assist_infoList, node) {
            OpenAPI_qos_policy_assist_sets_per_ts_free(node->data);
        }
        OpenAPI_list_free(qos_pol_assist_infoList);
        qos_pol_assist_infoList = NULL;
    }
    return NULL;
}

OpenAPI_qos_policy_assist_info_t *OpenAPI_qos_policy_assist_info_copy(OpenAPI_qos_policy_assist_info_t *dst, OpenAPI_qos_policy_assist_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_policy_assist_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_policy_assist_info_convertToJSON() failed");
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

    OpenAPI_qos_policy_assist_info_free(dst);
    dst = OpenAPI_qos_policy_assist_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "limit_id_to_monitoring_key.h"

OpenAPI_limit_id_to_monitoring_key_t *OpenAPI_limit_id_to_monitoring_key_create(
    char *limit_id,
    OpenAPI_list_t *monkey
    )
{
    OpenAPI_limit_id_to_monitoring_key_t *limit_id_to_monitoring_key_local_var = OpenAPI_malloc(sizeof(OpenAPI_limit_id_to_monitoring_key_t));
    if (!limit_id_to_monitoring_key_local_var) {
        return NULL;
    }
    limit_id_to_monitoring_key_local_var->limit_id = limit_id;
    limit_id_to_monitoring_key_local_var->monkey = monkey;

    return limit_id_to_monitoring_key_local_var;
}

void OpenAPI_limit_id_to_monitoring_key_free(OpenAPI_limit_id_to_monitoring_key_t *limit_id_to_monitoring_key)
{
    if (NULL == limit_id_to_monitoring_key) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(limit_id_to_monitoring_key->limit_id);
    OpenAPI_list_for_each(limit_id_to_monitoring_key->monkey, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(limit_id_to_monitoring_key->monkey);
    ogs_free(limit_id_to_monitoring_key);
}

cJSON *OpenAPI_limit_id_to_monitoring_key_convertToJSON(OpenAPI_limit_id_to_monitoring_key_t *limit_id_to_monitoring_key)
{
    cJSON *item = NULL;

    if (limit_id_to_monitoring_key == NULL) {
        ogs_error("OpenAPI_limit_id_to_monitoring_key_convertToJSON() failed [LimitIdToMonitoringKey]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!limit_id_to_monitoring_key->limit_id) {
        ogs_error("OpenAPI_limit_id_to_monitoring_key_convertToJSON() failed [limit_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "limitId", limit_id_to_monitoring_key->limit_id) == NULL) {
        ogs_error("OpenAPI_limit_id_to_monitoring_key_convertToJSON() failed [limit_id]");
        goto end;
    }

    if (limit_id_to_monitoring_key->monkey) {
        cJSON *monkey = cJSON_AddArrayToObject(item, "monkey");
        if (monkey == NULL) {
            ogs_error("OpenAPI_limit_id_to_monitoring_key_convertToJSON() failed [monkey]");
            goto end;
        }

        OpenAPI_lnode_t *monkey_node;
        OpenAPI_list_for_each(limit_id_to_monitoring_key->monkey, monkey_node)  {
            if (cJSON_AddStringToObject(monkey, "", (char*)monkey_node->data) == NULL) {
                ogs_error("OpenAPI_limit_id_to_monitoring_key_convertToJSON() failed [monkey]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_limit_id_to_monitoring_key_t *OpenAPI_limit_id_to_monitoring_key_parseFromJSON(cJSON *limit_id_to_monitoring_keyJSON)
{
    OpenAPI_limit_id_to_monitoring_key_t *limit_id_to_monitoring_key_local_var = NULL;
    cJSON *limit_id = cJSON_GetObjectItemCaseSensitive(limit_id_to_monitoring_keyJSON, "limitId");
    if (!limit_id) {
        ogs_error("OpenAPI_limit_id_to_monitoring_key_parseFromJSON() failed [limit_id]");
        goto end;
    }


    if (!cJSON_IsString(limit_id)) {
        ogs_error("OpenAPI_limit_id_to_monitoring_key_parseFromJSON() failed [limit_id]");
        goto end;
    }

    cJSON *monkey = cJSON_GetObjectItemCaseSensitive(limit_id_to_monitoring_keyJSON, "monkey");

    OpenAPI_list_t *monkeyList;
    if (monkey) {
        cJSON *monkey_local;
        if (!cJSON_IsArray(monkey)) {
            ogs_error("OpenAPI_limit_id_to_monitoring_key_parseFromJSON() failed [monkey]");
            goto end;
        }
        monkeyList = OpenAPI_list_create();

        cJSON_ArrayForEach(monkey_local, monkey) {
            if (!cJSON_IsString(monkey_local)) {
                ogs_error("OpenAPI_limit_id_to_monitoring_key_parseFromJSON() failed [monkey]");
                goto end;
            }
            OpenAPI_list_add(monkeyList, ogs_strdup(monkey_local->valuestring));
        }
    }

    limit_id_to_monitoring_key_local_var = OpenAPI_limit_id_to_monitoring_key_create (
        ogs_strdup(limit_id->valuestring),
        monkey ? monkeyList : NULL
        );

    return limit_id_to_monitoring_key_local_var;
end:
    return NULL;
}


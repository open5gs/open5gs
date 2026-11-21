
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsnqo_s_information.h"

OpenAPI_tsnqo_s_information_t *OpenAPI_tsnqo_s_information_create(
    bool is_priority,
    int priority,
    OpenAPI_list_t *bridge_delay
)
{
    OpenAPI_tsnqo_s_information_t *tsnqo_s_information_local_var = ogs_malloc(sizeof(OpenAPI_tsnqo_s_information_t));
    ogs_assert(tsnqo_s_information_local_var);

    tsnqo_s_information_local_var->is_priority = is_priority;
    tsnqo_s_information_local_var->priority = priority;
    tsnqo_s_information_local_var->bridge_delay = bridge_delay;

    return tsnqo_s_information_local_var;
}

void OpenAPI_tsnqo_s_information_free(OpenAPI_tsnqo_s_information_t *tsnqo_s_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tsnqo_s_information) {
        return;
    }
    if (tsnqo_s_information->bridge_delay) {
        OpenAPI_list_for_each(tsnqo_s_information->bridge_delay, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(tsnqo_s_information->bridge_delay);
        tsnqo_s_information->bridge_delay = NULL;
    }
    ogs_free(tsnqo_s_information);
}

cJSON *OpenAPI_tsnqo_s_information_convertToJSON(OpenAPI_tsnqo_s_information_t *tsnqo_s_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tsnqo_s_information == NULL) {
        ogs_error("OpenAPI_tsnqo_s_information_convertToJSON() failed [TSNQoSInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tsnqo_s_information->is_priority) {
    if (cJSON_AddNumberToObject(item, "priority", tsnqo_s_information->priority) == NULL) {
        ogs_error("OpenAPI_tsnqo_s_information_convertToJSON() failed [priority]");
        goto end;
    }
    }

    if (tsnqo_s_information->bridge_delay) {
    cJSON *bridge_delayList = cJSON_AddArrayToObject(item, "bridgeDelay");
    if (bridge_delayList == NULL) {
        ogs_error("OpenAPI_tsnqo_s_information_convertToJSON() failed [bridge_delay]");
        goto end;
    }
    OpenAPI_list_for_each(tsnqo_s_information->bridge_delay, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_tsnqo_s_information_convertToJSON() failed [bridge_delay]");
            goto end;
        }
        if (cJSON_AddNumberToObject(bridge_delayList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_tsnqo_s_information_convertToJSON() failed [bridge_delay]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_tsnqo_s_information_t *OpenAPI_tsnqo_s_information_parseFromJSON(cJSON *tsnqo_s_informationJSON)
{
    OpenAPI_tsnqo_s_information_t *tsnqo_s_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *priority = NULL;
    cJSON *bridge_delay = NULL;
    OpenAPI_list_t *bridge_delayList = NULL;
    priority = cJSON_GetObjectItemCaseSensitive(tsnqo_s_informationJSON, "priority");
    if (priority) {
    if (!cJSON_IsNumber(priority)) {
        ogs_error("OpenAPI_tsnqo_s_information_parseFromJSON() failed [priority]");
        goto end;
    }
    }

    bridge_delay = cJSON_GetObjectItemCaseSensitive(tsnqo_s_informationJSON, "bridgeDelay");
    if (bridge_delay) {
        cJSON *bridge_delay_local = NULL;
        if (!cJSON_IsArray(bridge_delay)) {
            ogs_error("OpenAPI_tsnqo_s_information_parseFromJSON() failed [bridge_delay]");
            goto end;
        }

        bridge_delayList = OpenAPI_list_create();

        cJSON_ArrayForEach(bridge_delay_local, bridge_delay) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(bridge_delay_local)) {
                ogs_error("OpenAPI_tsnqo_s_information_parseFromJSON() failed [bridge_delay]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_tsnqo_s_information_parseFromJSON() failed [bridge_delay]");
                goto end;
            }
            *localDouble = bridge_delay_local->valuedouble;
            OpenAPI_list_add(bridge_delayList, localDouble);
        }
    }

    tsnqo_s_information_local_var = OpenAPI_tsnqo_s_information_create (
        priority ? true : false,
        priority ? priority->valuedouble : 0,
        bridge_delay ? bridge_delayList : NULL
    );

    return tsnqo_s_information_local_var;
end:
    if (bridge_delayList) {
        OpenAPI_list_for_each(bridge_delayList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(bridge_delayList);
        bridge_delayList = NULL;
    }
    return NULL;
}

OpenAPI_tsnqo_s_information_t *OpenAPI_tsnqo_s_information_copy(OpenAPI_tsnqo_s_information_t *dst, OpenAPI_tsnqo_s_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tsnqo_s_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tsnqo_s_information_convertToJSON() failed");
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

    OpenAPI_tsnqo_s_information_free(dst);
    dst = OpenAPI_tsnqo_s_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


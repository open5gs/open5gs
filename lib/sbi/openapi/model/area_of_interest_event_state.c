
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "area_of_interest_event_state.h"

OpenAPI_area_of_interest_event_state_t *OpenAPI_area_of_interest_event_state_create(
    OpenAPI_presence_state_e presence,
    OpenAPI_list_t *individual_pra_id_list
)
{
    OpenAPI_area_of_interest_event_state_t *area_of_interest_event_state_local_var = ogs_malloc(sizeof(OpenAPI_area_of_interest_event_state_t));
    ogs_assert(area_of_interest_event_state_local_var);

    area_of_interest_event_state_local_var->presence = presence;
    area_of_interest_event_state_local_var->individual_pra_id_list = individual_pra_id_list;

    return area_of_interest_event_state_local_var;
}

void OpenAPI_area_of_interest_event_state_free(OpenAPI_area_of_interest_event_state_t *area_of_interest_event_state)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == area_of_interest_event_state) {
        return;
    }
    if (area_of_interest_event_state->individual_pra_id_list) {
        OpenAPI_list_for_each(area_of_interest_event_state->individual_pra_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(area_of_interest_event_state->individual_pra_id_list);
        area_of_interest_event_state->individual_pra_id_list = NULL;
    }
    ogs_free(area_of_interest_event_state);
}

cJSON *OpenAPI_area_of_interest_event_state_convertToJSON(OpenAPI_area_of_interest_event_state_t *area_of_interest_event_state)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (area_of_interest_event_state == NULL) {
        ogs_error("OpenAPI_area_of_interest_event_state_convertToJSON() failed [AreaOfInterestEventState]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (area_of_interest_event_state->presence == OpenAPI_presence_state_NULL) {
        ogs_error("OpenAPI_area_of_interest_event_state_convertToJSON() failed [presence]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "presence", OpenAPI_presence_state_ToString(area_of_interest_event_state->presence)) == NULL) {
        ogs_error("OpenAPI_area_of_interest_event_state_convertToJSON() failed [presence]");
        goto end;
    }

    if (area_of_interest_event_state->individual_pra_id_list) {
    cJSON *individual_pra_id_listList = cJSON_AddArrayToObject(item, "individualPraIdList");
    if (individual_pra_id_listList == NULL) {
        ogs_error("OpenAPI_area_of_interest_event_state_convertToJSON() failed [individual_pra_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(area_of_interest_event_state->individual_pra_id_list, node) {
        if (cJSON_AddStringToObject(individual_pra_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_area_of_interest_event_state_convertToJSON() failed [individual_pra_id_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_area_of_interest_event_state_t *OpenAPI_area_of_interest_event_state_parseFromJSON(cJSON *area_of_interest_event_stateJSON)
{
    OpenAPI_area_of_interest_event_state_t *area_of_interest_event_state_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *presence = NULL;
    OpenAPI_presence_state_e presenceVariable = 0;
    cJSON *individual_pra_id_list = NULL;
    OpenAPI_list_t *individual_pra_id_listList = NULL;
    presence = cJSON_GetObjectItemCaseSensitive(area_of_interest_event_stateJSON, "presence");
    if (!presence) {
        ogs_error("OpenAPI_area_of_interest_event_state_parseFromJSON() failed [presence]");
        goto end;
    }
    if (!cJSON_IsString(presence)) {
        ogs_error("OpenAPI_area_of_interest_event_state_parseFromJSON() failed [presence]");
        goto end;
    }
    presenceVariable = OpenAPI_presence_state_FromString(presence->valuestring);

    individual_pra_id_list = cJSON_GetObjectItemCaseSensitive(area_of_interest_event_stateJSON, "individualPraIdList");
    if (individual_pra_id_list) {
        cJSON *individual_pra_id_list_local = NULL;
        if (!cJSON_IsArray(individual_pra_id_list)) {
            ogs_error("OpenAPI_area_of_interest_event_state_parseFromJSON() failed [individual_pra_id_list]");
            goto end;
        }

        individual_pra_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(individual_pra_id_list_local, individual_pra_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(individual_pra_id_list_local)) {
                ogs_error("OpenAPI_area_of_interest_event_state_parseFromJSON() failed [individual_pra_id_list]");
                goto end;
            }
            OpenAPI_list_add(individual_pra_id_listList, ogs_strdup(individual_pra_id_list_local->valuestring));
        }
    }

    area_of_interest_event_state_local_var = OpenAPI_area_of_interest_event_state_create (
        presenceVariable,
        individual_pra_id_list ? individual_pra_id_listList : NULL
    );

    return area_of_interest_event_state_local_var;
end:
    if (individual_pra_id_listList) {
        OpenAPI_list_for_each(individual_pra_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(individual_pra_id_listList);
        individual_pra_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_area_of_interest_event_state_t *OpenAPI_area_of_interest_event_state_copy(OpenAPI_area_of_interest_event_state_t *dst, OpenAPI_area_of_interest_event_state_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_area_of_interest_event_state_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_area_of_interest_event_state_convertToJSON() failed");
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

    OpenAPI_area_of_interest_event_state_free(dst);
    dst = OpenAPI_area_of_interest_event_state_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


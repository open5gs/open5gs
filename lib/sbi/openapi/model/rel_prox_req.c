
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rel_prox_req.h"

OpenAPI_rel_prox_req_t *OpenAPI_rel_prox_req_create(
    OpenAPI_list_t *direction,
    bool is_num_of_ue,
    int num_of_ue,
    OpenAPI_list_t *proximity_crits
)
{
    OpenAPI_rel_prox_req_t *rel_prox_req_local_var = ogs_malloc(sizeof(OpenAPI_rel_prox_req_t));
    ogs_assert(rel_prox_req_local_var);

    rel_prox_req_local_var->direction = direction;
    rel_prox_req_local_var->is_num_of_ue = is_num_of_ue;
    rel_prox_req_local_var->num_of_ue = num_of_ue;
    rel_prox_req_local_var->proximity_crits = proximity_crits;

    return rel_prox_req_local_var;
}

void OpenAPI_rel_prox_req_free(OpenAPI_rel_prox_req_t *rel_prox_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rel_prox_req) {
        return;
    }
    if (rel_prox_req->direction) {
        OpenAPI_list_free(rel_prox_req->direction);
        rel_prox_req->direction = NULL;
    }
    if (rel_prox_req->proximity_crits) {
        OpenAPI_list_free(rel_prox_req->proximity_crits);
        rel_prox_req->proximity_crits = NULL;
    }
    ogs_free(rel_prox_req);
}

cJSON *OpenAPI_rel_prox_req_convertToJSON(OpenAPI_rel_prox_req_t *rel_prox_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rel_prox_req == NULL) {
        ogs_error("OpenAPI_rel_prox_req_convertToJSON() failed [RelProxReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (rel_prox_req->direction != OpenAPI_direction_NULL) {
    cJSON *directionList = cJSON_AddArrayToObject(item, "direction");
    if (directionList == NULL) {
        ogs_error("OpenAPI_rel_prox_req_convertToJSON() failed [direction]");
        goto end;
    }
    OpenAPI_list_for_each(rel_prox_req->direction, node) {
        if (cJSON_AddStringToObject(directionList, "", OpenAPI_direction_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_rel_prox_req_convertToJSON() failed [direction]");
            goto end;
        }
    }
    }

    if (rel_prox_req->is_num_of_ue) {
    if (cJSON_AddNumberToObject(item, "numOfUe", rel_prox_req->num_of_ue) == NULL) {
        ogs_error("OpenAPI_rel_prox_req_convertToJSON() failed [num_of_ue]");
        goto end;
    }
    }

    if (rel_prox_req->proximity_crits != OpenAPI_proximity_criterion_NULL) {
    cJSON *proximity_critsList = cJSON_AddArrayToObject(item, "proximityCrits");
    if (proximity_critsList == NULL) {
        ogs_error("OpenAPI_rel_prox_req_convertToJSON() failed [proximity_crits]");
        goto end;
    }
    OpenAPI_list_for_each(rel_prox_req->proximity_crits, node) {
        if (cJSON_AddStringToObject(proximity_critsList, "", OpenAPI_proximity_criterion_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_rel_prox_req_convertToJSON() failed [proximity_crits]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_rel_prox_req_t *OpenAPI_rel_prox_req_parseFromJSON(cJSON *rel_prox_reqJSON)
{
    OpenAPI_rel_prox_req_t *rel_prox_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *direction = NULL;
    OpenAPI_list_t *directionList = NULL;
    cJSON *num_of_ue = NULL;
    cJSON *proximity_crits = NULL;
    OpenAPI_list_t *proximity_critsList = NULL;
    direction = cJSON_GetObjectItemCaseSensitive(rel_prox_reqJSON, "direction");
    if (direction) {
        cJSON *direction_local = NULL;
        if (!cJSON_IsArray(direction)) {
            ogs_error("OpenAPI_rel_prox_req_parseFromJSON() failed [direction]");
            goto end;
        }

        directionList = OpenAPI_list_create();

        cJSON_ArrayForEach(direction_local, direction) {
            OpenAPI_direction_e localEnum = OpenAPI_direction_NULL;
            if (!cJSON_IsString(direction_local)) {
                ogs_error("OpenAPI_rel_prox_req_parseFromJSON() failed [direction]");
                goto end;
            }
            localEnum = OpenAPI_direction_FromString(direction_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"direction\" is not supported. Ignoring it ...",
                         direction_local->valuestring);
            } else {
                OpenAPI_list_add(directionList, (void *)localEnum);
            }
        }
        if (directionList->count == 0) {
            ogs_error("OpenAPI_rel_prox_req_parseFromJSON() failed: Expected directionList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    num_of_ue = cJSON_GetObjectItemCaseSensitive(rel_prox_reqJSON, "numOfUe");
    if (num_of_ue) {
    if (!cJSON_IsNumber(num_of_ue)) {
        ogs_error("OpenAPI_rel_prox_req_parseFromJSON() failed [num_of_ue]");
        goto end;
    }
    }

    proximity_crits = cJSON_GetObjectItemCaseSensitive(rel_prox_reqJSON, "proximityCrits");
    if (proximity_crits) {
        cJSON *proximity_crits_local = NULL;
        if (!cJSON_IsArray(proximity_crits)) {
            ogs_error("OpenAPI_rel_prox_req_parseFromJSON() failed [proximity_crits]");
            goto end;
        }

        proximity_critsList = OpenAPI_list_create();

        cJSON_ArrayForEach(proximity_crits_local, proximity_crits) {
            OpenAPI_proximity_criterion_e localEnum = OpenAPI_proximity_criterion_NULL;
            if (!cJSON_IsString(proximity_crits_local)) {
                ogs_error("OpenAPI_rel_prox_req_parseFromJSON() failed [proximity_crits]");
                goto end;
            }
            localEnum = OpenAPI_proximity_criterion_FromString(proximity_crits_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"proximity_crits\" is not supported. Ignoring it ...",
                         proximity_crits_local->valuestring);
            } else {
                OpenAPI_list_add(proximity_critsList, (void *)localEnum);
            }
        }
        if (proximity_critsList->count == 0) {
            ogs_error("OpenAPI_rel_prox_req_parseFromJSON() failed: Expected proximity_critsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    rel_prox_req_local_var = OpenAPI_rel_prox_req_create (
        direction ? directionList : NULL,
        num_of_ue ? true : false,
        num_of_ue ? num_of_ue->valuedouble : 0,
        proximity_crits ? proximity_critsList : NULL
    );

    return rel_prox_req_local_var;
end:
    if (directionList) {
        OpenAPI_list_free(directionList);
        directionList = NULL;
    }
    if (proximity_critsList) {
        OpenAPI_list_free(proximity_critsList);
        proximity_critsList = NULL;
    }
    return NULL;
}

OpenAPI_rel_prox_req_t *OpenAPI_rel_prox_req_copy(OpenAPI_rel_prox_req_t *dst, OpenAPI_rel_prox_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rel_prox_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rel_prox_req_convertToJSON() failed");
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

    OpenAPI_rel_prox_req_free(dst);
    dst = OpenAPI_rel_prox_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


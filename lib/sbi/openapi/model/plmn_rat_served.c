
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_rat_served.h"

OpenAPI_plmn_rat_served_t *OpenAPI_plmn_rat_served_create(
    OpenAPI_plmn_id_t *plmn,
    OpenAPI_list_t *rats
    )
{
    OpenAPI_plmn_rat_served_t *plmn_rat_served_local_var = OpenAPI_malloc(sizeof(OpenAPI_plmn_rat_served_t));
    if (!plmn_rat_served_local_var) {
        return NULL;
    }
    plmn_rat_served_local_var->plmn = plmn;
    plmn_rat_served_local_var->rats = rats;

    return plmn_rat_served_local_var;
}

void OpenAPI_plmn_rat_served_free(OpenAPI_plmn_rat_served_t *plmn_rat_served)
{
    if (NULL == plmn_rat_served) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_plmn_id_free(plmn_rat_served->plmn);
    OpenAPI_list_for_each(plmn_rat_served->rats, node) {
        OpenAPI_v2x_rat_type_free(node->data);
    }
    OpenAPI_list_free(plmn_rat_served->rats);
    ogs_free(plmn_rat_served);
}

cJSON *OpenAPI_plmn_rat_served_convertToJSON(OpenAPI_plmn_rat_served_t *plmn_rat_served)
{
    cJSON *item = NULL;

    if (plmn_rat_served == NULL) {
        ogs_error("OpenAPI_plmn_rat_served_convertToJSON() failed [PlmnRatServed]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!plmn_rat_served->plmn) {
        ogs_error("OpenAPI_plmn_rat_served_convertToJSON() failed [plmn]");
        goto end;
    }
    cJSON *plmn_local_JSON = OpenAPI_plmn_id_convertToJSON(plmn_rat_served->plmn);
    if (plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_plmn_rat_served_convertToJSON() failed [plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmn", plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_plmn_rat_served_convertToJSON() failed [plmn]");
        goto end;
    }

    if (!plmn_rat_served->rats) {
        ogs_error("OpenAPI_plmn_rat_served_convertToJSON() failed [rats]");
        goto end;
    }
    cJSON *ratsList = cJSON_AddArrayToObject(item, "rats");
    if (ratsList == NULL) {
        ogs_error("OpenAPI_plmn_rat_served_convertToJSON() failed [rats]");
        goto end;
    }

    OpenAPI_lnode_t *rats_node;
    if (plmn_rat_served->rats) {
        OpenAPI_list_for_each(plmn_rat_served->rats, rats_node) {
            cJSON *itemLocal = OpenAPI_v2x_rat_type_convertToJSON(rats_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_plmn_rat_served_convertToJSON() failed [rats]");
                goto end;
            }
            cJSON_AddItemToArray(ratsList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_plmn_rat_served_t *OpenAPI_plmn_rat_served_parseFromJSON(cJSON *plmn_rat_servedJSON)
{
    OpenAPI_plmn_rat_served_t *plmn_rat_served_local_var = NULL;
    cJSON *plmn = cJSON_GetObjectItemCaseSensitive(plmn_rat_servedJSON, "plmn");
    if (!plmn) {
        ogs_error("OpenAPI_plmn_rat_served_parseFromJSON() failed [plmn]");
        goto end;
    }

    OpenAPI_plmn_id_t *plmn_local_nonprim = NULL;

    plmn_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn);

    cJSON *rats = cJSON_GetObjectItemCaseSensitive(plmn_rat_servedJSON, "rats");
    if (!rats) {
        ogs_error("OpenAPI_plmn_rat_served_parseFromJSON() failed [rats]");
        goto end;
    }

    OpenAPI_list_t *ratsList;

    cJSON *rats_local_nonprimitive;
    if (!cJSON_IsArray(rats)) {
        ogs_error("OpenAPI_plmn_rat_served_parseFromJSON() failed [rats]");
        goto end;
    }

    ratsList = OpenAPI_list_create();

    cJSON_ArrayForEach(rats_local_nonprimitive, rats ) {
        if (!cJSON_IsObject(rats_local_nonprimitive)) {
            ogs_error("OpenAPI_plmn_rat_served_parseFromJSON() failed [rats]");
            goto end;
        }
        OpenAPI_v2x_rat_type_t *ratsItem = OpenAPI_v2x_rat_type_parseFromJSON(rats_local_nonprimitive);

        OpenAPI_list_add(ratsList, ratsItem);
    }

    plmn_rat_served_local_var = OpenAPI_plmn_rat_served_create (
        plmn_local_nonprim,
        ratsList
        );

    return plmn_rat_served_local_var;
end:
    return NULL;
}

OpenAPI_plmn_rat_served_t *OpenAPI_plmn_rat_served_copy(OpenAPI_plmn_rat_served_t *dst, OpenAPI_plmn_rat_served_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_rat_served_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_rat_served_convertToJSON() failed");
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

    OpenAPI_plmn_rat_served_free(dst);
    dst = OpenAPI_plmn_rat_served_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


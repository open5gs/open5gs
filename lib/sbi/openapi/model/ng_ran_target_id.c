
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ng_ran_target_id.h"

OpenAPI_ng_ran_target_id_t *OpenAPI_ng_ran_target_id_create(
    OpenAPI_global_ran_node_id_t *ran_node_id,
    OpenAPI_tai_t *tai
)
{
    OpenAPI_ng_ran_target_id_t *ng_ran_target_id_local_var = ogs_malloc(sizeof(OpenAPI_ng_ran_target_id_t));
    ogs_assert(ng_ran_target_id_local_var);

    ng_ran_target_id_local_var->ran_node_id = ran_node_id;
    ng_ran_target_id_local_var->tai = tai;

    return ng_ran_target_id_local_var;
}

void OpenAPI_ng_ran_target_id_free(OpenAPI_ng_ran_target_id_t *ng_ran_target_id)
{
    if (NULL == ng_ran_target_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_global_ran_node_id_free(ng_ran_target_id->ran_node_id);
    OpenAPI_tai_free(ng_ran_target_id->tai);
    ogs_free(ng_ran_target_id);
}

cJSON *OpenAPI_ng_ran_target_id_convertToJSON(OpenAPI_ng_ran_target_id_t *ng_ran_target_id)
{
    cJSON *item = NULL;

    if (ng_ran_target_id == NULL) {
        ogs_error("OpenAPI_ng_ran_target_id_convertToJSON() failed [NgRanTargetId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *ran_node_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(ng_ran_target_id->ran_node_id);
    if (ran_node_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ng_ran_target_id_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ranNodeId", ran_node_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ng_ran_target_id_convertToJSON() failed [ran_node_id]");
        goto end;
    }

    cJSON *tai_local_JSON = OpenAPI_tai_convertToJSON(ng_ran_target_id->tai);
    if (tai_local_JSON == NULL) {
        ogs_error("OpenAPI_ng_ran_target_id_convertToJSON() failed [tai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tai", tai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ng_ran_target_id_convertToJSON() failed [tai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_ng_ran_target_id_t *OpenAPI_ng_ran_target_id_parseFromJSON(cJSON *ng_ran_target_idJSON)
{
    OpenAPI_ng_ran_target_id_t *ng_ran_target_id_local_var = NULL;
    cJSON *ran_node_id = cJSON_GetObjectItemCaseSensitive(ng_ran_target_idJSON, "ranNodeId");
    if (!ran_node_id) {
        ogs_error("OpenAPI_ng_ran_target_id_parseFromJSON() failed [ran_node_id]");
        goto end;
    }

    OpenAPI_global_ran_node_id_t *ran_node_id_local_nonprim = NULL;
    ran_node_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_id);

    cJSON *tai = cJSON_GetObjectItemCaseSensitive(ng_ran_target_idJSON, "tai");
    if (!tai) {
        ogs_error("OpenAPI_ng_ran_target_id_parseFromJSON() failed [tai]");
        goto end;
    }

    OpenAPI_tai_t *tai_local_nonprim = NULL;
    tai_local_nonprim = OpenAPI_tai_parseFromJSON(tai);

    ng_ran_target_id_local_var = OpenAPI_ng_ran_target_id_create (
        ran_node_id_local_nonprim,
        tai_local_nonprim
    );

    return ng_ran_target_id_local_var;
end:
    return NULL;
}

OpenAPI_ng_ran_target_id_t *OpenAPI_ng_ran_target_id_copy(OpenAPI_ng_ran_target_id_t *dst, OpenAPI_ng_ran_target_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ng_ran_target_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ng_ran_target_id_convertToJSON() failed");
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

    OpenAPI_ng_ran_target_id_free(dst);
    dst = OpenAPI_ng_ran_target_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


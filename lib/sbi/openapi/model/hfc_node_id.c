
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hfc_node_id.h"

OpenAPI_hfc_node_id_t *OpenAPI_hfc_node_id_create(
    char *hfc_nid
)
{
    OpenAPI_hfc_node_id_t *hfc_node_id_local_var = ogs_malloc(sizeof(OpenAPI_hfc_node_id_t));
    ogs_assert(hfc_node_id_local_var);

    hfc_node_id_local_var->hfc_nid = hfc_nid;

    return hfc_node_id_local_var;
}

void OpenAPI_hfc_node_id_free(OpenAPI_hfc_node_id_t *hfc_node_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == hfc_node_id) {
        return;
    }
    if (hfc_node_id->hfc_nid) {
        ogs_free(hfc_node_id->hfc_nid);
        hfc_node_id->hfc_nid = NULL;
    }
    ogs_free(hfc_node_id);
}

cJSON *OpenAPI_hfc_node_id_convertToJSON(OpenAPI_hfc_node_id_t *hfc_node_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (hfc_node_id == NULL) {
        ogs_error("OpenAPI_hfc_node_id_convertToJSON() failed [HfcNodeId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!hfc_node_id->hfc_nid) {
        ogs_error("OpenAPI_hfc_node_id_convertToJSON() failed [hfc_nid]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "hfcNId", hfc_node_id->hfc_nid) == NULL) {
        ogs_error("OpenAPI_hfc_node_id_convertToJSON() failed [hfc_nid]");
        goto end;
    }

end:
    return item;
}

OpenAPI_hfc_node_id_t *OpenAPI_hfc_node_id_parseFromJSON(cJSON *hfc_node_idJSON)
{
    OpenAPI_hfc_node_id_t *hfc_node_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *hfc_nid = NULL;
    hfc_nid = cJSON_GetObjectItemCaseSensitive(hfc_node_idJSON, "hfcNId");
    if (!hfc_nid) {
        ogs_error("OpenAPI_hfc_node_id_parseFromJSON() failed [hfc_nid]");
        goto end;
    }
    if (!cJSON_IsString(hfc_nid)) {
        ogs_error("OpenAPI_hfc_node_id_parseFromJSON() failed [hfc_nid]");
        goto end;
    }

    hfc_node_id_local_var = OpenAPI_hfc_node_id_create (
        ogs_strdup(hfc_nid->valuestring)
    );

    return hfc_node_id_local_var;
end:
    return NULL;
}

OpenAPI_hfc_node_id_t *OpenAPI_hfc_node_id_copy(OpenAPI_hfc_node_id_t *dst, OpenAPI_hfc_node_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hfc_node_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hfc_node_id_convertToJSON() failed");
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

    OpenAPI_hfc_node_id_free(dst);
    dst = OpenAPI_hfc_node_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


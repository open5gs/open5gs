
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hfc_node_id.h"

OpenAPI_hfc_node_id_t *OpenAPI_hfc_node_id_create(
    char *hfc_n_id
    )
{
    OpenAPI_hfc_node_id_t *hfc_node_id_local_var = OpenAPI_malloc(sizeof(OpenAPI_hfc_node_id_t));
    if (!hfc_node_id_local_var) {
        return NULL;
    }
    hfc_node_id_local_var->hfc_n_id = hfc_n_id;

    return hfc_node_id_local_var;
}

void OpenAPI_hfc_node_id_free(OpenAPI_hfc_node_id_t *hfc_node_id)
{
    if (NULL == hfc_node_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(hfc_node_id->hfc_n_id);
    ogs_free(hfc_node_id);
}

cJSON *OpenAPI_hfc_node_id_convertToJSON(OpenAPI_hfc_node_id_t *hfc_node_id)
{
    cJSON *item = NULL;

    if (hfc_node_id == NULL) {
        ogs_error("OpenAPI_hfc_node_id_convertToJSON() failed [HfcNodeId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!hfc_node_id->hfc_n_id) {
        ogs_error("OpenAPI_hfc_node_id_convertToJSON() failed [hfc_n_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "hfcNId", hfc_node_id->hfc_n_id) == NULL) {
        ogs_error("OpenAPI_hfc_node_id_convertToJSON() failed [hfc_n_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_hfc_node_id_t *OpenAPI_hfc_node_id_parseFromJSON(cJSON *hfc_node_idJSON)
{
    OpenAPI_hfc_node_id_t *hfc_node_id_local_var = NULL;
    cJSON *hfc_n_id = cJSON_GetObjectItemCaseSensitive(hfc_node_idJSON, "hfcNId");
    if (!hfc_n_id) {
        ogs_error("OpenAPI_hfc_node_id_parseFromJSON() failed [hfc_n_id]");
        goto end;
    }


    if (!cJSON_IsString(hfc_n_id)) {
        ogs_error("OpenAPI_hfc_node_id_parseFromJSON() failed [hfc_n_id]");
        goto end;
    }

    hfc_node_id_local_var = OpenAPI_hfc_node_id_create (
        ogs_strdup(hfc_n_id->valuestring)
        );

    return hfc_node_id_local_var;
end:
    return NULL;
}


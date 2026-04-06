
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "comb_gci_and_hfc_n_ids.h"

OpenAPI_comb_gci_and_hfc_n_ids_t *OpenAPI_comb_gci_and_hfc_n_ids_create(
    char *global_cable_id,
    char *hfc_nid
)
{
    OpenAPI_comb_gci_and_hfc_n_ids_t *comb_gci_and_hfc_n_ids_local_var = ogs_malloc(sizeof(OpenAPI_comb_gci_and_hfc_n_ids_t));
    ogs_assert(comb_gci_and_hfc_n_ids_local_var);

    comb_gci_and_hfc_n_ids_local_var->global_cable_id = global_cable_id;
    comb_gci_and_hfc_n_ids_local_var->hfc_nid = hfc_nid;

    return comb_gci_and_hfc_n_ids_local_var;
}

void OpenAPI_comb_gci_and_hfc_n_ids_free(OpenAPI_comb_gci_and_hfc_n_ids_t *comb_gci_and_hfc_n_ids)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == comb_gci_and_hfc_n_ids) {
        return;
    }
    if (comb_gci_and_hfc_n_ids->global_cable_id) {
        ogs_free(comb_gci_and_hfc_n_ids->global_cable_id);
        comb_gci_and_hfc_n_ids->global_cable_id = NULL;
    }
    if (comb_gci_and_hfc_n_ids->hfc_nid) {
        ogs_free(comb_gci_and_hfc_n_ids->hfc_nid);
        comb_gci_and_hfc_n_ids->hfc_nid = NULL;
    }
    ogs_free(comb_gci_and_hfc_n_ids);
}

cJSON *OpenAPI_comb_gci_and_hfc_n_ids_convertToJSON(OpenAPI_comb_gci_and_hfc_n_ids_t *comb_gci_and_hfc_n_ids)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (comb_gci_and_hfc_n_ids == NULL) {
        ogs_error("OpenAPI_comb_gci_and_hfc_n_ids_convertToJSON() failed [CombGciAndHfcNIds]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (comb_gci_and_hfc_n_ids->global_cable_id) {
    if (cJSON_AddStringToObject(item, "globalCableId", comb_gci_and_hfc_n_ids->global_cable_id) == NULL) {
        ogs_error("OpenAPI_comb_gci_and_hfc_n_ids_convertToJSON() failed [global_cable_id]");
        goto end;
    }
    }

    if (comb_gci_and_hfc_n_ids->hfc_nid) {
    if (cJSON_AddStringToObject(item, "hfcNId", comb_gci_and_hfc_n_ids->hfc_nid) == NULL) {
        ogs_error("OpenAPI_comb_gci_and_hfc_n_ids_convertToJSON() failed [hfc_nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_comb_gci_and_hfc_n_ids_t *OpenAPI_comb_gci_and_hfc_n_ids_parseFromJSON(cJSON *comb_gci_and_hfc_n_idsJSON)
{
    OpenAPI_comb_gci_and_hfc_n_ids_t *comb_gci_and_hfc_n_ids_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *global_cable_id = NULL;
    cJSON *hfc_nid = NULL;
    global_cable_id = cJSON_GetObjectItemCaseSensitive(comb_gci_and_hfc_n_idsJSON, "globalCableId");
    if (global_cable_id) {
    if (!cJSON_IsString(global_cable_id) && !cJSON_IsNull(global_cable_id)) {
        ogs_error("OpenAPI_comb_gci_and_hfc_n_ids_parseFromJSON() failed [global_cable_id]");
        goto end;
    }
    }

    hfc_nid = cJSON_GetObjectItemCaseSensitive(comb_gci_and_hfc_n_idsJSON, "hfcNId");
    if (hfc_nid) {
    if (!cJSON_IsString(hfc_nid) && !cJSON_IsNull(hfc_nid)) {
        ogs_error("OpenAPI_comb_gci_and_hfc_n_ids_parseFromJSON() failed [hfc_nid]");
        goto end;
    }
    }

    comb_gci_and_hfc_n_ids_local_var = OpenAPI_comb_gci_and_hfc_n_ids_create (
        global_cable_id && !cJSON_IsNull(global_cable_id) ? ogs_strdup(global_cable_id->valuestring) : NULL,
        hfc_nid && !cJSON_IsNull(hfc_nid) ? ogs_strdup(hfc_nid->valuestring) : NULL
    );

    return comb_gci_and_hfc_n_ids_local_var;
end:
    return NULL;
}

OpenAPI_comb_gci_and_hfc_n_ids_t *OpenAPI_comb_gci_and_hfc_n_ids_copy(OpenAPI_comb_gci_and_hfc_n_ids_t *dst, OpenAPI_comb_gci_and_hfc_n_ids_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_comb_gci_and_hfc_n_ids_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_comb_gci_and_hfc_n_ids_convertToJSON() failed");
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

    OpenAPI_comb_gci_and_hfc_n_ids_free(dst);
    dst = OpenAPI_comb_gci_and_hfc_n_ids_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


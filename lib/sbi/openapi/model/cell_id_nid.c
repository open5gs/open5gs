
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cell_id_nid.h"

OpenAPI_cell_id_nid_t *OpenAPI_cell_id_nid_create(
    char *cell_id,
    char *nid
)
{
    OpenAPI_cell_id_nid_t *cell_id_nid_local_var = ogs_malloc(sizeof(OpenAPI_cell_id_nid_t));
    ogs_assert(cell_id_nid_local_var);

    cell_id_nid_local_var->cell_id = cell_id;
    cell_id_nid_local_var->nid = nid;

    return cell_id_nid_local_var;
}

void OpenAPI_cell_id_nid_free(OpenAPI_cell_id_nid_t *cell_id_nid)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cell_id_nid) {
        return;
    }
    if (cell_id_nid->cell_id) {
        ogs_free(cell_id_nid->cell_id);
        cell_id_nid->cell_id = NULL;
    }
    if (cell_id_nid->nid) {
        ogs_free(cell_id_nid->nid);
        cell_id_nid->nid = NULL;
    }
    ogs_free(cell_id_nid);
}

cJSON *OpenAPI_cell_id_nid_convertToJSON(OpenAPI_cell_id_nid_t *cell_id_nid)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cell_id_nid == NULL) {
        ogs_error("OpenAPI_cell_id_nid_convertToJSON() failed [CellIdNid]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cell_id_nid->cell_id) {
        ogs_error("OpenAPI_cell_id_nid_convertToJSON() failed [cell_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "cellId", cell_id_nid->cell_id) == NULL) {
        ogs_error("OpenAPI_cell_id_nid_convertToJSON() failed [cell_id]");
        goto end;
    }

    if (!cell_id_nid->nid) {
        ogs_error("OpenAPI_cell_id_nid_convertToJSON() failed [nid]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nid", cell_id_nid->nid) == NULL) {
        ogs_error("OpenAPI_cell_id_nid_convertToJSON() failed [nid]");
        goto end;
    }

end:
    return item;
}

OpenAPI_cell_id_nid_t *OpenAPI_cell_id_nid_parseFromJSON(cJSON *cell_id_nidJSON)
{
    OpenAPI_cell_id_nid_t *cell_id_nid_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cell_id = NULL;
    cJSON *nid = NULL;
    cell_id = cJSON_GetObjectItemCaseSensitive(cell_id_nidJSON, "cellId");
    if (!cell_id) {
        ogs_error("OpenAPI_cell_id_nid_parseFromJSON() failed [cell_id]");
        goto end;
    }
    if (!cJSON_IsString(cell_id)) {
        ogs_error("OpenAPI_cell_id_nid_parseFromJSON() failed [cell_id]");
        goto end;
    }

    nid = cJSON_GetObjectItemCaseSensitive(cell_id_nidJSON, "nid");
    if (!nid) {
        ogs_error("OpenAPI_cell_id_nid_parseFromJSON() failed [nid]");
        goto end;
    }
    if (!cJSON_IsString(nid)) {
        ogs_error("OpenAPI_cell_id_nid_parseFromJSON() failed [nid]");
        goto end;
    }

    cell_id_nid_local_var = OpenAPI_cell_id_nid_create (
        ogs_strdup(cell_id->valuestring),
        ogs_strdup(nid->valuestring)
    );

    return cell_id_nid_local_var;
end:
    return NULL;
}

OpenAPI_cell_id_nid_t *OpenAPI_cell_id_nid_copy(OpenAPI_cell_id_nid_t *dst, OpenAPI_cell_id_nid_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cell_id_nid_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cell_id_nid_convertToJSON() failed");
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

    OpenAPI_cell_id_nid_free(dst);
    dst = OpenAPI_cell_id_nid_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


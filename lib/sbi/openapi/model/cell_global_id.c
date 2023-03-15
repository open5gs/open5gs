
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cell_global_id.h"

OpenAPI_cell_global_id_t *OpenAPI_cell_global_id_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *lac,
    char *cell_id
)
{
    OpenAPI_cell_global_id_t *cell_global_id_local_var = ogs_malloc(sizeof(OpenAPI_cell_global_id_t));
    ogs_assert(cell_global_id_local_var);

    cell_global_id_local_var->plmn_id = plmn_id;
    cell_global_id_local_var->lac = lac;
    cell_global_id_local_var->cell_id = cell_id;

    return cell_global_id_local_var;
}

void OpenAPI_cell_global_id_free(OpenAPI_cell_global_id_t *cell_global_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cell_global_id) {
        return;
    }
    if (cell_global_id->plmn_id) {
        OpenAPI_plmn_id_free(cell_global_id->plmn_id);
        cell_global_id->plmn_id = NULL;
    }
    if (cell_global_id->lac) {
        ogs_free(cell_global_id->lac);
        cell_global_id->lac = NULL;
    }
    if (cell_global_id->cell_id) {
        ogs_free(cell_global_id->cell_id);
        cell_global_id->cell_id = NULL;
    }
    ogs_free(cell_global_id);
}

cJSON *OpenAPI_cell_global_id_convertToJSON(OpenAPI_cell_global_id_t *cell_global_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cell_global_id == NULL) {
        ogs_error("OpenAPI_cell_global_id_convertToJSON() failed [CellGlobalId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cell_global_id->plmn_id) {
        ogs_error("OpenAPI_cell_global_id_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(cell_global_id->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_cell_global_id_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_cell_global_id_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!cell_global_id->lac) {
        ogs_error("OpenAPI_cell_global_id_convertToJSON() failed [lac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "lac", cell_global_id->lac) == NULL) {
        ogs_error("OpenAPI_cell_global_id_convertToJSON() failed [lac]");
        goto end;
    }

    if (!cell_global_id->cell_id) {
        ogs_error("OpenAPI_cell_global_id_convertToJSON() failed [cell_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "cellId", cell_global_id->cell_id) == NULL) {
        ogs_error("OpenAPI_cell_global_id_convertToJSON() failed [cell_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_cell_global_id_t *OpenAPI_cell_global_id_parseFromJSON(cJSON *cell_global_idJSON)
{
    OpenAPI_cell_global_id_t *cell_global_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *lac = NULL;
    cJSON *cell_id = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(cell_global_idJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_cell_global_id_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    lac = cJSON_GetObjectItemCaseSensitive(cell_global_idJSON, "lac");
    if (!lac) {
        ogs_error("OpenAPI_cell_global_id_parseFromJSON() failed [lac]");
        goto end;
    }
    if (!cJSON_IsString(lac)) {
        ogs_error("OpenAPI_cell_global_id_parseFromJSON() failed [lac]");
        goto end;
    }

    cell_id = cJSON_GetObjectItemCaseSensitive(cell_global_idJSON, "cellId");
    if (!cell_id) {
        ogs_error("OpenAPI_cell_global_id_parseFromJSON() failed [cell_id]");
        goto end;
    }
    if (!cJSON_IsString(cell_id)) {
        ogs_error("OpenAPI_cell_global_id_parseFromJSON() failed [cell_id]");
        goto end;
    }

    cell_global_id_local_var = OpenAPI_cell_global_id_create (
        plmn_id_local_nonprim,
        ogs_strdup(lac->valuestring),
        ogs_strdup(cell_id->valuestring)
    );

    return cell_global_id_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_cell_global_id_t *OpenAPI_cell_global_id_copy(OpenAPI_cell_global_id_t *dst, OpenAPI_cell_global_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cell_global_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cell_global_id_convertToJSON() failed");
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

    OpenAPI_cell_global_id_free(dst);
    dst = OpenAPI_cell_global_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ncgi_1.h"

OpenAPI_ncgi_1_t *OpenAPI_ncgi_1_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    char *nr_cell_id,
    char *nid
)
{
    OpenAPI_ncgi_1_t *ncgi_1_local_var = ogs_malloc(sizeof(OpenAPI_ncgi_1_t));
    ogs_assert(ncgi_1_local_var);

    ncgi_1_local_var->plmn_id = plmn_id;
    ncgi_1_local_var->nr_cell_id = nr_cell_id;
    ncgi_1_local_var->nid = nid;

    return ncgi_1_local_var;
}

void OpenAPI_ncgi_1_free(OpenAPI_ncgi_1_t *ncgi_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ncgi_1) {
        return;
    }
    if (ncgi_1->plmn_id) {
        OpenAPI_plmn_id_1_free(ncgi_1->plmn_id);
        ncgi_1->plmn_id = NULL;
    }
    if (ncgi_1->nr_cell_id) {
        ogs_free(ncgi_1->nr_cell_id);
        ncgi_1->nr_cell_id = NULL;
    }
    if (ncgi_1->nid) {
        ogs_free(ncgi_1->nid);
        ncgi_1->nid = NULL;
    }
    ogs_free(ncgi_1);
}

cJSON *OpenAPI_ncgi_1_convertToJSON(OpenAPI_ncgi_1_t *ncgi_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ncgi_1 == NULL) {
        ogs_error("OpenAPI_ncgi_1_convertToJSON() failed [Ncgi_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ncgi_1->plmn_id) {
        ogs_error("OpenAPI_ncgi_1_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(ncgi_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ncgi_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ncgi_1_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!ncgi_1->nr_cell_id) {
        ogs_error("OpenAPI_ncgi_1_convertToJSON() failed [nr_cell_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nrCellId", ncgi_1->nr_cell_id) == NULL) {
        ogs_error("OpenAPI_ncgi_1_convertToJSON() failed [nr_cell_id]");
        goto end;
    }

    if (ncgi_1->nid) {
    if (cJSON_AddStringToObject(item, "nid", ncgi_1->nid) == NULL) {
        ogs_error("OpenAPI_ncgi_1_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ncgi_1_t *OpenAPI_ncgi_1_parseFromJSON(cJSON *ncgi_1JSON)
{
    OpenAPI_ncgi_1_t *ncgi_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    cJSON *nr_cell_id = NULL;
    cJSON *nid = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(ncgi_1JSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_ncgi_1_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON failed [plmn_id]");
        goto end;
    }

    nr_cell_id = cJSON_GetObjectItemCaseSensitive(ncgi_1JSON, "nrCellId");
    if (!nr_cell_id) {
        ogs_error("OpenAPI_ncgi_1_parseFromJSON() failed [nr_cell_id]");
        goto end;
    }
    if (!cJSON_IsString(nr_cell_id)) {
        ogs_error("OpenAPI_ncgi_1_parseFromJSON() failed [nr_cell_id]");
        goto end;
    }

    nid = cJSON_GetObjectItemCaseSensitive(ncgi_1JSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_ncgi_1_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    ncgi_1_local_var = OpenAPI_ncgi_1_create (
        plmn_id_local_nonprim,
        ogs_strdup(nr_cell_id->valuestring),
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL
    );

    return ncgi_1_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_1_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ncgi_1_t *OpenAPI_ncgi_1_copy(OpenAPI_ncgi_1_t *dst, OpenAPI_ncgi_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ncgi_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ncgi_1_convertToJSON() failed");
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

    OpenAPI_ncgi_1_free(dst);
    dst = OpenAPI_ncgi_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


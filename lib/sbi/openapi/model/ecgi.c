
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecgi.h"

OpenAPI_ecgi_t *OpenAPI_ecgi_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *eutra_cell_id,
    char *nid
)
{
    OpenAPI_ecgi_t *ecgi_local_var = ogs_malloc(sizeof(OpenAPI_ecgi_t));
    ogs_assert(ecgi_local_var);

    ecgi_local_var->plmn_id = plmn_id;
    ecgi_local_var->eutra_cell_id = eutra_cell_id;
    ecgi_local_var->nid = nid;

    return ecgi_local_var;
}

void OpenAPI_ecgi_free(OpenAPI_ecgi_t *ecgi)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ecgi) {
        return;
    }
    if (ecgi->plmn_id) {
        OpenAPI_plmn_id_free(ecgi->plmn_id);
        ecgi->plmn_id = NULL;
    }
    if (ecgi->eutra_cell_id) {
        ogs_free(ecgi->eutra_cell_id);
        ecgi->eutra_cell_id = NULL;
    }
    if (ecgi->nid) {
        ogs_free(ecgi->nid);
        ecgi->nid = NULL;
    }
    ogs_free(ecgi);
}

cJSON *OpenAPI_ecgi_convertToJSON(OpenAPI_ecgi_t *ecgi)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ecgi == NULL) {
        ogs_error("OpenAPI_ecgi_convertToJSON() failed [Ecgi]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ecgi->plmn_id) {
        ogs_error("OpenAPI_ecgi_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(ecgi->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ecgi_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecgi_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!ecgi->eutra_cell_id) {
        ogs_error("OpenAPI_ecgi_convertToJSON() failed [eutra_cell_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "eutraCellId", ecgi->eutra_cell_id) == NULL) {
        ogs_error("OpenAPI_ecgi_convertToJSON() failed [eutra_cell_id]");
        goto end;
    }

    if (ecgi->nid) {
    if (cJSON_AddStringToObject(item, "nid", ecgi->nid) == NULL) {
        ogs_error("OpenAPI_ecgi_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ecgi_t *OpenAPI_ecgi_parseFromJSON(cJSON *ecgiJSON)
{
    OpenAPI_ecgi_t *ecgi_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *eutra_cell_id = NULL;
    cJSON *nid = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(ecgiJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_ecgi_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    eutra_cell_id = cJSON_GetObjectItemCaseSensitive(ecgiJSON, "eutraCellId");
    if (!eutra_cell_id) {
        ogs_error("OpenAPI_ecgi_parseFromJSON() failed [eutra_cell_id]");
        goto end;
    }
    if (!cJSON_IsString(eutra_cell_id)) {
        ogs_error("OpenAPI_ecgi_parseFromJSON() failed [eutra_cell_id]");
        goto end;
    }

    nid = cJSON_GetObjectItemCaseSensitive(ecgiJSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_ecgi_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    ecgi_local_var = OpenAPI_ecgi_create (
        plmn_id_local_nonprim,
        ogs_strdup(eutra_cell_id->valuestring),
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL
    );

    return ecgi_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ecgi_t *OpenAPI_ecgi_copy(OpenAPI_ecgi_t *dst, OpenAPI_ecgi_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ecgi_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ecgi_convertToJSON() failed");
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

    OpenAPI_ecgi_free(dst);
    dst = OpenAPI_ecgi_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


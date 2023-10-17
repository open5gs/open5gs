
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ecgi_1.h"

OpenAPI_ecgi_1_t *OpenAPI_ecgi_1_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    char *eutra_cell_id,
    char *nid
)
{
    OpenAPI_ecgi_1_t *ecgi_1_local_var = ogs_malloc(sizeof(OpenAPI_ecgi_1_t));
    ogs_assert(ecgi_1_local_var);

    ecgi_1_local_var->plmn_id = plmn_id;
    ecgi_1_local_var->eutra_cell_id = eutra_cell_id;
    ecgi_1_local_var->nid = nid;

    return ecgi_1_local_var;
}

void OpenAPI_ecgi_1_free(OpenAPI_ecgi_1_t *ecgi_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ecgi_1) {
        return;
    }
    if (ecgi_1->plmn_id) {
        OpenAPI_plmn_id_1_free(ecgi_1->plmn_id);
        ecgi_1->plmn_id = NULL;
    }
    if (ecgi_1->eutra_cell_id) {
        ogs_free(ecgi_1->eutra_cell_id);
        ecgi_1->eutra_cell_id = NULL;
    }
    if (ecgi_1->nid) {
        ogs_free(ecgi_1->nid);
        ecgi_1->nid = NULL;
    }
    ogs_free(ecgi_1);
}

cJSON *OpenAPI_ecgi_1_convertToJSON(OpenAPI_ecgi_1_t *ecgi_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ecgi_1 == NULL) {
        ogs_error("OpenAPI_ecgi_1_convertToJSON() failed [Ecgi_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ecgi_1->plmn_id) {
        ogs_error("OpenAPI_ecgi_1_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(ecgi_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_ecgi_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ecgi_1_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!ecgi_1->eutra_cell_id) {
        ogs_error("OpenAPI_ecgi_1_convertToJSON() failed [eutra_cell_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "eutraCellId", ecgi_1->eutra_cell_id) == NULL) {
        ogs_error("OpenAPI_ecgi_1_convertToJSON() failed [eutra_cell_id]");
        goto end;
    }

    if (ecgi_1->nid) {
    if (cJSON_AddStringToObject(item, "nid", ecgi_1->nid) == NULL) {
        ogs_error("OpenAPI_ecgi_1_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ecgi_1_t *OpenAPI_ecgi_1_parseFromJSON(cJSON *ecgi_1JSON)
{
    OpenAPI_ecgi_1_t *ecgi_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    cJSON *eutra_cell_id = NULL;
    cJSON *nid = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(ecgi_1JSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_ecgi_1_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON failed [plmn_id]");
        goto end;
    }

    eutra_cell_id = cJSON_GetObjectItemCaseSensitive(ecgi_1JSON, "eutraCellId");
    if (!eutra_cell_id) {
        ogs_error("OpenAPI_ecgi_1_parseFromJSON() failed [eutra_cell_id]");
        goto end;
    }
    if (!cJSON_IsString(eutra_cell_id)) {
        ogs_error("OpenAPI_ecgi_1_parseFromJSON() failed [eutra_cell_id]");
        goto end;
    }

    nid = cJSON_GetObjectItemCaseSensitive(ecgi_1JSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_ecgi_1_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    ecgi_1_local_var = OpenAPI_ecgi_1_create (
        plmn_id_local_nonprim,
        ogs_strdup(eutra_cell_id->valuestring),
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL
    );

    return ecgi_1_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_1_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ecgi_1_t *OpenAPI_ecgi_1_copy(OpenAPI_ecgi_1_t *dst, OpenAPI_ecgi_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ecgi_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ecgi_1_convertToJSON() failed");
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

    OpenAPI_ecgi_1_free(dst);
    dst = OpenAPI_ecgi_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tai_1.h"

OpenAPI_tai_1_t *OpenAPI_tai_1_create(
    OpenAPI_plmn_id_1_t *plmn_id,
    char *tac,
    char *nid
)
{
    OpenAPI_tai_1_t *tai_1_local_var = ogs_malloc(sizeof(OpenAPI_tai_1_t));
    ogs_assert(tai_1_local_var);

    tai_1_local_var->plmn_id = plmn_id;
    tai_1_local_var->tac = tac;
    tai_1_local_var->nid = nid;

    return tai_1_local_var;
}

void OpenAPI_tai_1_free(OpenAPI_tai_1_t *tai_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tai_1) {
        return;
    }
    if (tai_1->plmn_id) {
        OpenAPI_plmn_id_1_free(tai_1->plmn_id);
        tai_1->plmn_id = NULL;
    }
    if (tai_1->tac) {
        ogs_free(tai_1->tac);
        tai_1->tac = NULL;
    }
    if (tai_1->nid) {
        ogs_free(tai_1->nid);
        tai_1->nid = NULL;
    }
    ogs_free(tai_1);
}

cJSON *OpenAPI_tai_1_convertToJSON(OpenAPI_tai_1_t *tai_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tai_1 == NULL) {
        ogs_error("OpenAPI_tai_1_convertToJSON() failed [Tai_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tai_1->plmn_id) {
        ogs_error("OpenAPI_tai_1_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(tai_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_tai_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tai_1_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!tai_1->tac) {
        ogs_error("OpenAPI_tai_1_convertToJSON() failed [tac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tac", tai_1->tac) == NULL) {
        ogs_error("OpenAPI_tai_1_convertToJSON() failed [tac]");
        goto end;
    }

    if (tai_1->nid) {
    if (cJSON_AddStringToObject(item, "nid", tai_1->nid) == NULL) {
        ogs_error("OpenAPI_tai_1_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tai_1_t *OpenAPI_tai_1_parseFromJSON(cJSON *tai_1JSON)
{
    OpenAPI_tai_1_t *tai_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    cJSON *tac = NULL;
    cJSON *nid = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(tai_1JSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_tai_1_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON failed [plmn_id]");
        goto end;
    }

    tac = cJSON_GetObjectItemCaseSensitive(tai_1JSON, "tac");
    if (!tac) {
        ogs_error("OpenAPI_tai_1_parseFromJSON() failed [tac]");
        goto end;
    }
    if (!cJSON_IsString(tac)) {
        ogs_error("OpenAPI_tai_1_parseFromJSON() failed [tac]");
        goto end;
    }

    nid = cJSON_GetObjectItemCaseSensitive(tai_1JSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_tai_1_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    tai_1_local_var = OpenAPI_tai_1_create (
        plmn_id_local_nonprim,
        ogs_strdup(tac->valuestring),
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL
    );

    return tai_1_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_1_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_tai_1_t *OpenAPI_tai_1_copy(OpenAPI_tai_1_t *dst, OpenAPI_tai_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tai_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tai_1_convertToJSON() failed");
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

    OpenAPI_tai_1_free(dst);
    dst = OpenAPI_tai_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


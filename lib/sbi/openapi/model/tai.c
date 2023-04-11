
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tai.h"

OpenAPI_tai_t *OpenAPI_tai_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *tac,
    char *nid
)
{
    OpenAPI_tai_t *tai_local_var = ogs_malloc(sizeof(OpenAPI_tai_t));
    ogs_assert(tai_local_var);

    tai_local_var->plmn_id = plmn_id;
    tai_local_var->tac = tac;
    tai_local_var->nid = nid;

    return tai_local_var;
}

void OpenAPI_tai_free(OpenAPI_tai_t *tai)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tai) {
        return;
    }
    if (tai->plmn_id) {
        OpenAPI_plmn_id_free(tai->plmn_id);
        tai->plmn_id = NULL;
    }
    if (tai->tac) {
        ogs_free(tai->tac);
        tai->tac = NULL;
    }
    if (tai->nid) {
        ogs_free(tai->nid);
        tai->nid = NULL;
    }
    ogs_free(tai);
}

cJSON *OpenAPI_tai_convertToJSON(OpenAPI_tai_t *tai)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tai == NULL) {
        ogs_error("OpenAPI_tai_convertToJSON() failed [Tai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tai->plmn_id) {
        ogs_error("OpenAPI_tai_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(tai->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_tai_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tai_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (!tai->tac) {
        ogs_error("OpenAPI_tai_convertToJSON() failed [tac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tac", tai->tac) == NULL) {
        ogs_error("OpenAPI_tai_convertToJSON() failed [tac]");
        goto end;
    }

    if (tai->nid) {
    if (cJSON_AddStringToObject(item, "nid", tai->nid) == NULL) {
        ogs_error("OpenAPI_tai_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tai_t *OpenAPI_tai_parseFromJSON(cJSON *taiJSON)
{
    OpenAPI_tai_t *tai_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *tac = NULL;
    cJSON *nid = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(taiJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_tai_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    tac = cJSON_GetObjectItemCaseSensitive(taiJSON, "tac");
    if (!tac) {
        ogs_error("OpenAPI_tai_parseFromJSON() failed [tac]");
        goto end;
    }
    if (!cJSON_IsString(tac)) {
        ogs_error("OpenAPI_tai_parseFromJSON() failed [tac]");
        goto end;
    }

    nid = cJSON_GetObjectItemCaseSensitive(taiJSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_tai_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    tai_local_var = OpenAPI_tai_create (
        plmn_id_local_nonprim,
        ogs_strdup(tac->valuestring),
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL
    );

    return tai_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_tai_t *OpenAPI_tai_copy(OpenAPI_tai_t *dst, OpenAPI_tai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tai_convertToJSON() failed");
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

    OpenAPI_tai_free(dst);
    dst = OpenAPI_tai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


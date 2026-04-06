
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tac_nid.h"

OpenAPI_tac_nid_t *OpenAPI_tac_nid_create(
    char *tac,
    char *nid
)
{
    OpenAPI_tac_nid_t *tac_nid_local_var = ogs_malloc(sizeof(OpenAPI_tac_nid_t));
    ogs_assert(tac_nid_local_var);

    tac_nid_local_var->tac = tac;
    tac_nid_local_var->nid = nid;

    return tac_nid_local_var;
}

void OpenAPI_tac_nid_free(OpenAPI_tac_nid_t *tac_nid)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tac_nid) {
        return;
    }
    if (tac_nid->tac) {
        ogs_free(tac_nid->tac);
        tac_nid->tac = NULL;
    }
    if (tac_nid->nid) {
        ogs_free(tac_nid->nid);
        tac_nid->nid = NULL;
    }
    ogs_free(tac_nid);
}

cJSON *OpenAPI_tac_nid_convertToJSON(OpenAPI_tac_nid_t *tac_nid)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tac_nid == NULL) {
        ogs_error("OpenAPI_tac_nid_convertToJSON() failed [TacNid]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tac_nid->tac) {
        ogs_error("OpenAPI_tac_nid_convertToJSON() failed [tac]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tac", tac_nid->tac) == NULL) {
        ogs_error("OpenAPI_tac_nid_convertToJSON() failed [tac]");
        goto end;
    }

    if (!tac_nid->nid) {
        ogs_error("OpenAPI_tac_nid_convertToJSON() failed [nid]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nid", tac_nid->nid) == NULL) {
        ogs_error("OpenAPI_tac_nid_convertToJSON() failed [nid]");
        goto end;
    }

end:
    return item;
}

OpenAPI_tac_nid_t *OpenAPI_tac_nid_parseFromJSON(cJSON *tac_nidJSON)
{
    OpenAPI_tac_nid_t *tac_nid_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tac = NULL;
    cJSON *nid = NULL;
    tac = cJSON_GetObjectItemCaseSensitive(tac_nidJSON, "tac");
    if (!tac) {
        ogs_error("OpenAPI_tac_nid_parseFromJSON() failed [tac]");
        goto end;
    }
    if (!cJSON_IsString(tac)) {
        ogs_error("OpenAPI_tac_nid_parseFromJSON() failed [tac]");
        goto end;
    }

    nid = cJSON_GetObjectItemCaseSensitive(tac_nidJSON, "nid");
    if (!nid) {
        ogs_error("OpenAPI_tac_nid_parseFromJSON() failed [nid]");
        goto end;
    }
    if (!cJSON_IsString(nid)) {
        ogs_error("OpenAPI_tac_nid_parseFromJSON() failed [nid]");
        goto end;
    }

    tac_nid_local_var = OpenAPI_tac_nid_create (
        ogs_strdup(tac->valuestring),
        ogs_strdup(nid->valuestring)
    );

    return tac_nid_local_var;
end:
    return NULL;
}

OpenAPI_tac_nid_t *OpenAPI_tac_nid_copy(OpenAPI_tac_nid_t *dst, OpenAPI_tac_nid_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tac_nid_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tac_nid_convertToJSON() failed");
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

    OpenAPI_tac_nid_free(dst);
    dst = OpenAPI_tac_nid_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


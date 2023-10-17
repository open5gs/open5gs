
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_session_id.h"

OpenAPI_mbs_session_id_t *OpenAPI_mbs_session_id_create(
    OpenAPI_tmgi_t *tmgi,
    OpenAPI_ssm_t *ssm,
    char *nid
)
{
    OpenAPI_mbs_session_id_t *mbs_session_id_local_var = ogs_malloc(sizeof(OpenAPI_mbs_session_id_t));
    ogs_assert(mbs_session_id_local_var);

    mbs_session_id_local_var->tmgi = tmgi;
    mbs_session_id_local_var->ssm = ssm;
    mbs_session_id_local_var->nid = nid;

    return mbs_session_id_local_var;
}

void OpenAPI_mbs_session_id_free(OpenAPI_mbs_session_id_t *mbs_session_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_session_id) {
        return;
    }
    if (mbs_session_id->tmgi) {
        OpenAPI_tmgi_free(mbs_session_id->tmgi);
        mbs_session_id->tmgi = NULL;
    }
    if (mbs_session_id->ssm) {
        OpenAPI_ssm_free(mbs_session_id->ssm);
        mbs_session_id->ssm = NULL;
    }
    if (mbs_session_id->nid) {
        ogs_free(mbs_session_id->nid);
        mbs_session_id->nid = NULL;
    }
    ogs_free(mbs_session_id);
}

cJSON *OpenAPI_mbs_session_id_convertToJSON(OpenAPI_mbs_session_id_t *mbs_session_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_session_id == NULL) {
        ogs_error("OpenAPI_mbs_session_id_convertToJSON() failed [MbsSessionId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbs_session_id->tmgi) {
    cJSON *tmgi_local_JSON = OpenAPI_tmgi_convertToJSON(mbs_session_id->tmgi);
    if (tmgi_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_session_id_convertToJSON() failed [tmgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tmgi", tmgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_session_id_convertToJSON() failed [tmgi]");
        goto end;
    }
    }

    if (mbs_session_id->ssm) {
    cJSON *ssm_local_JSON = OpenAPI_ssm_convertToJSON(mbs_session_id->ssm);
    if (ssm_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_session_id_convertToJSON() failed [ssm]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ssm", ssm_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_session_id_convertToJSON() failed [ssm]");
        goto end;
    }
    }

    if (mbs_session_id->nid) {
    if (cJSON_AddStringToObject(item, "nid", mbs_session_id->nid) == NULL) {
        ogs_error("OpenAPI_mbs_session_id_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mbs_session_id_t *OpenAPI_mbs_session_id_parseFromJSON(cJSON *mbs_session_idJSON)
{
    OpenAPI_mbs_session_id_t *mbs_session_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tmgi = NULL;
    OpenAPI_tmgi_t *tmgi_local_nonprim = NULL;
    cJSON *ssm = NULL;
    OpenAPI_ssm_t *ssm_local_nonprim = NULL;
    cJSON *nid = NULL;
    tmgi = cJSON_GetObjectItemCaseSensitive(mbs_session_idJSON, "tmgi");
    if (tmgi) {
    tmgi_local_nonprim = OpenAPI_tmgi_parseFromJSON(tmgi);
    if (!tmgi_local_nonprim) {
        ogs_error("OpenAPI_tmgi_parseFromJSON failed [tmgi]");
        goto end;
    }
    }

    ssm = cJSON_GetObjectItemCaseSensitive(mbs_session_idJSON, "ssm");
    if (ssm) {
    ssm_local_nonprim = OpenAPI_ssm_parseFromJSON(ssm);
    if (!ssm_local_nonprim) {
        ogs_error("OpenAPI_ssm_parseFromJSON failed [ssm]");
        goto end;
    }
    }

    nid = cJSON_GetObjectItemCaseSensitive(mbs_session_idJSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_mbs_session_id_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    mbs_session_id_local_var = OpenAPI_mbs_session_id_create (
        tmgi ? tmgi_local_nonprim : NULL,
        ssm ? ssm_local_nonprim : NULL,
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL
    );

    return mbs_session_id_local_var;
end:
    if (tmgi_local_nonprim) {
        OpenAPI_tmgi_free(tmgi_local_nonprim);
        tmgi_local_nonprim = NULL;
    }
    if (ssm_local_nonprim) {
        OpenAPI_ssm_free(ssm_local_nonprim);
        ssm_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_mbs_session_id_t *OpenAPI_mbs_session_id_copy(OpenAPI_mbs_session_id_t *dst, OpenAPI_mbs_session_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_session_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_session_id_convertToJSON() failed");
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

    OpenAPI_mbs_session_id_free(dst);
    dst = OpenAPI_mbs_session_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


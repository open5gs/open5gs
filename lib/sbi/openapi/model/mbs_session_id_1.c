
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mbs_session_id_1.h"

OpenAPI_mbs_session_id_1_t *OpenAPI_mbs_session_id_1_create(
    OpenAPI_tmgi_1_t *tmgi,
    OpenAPI_ssm_1_t *ssm,
    char *nid
)
{
    OpenAPI_mbs_session_id_1_t *mbs_session_id_1_local_var = ogs_malloc(sizeof(OpenAPI_mbs_session_id_1_t));
    ogs_assert(mbs_session_id_1_local_var);

    mbs_session_id_1_local_var->tmgi = tmgi;
    mbs_session_id_1_local_var->ssm = ssm;
    mbs_session_id_1_local_var->nid = nid;

    return mbs_session_id_1_local_var;
}

void OpenAPI_mbs_session_id_1_free(OpenAPI_mbs_session_id_1_t *mbs_session_id_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mbs_session_id_1) {
        return;
    }
    if (mbs_session_id_1->tmgi) {
        OpenAPI_tmgi_1_free(mbs_session_id_1->tmgi);
        mbs_session_id_1->tmgi = NULL;
    }
    if (mbs_session_id_1->ssm) {
        OpenAPI_ssm_1_free(mbs_session_id_1->ssm);
        mbs_session_id_1->ssm = NULL;
    }
    if (mbs_session_id_1->nid) {
        ogs_free(mbs_session_id_1->nid);
        mbs_session_id_1->nid = NULL;
    }
    ogs_free(mbs_session_id_1);
}

cJSON *OpenAPI_mbs_session_id_1_convertToJSON(OpenAPI_mbs_session_id_1_t *mbs_session_id_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mbs_session_id_1 == NULL) {
        ogs_error("OpenAPI_mbs_session_id_1_convertToJSON() failed [MbsSessionId_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mbs_session_id_1->tmgi) {
    cJSON *tmgi_local_JSON = OpenAPI_tmgi_1_convertToJSON(mbs_session_id_1->tmgi);
    if (tmgi_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_session_id_1_convertToJSON() failed [tmgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tmgi", tmgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_session_id_1_convertToJSON() failed [tmgi]");
        goto end;
    }
    }

    if (mbs_session_id_1->ssm) {
    cJSON *ssm_local_JSON = OpenAPI_ssm_1_convertToJSON(mbs_session_id_1->ssm);
    if (ssm_local_JSON == NULL) {
        ogs_error("OpenAPI_mbs_session_id_1_convertToJSON() failed [ssm]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ssm", ssm_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mbs_session_id_1_convertToJSON() failed [ssm]");
        goto end;
    }
    }

    if (mbs_session_id_1->nid) {
    if (cJSON_AddStringToObject(item, "nid", mbs_session_id_1->nid) == NULL) {
        ogs_error("OpenAPI_mbs_session_id_1_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mbs_session_id_1_t *OpenAPI_mbs_session_id_1_parseFromJSON(cJSON *mbs_session_id_1JSON)
{
    OpenAPI_mbs_session_id_1_t *mbs_session_id_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tmgi = NULL;
    OpenAPI_tmgi_1_t *tmgi_local_nonprim = NULL;
    cJSON *ssm = NULL;
    OpenAPI_ssm_1_t *ssm_local_nonprim = NULL;
    cJSON *nid = NULL;
    tmgi = cJSON_GetObjectItemCaseSensitive(mbs_session_id_1JSON, "tmgi");
    if (tmgi) {
    tmgi_local_nonprim = OpenAPI_tmgi_1_parseFromJSON(tmgi);
    if (!tmgi_local_nonprim) {
        ogs_error("OpenAPI_tmgi_1_parseFromJSON failed [tmgi]");
        goto end;
    }
    }

    ssm = cJSON_GetObjectItemCaseSensitive(mbs_session_id_1JSON, "ssm");
    if (ssm) {
    ssm_local_nonprim = OpenAPI_ssm_1_parseFromJSON(ssm);
    if (!ssm_local_nonprim) {
        ogs_error("OpenAPI_ssm_1_parseFromJSON failed [ssm]");
        goto end;
    }
    }

    nid = cJSON_GetObjectItemCaseSensitive(mbs_session_id_1JSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_mbs_session_id_1_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    mbs_session_id_1_local_var = OpenAPI_mbs_session_id_1_create (
        tmgi ? tmgi_local_nonprim : NULL,
        ssm ? ssm_local_nonprim : NULL,
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL
    );

    return mbs_session_id_1_local_var;
end:
    if (tmgi_local_nonprim) {
        OpenAPI_tmgi_1_free(tmgi_local_nonprim);
        tmgi_local_nonprim = NULL;
    }
    if (ssm_local_nonprim) {
        OpenAPI_ssm_1_free(ssm_local_nonprim);
        ssm_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_mbs_session_id_1_t *OpenAPI_mbs_session_id_1_copy(OpenAPI_mbs_session_id_1_t *dst, OpenAPI_mbs_session_id_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mbs_session_id_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mbs_session_id_1_convertToJSON() failed");
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

    OpenAPI_mbs_session_id_1_free(dst);
    dst = OpenAPI_mbs_session_id_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


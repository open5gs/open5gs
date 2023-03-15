
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tmgi_range.h"

OpenAPI_tmgi_range_t *OpenAPI_tmgi_range_create(
    char *mbs_service_id_start,
    char *mbs_service_id_end,
    OpenAPI_plmn_id_t *plmn_id,
    char *nid
)
{
    OpenAPI_tmgi_range_t *tmgi_range_local_var = ogs_malloc(sizeof(OpenAPI_tmgi_range_t));
    ogs_assert(tmgi_range_local_var);

    tmgi_range_local_var->mbs_service_id_start = mbs_service_id_start;
    tmgi_range_local_var->mbs_service_id_end = mbs_service_id_end;
    tmgi_range_local_var->plmn_id = plmn_id;
    tmgi_range_local_var->nid = nid;

    return tmgi_range_local_var;
}

void OpenAPI_tmgi_range_free(OpenAPI_tmgi_range_t *tmgi_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tmgi_range) {
        return;
    }
    if (tmgi_range->mbs_service_id_start) {
        ogs_free(tmgi_range->mbs_service_id_start);
        tmgi_range->mbs_service_id_start = NULL;
    }
    if (tmgi_range->mbs_service_id_end) {
        ogs_free(tmgi_range->mbs_service_id_end);
        tmgi_range->mbs_service_id_end = NULL;
    }
    if (tmgi_range->plmn_id) {
        OpenAPI_plmn_id_free(tmgi_range->plmn_id);
        tmgi_range->plmn_id = NULL;
    }
    if (tmgi_range->nid) {
        ogs_free(tmgi_range->nid);
        tmgi_range->nid = NULL;
    }
    ogs_free(tmgi_range);
}

cJSON *OpenAPI_tmgi_range_convertToJSON(OpenAPI_tmgi_range_t *tmgi_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tmgi_range == NULL) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed [TmgiRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tmgi_range->mbs_service_id_start) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed [mbs_service_id_start]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "mbsServiceIdStart", tmgi_range->mbs_service_id_start) == NULL) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed [mbs_service_id_start]");
        goto end;
    }

    if (!tmgi_range->mbs_service_id_end) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed [mbs_service_id_end]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "mbsServiceIdEnd", tmgi_range->mbs_service_id_end) == NULL) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed [mbs_service_id_end]");
        goto end;
    }

    if (!tmgi_range->plmn_id) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(tmgi_range->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (tmgi_range->nid) {
    if (cJSON_AddStringToObject(item, "nid", tmgi_range->nid) == NULL) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed [nid]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tmgi_range_t *OpenAPI_tmgi_range_parseFromJSON(cJSON *tmgi_rangeJSON)
{
    OpenAPI_tmgi_range_t *tmgi_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbs_service_id_start = NULL;
    cJSON *mbs_service_id_end = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *nid = NULL;
    mbs_service_id_start = cJSON_GetObjectItemCaseSensitive(tmgi_rangeJSON, "mbsServiceIdStart");
    if (!mbs_service_id_start) {
        ogs_error("OpenAPI_tmgi_range_parseFromJSON() failed [mbs_service_id_start]");
        goto end;
    }
    if (!cJSON_IsString(mbs_service_id_start)) {
        ogs_error("OpenAPI_tmgi_range_parseFromJSON() failed [mbs_service_id_start]");
        goto end;
    }

    mbs_service_id_end = cJSON_GetObjectItemCaseSensitive(tmgi_rangeJSON, "mbsServiceIdEnd");
    if (!mbs_service_id_end) {
        ogs_error("OpenAPI_tmgi_range_parseFromJSON() failed [mbs_service_id_end]");
        goto end;
    }
    if (!cJSON_IsString(mbs_service_id_end)) {
        ogs_error("OpenAPI_tmgi_range_parseFromJSON() failed [mbs_service_id_end]");
        goto end;
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(tmgi_rangeJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_tmgi_range_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    nid = cJSON_GetObjectItemCaseSensitive(tmgi_rangeJSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_tmgi_range_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    tmgi_range_local_var = OpenAPI_tmgi_range_create (
        ogs_strdup(mbs_service_id_start->valuestring),
        ogs_strdup(mbs_service_id_end->valuestring),
        plmn_id_local_nonprim,
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL
    );

    return tmgi_range_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_tmgi_range_t *OpenAPI_tmgi_range_copy(OpenAPI_tmgi_range_t *dst, OpenAPI_tmgi_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tmgi_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tmgi_range_convertToJSON() failed");
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

    OpenAPI_tmgi_range_free(dst);
    dst = OpenAPI_tmgi_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


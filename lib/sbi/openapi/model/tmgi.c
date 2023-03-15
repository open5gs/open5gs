
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tmgi.h"

OpenAPI_tmgi_t *OpenAPI_tmgi_create(
    char *mbs_service_id,
    OpenAPI_plmn_id_t *plmn_id
)
{
    OpenAPI_tmgi_t *tmgi_local_var = ogs_malloc(sizeof(OpenAPI_tmgi_t));
    ogs_assert(tmgi_local_var);

    tmgi_local_var->mbs_service_id = mbs_service_id;
    tmgi_local_var->plmn_id = plmn_id;

    return tmgi_local_var;
}

void OpenAPI_tmgi_free(OpenAPI_tmgi_t *tmgi)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tmgi) {
        return;
    }
    if (tmgi->mbs_service_id) {
        ogs_free(tmgi->mbs_service_id);
        tmgi->mbs_service_id = NULL;
    }
    if (tmgi->plmn_id) {
        OpenAPI_plmn_id_free(tmgi->plmn_id);
        tmgi->plmn_id = NULL;
    }
    ogs_free(tmgi);
}

cJSON *OpenAPI_tmgi_convertToJSON(OpenAPI_tmgi_t *tmgi)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tmgi == NULL) {
        ogs_error("OpenAPI_tmgi_convertToJSON() failed [Tmgi]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tmgi->mbs_service_id) {
        ogs_error("OpenAPI_tmgi_convertToJSON() failed [mbs_service_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "mbsServiceId", tmgi->mbs_service_id) == NULL) {
        ogs_error("OpenAPI_tmgi_convertToJSON() failed [mbs_service_id]");
        goto end;
    }

    if (!tmgi->plmn_id) {
        ogs_error("OpenAPI_tmgi_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(tmgi->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_tmgi_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tmgi_convertToJSON() failed [plmn_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_tmgi_t *OpenAPI_tmgi_parseFromJSON(cJSON *tmgiJSON)
{
    OpenAPI_tmgi_t *tmgi_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbs_service_id = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    mbs_service_id = cJSON_GetObjectItemCaseSensitive(tmgiJSON, "mbsServiceId");
    if (!mbs_service_id) {
        ogs_error("OpenAPI_tmgi_parseFromJSON() failed [mbs_service_id]");
        goto end;
    }
    if (!cJSON_IsString(mbs_service_id)) {
        ogs_error("OpenAPI_tmgi_parseFromJSON() failed [mbs_service_id]");
        goto end;
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(tmgiJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_tmgi_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    tmgi_local_var = OpenAPI_tmgi_create (
        ogs_strdup(mbs_service_id->valuestring),
        plmn_id_local_nonprim
    );

    return tmgi_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_tmgi_t *OpenAPI_tmgi_copy(OpenAPI_tmgi_t *dst, OpenAPI_tmgi_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tmgi_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tmgi_convertToJSON() failed");
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

    OpenAPI_tmgi_free(dst);
    dst = OpenAPI_tmgi_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


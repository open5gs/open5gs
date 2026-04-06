
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "aiot_area_id.h"

OpenAPI_aiot_area_id_t *OpenAPI_aiot_area_id_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *nid,
    char *aiot_area_code
)
{
    OpenAPI_aiot_area_id_t *aiot_area_id_local_var = ogs_malloc(sizeof(OpenAPI_aiot_area_id_t));
    ogs_assert(aiot_area_id_local_var);

    aiot_area_id_local_var->plmn_id = plmn_id;
    aiot_area_id_local_var->nid = nid;
    aiot_area_id_local_var->aiot_area_code = aiot_area_code;

    return aiot_area_id_local_var;
}

void OpenAPI_aiot_area_id_free(OpenAPI_aiot_area_id_t *aiot_area_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == aiot_area_id) {
        return;
    }
    if (aiot_area_id->plmn_id) {
        OpenAPI_plmn_id_free(aiot_area_id->plmn_id);
        aiot_area_id->plmn_id = NULL;
    }
    if (aiot_area_id->nid) {
        ogs_free(aiot_area_id->nid);
        aiot_area_id->nid = NULL;
    }
    if (aiot_area_id->aiot_area_code) {
        ogs_free(aiot_area_id->aiot_area_code);
        aiot_area_id->aiot_area_code = NULL;
    }
    ogs_free(aiot_area_id);
}

cJSON *OpenAPI_aiot_area_id_convertToJSON(OpenAPI_aiot_area_id_t *aiot_area_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (aiot_area_id == NULL) {
        ogs_error("OpenAPI_aiot_area_id_convertToJSON() failed [AiotAreaId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!aiot_area_id->plmn_id) {
        ogs_error("OpenAPI_aiot_area_id_convertToJSON() failed [plmn_id]");
        return NULL;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(aiot_area_id->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_aiot_area_id_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_aiot_area_id_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (aiot_area_id->nid) {
    if (cJSON_AddStringToObject(item, "nid", aiot_area_id->nid) == NULL) {
        ogs_error("OpenAPI_aiot_area_id_convertToJSON() failed [nid]");
        goto end;
    }
    }

    if (!aiot_area_id->aiot_area_code) {
        ogs_error("OpenAPI_aiot_area_id_convertToJSON() failed [aiot_area_code]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "aiotAreaCode", aiot_area_id->aiot_area_code) == NULL) {
        ogs_error("OpenAPI_aiot_area_id_convertToJSON() failed [aiot_area_code]");
        goto end;
    }

end:
    return item;
}

OpenAPI_aiot_area_id_t *OpenAPI_aiot_area_id_parseFromJSON(cJSON *aiot_area_idJSON)
{
    OpenAPI_aiot_area_id_t *aiot_area_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *nid = NULL;
    cJSON *aiot_area_code = NULL;
    plmn_id = cJSON_GetObjectItemCaseSensitive(aiot_area_idJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_aiot_area_id_parseFromJSON() failed [plmn_id]");
        goto end;
    }
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }

    nid = cJSON_GetObjectItemCaseSensitive(aiot_area_idJSON, "nid");
    if (nid) {
    if (!cJSON_IsString(nid) && !cJSON_IsNull(nid)) {
        ogs_error("OpenAPI_aiot_area_id_parseFromJSON() failed [nid]");
        goto end;
    }
    }

    aiot_area_code = cJSON_GetObjectItemCaseSensitive(aiot_area_idJSON, "aiotAreaCode");
    if (!aiot_area_code) {
        ogs_error("OpenAPI_aiot_area_id_parseFromJSON() failed [aiot_area_code]");
        goto end;
    }
    if (!cJSON_IsString(aiot_area_code)) {
        ogs_error("OpenAPI_aiot_area_id_parseFromJSON() failed [aiot_area_code]");
        goto end;
    }

    aiot_area_id_local_var = OpenAPI_aiot_area_id_create (
        plmn_id_local_nonprim,
        nid && !cJSON_IsNull(nid) ? ogs_strdup(nid->valuestring) : NULL,
        ogs_strdup(aiot_area_code->valuestring)
    );

    return aiot_area_id_local_var;
end:
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_aiot_area_id_t *OpenAPI_aiot_area_id_copy(OpenAPI_aiot_area_id_t *dst, OpenAPI_aiot_area_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_aiot_area_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_aiot_area_id_convertToJSON() failed");
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

    OpenAPI_aiot_area_id_free(dst);
    dst = OpenAPI_aiot_area_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vplmn_dl_ambr.h"

OpenAPI_vplmn_dl_ambr_t *OpenAPI_vplmn_dl_ambr_create(
    OpenAPI_plmn_id_t *vplmn_id,
    char *session_dl_ambr
)
{
    OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr_local_var = ogs_malloc(sizeof(OpenAPI_vplmn_dl_ambr_t));
    ogs_assert(vplmn_dl_ambr_local_var);

    vplmn_dl_ambr_local_var->vplmn_id = vplmn_id;
    vplmn_dl_ambr_local_var->session_dl_ambr = session_dl_ambr;

    return vplmn_dl_ambr_local_var;
}

void OpenAPI_vplmn_dl_ambr_free(OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == vplmn_dl_ambr) {
        return;
    }
    if (vplmn_dl_ambr->vplmn_id) {
        OpenAPI_plmn_id_free(vplmn_dl_ambr->vplmn_id);
        vplmn_dl_ambr->vplmn_id = NULL;
    }
    if (vplmn_dl_ambr->session_dl_ambr) {
        ogs_free(vplmn_dl_ambr->session_dl_ambr);
        vplmn_dl_ambr->session_dl_ambr = NULL;
    }
    ogs_free(vplmn_dl_ambr);
}

cJSON *OpenAPI_vplmn_dl_ambr_convertToJSON(OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (vplmn_dl_ambr == NULL) {
        ogs_error("OpenAPI_vplmn_dl_ambr_convertToJSON() failed [VplmnDlAmbr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!vplmn_dl_ambr->vplmn_id) {
        ogs_error("OpenAPI_vplmn_dl_ambr_convertToJSON() failed [vplmn_id]");
        return NULL;
    }
    cJSON *vplmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(vplmn_dl_ambr->vplmn_id);
    if (vplmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_vplmn_dl_ambr_convertToJSON() failed [vplmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "vplmnId", vplmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vplmn_dl_ambr_convertToJSON() failed [vplmn_id]");
        goto end;
    }

    if (!vplmn_dl_ambr->session_dl_ambr) {
        ogs_error("OpenAPI_vplmn_dl_ambr_convertToJSON() failed [session_dl_ambr]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "sessionDlAmbr", vplmn_dl_ambr->session_dl_ambr) == NULL) {
        ogs_error("OpenAPI_vplmn_dl_ambr_convertToJSON() failed [session_dl_ambr]");
        goto end;
    }

end:
    return item;
}

OpenAPI_vplmn_dl_ambr_t *OpenAPI_vplmn_dl_ambr_parseFromJSON(cJSON *vplmn_dl_ambrJSON)
{
    OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *vplmn_id = NULL;
    OpenAPI_plmn_id_t *vplmn_id_local_nonprim = NULL;
    cJSON *session_dl_ambr = NULL;
    vplmn_id = cJSON_GetObjectItemCaseSensitive(vplmn_dl_ambrJSON, "vplmnId");
    if (!vplmn_id) {
        ogs_error("OpenAPI_vplmn_dl_ambr_parseFromJSON() failed [vplmn_id]");
        goto end;
    }
    vplmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(vplmn_id);
    if (!vplmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [vplmn_id]");
        goto end;
    }

    session_dl_ambr = cJSON_GetObjectItemCaseSensitive(vplmn_dl_ambrJSON, "sessionDlAmbr");
    if (!session_dl_ambr) {
        ogs_error("OpenAPI_vplmn_dl_ambr_parseFromJSON() failed [session_dl_ambr]");
        goto end;
    }
    if (!cJSON_IsString(session_dl_ambr)) {
        ogs_error("OpenAPI_vplmn_dl_ambr_parseFromJSON() failed [session_dl_ambr]");
        goto end;
    }

    vplmn_dl_ambr_local_var = OpenAPI_vplmn_dl_ambr_create (
        vplmn_id_local_nonprim,
        ogs_strdup(session_dl_ambr->valuestring)
    );

    return vplmn_dl_ambr_local_var;
end:
    if (vplmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(vplmn_id_local_nonprim);
        vplmn_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_vplmn_dl_ambr_t *OpenAPI_vplmn_dl_ambr_copy(OpenAPI_vplmn_dl_ambr_t *dst, OpenAPI_vplmn_dl_ambr_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vplmn_dl_ambr_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vplmn_dl_ambr_convertToJSON() failed");
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

    OpenAPI_vplmn_dl_ambr_free(dst);
    dst = OpenAPI_vplmn_dl_ambr_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


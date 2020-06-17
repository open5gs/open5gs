
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nef_info.h"

OpenAPI_nef_info_t *OpenAPI_nef_info_create(
    char *nef_id,
    OpenAPI_pfd_data_t *pfd_data,
    OpenAPI_af_event_exposure_data_t *af_ee_data
    )
{
    OpenAPI_nef_info_t *nef_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_nef_info_t));
    if (!nef_info_local_var) {
        return NULL;
    }
    nef_info_local_var->nef_id = nef_id;
    nef_info_local_var->pfd_data = pfd_data;
    nef_info_local_var->af_ee_data = af_ee_data;

    return nef_info_local_var;
}

void OpenAPI_nef_info_free(OpenAPI_nef_info_t *nef_info)
{
    if (NULL == nef_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nef_info->nef_id);
    OpenAPI_pfd_data_free(nef_info->pfd_data);
    OpenAPI_af_event_exposure_data_free(nef_info->af_ee_data);
    ogs_free(nef_info);
}

cJSON *OpenAPI_nef_info_convertToJSON(OpenAPI_nef_info_t *nef_info)
{
    cJSON *item = NULL;

    if (nef_info == NULL) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed [NefInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nef_info->nef_id) {
        if (cJSON_AddStringToObject(item, "nefId", nef_info->nef_id) == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [nef_id]");
            goto end;
        }
    }

    if (nef_info->pfd_data) {
        cJSON *pfd_data_local_JSON = OpenAPI_pfd_data_convertToJSON(nef_info->pfd_data);
        if (pfd_data_local_JSON == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [pfd_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "pfdData", pfd_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [pfd_data]");
            goto end;
        }
    }

    if (nef_info->af_ee_data) {
        cJSON *af_ee_data_local_JSON = OpenAPI_af_event_exposure_data_convertToJSON(nef_info->af_ee_data);
        if (af_ee_data_local_JSON == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [af_ee_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "afEeData", af_ee_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nef_info_convertToJSON() failed [af_ee_data]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_nef_info_t *OpenAPI_nef_info_parseFromJSON(cJSON *nef_infoJSON)
{
    OpenAPI_nef_info_t *nef_info_local_var = NULL;
    cJSON *nef_id = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "nefId");

    if (nef_id) {
        if (!cJSON_IsString(nef_id)) {
            ogs_error("OpenAPI_nef_info_parseFromJSON() failed [nef_id]");
            goto end;
        }
    }

    cJSON *pfd_data = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "pfdData");

    OpenAPI_pfd_data_t *pfd_data_local_nonprim = NULL;
    if (pfd_data) {
        pfd_data_local_nonprim = OpenAPI_pfd_data_parseFromJSON(pfd_data);
    }

    cJSON *af_ee_data = cJSON_GetObjectItemCaseSensitive(nef_infoJSON, "afEeData");

    OpenAPI_af_event_exposure_data_t *af_ee_data_local_nonprim = NULL;
    if (af_ee_data) {
        af_ee_data_local_nonprim = OpenAPI_af_event_exposure_data_parseFromJSON(af_ee_data);
    }

    nef_info_local_var = OpenAPI_nef_info_create (
        nef_id ? ogs_strdup(nef_id->valuestring) : NULL,
        pfd_data ? pfd_data_local_nonprim : NULL,
        af_ee_data ? af_ee_data_local_nonprim : NULL
        );

    return nef_info_local_var;
end:
    return NULL;
}

OpenAPI_nef_info_t *OpenAPI_nef_info_copy(OpenAPI_nef_info_t *dst, OpenAPI_nef_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nef_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nef_info_convertToJSON() failed");
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

    OpenAPI_nef_info_free(dst);
    dst = OpenAPI_nef_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


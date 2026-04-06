
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tss_rsp_per_ngran.h"

OpenAPI_tss_rsp_per_ngran_t *OpenAPI_tss_rsp_per_ngran_create(
    OpenAPI_global_ran_node_id_t *ngran_id,
    OpenAPI_ngran_failure_info_e ngran_failure_info,
    OpenAPI_n2_info_content_t *tss_container
)
{
    OpenAPI_tss_rsp_per_ngran_t *tss_rsp_per_ngran_local_var = ogs_malloc(sizeof(OpenAPI_tss_rsp_per_ngran_t));
    ogs_assert(tss_rsp_per_ngran_local_var);

    tss_rsp_per_ngran_local_var->ngran_id = ngran_id;
    tss_rsp_per_ngran_local_var->ngran_failure_info = ngran_failure_info;
    tss_rsp_per_ngran_local_var->tss_container = tss_container;

    return tss_rsp_per_ngran_local_var;
}

void OpenAPI_tss_rsp_per_ngran_free(OpenAPI_tss_rsp_per_ngran_t *tss_rsp_per_ngran)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tss_rsp_per_ngran) {
        return;
    }
    if (tss_rsp_per_ngran->ngran_id) {
        OpenAPI_global_ran_node_id_free(tss_rsp_per_ngran->ngran_id);
        tss_rsp_per_ngran->ngran_id = NULL;
    }
    if (tss_rsp_per_ngran->tss_container) {
        OpenAPI_n2_info_content_free(tss_rsp_per_ngran->tss_container);
        tss_rsp_per_ngran->tss_container = NULL;
    }
    ogs_free(tss_rsp_per_ngran);
}

cJSON *OpenAPI_tss_rsp_per_ngran_convertToJSON(OpenAPI_tss_rsp_per_ngran_t *tss_rsp_per_ngran)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tss_rsp_per_ngran == NULL) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_convertToJSON() failed [TssRspPerNgran]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tss_rsp_per_ngran->ngran_id) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_convertToJSON() failed [ngran_id]");
        return NULL;
    }
    cJSON *ngran_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(tss_rsp_per_ngran->ngran_id);
    if (ngran_id_local_JSON == NULL) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_convertToJSON() failed [ngran_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngranId", ngran_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_convertToJSON() failed [ngran_id]");
        goto end;
    }

    if (tss_rsp_per_ngran->ngran_failure_info != OpenAPI_ngran_failure_info_NULL) {
    if (cJSON_AddStringToObject(item, "ngranFailureInfo", OpenAPI_ngran_failure_info_ToString(tss_rsp_per_ngran->ngran_failure_info)) == NULL) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_convertToJSON() failed [ngran_failure_info]");
        goto end;
    }
    }

    if (tss_rsp_per_ngran->tss_container) {
    cJSON *tss_container_local_JSON = OpenAPI_n2_info_content_convertToJSON(tss_rsp_per_ngran->tss_container);
    if (tss_container_local_JSON == NULL) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_convertToJSON() failed [tss_container]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tssContainer", tss_container_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_convertToJSON() failed [tss_container]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tss_rsp_per_ngran_t *OpenAPI_tss_rsp_per_ngran_parseFromJSON(cJSON *tss_rsp_per_ngranJSON)
{
    OpenAPI_tss_rsp_per_ngran_t *tss_rsp_per_ngran_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ngran_id = NULL;
    OpenAPI_global_ran_node_id_t *ngran_id_local_nonprim = NULL;
    cJSON *ngran_failure_info = NULL;
    OpenAPI_ngran_failure_info_e ngran_failure_infoVariable = 0;
    cJSON *tss_container = NULL;
    OpenAPI_n2_info_content_t *tss_container_local_nonprim = NULL;
    ngran_id = cJSON_GetObjectItemCaseSensitive(tss_rsp_per_ngranJSON, "ngranId");
    if (!ngran_id) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_parseFromJSON() failed [ngran_id]");
        goto end;
    }
    ngran_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(ngran_id);
    if (!ngran_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [ngran_id]");
        goto end;
    }

    ngran_failure_info = cJSON_GetObjectItemCaseSensitive(tss_rsp_per_ngranJSON, "ngranFailureInfo");
    if (ngran_failure_info) {
    if (!cJSON_IsString(ngran_failure_info)) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_parseFromJSON() failed [ngran_failure_info]");
        goto end;
    }
    ngran_failure_infoVariable = OpenAPI_ngran_failure_info_FromString(ngran_failure_info->valuestring);
    }

    tss_container = cJSON_GetObjectItemCaseSensitive(tss_rsp_per_ngranJSON, "tssContainer");
    if (tss_container) {
    tss_container_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(tss_container);
    if (!tss_container_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [tss_container]");
        goto end;
    }
    }

    tss_rsp_per_ngran_local_var = OpenAPI_tss_rsp_per_ngran_create (
        ngran_id_local_nonprim,
        ngran_failure_info ? ngran_failure_infoVariable : 0,
        tss_container ? tss_container_local_nonprim : NULL
    );

    return tss_rsp_per_ngran_local_var;
end:
    if (ngran_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(ngran_id_local_nonprim);
        ngran_id_local_nonprim = NULL;
    }
    if (tss_container_local_nonprim) {
        OpenAPI_n2_info_content_free(tss_container_local_nonprim);
        tss_container_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_tss_rsp_per_ngran_t *OpenAPI_tss_rsp_per_ngran_copy(OpenAPI_tss_rsp_per_ngran_t *dst, OpenAPI_tss_rsp_per_ngran_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tss_rsp_per_ngran_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tss_rsp_per_ngran_convertToJSON() failed");
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

    OpenAPI_tss_rsp_per_ngran_free(dst);
    dst = OpenAPI_tss_rsp_per_ngran_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


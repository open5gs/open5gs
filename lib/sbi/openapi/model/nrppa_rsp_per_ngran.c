
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrppa_rsp_per_ngran.h"

OpenAPI_nrppa_rsp_per_ngran_t *OpenAPI_nrppa_rsp_per_ngran_create(
    OpenAPI_global_ran_node_id_t *ngran_id,
    OpenAPI_nrppa_trans_failure_e cause
)
{
    OpenAPI_nrppa_rsp_per_ngran_t *nrppa_rsp_per_ngran_local_var = ogs_malloc(sizeof(OpenAPI_nrppa_rsp_per_ngran_t));
    ogs_assert(nrppa_rsp_per_ngran_local_var);

    nrppa_rsp_per_ngran_local_var->ngran_id = ngran_id;
    nrppa_rsp_per_ngran_local_var->cause = cause;

    return nrppa_rsp_per_ngran_local_var;
}

void OpenAPI_nrppa_rsp_per_ngran_free(OpenAPI_nrppa_rsp_per_ngran_t *nrppa_rsp_per_ngran)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrppa_rsp_per_ngran) {
        return;
    }
    if (nrppa_rsp_per_ngran->ngran_id) {
        OpenAPI_global_ran_node_id_free(nrppa_rsp_per_ngran->ngran_id);
        nrppa_rsp_per_ngran->ngran_id = NULL;
    }
    ogs_free(nrppa_rsp_per_ngran);
}

cJSON *OpenAPI_nrppa_rsp_per_ngran_convertToJSON(OpenAPI_nrppa_rsp_per_ngran_t *nrppa_rsp_per_ngran)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrppa_rsp_per_ngran == NULL) {
        ogs_error("OpenAPI_nrppa_rsp_per_ngran_convertToJSON() failed [NrppaRspPerNgran]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nrppa_rsp_per_ngran->ngran_id) {
        ogs_error("OpenAPI_nrppa_rsp_per_ngran_convertToJSON() failed [ngran_id]");
        return NULL;
    }
    cJSON *ngran_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(nrppa_rsp_per_ngran->ngran_id);
    if (ngran_id_local_JSON == NULL) {
        ogs_error("OpenAPI_nrppa_rsp_per_ngran_convertToJSON() failed [ngran_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngranId", ngran_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nrppa_rsp_per_ngran_convertToJSON() failed [ngran_id]");
        goto end;
    }

    if (nrppa_rsp_per_ngran->cause != OpenAPI_nrppa_trans_failure_NULL) {
    if (cJSON_AddStringToObject(item, "cause", OpenAPI_nrppa_trans_failure_ToString(nrppa_rsp_per_ngran->cause)) == NULL) {
        ogs_error("OpenAPI_nrppa_rsp_per_ngran_convertToJSON() failed [cause]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nrppa_rsp_per_ngran_t *OpenAPI_nrppa_rsp_per_ngran_parseFromJSON(cJSON *nrppa_rsp_per_ngranJSON)
{
    OpenAPI_nrppa_rsp_per_ngran_t *nrppa_rsp_per_ngran_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ngran_id = NULL;
    OpenAPI_global_ran_node_id_t *ngran_id_local_nonprim = NULL;
    cJSON *cause = NULL;
    OpenAPI_nrppa_trans_failure_e causeVariable = 0;
    ngran_id = cJSON_GetObjectItemCaseSensitive(nrppa_rsp_per_ngranJSON, "ngranId");
    if (!ngran_id) {
        ogs_error("OpenAPI_nrppa_rsp_per_ngran_parseFromJSON() failed [ngran_id]");
        goto end;
    }
    ngran_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(ngran_id);
    if (!ngran_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [ngran_id]");
        goto end;
    }

    cause = cJSON_GetObjectItemCaseSensitive(nrppa_rsp_per_ngranJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_nrppa_rsp_per_ngran_parseFromJSON() failed [cause]");
        goto end;
    }
    causeVariable = OpenAPI_nrppa_trans_failure_FromString(cause->valuestring);
    }

    nrppa_rsp_per_ngran_local_var = OpenAPI_nrppa_rsp_per_ngran_create (
        ngran_id_local_nonprim,
        cause ? causeVariable : 0
    );

    return nrppa_rsp_per_ngran_local_var;
end:
    if (ngran_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(ngran_id_local_nonprim);
        ngran_id_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nrppa_rsp_per_ngran_t *OpenAPI_nrppa_rsp_per_ngran_copy(OpenAPI_nrppa_rsp_per_ngran_t *dst, OpenAPI_nrppa_rsp_per_ngran_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrppa_rsp_per_ngran_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrppa_rsp_per_ngran_convertToJSON() failed");
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

    OpenAPI_nrppa_rsp_per_ngran_free(dst);
    dst = OpenAPI_nrppa_rsp_per_ngran_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


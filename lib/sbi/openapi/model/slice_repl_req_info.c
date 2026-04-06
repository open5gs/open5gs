
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_repl_req_info.h"

OpenAPI_slice_repl_req_info_t *OpenAPI_slice_repl_req_info_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_snssai_t *alt_snssai
)
{
    OpenAPI_slice_repl_req_info_t *slice_repl_req_info_local_var = ogs_malloc(sizeof(OpenAPI_slice_repl_req_info_t));
    ogs_assert(slice_repl_req_info_local_var);

    slice_repl_req_info_local_var->snssai = snssai;
    slice_repl_req_info_local_var->alt_snssai = alt_snssai;

    return slice_repl_req_info_local_var;
}

void OpenAPI_slice_repl_req_info_free(OpenAPI_slice_repl_req_info_t *slice_repl_req_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_repl_req_info) {
        return;
    }
    if (slice_repl_req_info->snssai) {
        OpenAPI_snssai_free(slice_repl_req_info->snssai);
        slice_repl_req_info->snssai = NULL;
    }
    if (slice_repl_req_info->alt_snssai) {
        OpenAPI_snssai_free(slice_repl_req_info->alt_snssai);
        slice_repl_req_info->alt_snssai = NULL;
    }
    ogs_free(slice_repl_req_info);
}

cJSON *OpenAPI_slice_repl_req_info_convertToJSON(OpenAPI_slice_repl_req_info_t *slice_repl_req_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_repl_req_info == NULL) {
        ogs_error("OpenAPI_slice_repl_req_info_convertToJSON() failed [SliceReplReqInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!slice_repl_req_info->snssai) {
        ogs_error("OpenAPI_slice_repl_req_info_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(slice_repl_req_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_repl_req_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_repl_req_info_convertToJSON() failed [snssai]");
        goto end;
    }

    if (!slice_repl_req_info->alt_snssai) {
        ogs_error("OpenAPI_slice_repl_req_info_convertToJSON() failed [alt_snssai]");
        return NULL;
    }
    cJSON *alt_snssai_local_JSON = OpenAPI_snssai_convertToJSON(slice_repl_req_info->alt_snssai);
    if (alt_snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_repl_req_info_convertToJSON() failed [alt_snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "altSnssai", alt_snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_repl_req_info_convertToJSON() failed [alt_snssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_slice_repl_req_info_t *OpenAPI_slice_repl_req_info_parseFromJSON(cJSON *slice_repl_req_infoJSON)
{
    OpenAPI_slice_repl_req_info_t *slice_repl_req_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *alt_snssai = NULL;
    OpenAPI_snssai_t *alt_snssai_local_nonprim = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(slice_repl_req_infoJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_slice_repl_req_info_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    alt_snssai = cJSON_GetObjectItemCaseSensitive(slice_repl_req_infoJSON, "altSnssai");
    if (!alt_snssai) {
        ogs_error("OpenAPI_slice_repl_req_info_parseFromJSON() failed [alt_snssai]");
        goto end;
    }
    alt_snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(alt_snssai);
    if (!alt_snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [alt_snssai]");
        goto end;
    }

    slice_repl_req_info_local_var = OpenAPI_slice_repl_req_info_create (
        snssai_local_nonprim,
        alt_snssai_local_nonprim
    );

    return slice_repl_req_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (alt_snssai_local_nonprim) {
        OpenAPI_snssai_free(alt_snssai_local_nonprim);
        alt_snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_slice_repl_req_info_t *OpenAPI_slice_repl_req_info_copy(OpenAPI_slice_repl_req_info_t *dst, OpenAPI_slice_repl_req_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_repl_req_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_repl_req_info_convertToJSON() failed");
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

    OpenAPI_slice_repl_req_info_free(dst);
    dst = OpenAPI_slice_repl_req_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


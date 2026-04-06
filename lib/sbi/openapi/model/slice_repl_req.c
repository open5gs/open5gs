
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_repl_req.h"

OpenAPI_slice_repl_req_t *OpenAPI_slice_repl_req_create(
    OpenAPI_slice_repl_type_e type,
    OpenAPI_slice_repl_req_info_t *af_slice_repl_req
)
{
    OpenAPI_slice_repl_req_t *slice_repl_req_local_var = ogs_malloc(sizeof(OpenAPI_slice_repl_req_t));
    ogs_assert(slice_repl_req_local_var);

    slice_repl_req_local_var->type = type;
    slice_repl_req_local_var->af_slice_repl_req = af_slice_repl_req;

    return slice_repl_req_local_var;
}

void OpenAPI_slice_repl_req_free(OpenAPI_slice_repl_req_t *slice_repl_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_repl_req) {
        return;
    }
    if (slice_repl_req->af_slice_repl_req) {
        OpenAPI_slice_repl_req_info_free(slice_repl_req->af_slice_repl_req);
        slice_repl_req->af_slice_repl_req = NULL;
    }
    ogs_free(slice_repl_req);
}

cJSON *OpenAPI_slice_repl_req_convertToJSON(OpenAPI_slice_repl_req_t *slice_repl_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_repl_req == NULL) {
        ogs_error("OpenAPI_slice_repl_req_convertToJSON() failed [SliceReplReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (slice_repl_req->type == OpenAPI_slice_repl_type_NULL) {
        ogs_error("OpenAPI_slice_repl_req_convertToJSON() failed [type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "type", OpenAPI_slice_repl_type_ToString(slice_repl_req->type)) == NULL) {
        ogs_error("OpenAPI_slice_repl_req_convertToJSON() failed [type]");
        goto end;
    }

    if (slice_repl_req->af_slice_repl_req) {
    cJSON *af_slice_repl_req_local_JSON = OpenAPI_slice_repl_req_info_convertToJSON(slice_repl_req->af_slice_repl_req);
    if (af_slice_repl_req_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_repl_req_convertToJSON() failed [af_slice_repl_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "afSliceReplReq", af_slice_repl_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_repl_req_convertToJSON() failed [af_slice_repl_req]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_slice_repl_req_t *OpenAPI_slice_repl_req_parseFromJSON(cJSON *slice_repl_reqJSON)
{
    OpenAPI_slice_repl_req_t *slice_repl_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *type = NULL;
    OpenAPI_slice_repl_type_e typeVariable = 0;
    cJSON *af_slice_repl_req = NULL;
    OpenAPI_slice_repl_req_info_t *af_slice_repl_req_local_nonprim = NULL;
    type = cJSON_GetObjectItemCaseSensitive(slice_repl_reqJSON, "type");
    if (!type) {
        ogs_error("OpenAPI_slice_repl_req_parseFromJSON() failed [type]");
        goto end;
    }
    if (!cJSON_IsString(type)) {
        ogs_error("OpenAPI_slice_repl_req_parseFromJSON() failed [type]");
        goto end;
    }
    typeVariable = OpenAPI_slice_repl_type_FromString(type->valuestring);

    af_slice_repl_req = cJSON_GetObjectItemCaseSensitive(slice_repl_reqJSON, "afSliceReplReq");
    if (af_slice_repl_req) {
    af_slice_repl_req_local_nonprim = OpenAPI_slice_repl_req_info_parseFromJSON(af_slice_repl_req);
    if (!af_slice_repl_req_local_nonprim) {
        ogs_error("OpenAPI_slice_repl_req_info_parseFromJSON failed [af_slice_repl_req]");
        goto end;
    }
    }

    slice_repl_req_local_var = OpenAPI_slice_repl_req_create (
        typeVariable,
        af_slice_repl_req ? af_slice_repl_req_local_nonprim : NULL
    );

    return slice_repl_req_local_var;
end:
    if (af_slice_repl_req_local_nonprim) {
        OpenAPI_slice_repl_req_info_free(af_slice_repl_req_local_nonprim);
        af_slice_repl_req_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_slice_repl_req_t *OpenAPI_slice_repl_req_copy(OpenAPI_slice_repl_req_t *dst, OpenAPI_slice_repl_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_repl_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_repl_req_convertToJSON() failed");
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

    OpenAPI_slice_repl_req_free(dst);
    dst = OpenAPI_slice_repl_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_sig_protocol.h"

OpenAPI_af_sig_protocol_t *OpenAPI_af_sig_protocol_create(
    )
{
    OpenAPI_af_sig_protocol_t *af_sig_protocol_local_var = OpenAPI_malloc(sizeof(OpenAPI_af_sig_protocol_t));
    if (!af_sig_protocol_local_var) {
        return NULL;
    }

    return af_sig_protocol_local_var;
}

void OpenAPI_af_sig_protocol_free(OpenAPI_af_sig_protocol_t *af_sig_protocol)
{
    if (NULL == af_sig_protocol) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(af_sig_protocol);
}

cJSON *OpenAPI_af_sig_protocol_convertToJSON(OpenAPI_af_sig_protocol_t *af_sig_protocol)
{
    cJSON *item = NULL;

    if (af_sig_protocol == NULL) {
        ogs_error("OpenAPI_af_sig_protocol_convertToJSON() failed [AfSigProtocol]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_af_sig_protocol_t *OpenAPI_af_sig_protocol_parseFromJSON(cJSON *af_sig_protocolJSON)
{
    OpenAPI_af_sig_protocol_t *af_sig_protocol_local_var = NULL;
    af_sig_protocol_local_var = OpenAPI_af_sig_protocol_create (
        );

    return af_sig_protocol_local_var;
end:
    return NULL;
}

OpenAPI_af_sig_protocol_t *OpenAPI_af_sig_protocol_copy(OpenAPI_af_sig_protocol_t *dst, OpenAPI_af_sig_protocol_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_sig_protocol_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_sig_protocol_convertToJSON() failed");
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

    OpenAPI_af_sig_protocol_free(dst);
    dst = OpenAPI_af_sig_protocol_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


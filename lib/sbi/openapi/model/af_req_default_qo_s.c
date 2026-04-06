
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_req_default_qo_s.h"

OpenAPI_af_req_default_qo_s_t *OpenAPI_af_req_default_qo_s_create(
    int _5qi,
    OpenAPI_arp_t *arp,
    bool is_priority_level,
    int priority_level
)
{
    OpenAPI_af_req_default_qo_s_t *af_req_default_qo_s_local_var = ogs_malloc(sizeof(OpenAPI_af_req_default_qo_s_t));
    ogs_assert(af_req_default_qo_s_local_var);

    af_req_default_qo_s_local_var->_5qi = _5qi;
    af_req_default_qo_s_local_var->arp = arp;
    af_req_default_qo_s_local_var->is_priority_level = is_priority_level;
    af_req_default_qo_s_local_var->priority_level = priority_level;

    return af_req_default_qo_s_local_var;
}

void OpenAPI_af_req_default_qo_s_free(OpenAPI_af_req_default_qo_s_t *af_req_default_qo_s)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_req_default_qo_s) {
        return;
    }
    if (af_req_default_qo_s->arp) {
        OpenAPI_arp_free(af_req_default_qo_s->arp);
        af_req_default_qo_s->arp = NULL;
    }
    ogs_free(af_req_default_qo_s);
}

cJSON *OpenAPI_af_req_default_qo_s_convertToJSON(OpenAPI_af_req_default_qo_s_t *af_req_default_qo_s)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_req_default_qo_s == NULL) {
        ogs_error("OpenAPI_af_req_default_qo_s_convertToJSON() failed [AfReqDefaultQoS]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "5qi", af_req_default_qo_s->_5qi) == NULL) {
        ogs_error("OpenAPI_af_req_default_qo_s_convertToJSON() failed [_5qi]");
        goto end;
    }

    if (!af_req_default_qo_s->arp) {
        ogs_error("OpenAPI_af_req_default_qo_s_convertToJSON() failed [arp]");
        return NULL;
    }
    cJSON *arp_local_JSON = OpenAPI_arp_convertToJSON(af_req_default_qo_s->arp);
    if (arp_local_JSON == NULL) {
        ogs_error("OpenAPI_af_req_default_qo_s_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "arp", arp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_req_default_qo_s_convertToJSON() failed [arp]");
        goto end;
    }

    if (af_req_default_qo_s->is_priority_level) {
    if (cJSON_AddNumberToObject(item, "priorityLevel", af_req_default_qo_s->priority_level) == NULL) {
        ogs_error("OpenAPI_af_req_default_qo_s_convertToJSON() failed [priority_level]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_af_req_default_qo_s_t *OpenAPI_af_req_default_qo_s_parseFromJSON(cJSON *af_req_default_qo_sJSON)
{
    OpenAPI_af_req_default_qo_s_t *af_req_default_qo_s_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *_5qi = NULL;
    cJSON *arp = NULL;
    OpenAPI_arp_t *arp_local_nonprim = NULL;
    cJSON *priority_level = NULL;
    _5qi = cJSON_GetObjectItemCaseSensitive(af_req_default_qo_sJSON, "5qi");
    if (!_5qi) {
        ogs_error("OpenAPI_af_req_default_qo_s_parseFromJSON() failed [_5qi]");
        goto end;
    }
    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_af_req_default_qo_s_parseFromJSON() failed [_5qi]");
        goto end;
    }

    arp = cJSON_GetObjectItemCaseSensitive(af_req_default_qo_sJSON, "arp");
    if (!arp) {
        ogs_error("OpenAPI_af_req_default_qo_s_parseFromJSON() failed [arp]");
        goto end;
    }
    arp_local_nonprim = OpenAPI_arp_parseFromJSON(arp);
    if (!arp_local_nonprim) {
        ogs_error("OpenAPI_arp_parseFromJSON failed [arp]");
        goto end;
    }

    priority_level = cJSON_GetObjectItemCaseSensitive(af_req_default_qo_sJSON, "priorityLevel");
    if (priority_level) {
    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_af_req_default_qo_s_parseFromJSON() failed [priority_level]");
        goto end;
    }
    }

    af_req_default_qo_s_local_var = OpenAPI_af_req_default_qo_s_create (
        
        _5qi->valuedouble,
        arp_local_nonprim,
        priority_level ? true : false,
        priority_level ? priority_level->valuedouble : 0
    );

    return af_req_default_qo_s_local_var;
end:
    if (arp_local_nonprim) {
        OpenAPI_arp_free(arp_local_nonprim);
        arp_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_af_req_default_qo_s_t *OpenAPI_af_req_default_qo_s_copy(OpenAPI_af_req_default_qo_s_t *dst, OpenAPI_af_req_default_qo_s_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_req_default_qo_s_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_req_default_qo_s_convertToJSON() failed");
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

    OpenAPI_af_req_default_qo_s_free(dst);
    dst = OpenAPI_af_req_default_qo_s_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ran_nas_rel_cause.h"

OpenAPI_ran_nas_rel_cause_t *OpenAPI_ran_nas_rel_cause_create(
    OpenAPI_ng_ap_cause_t *ng_ap_cause,
    int _5g_mm_cause,
    int _5g_sm_cause
    )
{
    OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_cause_local_var = OpenAPI_malloc(sizeof(OpenAPI_ran_nas_rel_cause_t));
    if (!ran_nas_rel_cause_local_var) {
        return NULL;
    }
    ran_nas_rel_cause_local_var->ng_ap_cause = ng_ap_cause;
    ran_nas_rel_cause_local_var->_5g_mm_cause = _5g_mm_cause;
    ran_nas_rel_cause_local_var->_5g_sm_cause = _5g_sm_cause;

    return ran_nas_rel_cause_local_var;
}

void OpenAPI_ran_nas_rel_cause_free(OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_cause)
{
    if (NULL == ran_nas_rel_cause) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ng_ap_cause_free(ran_nas_rel_cause->ng_ap_cause);
    ogs_free(ran_nas_rel_cause);
}

cJSON *OpenAPI_ran_nas_rel_cause_convertToJSON(OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_cause)
{
    cJSON *item = NULL;

    if (ran_nas_rel_cause == NULL) {
        ogs_error("OpenAPI_ran_nas_rel_cause_convertToJSON() failed [RanNasRelCause]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ran_nas_rel_cause->ng_ap_cause) {
        cJSON *ng_ap_cause_local_JSON = OpenAPI_ng_ap_cause_convertToJSON(ran_nas_rel_cause->ng_ap_cause);
        if (ng_ap_cause_local_JSON == NULL) {
            ogs_error("OpenAPI_ran_nas_rel_cause_convertToJSON() failed [ng_ap_cause]");
            goto end;
        }
        cJSON_AddItemToObject(item, "ngApCause", ng_ap_cause_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_ran_nas_rel_cause_convertToJSON() failed [ng_ap_cause]");
            goto end;
        }
    }

    if (ran_nas_rel_cause->_5g_mm_cause) {
        if (cJSON_AddNumberToObject(item, "5gMmCause", ran_nas_rel_cause->_5g_mm_cause) == NULL) {
            ogs_error("OpenAPI_ran_nas_rel_cause_convertToJSON() failed [_5g_mm_cause]");
            goto end;
        }
    }

    if (ran_nas_rel_cause->_5g_sm_cause) {
        if (cJSON_AddNumberToObject(item, "5gSmCause", ran_nas_rel_cause->_5g_sm_cause) == NULL) {
            ogs_error("OpenAPI_ran_nas_rel_cause_convertToJSON() failed [_5g_sm_cause]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_ran_nas_rel_cause_t *OpenAPI_ran_nas_rel_cause_parseFromJSON(cJSON *ran_nas_rel_causeJSON)
{
    OpenAPI_ran_nas_rel_cause_t *ran_nas_rel_cause_local_var = NULL;
    cJSON *ng_ap_cause = cJSON_GetObjectItemCaseSensitive(ran_nas_rel_causeJSON, "ngApCause");

    OpenAPI_ng_ap_cause_t *ng_ap_cause_local_nonprim = NULL;
    if (ng_ap_cause) {
        ng_ap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ng_ap_cause);
    }

    cJSON *_5g_mm_cause = cJSON_GetObjectItemCaseSensitive(ran_nas_rel_causeJSON, "5gMmCause");

    if (_5g_mm_cause) {
        if (!cJSON_IsNumber(_5g_mm_cause)) {
            ogs_error("OpenAPI_ran_nas_rel_cause_parseFromJSON() failed [_5g_mm_cause]");
            goto end;
        }
    }

    cJSON *_5g_sm_cause = cJSON_GetObjectItemCaseSensitive(ran_nas_rel_causeJSON, "5gSmCause");

    if (_5g_sm_cause) {
        if (!cJSON_IsNumber(_5g_sm_cause)) {
            ogs_error("OpenAPI_ran_nas_rel_cause_parseFromJSON() failed [_5g_sm_cause]");
            goto end;
        }
    }

    ran_nas_rel_cause_local_var = OpenAPI_ran_nas_rel_cause_create (
        ng_ap_cause ? ng_ap_cause_local_nonprim : NULL,
        _5g_mm_cause ? _5g_mm_cause->valuedouble : 0,
        _5g_sm_cause ? _5g_sm_cause->valuedouble : 0
        );

    return ran_nas_rel_cause_local_var;
end:
    return NULL;
}

OpenAPI_ran_nas_rel_cause_t *OpenAPI_ran_nas_rel_cause_copy(OpenAPI_ran_nas_rel_cause_t *dst, OpenAPI_ran_nas_rel_cause_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ran_nas_rel_cause_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ran_nas_rel_cause_convertToJSON() failed");
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

    OpenAPI_ran_nas_rel_cause_free(dst);
    dst = OpenAPI_ran_nas_rel_cause_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_usage_control_info_1.h"

OpenAPI_slice_usage_control_info_1_t *OpenAPI_slice_usage_control_info_1_create(
    OpenAPI_snssai_t *s_nssai,
    bool is_dereg_inact_timer,
    int dereg_inact_timer,
    bool is_sess_inact_timer,
    int sess_inact_timer
)
{
    OpenAPI_slice_usage_control_info_1_t *slice_usage_control_info_1_local_var = ogs_malloc(sizeof(OpenAPI_slice_usage_control_info_1_t));
    ogs_assert(slice_usage_control_info_1_local_var);

    slice_usage_control_info_1_local_var->s_nssai = s_nssai;
    slice_usage_control_info_1_local_var->is_dereg_inact_timer = is_dereg_inact_timer;
    slice_usage_control_info_1_local_var->dereg_inact_timer = dereg_inact_timer;
    slice_usage_control_info_1_local_var->is_sess_inact_timer = is_sess_inact_timer;
    slice_usage_control_info_1_local_var->sess_inact_timer = sess_inact_timer;

    return slice_usage_control_info_1_local_var;
}

void OpenAPI_slice_usage_control_info_1_free(OpenAPI_slice_usage_control_info_1_t *slice_usage_control_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_usage_control_info_1) {
        return;
    }
    if (slice_usage_control_info_1->s_nssai) {
        OpenAPI_snssai_free(slice_usage_control_info_1->s_nssai);
        slice_usage_control_info_1->s_nssai = NULL;
    }
    ogs_free(slice_usage_control_info_1);
}

cJSON *OpenAPI_slice_usage_control_info_1_convertToJSON(OpenAPI_slice_usage_control_info_1_t *slice_usage_control_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_usage_control_info_1 == NULL) {
        ogs_error("OpenAPI_slice_usage_control_info_1_convertToJSON() failed [SliceUsageControlInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!slice_usage_control_info_1->s_nssai) {
        ogs_error("OpenAPI_slice_usage_control_info_1_convertToJSON() failed [s_nssai]");
        return NULL;
    }
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(slice_usage_control_info_1->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_slice_usage_control_info_1_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_slice_usage_control_info_1_convertToJSON() failed [s_nssai]");
        goto end;
    }

    if (slice_usage_control_info_1->is_dereg_inact_timer) {
    if (cJSON_AddNumberToObject(item, "deregInactTimer", slice_usage_control_info_1->dereg_inact_timer) == NULL) {
        ogs_error("OpenAPI_slice_usage_control_info_1_convertToJSON() failed [dereg_inact_timer]");
        goto end;
    }
    }

    if (slice_usage_control_info_1->is_sess_inact_timer) {
    if (cJSON_AddNumberToObject(item, "sessInactTimer", slice_usage_control_info_1->sess_inact_timer) == NULL) {
        ogs_error("OpenAPI_slice_usage_control_info_1_convertToJSON() failed [sess_inact_timer]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_slice_usage_control_info_1_t *OpenAPI_slice_usage_control_info_1_parseFromJSON(cJSON *slice_usage_control_info_1JSON)
{
    OpenAPI_slice_usage_control_info_1_t *slice_usage_control_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *dereg_inact_timer = NULL;
    cJSON *sess_inact_timer = NULL;
    s_nssai = cJSON_GetObjectItemCaseSensitive(slice_usage_control_info_1JSON, "sNssai");
    if (!s_nssai) {
        ogs_error("OpenAPI_slice_usage_control_info_1_parseFromJSON() failed [s_nssai]");
        goto end;
    }
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }

    dereg_inact_timer = cJSON_GetObjectItemCaseSensitive(slice_usage_control_info_1JSON, "deregInactTimer");
    if (dereg_inact_timer) {
    if (!cJSON_IsNumber(dereg_inact_timer)) {
        ogs_error("OpenAPI_slice_usage_control_info_1_parseFromJSON() failed [dereg_inact_timer]");
        goto end;
    }
    }

    sess_inact_timer = cJSON_GetObjectItemCaseSensitive(slice_usage_control_info_1JSON, "sessInactTimer");
    if (sess_inact_timer) {
    if (!cJSON_IsNumber(sess_inact_timer)) {
        ogs_error("OpenAPI_slice_usage_control_info_1_parseFromJSON() failed [sess_inact_timer]");
        goto end;
    }
    }

    slice_usage_control_info_1_local_var = OpenAPI_slice_usage_control_info_1_create (
        s_nssai_local_nonprim,
        dereg_inact_timer ? true : false,
        dereg_inact_timer ? dereg_inact_timer->valuedouble : 0,
        sess_inact_timer ? true : false,
        sess_inact_timer ? sess_inact_timer->valuedouble : 0
    );

    return slice_usage_control_info_1_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_slice_usage_control_info_1_t *OpenAPI_slice_usage_control_info_1_copy(OpenAPI_slice_usage_control_info_1_t *dst, OpenAPI_slice_usage_control_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_usage_control_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_usage_control_info_1_convertToJSON() failed");
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

    OpenAPI_slice_usage_control_info_1_free(dst);
    dst = OpenAPI_slice_usage_control_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


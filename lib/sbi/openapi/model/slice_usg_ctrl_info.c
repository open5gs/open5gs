
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_usg_ctrl_info.h"

OpenAPI_slice_usg_ctrl_info_t *OpenAPI_slice_usg_ctrl_info_create(
    bool is_pdu_sess_inactiv_timer_null,
    bool is_pdu_sess_inactiv_timer,
    int pdu_sess_inactiv_timer
)
{
    OpenAPI_slice_usg_ctrl_info_t *slice_usg_ctrl_info_local_var = ogs_malloc(sizeof(OpenAPI_slice_usg_ctrl_info_t));
    ogs_assert(slice_usg_ctrl_info_local_var);

    slice_usg_ctrl_info_local_var->is_pdu_sess_inactiv_timer_null = is_pdu_sess_inactiv_timer_null;
    slice_usg_ctrl_info_local_var->is_pdu_sess_inactiv_timer = is_pdu_sess_inactiv_timer;
    slice_usg_ctrl_info_local_var->pdu_sess_inactiv_timer = pdu_sess_inactiv_timer;

    return slice_usg_ctrl_info_local_var;
}

void OpenAPI_slice_usg_ctrl_info_free(OpenAPI_slice_usg_ctrl_info_t *slice_usg_ctrl_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_usg_ctrl_info) {
        return;
    }
    ogs_free(slice_usg_ctrl_info);
}

cJSON *OpenAPI_slice_usg_ctrl_info_convertToJSON(OpenAPI_slice_usg_ctrl_info_t *slice_usg_ctrl_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_usg_ctrl_info == NULL) {
        ogs_error("OpenAPI_slice_usg_ctrl_info_convertToJSON() failed [SliceUsgCtrlInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (slice_usg_ctrl_info->is_pdu_sess_inactiv_timer) {
    if (cJSON_AddNumberToObject(item, "pduSessInactivTimer", slice_usg_ctrl_info->pdu_sess_inactiv_timer) == NULL) {
        ogs_error("OpenAPI_slice_usg_ctrl_info_convertToJSON() failed [pdu_sess_inactiv_timer]");
        goto end;
    }
    } else if (slice_usg_ctrl_info->is_pdu_sess_inactiv_timer_null) {
        if (cJSON_AddNullToObject(item, "pduSessInactivTimer") == NULL) {
            ogs_error("OpenAPI_slice_usg_ctrl_info_convertToJSON() failed [pdu_sess_inactiv_timer]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_slice_usg_ctrl_info_t *OpenAPI_slice_usg_ctrl_info_parseFromJSON(cJSON *slice_usg_ctrl_infoJSON)
{
    OpenAPI_slice_usg_ctrl_info_t *slice_usg_ctrl_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_sess_inactiv_timer = NULL;
    pdu_sess_inactiv_timer = cJSON_GetObjectItemCaseSensitive(slice_usg_ctrl_infoJSON, "pduSessInactivTimer");
    if (pdu_sess_inactiv_timer) {
    if (!cJSON_IsNull(pdu_sess_inactiv_timer)) {
    if (!cJSON_IsNumber(pdu_sess_inactiv_timer)) {
        ogs_error("OpenAPI_slice_usg_ctrl_info_parseFromJSON() failed [pdu_sess_inactiv_timer]");
        goto end;
    }
    }
    }

    slice_usg_ctrl_info_local_var = OpenAPI_slice_usg_ctrl_info_create (
        pdu_sess_inactiv_timer && cJSON_IsNull(pdu_sess_inactiv_timer) ? true : false,
        pdu_sess_inactiv_timer ? true : false,
        pdu_sess_inactiv_timer ? pdu_sess_inactiv_timer->valuedouble : 0
    );

    return slice_usg_ctrl_info_local_var;
end:
    return NULL;
}

OpenAPI_slice_usg_ctrl_info_t *OpenAPI_slice_usg_ctrl_info_copy(OpenAPI_slice_usg_ctrl_info_t *dst, OpenAPI_slice_usg_ctrl_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_usg_ctrl_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_usg_ctrl_info_convertToJSON() failed");
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

    OpenAPI_slice_usg_ctrl_info_free(dst);
    dst = OpenAPI_slice_usg_ctrl_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


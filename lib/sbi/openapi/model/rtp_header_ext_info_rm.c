
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rtp_header_ext_info_rm.h"

OpenAPI_rtp_header_ext_info_rm_t *OpenAPI_rtp_header_ext_info_rm_create(
    OpenAPI_rtp_header_ext_type_e rtp_header_ext_type,
    bool is_rtp_header_ext_id_null,
    bool is_rtp_header_ext_id,
    int rtp_header_ext_id,
    bool is_long_format_null,
    bool is_long_format,
    int long_format,
    bool is_pdu_set_size_active_null,
    bool is_pdu_set_size_active,
    int pdu_set_size_active,
    bool is_pdu_set_pdu_count_active_null,
    bool is_pdu_set_pdu_count_active,
    int pdu_set_pdu_count_active
)
{
    OpenAPI_rtp_header_ext_info_rm_t *rtp_header_ext_info_rm_local_var = ogs_malloc(sizeof(OpenAPI_rtp_header_ext_info_rm_t));
    ogs_assert(rtp_header_ext_info_rm_local_var);

    rtp_header_ext_info_rm_local_var->rtp_header_ext_type = rtp_header_ext_type;
    rtp_header_ext_info_rm_local_var->is_rtp_header_ext_id_null = is_rtp_header_ext_id_null;
    rtp_header_ext_info_rm_local_var->is_rtp_header_ext_id = is_rtp_header_ext_id;
    rtp_header_ext_info_rm_local_var->rtp_header_ext_id = rtp_header_ext_id;
    rtp_header_ext_info_rm_local_var->is_long_format_null = is_long_format_null;
    rtp_header_ext_info_rm_local_var->is_long_format = is_long_format;
    rtp_header_ext_info_rm_local_var->long_format = long_format;
    rtp_header_ext_info_rm_local_var->is_pdu_set_size_active_null = is_pdu_set_size_active_null;
    rtp_header_ext_info_rm_local_var->is_pdu_set_size_active = is_pdu_set_size_active;
    rtp_header_ext_info_rm_local_var->pdu_set_size_active = pdu_set_size_active;
    rtp_header_ext_info_rm_local_var->is_pdu_set_pdu_count_active_null = is_pdu_set_pdu_count_active_null;
    rtp_header_ext_info_rm_local_var->is_pdu_set_pdu_count_active = is_pdu_set_pdu_count_active;
    rtp_header_ext_info_rm_local_var->pdu_set_pdu_count_active = pdu_set_pdu_count_active;

    return rtp_header_ext_info_rm_local_var;
}

void OpenAPI_rtp_header_ext_info_rm_free(OpenAPI_rtp_header_ext_info_rm_t *rtp_header_ext_info_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rtp_header_ext_info_rm) {
        return;
    }
    ogs_free(rtp_header_ext_info_rm);
}

cJSON *OpenAPI_rtp_header_ext_info_rm_convertToJSON(OpenAPI_rtp_header_ext_info_rm_t *rtp_header_ext_info_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rtp_header_ext_info_rm == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [RtpHeaderExtInfoRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (rtp_header_ext_info_rm->rtp_header_ext_type != OpenAPI_rtp_header_ext_type_NULL) {
    if (cJSON_AddStringToObject(item, "rtpHeaderExtType", OpenAPI_rtp_header_ext_type_ToString(rtp_header_ext_info_rm->rtp_header_ext_type)) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [rtp_header_ext_type]");
        goto end;
    }
    }

    if (rtp_header_ext_info_rm->is_rtp_header_ext_id) {
    if (cJSON_AddNumberToObject(item, "rtpHeaderExtId", rtp_header_ext_info_rm->rtp_header_ext_id) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [rtp_header_ext_id]");
        goto end;
    }
    } else if (rtp_header_ext_info_rm->is_rtp_header_ext_id_null) {
        if (cJSON_AddNullToObject(item, "rtpHeaderExtId") == NULL) {
            ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [rtp_header_ext_id]");
            goto end;
        }
    }

    if (rtp_header_ext_info_rm->is_long_format) {
    if (cJSON_AddBoolToObject(item, "longFormat", rtp_header_ext_info_rm->long_format) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [long_format]");
        goto end;
    }
    } else if (rtp_header_ext_info_rm->is_long_format_null) {
        if (cJSON_AddNullToObject(item, "longFormat") == NULL) {
            ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [long_format]");
            goto end;
        }
    }

    if (rtp_header_ext_info_rm->is_pdu_set_size_active) {
    if (cJSON_AddBoolToObject(item, "pduSetSizeActive", rtp_header_ext_info_rm->pdu_set_size_active) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [pdu_set_size_active]");
        goto end;
    }
    } else if (rtp_header_ext_info_rm->is_pdu_set_size_active_null) {
        if (cJSON_AddNullToObject(item, "pduSetSizeActive") == NULL) {
            ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [pdu_set_size_active]");
            goto end;
        }
    }

    if (rtp_header_ext_info_rm->is_pdu_set_pdu_count_active) {
    if (cJSON_AddBoolToObject(item, "pduSetPduCountActive", rtp_header_ext_info_rm->pdu_set_pdu_count_active) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [pdu_set_pdu_count_active]");
        goto end;
    }
    } else if (rtp_header_ext_info_rm->is_pdu_set_pdu_count_active_null) {
        if (cJSON_AddNullToObject(item, "pduSetPduCountActive") == NULL) {
            ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed [pdu_set_pdu_count_active]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_rtp_header_ext_info_rm_t *OpenAPI_rtp_header_ext_info_rm_parseFromJSON(cJSON *rtp_header_ext_info_rmJSON)
{
    OpenAPI_rtp_header_ext_info_rm_t *rtp_header_ext_info_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rtp_header_ext_type = NULL;
    OpenAPI_rtp_header_ext_type_e rtp_header_ext_typeVariable = 0;
    cJSON *rtp_header_ext_id = NULL;
    cJSON *long_format = NULL;
    cJSON *pdu_set_size_active = NULL;
    cJSON *pdu_set_pdu_count_active = NULL;
    rtp_header_ext_type = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_info_rmJSON, "rtpHeaderExtType");
    if (rtp_header_ext_type) {
    if (!cJSON_IsString(rtp_header_ext_type)) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_parseFromJSON() failed [rtp_header_ext_type]");
        goto end;
    }
    rtp_header_ext_typeVariable = OpenAPI_rtp_header_ext_type_FromString(rtp_header_ext_type->valuestring);
    }

    rtp_header_ext_id = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_info_rmJSON, "rtpHeaderExtId");
    if (rtp_header_ext_id) {
    if (!cJSON_IsNull(rtp_header_ext_id)) {
    if (!cJSON_IsNumber(rtp_header_ext_id)) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_parseFromJSON() failed [rtp_header_ext_id]");
        goto end;
    }
    }
    }

    long_format = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_info_rmJSON, "longFormat");
    if (long_format) {
    if (!cJSON_IsNull(long_format)) {
    if (!cJSON_IsBool(long_format)) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_parseFromJSON() failed [long_format]");
        goto end;
    }
    }
    }

    pdu_set_size_active = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_info_rmJSON, "pduSetSizeActive");
    if (pdu_set_size_active) {
    if (!cJSON_IsNull(pdu_set_size_active)) {
    if (!cJSON_IsBool(pdu_set_size_active)) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_parseFromJSON() failed [pdu_set_size_active]");
        goto end;
    }
    }
    }

    pdu_set_pdu_count_active = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_info_rmJSON, "pduSetPduCountActive");
    if (pdu_set_pdu_count_active) {
    if (!cJSON_IsNull(pdu_set_pdu_count_active)) {
    if (!cJSON_IsBool(pdu_set_pdu_count_active)) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_parseFromJSON() failed [pdu_set_pdu_count_active]");
        goto end;
    }
    }
    }

    rtp_header_ext_info_rm_local_var = OpenAPI_rtp_header_ext_info_rm_create (
        rtp_header_ext_type ? rtp_header_ext_typeVariable : 0,
        rtp_header_ext_id && cJSON_IsNull(rtp_header_ext_id) ? true : false,
        rtp_header_ext_id ? true : false,
        rtp_header_ext_id ? rtp_header_ext_id->valuedouble : 0,
        long_format && cJSON_IsNull(long_format) ? true : false,
        long_format ? true : false,
        long_format ? long_format->valueint : 0,
        pdu_set_size_active && cJSON_IsNull(pdu_set_size_active) ? true : false,
        pdu_set_size_active ? true : false,
        pdu_set_size_active ? pdu_set_size_active->valueint : 0,
        pdu_set_pdu_count_active && cJSON_IsNull(pdu_set_pdu_count_active) ? true : false,
        pdu_set_pdu_count_active ? true : false,
        pdu_set_pdu_count_active ? pdu_set_pdu_count_active->valueint : 0
    );

    return rtp_header_ext_info_rm_local_var;
end:
    return NULL;
}

OpenAPI_rtp_header_ext_info_rm_t *OpenAPI_rtp_header_ext_info_rm_copy(OpenAPI_rtp_header_ext_info_rm_t *dst, OpenAPI_rtp_header_ext_info_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rtp_header_ext_info_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rtp_header_ext_info_rm_convertToJSON() failed");
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

    OpenAPI_rtp_header_ext_info_rm_free(dst);
    dst = OpenAPI_rtp_header_ext_info_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


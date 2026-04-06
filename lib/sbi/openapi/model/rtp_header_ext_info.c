
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rtp_header_ext_info.h"

OpenAPI_rtp_header_ext_info_t *OpenAPI_rtp_header_ext_info_create(
    OpenAPI_rtp_header_ext_type_e rtp_header_ext_type,
    bool is_rtp_header_ext_id,
    int rtp_header_ext_id,
    bool is_long_format,
    int long_format,
    bool is_pdu_set_size_active,
    int pdu_set_size_active,
    bool is_pdu_set_pdu_count_active,
    int pdu_set_pdu_count_active
)
{
    OpenAPI_rtp_header_ext_info_t *rtp_header_ext_info_local_var = ogs_malloc(sizeof(OpenAPI_rtp_header_ext_info_t));
    ogs_assert(rtp_header_ext_info_local_var);

    rtp_header_ext_info_local_var->rtp_header_ext_type = rtp_header_ext_type;
    rtp_header_ext_info_local_var->is_rtp_header_ext_id = is_rtp_header_ext_id;
    rtp_header_ext_info_local_var->rtp_header_ext_id = rtp_header_ext_id;
    rtp_header_ext_info_local_var->is_long_format = is_long_format;
    rtp_header_ext_info_local_var->long_format = long_format;
    rtp_header_ext_info_local_var->is_pdu_set_size_active = is_pdu_set_size_active;
    rtp_header_ext_info_local_var->pdu_set_size_active = pdu_set_size_active;
    rtp_header_ext_info_local_var->is_pdu_set_pdu_count_active = is_pdu_set_pdu_count_active;
    rtp_header_ext_info_local_var->pdu_set_pdu_count_active = pdu_set_pdu_count_active;

    return rtp_header_ext_info_local_var;
}

void OpenAPI_rtp_header_ext_info_free(OpenAPI_rtp_header_ext_info_t *rtp_header_ext_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == rtp_header_ext_info) {
        return;
    }
    ogs_free(rtp_header_ext_info);
}

cJSON *OpenAPI_rtp_header_ext_info_convertToJSON(OpenAPI_rtp_header_ext_info_t *rtp_header_ext_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (rtp_header_ext_info == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_convertToJSON() failed [RtpHeaderExtInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (rtp_header_ext_info->rtp_header_ext_type != OpenAPI_rtp_header_ext_type_NULL) {
    if (cJSON_AddStringToObject(item, "rtpHeaderExtType", OpenAPI_rtp_header_ext_type_ToString(rtp_header_ext_info->rtp_header_ext_type)) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_convertToJSON() failed [rtp_header_ext_type]");
        goto end;
    }
    }

    if (rtp_header_ext_info->is_rtp_header_ext_id) {
    if (cJSON_AddNumberToObject(item, "rtpHeaderExtId", rtp_header_ext_info->rtp_header_ext_id) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_convertToJSON() failed [rtp_header_ext_id]");
        goto end;
    }
    }

    if (rtp_header_ext_info->is_long_format) {
    if (cJSON_AddBoolToObject(item, "longFormat", rtp_header_ext_info->long_format) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_convertToJSON() failed [long_format]");
        goto end;
    }
    }

    if (rtp_header_ext_info->is_pdu_set_size_active) {
    if (cJSON_AddBoolToObject(item, "pduSetSizeActive", rtp_header_ext_info->pdu_set_size_active) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_convertToJSON() failed [pdu_set_size_active]");
        goto end;
    }
    }

    if (rtp_header_ext_info->is_pdu_set_pdu_count_active) {
    if (cJSON_AddBoolToObject(item, "pduSetPduCountActive", rtp_header_ext_info->pdu_set_pdu_count_active) == NULL) {
        ogs_error("OpenAPI_rtp_header_ext_info_convertToJSON() failed [pdu_set_pdu_count_active]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_rtp_header_ext_info_t *OpenAPI_rtp_header_ext_info_parseFromJSON(cJSON *rtp_header_ext_infoJSON)
{
    OpenAPI_rtp_header_ext_info_t *rtp_header_ext_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rtp_header_ext_type = NULL;
    OpenAPI_rtp_header_ext_type_e rtp_header_ext_typeVariable = 0;
    cJSON *rtp_header_ext_id = NULL;
    cJSON *long_format = NULL;
    cJSON *pdu_set_size_active = NULL;
    cJSON *pdu_set_pdu_count_active = NULL;
    rtp_header_ext_type = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_infoJSON, "rtpHeaderExtType");
    if (rtp_header_ext_type) {
    if (!cJSON_IsString(rtp_header_ext_type)) {
        ogs_error("OpenAPI_rtp_header_ext_info_parseFromJSON() failed [rtp_header_ext_type]");
        goto end;
    }
    rtp_header_ext_typeVariable = OpenAPI_rtp_header_ext_type_FromString(rtp_header_ext_type->valuestring);
    }

    rtp_header_ext_id = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_infoJSON, "rtpHeaderExtId");
    if (rtp_header_ext_id) {
    if (!cJSON_IsNumber(rtp_header_ext_id)) {
        ogs_error("OpenAPI_rtp_header_ext_info_parseFromJSON() failed [rtp_header_ext_id]");
        goto end;
    }
    }

    long_format = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_infoJSON, "longFormat");
    if (long_format) {
    if (!cJSON_IsBool(long_format)) {
        ogs_error("OpenAPI_rtp_header_ext_info_parseFromJSON() failed [long_format]");
        goto end;
    }
    }

    pdu_set_size_active = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_infoJSON, "pduSetSizeActive");
    if (pdu_set_size_active) {
    if (!cJSON_IsBool(pdu_set_size_active)) {
        ogs_error("OpenAPI_rtp_header_ext_info_parseFromJSON() failed [pdu_set_size_active]");
        goto end;
    }
    }

    pdu_set_pdu_count_active = cJSON_GetObjectItemCaseSensitive(rtp_header_ext_infoJSON, "pduSetPduCountActive");
    if (pdu_set_pdu_count_active) {
    if (!cJSON_IsBool(pdu_set_pdu_count_active)) {
        ogs_error("OpenAPI_rtp_header_ext_info_parseFromJSON() failed [pdu_set_pdu_count_active]");
        goto end;
    }
    }

    rtp_header_ext_info_local_var = OpenAPI_rtp_header_ext_info_create (
        rtp_header_ext_type ? rtp_header_ext_typeVariable : 0,
        rtp_header_ext_id ? true : false,
        rtp_header_ext_id ? rtp_header_ext_id->valuedouble : 0,
        long_format ? true : false,
        long_format ? long_format->valueint : 0,
        pdu_set_size_active ? true : false,
        pdu_set_size_active ? pdu_set_size_active->valueint : 0,
        pdu_set_pdu_count_active ? true : false,
        pdu_set_pdu_count_active ? pdu_set_pdu_count_active->valueint : 0
    );

    return rtp_header_ext_info_local_var;
end:
    return NULL;
}

OpenAPI_rtp_header_ext_info_t *OpenAPI_rtp_header_ext_info_copy(OpenAPI_rtp_header_ext_info_t *dst, OpenAPI_rtp_header_ext_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_rtp_header_ext_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_rtp_header_ext_info_convertToJSON() failed");
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

    OpenAPI_rtp_header_ext_info_free(dst);
    dst = OpenAPI_rtp_header_ext_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


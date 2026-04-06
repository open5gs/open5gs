
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mpx_media_info.h"

OpenAPI_mpx_media_info_t *OpenAPI_mpx_media_info_create(
    bool is_ssrc_id,
    int ssrc_id,
    bool is_payload_type,
    int payload_type,
    char *identification_tag,
    bool is_rtp_sdes_hdr_ext_id,
    int rtp_sdes_hdr_ext_id,
    bool is_rtcp_packet_type,
    int rtcp_packet_type
)
{
    OpenAPI_mpx_media_info_t *mpx_media_info_local_var = ogs_malloc(sizeof(OpenAPI_mpx_media_info_t));
    ogs_assert(mpx_media_info_local_var);

    mpx_media_info_local_var->is_ssrc_id = is_ssrc_id;
    mpx_media_info_local_var->ssrc_id = ssrc_id;
    mpx_media_info_local_var->is_payload_type = is_payload_type;
    mpx_media_info_local_var->payload_type = payload_type;
    mpx_media_info_local_var->identification_tag = identification_tag;
    mpx_media_info_local_var->is_rtp_sdes_hdr_ext_id = is_rtp_sdes_hdr_ext_id;
    mpx_media_info_local_var->rtp_sdes_hdr_ext_id = rtp_sdes_hdr_ext_id;
    mpx_media_info_local_var->is_rtcp_packet_type = is_rtcp_packet_type;
    mpx_media_info_local_var->rtcp_packet_type = rtcp_packet_type;

    return mpx_media_info_local_var;
}

void OpenAPI_mpx_media_info_free(OpenAPI_mpx_media_info_t *mpx_media_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mpx_media_info) {
        return;
    }
    if (mpx_media_info->identification_tag) {
        ogs_free(mpx_media_info->identification_tag);
        mpx_media_info->identification_tag = NULL;
    }
    ogs_free(mpx_media_info);
}

cJSON *OpenAPI_mpx_media_info_convertToJSON(OpenAPI_mpx_media_info_t *mpx_media_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mpx_media_info == NULL) {
        ogs_error("OpenAPI_mpx_media_info_convertToJSON() failed [MpxMediaInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mpx_media_info->is_ssrc_id) {
    if (cJSON_AddNumberToObject(item, "ssrcId", mpx_media_info->ssrc_id) == NULL) {
        ogs_error("OpenAPI_mpx_media_info_convertToJSON() failed [ssrc_id]");
        goto end;
    }
    }

    if (mpx_media_info->is_payload_type) {
    if (cJSON_AddNumberToObject(item, "payloadType", mpx_media_info->payload_type) == NULL) {
        ogs_error("OpenAPI_mpx_media_info_convertToJSON() failed [payload_type]");
        goto end;
    }
    }

    if (mpx_media_info->identification_tag) {
    if (cJSON_AddStringToObject(item, "identificationTag", mpx_media_info->identification_tag) == NULL) {
        ogs_error("OpenAPI_mpx_media_info_convertToJSON() failed [identification_tag]");
        goto end;
    }
    }

    if (mpx_media_info->is_rtp_sdes_hdr_ext_id) {
    if (cJSON_AddNumberToObject(item, "rtpSdesHdrExtId", mpx_media_info->rtp_sdes_hdr_ext_id) == NULL) {
        ogs_error("OpenAPI_mpx_media_info_convertToJSON() failed [rtp_sdes_hdr_ext_id]");
        goto end;
    }
    }

    if (mpx_media_info->is_rtcp_packet_type) {
    if (cJSON_AddNumberToObject(item, "rtcpPacketType", mpx_media_info->rtcp_packet_type) == NULL) {
        ogs_error("OpenAPI_mpx_media_info_convertToJSON() failed [rtcp_packet_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_mpx_media_info_t *OpenAPI_mpx_media_info_parseFromJSON(cJSON *mpx_media_infoJSON)
{
    OpenAPI_mpx_media_info_t *mpx_media_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ssrc_id = NULL;
    cJSON *payload_type = NULL;
    cJSON *identification_tag = NULL;
    cJSON *rtp_sdes_hdr_ext_id = NULL;
    cJSON *rtcp_packet_type = NULL;
    ssrc_id = cJSON_GetObjectItemCaseSensitive(mpx_media_infoJSON, "ssrcId");
    if (ssrc_id) {
    if (!cJSON_IsNumber(ssrc_id)) {
        ogs_error("OpenAPI_mpx_media_info_parseFromJSON() failed [ssrc_id]");
        goto end;
    }
    }

    payload_type = cJSON_GetObjectItemCaseSensitive(mpx_media_infoJSON, "payloadType");
    if (payload_type) {
    if (!cJSON_IsNumber(payload_type)) {
        ogs_error("OpenAPI_mpx_media_info_parseFromJSON() failed [payload_type]");
        goto end;
    }
    }

    identification_tag = cJSON_GetObjectItemCaseSensitive(mpx_media_infoJSON, "identificationTag");
    if (identification_tag) {
    if (!cJSON_IsString(identification_tag) && !cJSON_IsNull(identification_tag)) {
        ogs_error("OpenAPI_mpx_media_info_parseFromJSON() failed [identification_tag]");
        goto end;
    }
    }

    rtp_sdes_hdr_ext_id = cJSON_GetObjectItemCaseSensitive(mpx_media_infoJSON, "rtpSdesHdrExtId");
    if (rtp_sdes_hdr_ext_id) {
    if (!cJSON_IsNumber(rtp_sdes_hdr_ext_id)) {
        ogs_error("OpenAPI_mpx_media_info_parseFromJSON() failed [rtp_sdes_hdr_ext_id]");
        goto end;
    }
    }

    rtcp_packet_type = cJSON_GetObjectItemCaseSensitive(mpx_media_infoJSON, "rtcpPacketType");
    if (rtcp_packet_type) {
    if (!cJSON_IsNumber(rtcp_packet_type)) {
        ogs_error("OpenAPI_mpx_media_info_parseFromJSON() failed [rtcp_packet_type]");
        goto end;
    }
    }

    mpx_media_info_local_var = OpenAPI_mpx_media_info_create (
        ssrc_id ? true : false,
        ssrc_id ? ssrc_id->valuedouble : 0,
        payload_type ? true : false,
        payload_type ? payload_type->valuedouble : 0,
        identification_tag && !cJSON_IsNull(identification_tag) ? ogs_strdup(identification_tag->valuestring) : NULL,
        rtp_sdes_hdr_ext_id ? true : false,
        rtp_sdes_hdr_ext_id ? rtp_sdes_hdr_ext_id->valuedouble : 0,
        rtcp_packet_type ? true : false,
        rtcp_packet_type ? rtcp_packet_type->valuedouble : 0
    );

    return mpx_media_info_local_var;
end:
    return NULL;
}

OpenAPI_mpx_media_info_t *OpenAPI_mpx_media_info_copy(OpenAPI_mpx_media_info_t *dst, OpenAPI_mpx_media_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mpx_media_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mpx_media_info_convertToJSON() failed");
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

    OpenAPI_mpx_media_info_free(dst);
    dst = OpenAPI_mpx_media_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


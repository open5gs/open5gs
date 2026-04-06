/*
 * mpx_media_info.h
 *
 * Contains the (S)RTP Multiplexed Media Identification Information.
 */

#ifndef _OpenAPI_mpx_media_info_H_
#define _OpenAPI_mpx_media_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mpx_media_info_s OpenAPI_mpx_media_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mpx_media_info_s {
    bool is_ssrc_id;
    int ssrc_id;
    bool is_payload_type;
    int payload_type;
    char *identification_tag;
    bool is_rtp_sdes_hdr_ext_id;
    int rtp_sdes_hdr_ext_id;
    bool is_rtcp_packet_type;
    int rtcp_packet_type;
};

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
);
void OpenAPI_mpx_media_info_free(OpenAPI_mpx_media_info_t *mpx_media_info);
OpenAPI_mpx_media_info_t *OpenAPI_mpx_media_info_parseFromJSON(cJSON *mpx_media_infoJSON);
cJSON *OpenAPI_mpx_media_info_convertToJSON(OpenAPI_mpx_media_info_t *mpx_media_info);
OpenAPI_mpx_media_info_t *OpenAPI_mpx_media_info_copy(OpenAPI_mpx_media_info_t *dst, OpenAPI_mpx_media_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mpx_media_info_H_ */


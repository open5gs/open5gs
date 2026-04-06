/*
 * rtp_payload_info.h
 *
 * RtpPayloadInfo contains Rtp payload type and format.
 */

#ifndef _OpenAPI_rtp_payload_info_H_
#define _OpenAPI_rtp_payload_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rtp_payload_info_s OpenAPI_rtp_payload_info_t;
#include "rtp_payload_format.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rtp_payload_info_s {
    OpenAPI_list_t *rtp_payload_type_list;
    OpenAPI_rtp_payload_format_e rtp_payload_format;
};

OpenAPI_rtp_payload_info_t *OpenAPI_rtp_payload_info_create(
    OpenAPI_list_t *rtp_payload_type_list,
    OpenAPI_rtp_payload_format_e rtp_payload_format
);
void OpenAPI_rtp_payload_info_free(OpenAPI_rtp_payload_info_t *rtp_payload_info);
OpenAPI_rtp_payload_info_t *OpenAPI_rtp_payload_info_parseFromJSON(cJSON *rtp_payload_infoJSON);
cJSON *OpenAPI_rtp_payload_info_convertToJSON(OpenAPI_rtp_payload_info_t *rtp_payload_info);
OpenAPI_rtp_payload_info_t *OpenAPI_rtp_payload_info_copy(OpenAPI_rtp_payload_info_t *dst, OpenAPI_rtp_payload_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rtp_payload_info_H_ */


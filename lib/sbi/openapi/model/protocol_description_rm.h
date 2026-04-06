/*
 * protocol_description_rm.h
 *
 * Describes the modifications to the ProtocolDescription data type.
 */

#ifndef _OpenAPI_protocol_description_rm_H_
#define _OpenAPI_protocol_description_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_protocol_description_rm_s OpenAPI_protocol_description_rm_t;
#include "media_transport_proto.h"
#include "mri_transfer_method.h"
#include "rtp_header_ext_info.h"
#include "rtp_header_ext_info_rm.h"
#include "rtp_payload_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_protocol_description_rm_s {
    OpenAPI_media_transport_proto_e transport_proto;
    bool is_rtp_header_ext_info_null;
    struct OpenAPI_rtp_header_ext_info_rm_s *rtp_header_ext_info;
    bool is_add_rtp_header_ext_info_null;
    OpenAPI_list_t *add_rtp_header_ext_info;
    bool is_rtp_payload_info_list_null;
    OpenAPI_list_t *rtp_payload_info_list;
    OpenAPI_mri_transfer_method_e mri_transfer_info;
};

OpenAPI_protocol_description_rm_t *OpenAPI_protocol_description_rm_create(
    OpenAPI_media_transport_proto_e transport_proto,
    bool is_rtp_header_ext_info_null,
    OpenAPI_rtp_header_ext_info_rm_t *rtp_header_ext_info,
    bool is_add_rtp_header_ext_info_null,
    OpenAPI_list_t *add_rtp_header_ext_info,
    bool is_rtp_payload_info_list_null,
    OpenAPI_list_t *rtp_payload_info_list,
    OpenAPI_mri_transfer_method_e mri_transfer_info
);
void OpenAPI_protocol_description_rm_free(OpenAPI_protocol_description_rm_t *protocol_description_rm);
OpenAPI_protocol_description_rm_t *OpenAPI_protocol_description_rm_parseFromJSON(cJSON *protocol_description_rmJSON);
cJSON *OpenAPI_protocol_description_rm_convertToJSON(OpenAPI_protocol_description_rm_t *protocol_description_rm);
OpenAPI_protocol_description_rm_t *OpenAPI_protocol_description_rm_copy(OpenAPI_protocol_description_rm_t *dst, OpenAPI_protocol_description_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_protocol_description_rm_H_ */


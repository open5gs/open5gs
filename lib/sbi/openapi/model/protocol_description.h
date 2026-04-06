/*
 * protocol_description.h
 *
 * ProtocolDescription contains information to derive PDU set information.
 */

#ifndef _OpenAPI_protocol_description_H_
#define _OpenAPI_protocol_description_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_protocol_description_s OpenAPI_protocol_description_t;
#include "media_transport_proto.h"
#include "mri_transfer_method.h"
#include "rtp_header_ext_info.h"
#include "rtp_payload_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_protocol_description_s {
    OpenAPI_media_transport_proto_e transport_proto;
    struct OpenAPI_rtp_header_ext_info_s *rtp_header_ext_info;
    OpenAPI_list_t *add_rtp_header_ext_info;
    OpenAPI_list_t *rtp_payload_info_list;
    OpenAPI_mri_transfer_method_e mri_transfer_info;
};

OpenAPI_protocol_description_t *OpenAPI_protocol_description_create(
    OpenAPI_media_transport_proto_e transport_proto,
    OpenAPI_rtp_header_ext_info_t *rtp_header_ext_info,
    OpenAPI_list_t *add_rtp_header_ext_info,
    OpenAPI_list_t *rtp_payload_info_list,
    OpenAPI_mri_transfer_method_e mri_transfer_info
);
void OpenAPI_protocol_description_free(OpenAPI_protocol_description_t *protocol_description);
OpenAPI_protocol_description_t *OpenAPI_protocol_description_parseFromJSON(cJSON *protocol_descriptionJSON);
cJSON *OpenAPI_protocol_description_convertToJSON(OpenAPI_protocol_description_t *protocol_description);
OpenAPI_protocol_description_t *OpenAPI_protocol_description_copy(OpenAPI_protocol_description_t *dst, OpenAPI_protocol_description_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_protocol_description_H_ */


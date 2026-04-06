/*
 * rtp_header_ext_info.h
 *
 * RTP Header Extension information
 */

#ifndef _OpenAPI_rtp_header_ext_info_H_
#define _OpenAPI_rtp_header_ext_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rtp_header_ext_info_s OpenAPI_rtp_header_ext_info_t;
#include "rtp_header_ext_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rtp_header_ext_info_s {
    OpenAPI_rtp_header_ext_type_e rtp_header_ext_type;
    bool is_rtp_header_ext_id;
    int rtp_header_ext_id;
    bool is_long_format;
    int long_format;
    bool is_pdu_set_size_active;
    int pdu_set_size_active;
    bool is_pdu_set_pdu_count_active;
    int pdu_set_pdu_count_active;
};

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
);
void OpenAPI_rtp_header_ext_info_free(OpenAPI_rtp_header_ext_info_t *rtp_header_ext_info);
OpenAPI_rtp_header_ext_info_t *OpenAPI_rtp_header_ext_info_parseFromJSON(cJSON *rtp_header_ext_infoJSON);
cJSON *OpenAPI_rtp_header_ext_info_convertToJSON(OpenAPI_rtp_header_ext_info_t *rtp_header_ext_info);
OpenAPI_rtp_header_ext_info_t *OpenAPI_rtp_header_ext_info_copy(OpenAPI_rtp_header_ext_info_t *dst, OpenAPI_rtp_header_ext_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rtp_header_ext_info_H_ */


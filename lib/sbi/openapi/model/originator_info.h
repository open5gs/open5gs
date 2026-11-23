/*
 * originator_info.h
 *
 * 
 */

#ifndef _OpenAPI_originator_info_H_
#define _OpenAPI_originator_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_originator_info_s OpenAPI_originator_info_t;
#include "sm_address_info.h"
#include "sm_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_originator_info_s {
    char *originator_supi;
    char *originator_gpsi;
    struct OpenAPI_sm_address_info_s *originator_other_address;
    struct OpenAPI_sm_address_info_s *originator_received_address;
    char *originator_sccp_address;
    struct OpenAPI_sm_interface_s *s_m_originator_interface;
    char *s_m_originator_protocol_id;
};

OpenAPI_originator_info_t *OpenAPI_originator_info_create(
    char *originator_supi,
    char *originator_gpsi,
    OpenAPI_sm_address_info_t *originator_other_address,
    OpenAPI_sm_address_info_t *originator_received_address,
    char *originator_sccp_address,
    OpenAPI_sm_interface_t *s_m_originator_interface,
    char *s_m_originator_protocol_id
);
void OpenAPI_originator_info_free(OpenAPI_originator_info_t *originator_info);
OpenAPI_originator_info_t *OpenAPI_originator_info_parseFromJSON(cJSON *originator_infoJSON);
cJSON *OpenAPI_originator_info_convertToJSON(OpenAPI_originator_info_t *originator_info);
OpenAPI_originator_info_t *OpenAPI_originator_info_copy(OpenAPI_originator_info_t *dst, OpenAPI_originator_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_originator_info_H_ */


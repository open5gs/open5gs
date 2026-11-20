/*
 * recipient_info.h
 *
 * 
 */

#ifndef _OpenAPI_recipient_info_H_
#define _OpenAPI_recipient_info_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_recipient_info_s OpenAPI_recipient_info_t;
#include "recipient_address.h"
#include "sm_address_info.h"
#include "sm_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_recipient_info_s {
    char *recipient_supi;
    char *recipient_gpsi;
    struct OpenAPI_sm_address_info_s *recipient_other_address;
    OpenAPI_list_t *recipient_other_addresses;
    struct OpenAPI_sm_address_info_s *recipient_received_address;
    char *recipient_sccp_address;
    struct OpenAPI_sm_interface_s *s_m_destination_interface;
    char *s_mrecipient_protocol_id;
};

OpenAPI_recipient_info_t *OpenAPI_recipient_info_create(
    char *recipient_supi,
    char *recipient_gpsi,
    OpenAPI_sm_address_info_t *recipient_other_address,
    OpenAPI_list_t *recipient_other_addresses,
    OpenAPI_sm_address_info_t *recipient_received_address,
    char *recipient_sccp_address,
    OpenAPI_sm_interface_t *s_m_destination_interface,
    char *s_mrecipient_protocol_id
);
void OpenAPI_recipient_info_free(OpenAPI_recipient_info_t *recipient_info);
OpenAPI_recipient_info_t *OpenAPI_recipient_info_parseFromJSON(cJSON *recipient_infoJSON);
cJSON *OpenAPI_recipient_info_convertToJSON(OpenAPI_recipient_info_t *recipient_info);
OpenAPI_recipient_info_t *OpenAPI_recipient_info_copy(OpenAPI_recipient_info_t *dst, OpenAPI_recipient_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_recipient_info_H_ */


/*
 * sdp_media_component.h
 *
 * 
 */

#ifndef _OpenAPI_sdp_media_component_H_
#define _OpenAPI_sdp_media_component_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sdp_media_component_s OpenAPI_sdp_media_component_t;
#include "media_initiator_flag.h"
#include "sdp_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sdp_media_component_s {
    char *s_dp_media_name;
    OpenAPI_list_t *sdp_media_description;
    bool is_local_gw_inserted_indication;
    int local_gw_inserted_indication;
    bool is_ip_realm_default_indication;
    int ip_realm_default_indication;
    bool is_transcoder_inserted_indication;
    int transcoder_inserted_indication;
    OpenAPI_media_initiator_flag_e media_initiator_flag;
    char *media_initiator_party;
    char *three_gpp_charging_id;
    char *access_network_charging_identifier_value;
    OpenAPI_sdp_type_e s_dp_type;
};

OpenAPI_sdp_media_component_t *OpenAPI_sdp_media_component_create(
    char *s_dp_media_name,
    OpenAPI_list_t *sdp_media_description,
    bool is_local_gw_inserted_indication,
    int local_gw_inserted_indication,
    bool is_ip_realm_default_indication,
    int ip_realm_default_indication,
    bool is_transcoder_inserted_indication,
    int transcoder_inserted_indication,
    OpenAPI_media_initiator_flag_e media_initiator_flag,
    char *media_initiator_party,
    char *three_gpp_charging_id,
    char *access_network_charging_identifier_value,
    OpenAPI_sdp_type_e s_dp_type
);
void OpenAPI_sdp_media_component_free(OpenAPI_sdp_media_component_t *sdp_media_component);
OpenAPI_sdp_media_component_t *OpenAPI_sdp_media_component_parseFromJSON(cJSON *sdp_media_componentJSON);
cJSON *OpenAPI_sdp_media_component_convertToJSON(OpenAPI_sdp_media_component_t *sdp_media_component);
OpenAPI_sdp_media_component_t *OpenAPI_sdp_media_component_copy(OpenAPI_sdp_media_component_t *dst, OpenAPI_sdp_media_component_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sdp_media_component_H_ */


/*
 * nssaa_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_nssaa_charging_information_H_
#define _OpenAPI_nssaa_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nssaa_charging_information_s OpenAPI_nssaa_charging_information_t;
#include "auth_status.h"
#include "nssaa_message_type.h"
#include "server_addressing_info.h"
#include "user_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nssaa_charging_information_s {
    OpenAPI_nssaa_message_type_e n_ssaa_message_type;
    struct OpenAPI_user_information_s *user_identification;
    bool is_a_aap_address_null;
    struct OpenAPI_server_addressing_info_s *a_aap_address;
    bool is_a_aas_address_null;
    struct OpenAPI_server_addressing_info_s *a_aas_address;
    char *e_apid_response;
    OpenAPI_auth_status_e e_a_pauthstatus;
    char *a_mfid;
};

OpenAPI_nssaa_charging_information_t *OpenAPI_nssaa_charging_information_create(
    OpenAPI_nssaa_message_type_e n_ssaa_message_type,
    OpenAPI_user_information_t *user_identification,
    bool is_a_aap_address_null,
    OpenAPI_server_addressing_info_t *a_aap_address,
    bool is_a_aas_address_null,
    OpenAPI_server_addressing_info_t *a_aas_address,
    char *e_apid_response,
    OpenAPI_auth_status_e e_a_pauthstatus,
    char *a_mfid
);
void OpenAPI_nssaa_charging_information_free(OpenAPI_nssaa_charging_information_t *nssaa_charging_information);
OpenAPI_nssaa_charging_information_t *OpenAPI_nssaa_charging_information_parseFromJSON(cJSON *nssaa_charging_informationJSON);
cJSON *OpenAPI_nssaa_charging_information_convertToJSON(OpenAPI_nssaa_charging_information_t *nssaa_charging_information);
OpenAPI_nssaa_charging_information_t *OpenAPI_nssaa_charging_information_copy(OpenAPI_nssaa_charging_information_t *dst, OpenAPI_nssaa_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssaa_charging_information_H_ */


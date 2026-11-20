/*
 * network_sharing_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_network_sharing_charging_information_H_
#define _OpenAPI_network_sharing_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_network_sharing_charging_information_s OpenAPI_network_sharing_charging_information_t;
#include "plmn_id.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_network_sharing_charging_information_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    struct OpenAPI_snssai_s *single_nssai;
};

OpenAPI_network_sharing_charging_information_t *OpenAPI_network_sharing_charging_information_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_snssai_t *single_nssai
);
void OpenAPI_network_sharing_charging_information_free(OpenAPI_network_sharing_charging_information_t *network_sharing_charging_information);
OpenAPI_network_sharing_charging_information_t *OpenAPI_network_sharing_charging_information_parseFromJSON(cJSON *network_sharing_charging_informationJSON);
cJSON *OpenAPI_network_sharing_charging_information_convertToJSON(OpenAPI_network_sharing_charging_information_t *network_sharing_charging_information);
OpenAPI_network_sharing_charging_information_t *OpenAPI_network_sharing_charging_information_copy(OpenAPI_network_sharing_charging_information_t *dst, OpenAPI_network_sharing_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_sharing_charging_information_H_ */


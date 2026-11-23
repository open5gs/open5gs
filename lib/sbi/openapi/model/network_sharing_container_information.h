/*
 * network_sharing_container_information.h
 *
 * 
 */

#ifndef _OpenAPI_network_sharing_container_information_H_
#define _OpenAPI_network_sharing_container_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_network_sharing_container_information_s OpenAPI_network_sharing_container_information_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_network_sharing_container_information_s {
    bool is_uplink_volume;
    int uplink_volume;
    bool is_downlink_volume;
    int downlink_volume;
    bool is_number_of_pdu_sessions_req;
    int number_of_pdu_sessions_req;
    bool is_number_of_pdu_sessions_success;
    int number_of_pdu_sessions_success;
};

OpenAPI_network_sharing_container_information_t *OpenAPI_network_sharing_container_information_create(
    bool is_uplink_volume,
    int uplink_volume,
    bool is_downlink_volume,
    int downlink_volume,
    bool is_number_of_pdu_sessions_req,
    int number_of_pdu_sessions_req,
    bool is_number_of_pdu_sessions_success,
    int number_of_pdu_sessions_success
);
void OpenAPI_network_sharing_container_information_free(OpenAPI_network_sharing_container_information_t *network_sharing_container_information);
OpenAPI_network_sharing_container_information_t *OpenAPI_network_sharing_container_information_parseFromJSON(cJSON *network_sharing_container_informationJSON);
cJSON *OpenAPI_network_sharing_container_information_convertToJSON(OpenAPI_network_sharing_container_information_t *network_sharing_container_information);
OpenAPI_network_sharing_container_information_t *OpenAPI_network_sharing_container_information_copy(OpenAPI_network_sharing_container_information_t *dst, OpenAPI_network_sharing_container_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_network_sharing_container_information_H_ */


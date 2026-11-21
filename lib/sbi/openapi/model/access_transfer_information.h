/*
 * access_transfer_information.h
 *
 * 
 */

#ifndef _OpenAPI_access_transfer_information_H_
#define _OpenAPI_access_transfer_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_access_transfer_information_s OpenAPI_access_transfer_information_t;
#include "access_transfer_type.h"
#include "ims_address.h"
#include "ue_transfer_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_access_transfer_information_s {
    OpenAPI_access_transfer_type_e access_transfer_type;
    OpenAPI_list_t *access_network_information;
    char *cellular_network_information;
    OpenAPI_ue_transfer_type_e inter_ue_transfer;
    char *user_equipment_info;
    char *instance_id;
    char *related_ims_charging_identifier;
    bool is_related_ims_charging_identifier_node_null;
    struct OpenAPI_ims_address_s *related_ims_charging_identifier_node;
    char *change_time;
};

OpenAPI_access_transfer_information_t *OpenAPI_access_transfer_information_create(
    OpenAPI_access_transfer_type_e access_transfer_type,
    OpenAPI_list_t *access_network_information,
    char *cellular_network_information,
    OpenAPI_ue_transfer_type_e inter_ue_transfer,
    char *user_equipment_info,
    char *instance_id,
    char *related_ims_charging_identifier,
    bool is_related_ims_charging_identifier_node_null,
    OpenAPI_ims_address_t *related_ims_charging_identifier_node,
    char *change_time
);
void OpenAPI_access_transfer_information_free(OpenAPI_access_transfer_information_t *access_transfer_information);
OpenAPI_access_transfer_information_t *OpenAPI_access_transfer_information_parseFromJSON(cJSON *access_transfer_informationJSON);
cJSON *OpenAPI_access_transfer_information_convertToJSON(OpenAPI_access_transfer_information_t *access_transfer_information);
OpenAPI_access_transfer_information_t *OpenAPI_access_transfer_information_copy(OpenAPI_access_transfer_information_t *dst, OpenAPI_access_transfer_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_transfer_information_H_ */


/*
 * smsc_data.h
 *
 * Addresses of SM-Service Center entities with SMS wating to be delivered to the UE.
 */

#ifndef _OpenAPI_smsc_data_H_
#define _OpenAPI_smsc_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_node_diameter_address_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_smsc_data_s OpenAPI_smsc_data_t;
typedef struct OpenAPI_smsc_data_s {
    char *smsc_map_address;
    struct OpenAPI_network_node_diameter_address_1_s *smsc_diameter_address;
} OpenAPI_smsc_data_t;

OpenAPI_smsc_data_t *OpenAPI_smsc_data_create(
    char *smsc_map_address,
    OpenAPI_network_node_diameter_address_1_t *smsc_diameter_address
);
void OpenAPI_smsc_data_free(OpenAPI_smsc_data_t *smsc_data);
OpenAPI_smsc_data_t *OpenAPI_smsc_data_parseFromJSON(cJSON *smsc_dataJSON);
cJSON *OpenAPI_smsc_data_convertToJSON(OpenAPI_smsc_data_t *smsc_data);
OpenAPI_smsc_data_t *OpenAPI_smsc_data_copy(OpenAPI_smsc_data_t *dst, OpenAPI_smsc_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smsc_data_H_ */


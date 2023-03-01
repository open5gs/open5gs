/*
 * additional_measurement.h
 *
 * Represents additional measurement information.
 */

#ifndef _OpenAPI_additional_measurement_H_
#define _OpenAPI_additional_measurement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "address_list.h"
#include "circumstance_description.h"
#include "ip_eth_flow_description.h"
#include "network_area_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_additional_measurement_s OpenAPI_additional_measurement_t;
typedef struct OpenAPI_additional_measurement_s {
    struct OpenAPI_network_area_info_s *unexp_loc;
    OpenAPI_list_t *unexp_flow_teps;
    OpenAPI_list_t *unexp_wakes;
    struct OpenAPI_address_list_s *ddos_attack;
    struct OpenAPI_address_list_s *wrg_dest;
    OpenAPI_list_t *circums;
} OpenAPI_additional_measurement_t;

OpenAPI_additional_measurement_t *OpenAPI_additional_measurement_create(
    OpenAPI_network_area_info_t *unexp_loc,
    OpenAPI_list_t *unexp_flow_teps,
    OpenAPI_list_t *unexp_wakes,
    OpenAPI_address_list_t *ddos_attack,
    OpenAPI_address_list_t *wrg_dest,
    OpenAPI_list_t *circums
);
void OpenAPI_additional_measurement_free(OpenAPI_additional_measurement_t *additional_measurement);
OpenAPI_additional_measurement_t *OpenAPI_additional_measurement_parseFromJSON(cJSON *additional_measurementJSON);
cJSON *OpenAPI_additional_measurement_convertToJSON(OpenAPI_additional_measurement_t *additional_measurement);
OpenAPI_additional_measurement_t *OpenAPI_additional_measurement_copy(OpenAPI_additional_measurement_t *dst, OpenAPI_additional_measurement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_additional_measurement_H_ */


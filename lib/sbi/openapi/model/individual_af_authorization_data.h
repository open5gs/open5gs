/*
 * individual_af_authorization_data.h
 *
 * Contains the AF Authorization Data for an individual AF.
 */

#ifndef _OpenAPI_individual_af_authorization_data_H_
#define _OpenAPI_individual_af_authorization_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_individual_af_authorization_data_s OpenAPI_individual_af_authorization_data_t;
#include "aiot_area.h"
#include "allowed_service_operation.h"
#include "allowed_target_aiot_device.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_individual_af_authorization_data_s {
    char *af_id;
    struct OpenAPI_aiot_area_s *allowed_area;
    OpenAPI_list_t *allowed_service_operations;
    OpenAPI_list_t *allowed_target_aiot_devices;
};

OpenAPI_individual_af_authorization_data_t *OpenAPI_individual_af_authorization_data_create(
    char *af_id,
    OpenAPI_aiot_area_t *allowed_area,
    OpenAPI_list_t *allowed_service_operations,
    OpenAPI_list_t *allowed_target_aiot_devices
);
void OpenAPI_individual_af_authorization_data_free(OpenAPI_individual_af_authorization_data_t *individual_af_authorization_data);
OpenAPI_individual_af_authorization_data_t *OpenAPI_individual_af_authorization_data_parseFromJSON(cJSON *individual_af_authorization_dataJSON);
cJSON *OpenAPI_individual_af_authorization_data_convertToJSON(OpenAPI_individual_af_authorization_data_t *individual_af_authorization_data);
OpenAPI_individual_af_authorization_data_t *OpenAPI_individual_af_authorization_data_copy(OpenAPI_individual_af_authorization_data_t *dst, OpenAPI_individual_af_authorization_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_individual_af_authorization_data_H_ */


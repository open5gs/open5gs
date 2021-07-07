/*
 * charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_charging_information_H_
#define _OpenAPI_charging_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_charging_information_s OpenAPI_charging_information_t;
typedef struct OpenAPI_charging_information_s {
    char *primary_chf_address;
    char *secondary_chf_address;
    char *primary_chf_set_id;
    char *primary_chf_instance_id;
    char *secondary_chf_set_id;
    char *secondary_chf_instance_id;
} OpenAPI_charging_information_t;

OpenAPI_charging_information_t *OpenAPI_charging_information_create(
    char *primary_chf_address,
    char *secondary_chf_address,
    char *primary_chf_set_id,
    char *primary_chf_instance_id,
    char *secondary_chf_set_id,
    char *secondary_chf_instance_id
);
void OpenAPI_charging_information_free(OpenAPI_charging_information_t *charging_information);
OpenAPI_charging_information_t *OpenAPI_charging_information_parseFromJSON(cJSON *charging_informationJSON);
cJSON *OpenAPI_charging_information_convertToJSON(OpenAPI_charging_information_t *charging_information);
OpenAPI_charging_information_t *OpenAPI_charging_information_copy(OpenAPI_charging_information_t *dst, OpenAPI_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_information_H_ */


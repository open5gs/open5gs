/*
 * mm_tel_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_mm_tel_charging_information_H_
#define _OpenAPI_mm_tel_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mm_tel_charging_information_s OpenAPI_mm_tel_charging_information_t;
#include "supplementary_service.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mm_tel_charging_information_s {
    OpenAPI_list_t *supplementary_services;
};

OpenAPI_mm_tel_charging_information_t *OpenAPI_mm_tel_charging_information_create(
    OpenAPI_list_t *supplementary_services
);
void OpenAPI_mm_tel_charging_information_free(OpenAPI_mm_tel_charging_information_t *mm_tel_charging_information);
OpenAPI_mm_tel_charging_information_t *OpenAPI_mm_tel_charging_information_parseFromJSON(cJSON *mm_tel_charging_informationJSON);
cJSON *OpenAPI_mm_tel_charging_information_convertToJSON(OpenAPI_mm_tel_charging_information_t *mm_tel_charging_information);
OpenAPI_mm_tel_charging_information_t *OpenAPI_mm_tel_charging_information_copy(OpenAPI_mm_tel_charging_information_t *dst, OpenAPI_mm_tel_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mm_tel_charging_information_H_ */


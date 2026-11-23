/*
 * nspa_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_nspa_charging_information_H_
#define _OpenAPI_nspa_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nspa_charging_information_s OpenAPI_nspa_charging_information_t;
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nspa_charging_information_s {
    struct OpenAPI_snssai_s *single_nssai;
};

OpenAPI_nspa_charging_information_t *OpenAPI_nspa_charging_information_create(
    OpenAPI_snssai_t *single_nssai
);
void OpenAPI_nspa_charging_information_free(OpenAPI_nspa_charging_information_t *nspa_charging_information);
OpenAPI_nspa_charging_information_t *OpenAPI_nspa_charging_information_parseFromJSON(cJSON *nspa_charging_informationJSON);
cJSON *OpenAPI_nspa_charging_information_convertToJSON(OpenAPI_nspa_charging_information_t *nspa_charging_information);
OpenAPI_nspa_charging_information_t *OpenAPI_nspa_charging_information_copy(OpenAPI_nspa_charging_information_t *dst, OpenAPI_nspa_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nspa_charging_information_H_ */


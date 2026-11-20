/*
 * charging_user_location.h
 *
 * At least one of eutraLocation, nrLocation, n3gaLocation, utraLocation, geraLocation and plmnLocation shall be present. Several of them may be present. 
 */

#ifndef _OpenAPI_charging_user_location_H_
#define _OpenAPI_charging_user_location_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_charging_user_location_s OpenAPI_charging_user_location_t;
#include "charging_n3ga_location.h"
#include "eutra_location.h"
#include "gera_location.h"
#include "nr_location.h"
#include "utra_location.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_charging_user_location_s {
    struct OpenAPI_eutra_location_s *eutra_location;
    struct OpenAPI_nr_location_s *nr_location;
    struct OpenAPI_charging_n3ga_location_s *n3ga_location;
    struct OpenAPI_utra_location_s *utra_location;
    struct OpenAPI_gera_location_s *gera_location;
};

OpenAPI_charging_user_location_t *OpenAPI_charging_user_location_create(
    OpenAPI_eutra_location_t *eutra_location,
    OpenAPI_nr_location_t *nr_location,
    OpenAPI_charging_n3ga_location_t *n3ga_location,
    OpenAPI_utra_location_t *utra_location,
    OpenAPI_gera_location_t *gera_location
);
void OpenAPI_charging_user_location_free(OpenAPI_charging_user_location_t *charging_user_location);
OpenAPI_charging_user_location_t *OpenAPI_charging_user_location_parseFromJSON(cJSON *charging_user_locationJSON);
cJSON *OpenAPI_charging_user_location_convertToJSON(OpenAPI_charging_user_location_t *charging_user_location);
OpenAPI_charging_user_location_t *OpenAPI_charging_user_location_copy(OpenAPI_charging_user_location_t *dst, OpenAPI_charging_user_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_charging_user_location_H_ */


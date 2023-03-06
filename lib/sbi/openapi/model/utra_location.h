/*
 * utra_location.h
 *
 * Exactly one of cgi, sai or lai shall be present.
 */

#ifndef _OpenAPI_utra_location_H_
#define _OpenAPI_utra_location_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "cell_global_id.h"
#include "location_area_id.h"
#include "routing_area_id.h"
#include "service_area_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_utra_location_s OpenAPI_utra_location_t;
typedef struct OpenAPI_utra_location_s {
    struct OpenAPI_cell_global_id_s *cgi;
    struct OpenAPI_service_area_id_s *sai;
    struct OpenAPI_location_area_id_s *lai;
    struct OpenAPI_routing_area_id_s *rai;
    bool is_age_of_location_information;
    int age_of_location_information;
    char *ue_location_timestamp;
    char *geographical_information;
    char *geodetic_information;
} OpenAPI_utra_location_t;

OpenAPI_utra_location_t *OpenAPI_utra_location_create(
    OpenAPI_cell_global_id_t *cgi,
    OpenAPI_service_area_id_t *sai,
    OpenAPI_location_area_id_t *lai,
    OpenAPI_routing_area_id_t *rai,
    bool is_age_of_location_information,
    int age_of_location_information,
    char *ue_location_timestamp,
    char *geographical_information,
    char *geodetic_information
);
void OpenAPI_utra_location_free(OpenAPI_utra_location_t *utra_location);
OpenAPI_utra_location_t *OpenAPI_utra_location_parseFromJSON(cJSON *utra_locationJSON);
cJSON *OpenAPI_utra_location_convertToJSON(OpenAPI_utra_location_t *utra_location);
OpenAPI_utra_location_t *OpenAPI_utra_location_copy(OpenAPI_utra_location_t *dst, OpenAPI_utra_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_utra_location_H_ */


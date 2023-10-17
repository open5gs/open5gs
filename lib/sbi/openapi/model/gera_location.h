/*
 * gera_location.h
 *
 * Exactly one of cgi, sai or lai shall be present.
 */

#ifndef _OpenAPI_gera_location_H_
#define _OpenAPI_gera_location_H_

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

typedef struct OpenAPI_gera_location_s OpenAPI_gera_location_t;
typedef struct OpenAPI_gera_location_s {
    char *location_number;
    struct OpenAPI_cell_global_id_s *cgi;
    struct OpenAPI_routing_area_id_s *rai;
    struct OpenAPI_service_area_id_s *sai;
    struct OpenAPI_location_area_id_s *lai;
    char *vlr_number;
    char *msc_number;
    bool is_age_of_location_information;
    int age_of_location_information;
    char *ue_location_timestamp;
    char *geographical_information;
    char *geodetic_information;
} OpenAPI_gera_location_t;

OpenAPI_gera_location_t *OpenAPI_gera_location_create(
    char *location_number,
    OpenAPI_cell_global_id_t *cgi,
    OpenAPI_routing_area_id_t *rai,
    OpenAPI_service_area_id_t *sai,
    OpenAPI_location_area_id_t *lai,
    char *vlr_number,
    char *msc_number,
    bool is_age_of_location_information,
    int age_of_location_information,
    char *ue_location_timestamp,
    char *geographical_information,
    char *geodetic_information
);
void OpenAPI_gera_location_free(OpenAPI_gera_location_t *gera_location);
OpenAPI_gera_location_t *OpenAPI_gera_location_parseFromJSON(cJSON *gera_locationJSON);
cJSON *OpenAPI_gera_location_convertToJSON(OpenAPI_gera_location_t *gera_location);
OpenAPI_gera_location_t *OpenAPI_gera_location_copy(OpenAPI_gera_location_t *dst, OpenAPI_gera_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gera_location_H_ */


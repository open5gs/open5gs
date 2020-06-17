/*
 * eutra_location.h
 *
 *
 */

#ifndef _OpenAPI_eutra_location_H_
#define _OpenAPI_eutra_location_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ecgi.h"
#include "global_ran_node_id.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_eutra_location_s OpenAPI_eutra_location_t;
typedef struct OpenAPI_eutra_location_s {
    struct OpenAPI_tai_s *tai;
    struct OpenAPI_ecgi_s *ecgi;
    int ignore_ecgi;
    int age_of_location_information;
    char *ue_location_timestamp;
    char *geographical_information;
    char *geodetic_information;
    struct OpenAPI_global_ran_node_id_s *global_ngenb_id;
    struct OpenAPI_global_ran_node_id_s *global_e_nb_id;
} OpenAPI_eutra_location_t;

OpenAPI_eutra_location_t *OpenAPI_eutra_location_create(
    OpenAPI_tai_t *tai,
    OpenAPI_ecgi_t *ecgi,
    int ignore_ecgi,
    int age_of_location_information,
    char *ue_location_timestamp,
    char *geographical_information,
    char *geodetic_information,
    OpenAPI_global_ran_node_id_t *global_ngenb_id,
    OpenAPI_global_ran_node_id_t *global_e_nb_id
    );
void OpenAPI_eutra_location_free(OpenAPI_eutra_location_t *eutra_location);
OpenAPI_eutra_location_t *OpenAPI_eutra_location_parseFromJSON(cJSON *eutra_locationJSON);
cJSON *OpenAPI_eutra_location_convertToJSON(OpenAPI_eutra_location_t *eutra_location);
OpenAPI_eutra_location_t *OpenAPI_eutra_location_copy(OpenAPI_eutra_location_t *dst, OpenAPI_eutra_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eutra_location_H_ */


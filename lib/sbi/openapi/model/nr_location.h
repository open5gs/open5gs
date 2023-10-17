/*
 * nr_location.h
 *
 * Contains the NR user location.
 */

#ifndef _OpenAPI_nr_location_H_
#define _OpenAPI_nr_location_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "global_ran_node_id.h"
#include "ncgi.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nr_location_s OpenAPI_nr_location_t;
typedef struct OpenAPI_nr_location_s {
    struct OpenAPI_tai_s *tai;
    struct OpenAPI_ncgi_s *ncgi;
    bool is_ignore_ncgi;
    int ignore_ncgi;
    bool is_age_of_location_information;
    int age_of_location_information;
    char *ue_location_timestamp;
    char *geographical_information;
    char *geodetic_information;
    struct OpenAPI_global_ran_node_id_s *global_gnb_id;
} OpenAPI_nr_location_t;

OpenAPI_nr_location_t *OpenAPI_nr_location_create(
    OpenAPI_tai_t *tai,
    OpenAPI_ncgi_t *ncgi,
    bool is_ignore_ncgi,
    int ignore_ncgi,
    bool is_age_of_location_information,
    int age_of_location_information,
    char *ue_location_timestamp,
    char *geographical_information,
    char *geodetic_information,
    OpenAPI_global_ran_node_id_t *global_gnb_id
);
void OpenAPI_nr_location_free(OpenAPI_nr_location_t *nr_location);
OpenAPI_nr_location_t *OpenAPI_nr_location_parseFromJSON(cJSON *nr_locationJSON);
cJSON *OpenAPI_nr_location_convertToJSON(OpenAPI_nr_location_t *nr_location);
OpenAPI_nr_location_t *OpenAPI_nr_location_copy(OpenAPI_nr_location_t *dst, OpenAPI_nr_location_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nr_location_H_ */


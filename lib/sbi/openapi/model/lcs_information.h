/*
 * lcs_information.h
 *
 * 
 */

#ifndef _OpenAPI_lcs_information_H_
#define _OpenAPI_lcs_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_lcs_information_s OpenAPI_lcs_information_t;
#include "location_estimate.h"
#include "location_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_lcs_information_s {
    char *l_cs_client_id;
    OpenAPI_location_type_e location_type;
    struct OpenAPI_location_estimate_s *location_estimate;
    char *positioning_data;
    char *target_ueid;
};

OpenAPI_lcs_information_t *OpenAPI_lcs_information_create(
    char *l_cs_client_id,
    OpenAPI_location_type_e location_type,
    OpenAPI_location_estimate_t *location_estimate,
    char *positioning_data,
    char *target_ueid
);
void OpenAPI_lcs_information_free(OpenAPI_lcs_information_t *lcs_information);
OpenAPI_lcs_information_t *OpenAPI_lcs_information_parseFromJSON(cJSON *lcs_informationJSON);
cJSON *OpenAPI_lcs_information_convertToJSON(OpenAPI_lcs_information_t *lcs_information);
OpenAPI_lcs_information_t *OpenAPI_lcs_information_copy(OpenAPI_lcs_information_t *dst, OpenAPI_lcs_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_information_H_ */


/*
 * lpi_1.h
 *
 * 
 */

#ifndef _OpenAPI_lpi_1_H_
#define _OpenAPI_lpi_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "location_privacy_ind.h"
#include "valid_time_period_1.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lpi_1_s OpenAPI_lpi_1_t;
typedef struct OpenAPI_lpi_1_s {
    OpenAPI_location_privacy_ind_e location_privacy_ind;
    struct OpenAPI_valid_time_period_1_s *valid_time_period;
} OpenAPI_lpi_1_t;

OpenAPI_lpi_1_t *OpenAPI_lpi_1_create(
    OpenAPI_location_privacy_ind_e location_privacy_ind,
    OpenAPI_valid_time_period_1_t *valid_time_period
);
void OpenAPI_lpi_1_free(OpenAPI_lpi_1_t *lpi_1);
OpenAPI_lpi_1_t *OpenAPI_lpi_1_parseFromJSON(cJSON *lpi_1JSON);
cJSON *OpenAPI_lpi_1_convertToJSON(OpenAPI_lpi_1_t *lpi_1);
OpenAPI_lpi_1_t *OpenAPI_lpi_1_copy(OpenAPI_lpi_1_t *dst, OpenAPI_lpi_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lpi_1_H_ */


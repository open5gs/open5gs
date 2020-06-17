/*
 * lpi.h
 *
 *
 */

#ifndef _OpenAPI_lpi_H_
#define _OpenAPI_lpi_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "location_privacy_ind.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_lpi_s OpenAPI_lpi_t;
typedef struct OpenAPI_lpi_s {
    OpenAPI_location_privacy_ind_e location_privacy_ind;
    struct OpenAPI_valid_time_period_s *valid_time_period;
} OpenAPI_lpi_t;

OpenAPI_lpi_t *OpenAPI_lpi_create(
    OpenAPI_location_privacy_ind_e location_privacy_ind,
    OpenAPI_valid_time_period_t *valid_time_period
    );
void OpenAPI_lpi_free(OpenAPI_lpi_t *lpi);
OpenAPI_lpi_t *OpenAPI_lpi_parseFromJSON(cJSON *lpiJSON);
cJSON *OpenAPI_lpi_convertToJSON(OpenAPI_lpi_t *lpi);
OpenAPI_lpi_t *OpenAPI_lpi_copy(OpenAPI_lpi_t *dst, OpenAPI_lpi_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lpi_H_ */


/*
 * odb_exempted_conditions_1.h
 *
 * Conditions for the DNN being exempted from ODB
 */

#ifndef _OpenAPI_odb_exempted_conditions_1_H_
#define _OpenAPI_odb_exempted_conditions_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_odb_exempted_conditions_1_s OpenAPI_odb_exempted_conditions_1_t;
#include "spatial_validity_cond_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_odb_exempted_conditions_1_s {
    struct OpenAPI_spatial_validity_cond_1_s *spatial_validity_cond;
    char *validity_time;
};

OpenAPI_odb_exempted_conditions_1_t *OpenAPI_odb_exempted_conditions_1_create(
    OpenAPI_spatial_validity_cond_1_t *spatial_validity_cond,
    char *validity_time
);
void OpenAPI_odb_exempted_conditions_1_free(OpenAPI_odb_exempted_conditions_1_t *odb_exempted_conditions_1);
OpenAPI_odb_exempted_conditions_1_t *OpenAPI_odb_exempted_conditions_1_parseFromJSON(cJSON *odb_exempted_conditions_1JSON);
cJSON *OpenAPI_odb_exempted_conditions_1_convertToJSON(OpenAPI_odb_exempted_conditions_1_t *odb_exempted_conditions_1);
OpenAPI_odb_exempted_conditions_1_t *OpenAPI_odb_exempted_conditions_1_copy(OpenAPI_odb_exempted_conditions_1_t *dst, OpenAPI_odb_exempted_conditions_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_odb_exempted_conditions_1_H_ */


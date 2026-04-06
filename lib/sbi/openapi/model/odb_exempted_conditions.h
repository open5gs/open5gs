/*
 * odb_exempted_conditions.h
 *
 * Conditions for the DNN being exempted from ODB
 */

#ifndef _OpenAPI_odb_exempted_conditions_H_
#define _OpenAPI_odb_exempted_conditions_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_odb_exempted_conditions_s OpenAPI_odb_exempted_conditions_t;
#include "spatial_validity_cond.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_odb_exempted_conditions_s {
    struct OpenAPI_spatial_validity_cond_s *spatial_validity_cond;
    char *validity_time;
};

OpenAPI_odb_exempted_conditions_t *OpenAPI_odb_exempted_conditions_create(
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond,
    char *validity_time
);
void OpenAPI_odb_exempted_conditions_free(OpenAPI_odb_exempted_conditions_t *odb_exempted_conditions);
OpenAPI_odb_exempted_conditions_t *OpenAPI_odb_exempted_conditions_parseFromJSON(cJSON *odb_exempted_conditionsJSON);
cJSON *OpenAPI_odb_exempted_conditions_convertToJSON(OpenAPI_odb_exempted_conditions_t *odb_exempted_conditions);
OpenAPI_odb_exempted_conditions_t *OpenAPI_odb_exempted_conditions_copy(OpenAPI_odb_exempted_conditions_t *dst, OpenAPI_odb_exempted_conditions_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_odb_exempted_conditions_H_ */


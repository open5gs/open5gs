/*
 * relative_velocity_with_uncertainty.h
 *
 * Relative velocity with radial velocity and transverse velocity.
 */

#ifndef _OpenAPI_relative_velocity_with_uncertainty_H_
#define _OpenAPI_relative_velocity_with_uncertainty_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_relative_velocity_with_uncertainty_s OpenAPI_relative_velocity_with_uncertainty_t;
#include "angular_velocity.h"
#include "radial_velocity.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_relative_velocity_with_uncertainty_s {
    struct OpenAPI_radial_velocity_s *r_velocity;
    struct OpenAPI_angular_velocity_s *a_transverse_velocity;
    struct OpenAPI_angular_velocity_s *e_transverse_velocity;
};

OpenAPI_relative_velocity_with_uncertainty_t *OpenAPI_relative_velocity_with_uncertainty_create(
    OpenAPI_radial_velocity_t *r_velocity,
    OpenAPI_angular_velocity_t *a_transverse_velocity,
    OpenAPI_angular_velocity_t *e_transverse_velocity
);
void OpenAPI_relative_velocity_with_uncertainty_free(OpenAPI_relative_velocity_with_uncertainty_t *relative_velocity_with_uncertainty);
OpenAPI_relative_velocity_with_uncertainty_t *OpenAPI_relative_velocity_with_uncertainty_parseFromJSON(cJSON *relative_velocity_with_uncertaintyJSON);
cJSON *OpenAPI_relative_velocity_with_uncertainty_convertToJSON(OpenAPI_relative_velocity_with_uncertainty_t *relative_velocity_with_uncertainty);
OpenAPI_relative_velocity_with_uncertainty_t *OpenAPI_relative_velocity_with_uncertainty_copy(OpenAPI_relative_velocity_with_uncertainty_t *dst, OpenAPI_relative_velocity_with_uncertainty_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_relative_velocity_with_uncertainty_H_ */


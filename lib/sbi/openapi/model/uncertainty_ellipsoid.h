/*
 * uncertainty_ellipsoid.h
 *
 * Ellipsoid with uncertainty
 */

#ifndef _OpenAPI_uncertainty_ellipsoid_H_
#define _OpenAPI_uncertainty_ellipsoid_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_uncertainty_ellipsoid_s OpenAPI_uncertainty_ellipsoid_t;
typedef struct OpenAPI_uncertainty_ellipsoid_s {
    float semi_major;
    float semi_minor;
    float vertical;
    int orientation_major;
} OpenAPI_uncertainty_ellipsoid_t;

OpenAPI_uncertainty_ellipsoid_t *OpenAPI_uncertainty_ellipsoid_create(
    float semi_major,
    float semi_minor,
    float vertical,
    int orientation_major
);
void OpenAPI_uncertainty_ellipsoid_free(OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid);
OpenAPI_uncertainty_ellipsoid_t *OpenAPI_uncertainty_ellipsoid_parseFromJSON(cJSON *uncertainty_ellipsoidJSON);
cJSON *OpenAPI_uncertainty_ellipsoid_convertToJSON(OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid);
OpenAPI_uncertainty_ellipsoid_t *OpenAPI_uncertainty_ellipsoid_copy(OpenAPI_uncertainty_ellipsoid_t *dst, OpenAPI_uncertainty_ellipsoid_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uncertainty_ellipsoid_H_ */


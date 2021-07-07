/*
 * uncertainty_ellipse.h
 *
 * 
 */

#ifndef _OpenAPI_uncertainty_ellipse_H_
#define _OpenAPI_uncertainty_ellipse_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_uncertainty_ellipse_s OpenAPI_uncertainty_ellipse_t;
typedef struct OpenAPI_uncertainty_ellipse_s {
    float semi_major;
    float semi_minor;
    int orientation_major;
} OpenAPI_uncertainty_ellipse_t;

OpenAPI_uncertainty_ellipse_t *OpenAPI_uncertainty_ellipse_create(
    float semi_major,
    float semi_minor,
    int orientation_major
);
void OpenAPI_uncertainty_ellipse_free(OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse);
OpenAPI_uncertainty_ellipse_t *OpenAPI_uncertainty_ellipse_parseFromJSON(cJSON *uncertainty_ellipseJSON);
cJSON *OpenAPI_uncertainty_ellipse_convertToJSON(OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse);
OpenAPI_uncertainty_ellipse_t *OpenAPI_uncertainty_ellipse_copy(OpenAPI_uncertainty_ellipse_t *dst, OpenAPI_uncertainty_ellipse_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uncertainty_ellipse_H_ */


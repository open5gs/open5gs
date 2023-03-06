/*
 * dispersion_area.h
 *
 * Dispersion Area
 */

#ifndef _OpenAPI_dispersion_area_H_
#define _OpenAPI_dispersion_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ecgi.h"
#include "ncgi.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dispersion_area_s OpenAPI_dispersion_area_t;
typedef struct OpenAPI_dispersion_area_s {
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *ncgi_list;
    OpenAPI_list_t *ecgi_list;
    bool is_n3ga_ind;
    int n3ga_ind;
} OpenAPI_dispersion_area_t;

OpenAPI_dispersion_area_t *OpenAPI_dispersion_area_create(
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *ncgi_list,
    OpenAPI_list_t *ecgi_list,
    bool is_n3ga_ind,
    int n3ga_ind
);
void OpenAPI_dispersion_area_free(OpenAPI_dispersion_area_t *dispersion_area);
OpenAPI_dispersion_area_t *OpenAPI_dispersion_area_parseFromJSON(cJSON *dispersion_areaJSON);
cJSON *OpenAPI_dispersion_area_convertToJSON(OpenAPI_dispersion_area_t *dispersion_area);
OpenAPI_dispersion_area_t *OpenAPI_dispersion_area_copy(OpenAPI_dispersion_area_t *dst, OpenAPI_dispersion_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_area_H_ */


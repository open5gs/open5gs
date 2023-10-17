/*
 * circumstance_description.h
 *
 * Contains the description of a circumstance.
 */

#ifndef _OpenAPI_circumstance_description_H_
#define _OpenAPI_circumstance_description_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "network_area_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_circumstance_description_s OpenAPI_circumstance_description_t;
typedef struct OpenAPI_circumstance_description_s {
    bool is_freq;
    float freq;
    char *tm;
    struct OpenAPI_network_area_info_s *loc_area;
    bool is_vol;
    long vol;
} OpenAPI_circumstance_description_t;

OpenAPI_circumstance_description_t *OpenAPI_circumstance_description_create(
    bool is_freq,
    float freq,
    char *tm,
    OpenAPI_network_area_info_t *loc_area,
    bool is_vol,
    long vol
);
void OpenAPI_circumstance_description_free(OpenAPI_circumstance_description_t *circumstance_description);
OpenAPI_circumstance_description_t *OpenAPI_circumstance_description_parseFromJSON(cJSON *circumstance_descriptionJSON);
cJSON *OpenAPI_circumstance_description_convertToJSON(OpenAPI_circumstance_description_t *circumstance_description);
OpenAPI_circumstance_description_t *OpenAPI_circumstance_description_copy(OpenAPI_circumstance_description_t *dst, OpenAPI_circumstance_description_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_circumstance_description_H_ */


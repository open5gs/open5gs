/*
 * ue_area_indication.h
 *
 * Indicates area (country, area in a country or international area) where UE is located
 */

#ifndef _OpenAPI_ue_area_indication_H_
#define _OpenAPI_ue_area_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_area_indication_s OpenAPI_ue_area_indication_t;
typedef struct OpenAPI_ue_area_indication_s {
    char *country;
    bool is_international_area_ind;
    int international_area_ind;
} OpenAPI_ue_area_indication_t;

OpenAPI_ue_area_indication_t *OpenAPI_ue_area_indication_create(
    char *country,
    bool is_international_area_ind,
    int international_area_ind
);
void OpenAPI_ue_area_indication_free(OpenAPI_ue_area_indication_t *ue_area_indication);
OpenAPI_ue_area_indication_t *OpenAPI_ue_area_indication_parseFromJSON(cJSON *ue_area_indicationJSON);
cJSON *OpenAPI_ue_area_indication_convertToJSON(OpenAPI_ue_area_indication_t *ue_area_indication);
OpenAPI_ue_area_indication_t *OpenAPI_ue_area_indication_copy(OpenAPI_ue_area_indication_t *dst, OpenAPI_ue_area_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_area_indication_H_ */


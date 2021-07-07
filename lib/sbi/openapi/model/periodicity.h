/*
 * periodicity.h
 *
 * 
 */

#ifndef _OpenAPI_periodicity_H_
#define _OpenAPI_periodicity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "periodicity_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_periodicity_s OpenAPI_periodicity_t;
typedef struct OpenAPI_periodicity_s {
} OpenAPI_periodicity_t;

OpenAPI_periodicity_t *OpenAPI_periodicity_create(
);
void OpenAPI_periodicity_free(OpenAPI_periodicity_t *periodicity);
OpenAPI_periodicity_t *OpenAPI_periodicity_parseFromJSON(cJSON *periodicityJSON);
cJSON *OpenAPI_periodicity_convertToJSON(OpenAPI_periodicity_t *periodicity);
OpenAPI_periodicity_t *OpenAPI_periodicity_copy(OpenAPI_periodicity_t *dst, OpenAPI_periodicity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_periodicity_H_ */


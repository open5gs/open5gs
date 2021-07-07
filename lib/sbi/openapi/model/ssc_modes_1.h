/*
 * ssc_modes_1.h
 *
 * 
 */

#ifndef _OpenAPI_ssc_modes_1_H_
#define _OpenAPI_ssc_modes_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ssc_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ssc_modes_1_s OpenAPI_ssc_modes_1_t;
typedef struct OpenAPI_ssc_modes_1_s {
    OpenAPI_ssc_mode_e default_ssc_mode;
    OpenAPI_list_t *allowed_ssc_modes;
} OpenAPI_ssc_modes_1_t;

OpenAPI_ssc_modes_1_t *OpenAPI_ssc_modes_1_create(
    OpenAPI_ssc_mode_e default_ssc_mode,
    OpenAPI_list_t *allowed_ssc_modes
);
void OpenAPI_ssc_modes_1_free(OpenAPI_ssc_modes_1_t *ssc_modes_1);
OpenAPI_ssc_modes_1_t *OpenAPI_ssc_modes_1_parseFromJSON(cJSON *ssc_modes_1JSON);
cJSON *OpenAPI_ssc_modes_1_convertToJSON(OpenAPI_ssc_modes_1_t *ssc_modes_1);
OpenAPI_ssc_modes_1_t *OpenAPI_ssc_modes_1_copy(OpenAPI_ssc_modes_1_t *dst, OpenAPI_ssc_modes_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ssc_modes_1_H_ */


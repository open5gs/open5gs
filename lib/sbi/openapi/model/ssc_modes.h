/*
 * ssc_modes.h
 *
 *
 */

#ifndef _OpenAPI_ssc_modes_H_
#define _OpenAPI_ssc_modes_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ssc_mode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ssc_modes_s OpenAPI_ssc_modes_t;
typedef struct OpenAPI_ssc_modes_s {
    OpenAPI_ssc_mode_e default_ssc_mode;
    OpenAPI_list_t *allowed_ssc_modes;
} OpenAPI_ssc_modes_t;

OpenAPI_ssc_modes_t *OpenAPI_ssc_modes_create(
    OpenAPI_ssc_mode_e default_ssc_mode,
    OpenAPI_list_t *allowed_ssc_modes
    );
void OpenAPI_ssc_modes_free(OpenAPI_ssc_modes_t *ssc_modes);
OpenAPI_ssc_modes_t *OpenAPI_ssc_modes_parseFromJSON(cJSON *ssc_modesJSON);
cJSON *OpenAPI_ssc_modes_convertToJSON(OpenAPI_ssc_modes_t *ssc_modes);
OpenAPI_ssc_modes_t *OpenAPI_ssc_modes_copy(OpenAPI_ssc_modes_t *dst, OpenAPI_ssc_modes_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ssc_modes_H_ */


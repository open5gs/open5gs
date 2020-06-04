/*
 * ssc_mode.h
 *
 *
 */

#ifndef _OpenAPI_ssc_mode_H_
#define _OpenAPI_ssc_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ssc_mode_s OpenAPI_ssc_mode_t;
typedef struct OpenAPI_ssc_mode_s {
} OpenAPI_ssc_mode_t;

OpenAPI_ssc_mode_t *OpenAPI_ssc_mode_create(
    );
void OpenAPI_ssc_mode_free(OpenAPI_ssc_mode_t *ssc_mode);
OpenAPI_ssc_mode_t *OpenAPI_ssc_mode_parseFromJSON(cJSON *ssc_modeJSON);
cJSON *OpenAPI_ssc_mode_convertToJSON(OpenAPI_ssc_mode_t *ssc_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ssc_mode_H_ */


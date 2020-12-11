/*
 * reporting_frequency.h
 *
 *
 */

#ifndef _OpenAPI_reporting_frequency_H_
#define _OpenAPI_reporting_frequency_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reporting_frequency_s OpenAPI_reporting_frequency_t;
typedef struct OpenAPI_reporting_frequency_s {
} OpenAPI_reporting_frequency_t;

OpenAPI_reporting_frequency_t *OpenAPI_reporting_frequency_create(
    );
void OpenAPI_reporting_frequency_free(OpenAPI_reporting_frequency_t *reporting_frequency);
OpenAPI_reporting_frequency_t *OpenAPI_reporting_frequency_parseFromJSON(cJSON *reporting_frequencyJSON);
cJSON *OpenAPI_reporting_frequency_convertToJSON(OpenAPI_reporting_frequency_t *reporting_frequency);
OpenAPI_reporting_frequency_t *OpenAPI_reporting_frequency_copy(OpenAPI_reporting_frequency_t *dst, OpenAPI_reporting_frequency_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_frequency_H_ */


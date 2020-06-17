/*
 * service_id_to_frequency.h
 *
 *
 */

#ifndef _OpenAPI_service_id_to_frequency_H_
#define _OpenAPI_service_id_to_frequency_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_service_id_to_frequency_s OpenAPI_service_id_to_frequency_t;
typedef struct OpenAPI_service_id_to_frequency_s {
    OpenAPI_list_t *ser_ids;
    double frequency;
    char *geographical_area;
} OpenAPI_service_id_to_frequency_t;

OpenAPI_service_id_to_frequency_t *OpenAPI_service_id_to_frequency_create(
    OpenAPI_list_t *ser_ids,
    double frequency,
    char *geographical_area
    );
void OpenAPI_service_id_to_frequency_free(OpenAPI_service_id_to_frequency_t *service_id_to_frequency);
OpenAPI_service_id_to_frequency_t *OpenAPI_service_id_to_frequency_parseFromJSON(cJSON *service_id_to_frequencyJSON);
cJSON *OpenAPI_service_id_to_frequency_convertToJSON(OpenAPI_service_id_to_frequency_t *service_id_to_frequency);
OpenAPI_service_id_to_frequency_t *OpenAPI_service_id_to_frequency_copy(OpenAPI_service_id_to_frequency_t *dst, OpenAPI_service_id_to_frequency_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_id_to_frequency_H_ */


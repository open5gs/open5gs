/*
 * dispersion_info.h
 *
 * Represents the Dispersion information. When subscribed event is \&quot;DISPERSION\&quot;, the  \&quot;disperInfos\&quot; attribute shall be included. 
 */

#ifndef _OpenAPI_dispersion_info_H_
#define _OpenAPI_dispersion_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dispersion_collection.h"
#include "dispersion_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dispersion_info_s OpenAPI_dispersion_info_t;
typedef struct OpenAPI_dispersion_info_s {
    char *ts_start;
    int ts_duration;
    OpenAPI_list_t *disper_collects;
    struct OpenAPI_dispersion_type_s *disper_type;
} OpenAPI_dispersion_info_t;

OpenAPI_dispersion_info_t *OpenAPI_dispersion_info_create(
    char *ts_start,
    int ts_duration,
    OpenAPI_list_t *disper_collects,
    OpenAPI_dispersion_type_t *disper_type
);
void OpenAPI_dispersion_info_free(OpenAPI_dispersion_info_t *dispersion_info);
OpenAPI_dispersion_info_t *OpenAPI_dispersion_info_parseFromJSON(cJSON *dispersion_infoJSON);
cJSON *OpenAPI_dispersion_info_convertToJSON(OpenAPI_dispersion_info_t *dispersion_info);
OpenAPI_dispersion_info_t *OpenAPI_dispersion_info_copy(OpenAPI_dispersion_info_t *dst, OpenAPI_dispersion_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dispersion_info_H_ */


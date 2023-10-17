/*
 * analytics_metadata.h
 *
 * Possible values are: - NUM_OF_SAMPLES: Number of data samples used for the generation of the output analytics. - DATA_WINDOW: Data time window of the data samples. - DATA_STAT_PROPS: Dataset statistical properties of the data used to generate the analytics. - STRATEGY: Output strategy used for the reporting of the analytics. - ACCURACY: Level of accuracy reached for the analytics. 
 */

#ifndef _OpenAPI_analytics_metadata_H_
#define _OpenAPI_analytics_metadata_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "analytics_metadata_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_analytics_metadata_s OpenAPI_analytics_metadata_t;
typedef struct OpenAPI_analytics_metadata_s {
} OpenAPI_analytics_metadata_t;

OpenAPI_analytics_metadata_t *OpenAPI_analytics_metadata_create(
);
void OpenAPI_analytics_metadata_free(OpenAPI_analytics_metadata_t *analytics_metadata);
OpenAPI_analytics_metadata_t *OpenAPI_analytics_metadata_parseFromJSON(cJSON *analytics_metadataJSON);
cJSON *OpenAPI_analytics_metadata_convertToJSON(OpenAPI_analytics_metadata_t *analytics_metadata);
OpenAPI_analytics_metadata_t *OpenAPI_analytics_metadata_copy(OpenAPI_analytics_metadata_t *dst, OpenAPI_analytics_metadata_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_analytics_metadata_H_ */


/*
 * odb_data.h
 *
 * Contains information regarding operater  determined  barring.
 */

#ifndef _OpenAPI_odb_data_H_
#define _OpenAPI_odb_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "roaming_odb.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_odb_data_s OpenAPI_odb_data_t;
typedef struct OpenAPI_odb_data_s {
    OpenAPI_roaming_odb_e roaming_odb;
} OpenAPI_odb_data_t;

OpenAPI_odb_data_t *OpenAPI_odb_data_create(
    OpenAPI_roaming_odb_e roaming_odb
);
void OpenAPI_odb_data_free(OpenAPI_odb_data_t *odb_data);
OpenAPI_odb_data_t *OpenAPI_odb_data_parseFromJSON(cJSON *odb_dataJSON);
cJSON *OpenAPI_odb_data_convertToJSON(OpenAPI_odb_data_t *odb_data);
OpenAPI_odb_data_t *OpenAPI_odb_data_copy(OpenAPI_odb_data_t *dst, OpenAPI_odb_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_odb_data_H_ */


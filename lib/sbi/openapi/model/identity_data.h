/*
 * identity_data.h
 *
 *
 */

#ifndef _OpenAPI_identity_data_H_
#define _OpenAPI_identity_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_identity_data_s OpenAPI_identity_data_t;
typedef struct OpenAPI_identity_data_s {
    OpenAPI_list_t *supi_list;
    OpenAPI_list_t *gpsi_list;
} OpenAPI_identity_data_t;

OpenAPI_identity_data_t *OpenAPI_identity_data_create(
    OpenAPI_list_t *supi_list,
    OpenAPI_list_t *gpsi_list
    );
void OpenAPI_identity_data_free(OpenAPI_identity_data_t *identity_data);
OpenAPI_identity_data_t *OpenAPI_identity_data_parseFromJSON(cJSON *identity_dataJSON);
cJSON *OpenAPI_identity_data_convertToJSON(OpenAPI_identity_data_t *identity_data);
OpenAPI_identity_data_t *OpenAPI_identity_data_copy(OpenAPI_identity_data_t *dst, OpenAPI_identity_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_identity_data_H_ */


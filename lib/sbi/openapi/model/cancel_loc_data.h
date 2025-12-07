/*
 * cancel_loc_data.h
 *
 * Information within Cancel Location Request.
 */

#ifndef _OpenAPI_cancel_loc_data_H_
#define _OpenAPI_cancel_loc_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_cancel_loc_data_s OpenAPI_cancel_loc_data_t;
typedef struct OpenAPI_cancel_loc_data_s {
    char *hgmlc_call_back_uri;
    char *ldr_reference;
    char *supported_features;
} OpenAPI_cancel_loc_data_t;

OpenAPI_cancel_loc_data_t *OpenAPI_cancel_loc_data_create(
    char *hgmlc_call_back_uri,
    char *ldr_reference,
    char *supported_features
);
void OpenAPI_cancel_loc_data_free(OpenAPI_cancel_loc_data_t *cancel_loc_data);
OpenAPI_cancel_loc_data_t *OpenAPI_cancel_loc_data_parseFromJSON(cJSON *cancel_loc_dataJSON);
cJSON *OpenAPI_cancel_loc_data_convertToJSON(OpenAPI_cancel_loc_data_t *cancel_loc_data);
OpenAPI_cancel_loc_data_t *OpenAPI_cancel_loc_data_copy(OpenAPI_cancel_loc_data_t *dst, OpenAPI_cancel_loc_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cancel_loc_data_H_ */


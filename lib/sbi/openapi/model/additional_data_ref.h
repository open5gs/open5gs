/*
 * additional_data_ref.h
 *
 * References to additional data requested to be sent in notifications
 */

#ifndef _OpenAPI_additional_data_ref_H_
#define _OpenAPI_additional_data_ref_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_additional_data_ref_s OpenAPI_additional_data_ref_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_additional_data_ref_s {
    OpenAPI_list_t *monitored_resource_uris;
    OpenAPI_list_t *additional_data_uris;
};

OpenAPI_additional_data_ref_t *OpenAPI_additional_data_ref_create(
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_list_t *additional_data_uris
);
void OpenAPI_additional_data_ref_free(OpenAPI_additional_data_ref_t *additional_data_ref);
OpenAPI_additional_data_ref_t *OpenAPI_additional_data_ref_parseFromJSON(cJSON *additional_data_refJSON);
cJSON *OpenAPI_additional_data_ref_convertToJSON(OpenAPI_additional_data_ref_t *additional_data_ref);
OpenAPI_additional_data_ref_t *OpenAPI_additional_data_ref_copy(OpenAPI_additional_data_ref_t *dst, OpenAPI_additional_data_ref_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_additional_data_ref_H_ */


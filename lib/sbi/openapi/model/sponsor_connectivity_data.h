/*
 * sponsor_connectivity_data.h
 *
 * Contains the sponsored data connectivity related information for a sponsor identifier. 
 */

#ifndef _OpenAPI_sponsor_connectivity_data_H_
#define _OpenAPI_sponsor_connectivity_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sponsor_connectivity_data_s OpenAPI_sponsor_connectivity_data_t;
typedef struct OpenAPI_sponsor_connectivity_data_s {
    OpenAPI_list_t *asp_ids;
} OpenAPI_sponsor_connectivity_data_t;

OpenAPI_sponsor_connectivity_data_t *OpenAPI_sponsor_connectivity_data_create(
    OpenAPI_list_t *asp_ids
);
void OpenAPI_sponsor_connectivity_data_free(OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data);
OpenAPI_sponsor_connectivity_data_t *OpenAPI_sponsor_connectivity_data_parseFromJSON(cJSON *sponsor_connectivity_dataJSON);
cJSON *OpenAPI_sponsor_connectivity_data_convertToJSON(OpenAPI_sponsor_connectivity_data_t *sponsor_connectivity_data);
OpenAPI_sponsor_connectivity_data_t *OpenAPI_sponsor_connectivity_data_copy(OpenAPI_sponsor_connectivity_data_t *dst, OpenAPI_sponsor_connectivity_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sponsor_connectivity_data_H_ */


/*
 * data_ind.h
 *
 * Possible values are - PFD - IPTV - BDT - SVC_PARAM - AM 
 */

#ifndef _OpenAPI_data_ind_H_
#define _OpenAPI_data_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "data_ind_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_data_ind_s OpenAPI_data_ind_t;
typedef struct OpenAPI_data_ind_s {
} OpenAPI_data_ind_t;

OpenAPI_data_ind_t *OpenAPI_data_ind_create(
);
void OpenAPI_data_ind_free(OpenAPI_data_ind_t *data_ind);
OpenAPI_data_ind_t *OpenAPI_data_ind_parseFromJSON(cJSON *data_indJSON);
cJSON *OpenAPI_data_ind_convertToJSON(OpenAPI_data_ind_t *data_ind);
OpenAPI_data_ind_t *OpenAPI_data_ind_copy(OpenAPI_data_ind_t *dst, OpenAPI_data_ind_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_ind_H_ */


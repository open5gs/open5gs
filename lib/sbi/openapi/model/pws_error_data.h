/*
 * pws_error_data.h
 *
 * Data related to PWS error included in a N2 Information Transfer failure response
 */

#ifndef _OpenAPI_pws_error_data_H_
#define _OpenAPI_pws_error_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pws_error_data_s OpenAPI_pws_error_data_t;
typedef struct OpenAPI_pws_error_data_s {
    int namf_cause;
} OpenAPI_pws_error_data_t;

OpenAPI_pws_error_data_t *OpenAPI_pws_error_data_create(
    int namf_cause
);
void OpenAPI_pws_error_data_free(OpenAPI_pws_error_data_t *pws_error_data);
OpenAPI_pws_error_data_t *OpenAPI_pws_error_data_parseFromJSON(cJSON *pws_error_dataJSON);
cJSON *OpenAPI_pws_error_data_convertToJSON(OpenAPI_pws_error_data_t *pws_error_data);
OpenAPI_pws_error_data_t *OpenAPI_pws_error_data_copy(OpenAPI_pws_error_data_t *dst, OpenAPI_pws_error_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pws_error_data_H_ */


/*
 * mo_exception_data_flag.h
 *
 * Possible values are - \&quot;START\&quot;: Indicates the start of MO Exception Data delivery. - \&quot;STOP\&quot;: Indicates the stop of MO Exception Data delivery.
 */

#ifndef _OpenAPI_mo_exception_data_flag_H_
#define _OpenAPI_mo_exception_data_flag_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mo_exception_data_flag_s OpenAPI_mo_exception_data_flag_t;
typedef struct OpenAPI_mo_exception_data_flag_s {
} OpenAPI_mo_exception_data_flag_t;

OpenAPI_mo_exception_data_flag_t *OpenAPI_mo_exception_data_flag_create(
    );
void OpenAPI_mo_exception_data_flag_free(OpenAPI_mo_exception_data_flag_t *mo_exception_data_flag);
OpenAPI_mo_exception_data_flag_t *OpenAPI_mo_exception_data_flag_parseFromJSON(cJSON *mo_exception_data_flagJSON);
cJSON *OpenAPI_mo_exception_data_flag_convertToJSON(OpenAPI_mo_exception_data_flag_t *mo_exception_data_flag);
OpenAPI_mo_exception_data_flag_t *OpenAPI_mo_exception_data_flag_copy(OpenAPI_mo_exception_data_flag_t *dst, OpenAPI_mo_exception_data_flag_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mo_exception_data_flag_H_ */


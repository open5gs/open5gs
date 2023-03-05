/*
 * failed_modification_info.h
 *
 * Information on N32-f modifications block that failed to process
 */

#ifndef _OpenAPI_failed_modification_info_H_
#define _OpenAPI_failed_modification_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n32f_error_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_failed_modification_info_s OpenAPI_failed_modification_info_t;
typedef struct OpenAPI_failed_modification_info_s {
    char *ipx_id;
    OpenAPI_n32f_error_type_e n32f_error_type;
} OpenAPI_failed_modification_info_t;

OpenAPI_failed_modification_info_t *OpenAPI_failed_modification_info_create(
    char *ipx_id,
    OpenAPI_n32f_error_type_e n32f_error_type
);
void OpenAPI_failed_modification_info_free(OpenAPI_failed_modification_info_t *failed_modification_info);
OpenAPI_failed_modification_info_t *OpenAPI_failed_modification_info_parseFromJSON(cJSON *failed_modification_infoJSON);
cJSON *OpenAPI_failed_modification_info_convertToJSON(OpenAPI_failed_modification_info_t *failed_modification_info);
OpenAPI_failed_modification_info_t *OpenAPI_failed_modification_info_copy(OpenAPI_failed_modification_info_t *dst, OpenAPI_failed_modification_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_failed_modification_info_H_ */


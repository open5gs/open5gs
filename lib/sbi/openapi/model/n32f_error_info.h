/*
 * n32f_error_info.h
 *
 * N32-f error information
 */

#ifndef _OpenAPI_n32f_error_info_H_
#define _OpenAPI_n32f_error_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "failed_modification_info.h"
#include "n32f_error_detail.h"
#include "n32f_error_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n32f_error_info_s OpenAPI_n32f_error_info_t;
typedef struct OpenAPI_n32f_error_info_s {
    char *n32f_message_id;
    OpenAPI_n32f_error_type_e n32f_error_type;
    char *n32f_context_id;
    OpenAPI_list_t *failed_modification_list;
    OpenAPI_list_t *error_details_list;
} OpenAPI_n32f_error_info_t;

OpenAPI_n32f_error_info_t *OpenAPI_n32f_error_info_create(
    char *n32f_message_id,
    OpenAPI_n32f_error_type_e n32f_error_type,
    char *n32f_context_id,
    OpenAPI_list_t *failed_modification_list,
    OpenAPI_list_t *error_details_list
);
void OpenAPI_n32f_error_info_free(OpenAPI_n32f_error_info_t *n32f_error_info);
OpenAPI_n32f_error_info_t *OpenAPI_n32f_error_info_parseFromJSON(cJSON *n32f_error_infoJSON);
cJSON *OpenAPI_n32f_error_info_convertToJSON(OpenAPI_n32f_error_info_t *n32f_error_info);
OpenAPI_n32f_error_info_t *OpenAPI_n32f_error_info_copy(OpenAPI_n32f_error_info_t *dst, OpenAPI_n32f_error_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n32f_error_info_H_ */


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
typedef struct OpenAPI_n32f_error_info_s OpenAPI_n32f_error_info_t;
#include "failed_modification_info.h"
#include "invalid_param.h"
#include "n32f_error_detail.h"
#include "n32f_error_type.h"
#include "ri_error_information.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_n32f_error_info_s {
    char *n32f_message_id;
    OpenAPI_n32f_error_type_e n32f_error_type;
    char *n32f_context_id;
    OpenAPI_list_t *failed_modification_list;
    OpenAPI_list_t *error_details_list;
    OpenAPI_list_t *policy_mismatch_list;
    struct OpenAPI_ri_error_information_s *ri_error_information;
};

OpenAPI_n32f_error_info_t *OpenAPI_n32f_error_info_create(
    char *n32f_message_id,
    OpenAPI_n32f_error_type_e n32f_error_type,
    char *n32f_context_id,
    OpenAPI_list_t *failed_modification_list,
    OpenAPI_list_t *error_details_list,
    OpenAPI_list_t *policy_mismatch_list,
    OpenAPI_ri_error_information_t *ri_error_information
);
void OpenAPI_n32f_error_info_free(OpenAPI_n32f_error_info_t *n32f_error_info);
OpenAPI_n32f_error_info_t *OpenAPI_n32f_error_info_parseFromJSON(cJSON *n32f_error_infoJSON);
cJSON *OpenAPI_n32f_error_info_convertToJSON(OpenAPI_n32f_error_info_t *n32f_error_info);
OpenAPI_n32f_error_info_t *OpenAPI_n32f_error_info_copy(OpenAPI_n32f_error_info_t *dst, OpenAPI_n32f_error_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n32f_error_info_H_ */


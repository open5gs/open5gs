/*
 * up_security_info.h
 *
 * 
 */

#ifndef _OpenAPI_up_security_info_H_
#define _OpenAPI_up_security_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "max_integrity_protected_data_rate.h"
#include "security_result.h"
#include "up_security.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_up_security_info_s OpenAPI_up_security_info_t;
typedef struct OpenAPI_up_security_info_s {
    struct OpenAPI_up_security_s *up_security;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl;
    struct OpenAPI_security_result_s *security_result;
} OpenAPI_up_security_info_t;

OpenAPI_up_security_info_t *OpenAPI_up_security_info_create(
    OpenAPI_up_security_t *up_security,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
    OpenAPI_security_result_t *security_result
);
void OpenAPI_up_security_info_free(OpenAPI_up_security_info_t *up_security_info);
OpenAPI_up_security_info_t *OpenAPI_up_security_info_parseFromJSON(cJSON *up_security_infoJSON);
cJSON *OpenAPI_up_security_info_convertToJSON(OpenAPI_up_security_info_t *up_security_info);
OpenAPI_up_security_info_t *OpenAPI_up_security_info_copy(OpenAPI_up_security_info_t *dst, OpenAPI_up_security_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_security_info_H_ */


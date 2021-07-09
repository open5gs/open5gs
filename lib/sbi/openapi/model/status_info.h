/*
 * status_info.h
 *
 * 
 */

#ifndef _OpenAPI_status_info_H_
#define _OpenAPI_status_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "cause.h"
#include "cn_assisted_ran_para.h"
#include "resource_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_status_info_s OpenAPI_status_info_t;
typedef struct OpenAPI_status_info_s {
    OpenAPI_resource_status_e resource_status;
    OpenAPI_cause_e cause;
    struct OpenAPI_cn_assisted_ran_para_s *cn_assisted_ran_para;
    OpenAPI_access_type_e an_type;
} OpenAPI_status_info_t;

OpenAPI_status_info_t *OpenAPI_status_info_create(
    OpenAPI_resource_status_e resource_status,
    OpenAPI_cause_e cause,
    OpenAPI_cn_assisted_ran_para_t *cn_assisted_ran_para,
    OpenAPI_access_type_e an_type
);
void OpenAPI_status_info_free(OpenAPI_status_info_t *status_info);
OpenAPI_status_info_t *OpenAPI_status_info_parseFromJSON(cJSON *status_infoJSON);
cJSON *OpenAPI_status_info_convertToJSON(OpenAPI_status_info_t *status_info);
OpenAPI_status_info_t *OpenAPI_status_info_copy(OpenAPI_status_info_t *dst, OpenAPI_status_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_status_info_H_ */


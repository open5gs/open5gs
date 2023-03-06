/*
 * nsacf_info.h
 *
 * Information of a NSACF NF Instance
 */

#ifndef _OpenAPI_nsacf_info_H_
#define _OpenAPI_nsacf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nsacf_capability.h"
#include "tai.h"
#include "tai_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nsacf_info_s OpenAPI_nsacf_info_t;
typedef struct OpenAPI_nsacf_info_s {
    struct OpenAPI_nsacf_capability_s *nsacf_capability;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
} OpenAPI_nsacf_info_t;

OpenAPI_nsacf_info_t *OpenAPI_nsacf_info_create(
    OpenAPI_nsacf_capability_t *nsacf_capability,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list
);
void OpenAPI_nsacf_info_free(OpenAPI_nsacf_info_t *nsacf_info);
OpenAPI_nsacf_info_t *OpenAPI_nsacf_info_parseFromJSON(cJSON *nsacf_infoJSON);
cJSON *OpenAPI_nsacf_info_convertToJSON(OpenAPI_nsacf_info_t *nsacf_info);
OpenAPI_nsacf_info_t *OpenAPI_nsacf_info_copy(OpenAPI_nsacf_info_t *dst, OpenAPI_nsacf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsacf_info_H_ */


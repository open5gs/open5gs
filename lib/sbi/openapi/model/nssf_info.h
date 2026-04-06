/*
 * nssf_info.h
 *
 * Information of an NSSF Instance
 */

#ifndef _OpenAPI_nssf_info_H_
#define _OpenAPI_nssf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nssf_info_s OpenAPI_nssf_info_t;
#include "nssf_event_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nssf_info_s {
    OpenAPI_list_t *event_list;
};

OpenAPI_nssf_info_t *OpenAPI_nssf_info_create(
    OpenAPI_list_t *event_list
);
void OpenAPI_nssf_info_free(OpenAPI_nssf_info_t *nssf_info);
OpenAPI_nssf_info_t *OpenAPI_nssf_info_parseFromJSON(cJSON *nssf_infoJSON);
cJSON *OpenAPI_nssf_info_convertToJSON(OpenAPI_nssf_info_t *nssf_info);
OpenAPI_nssf_info_t *OpenAPI_nssf_info_copy(OpenAPI_nssf_info_t *dst, OpenAPI_nssf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssf_info_H_ */


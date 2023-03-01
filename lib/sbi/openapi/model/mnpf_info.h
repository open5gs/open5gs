/*
 * mnpf_info.h
 *
 * Information of an MNPF Instance
 */

#ifndef _OpenAPI_mnpf_info_H_
#define _OpenAPI_mnpf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "identity_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mnpf_info_s OpenAPI_mnpf_info_t;
typedef struct OpenAPI_mnpf_info_s {
    OpenAPI_list_t *msisdn_ranges;
} OpenAPI_mnpf_info_t;

OpenAPI_mnpf_info_t *OpenAPI_mnpf_info_create(
    OpenAPI_list_t *msisdn_ranges
);
void OpenAPI_mnpf_info_free(OpenAPI_mnpf_info_t *mnpf_info);
OpenAPI_mnpf_info_t *OpenAPI_mnpf_info_parseFromJSON(cJSON *mnpf_infoJSON);
cJSON *OpenAPI_mnpf_info_convertToJSON(OpenAPI_mnpf_info_t *mnpf_info);
OpenAPI_mnpf_info_t *OpenAPI_mnpf_info_copy(OpenAPI_mnpf_info_t *dst, OpenAPI_mnpf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mnpf_info_H_ */


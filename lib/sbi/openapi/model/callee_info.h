/*
 * callee_info.h
 *
 * Identifies the callee information.
 */

#ifndef _OpenAPI_callee_info_H_
#define _OpenAPI_callee_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_callee_info_s OpenAPI_callee_info_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_callee_info_s {
    char *called_party_addr;
    OpenAPI_list_t *request_party_addrs;
    OpenAPI_list_t *called_assert_ids;
};

OpenAPI_callee_info_t *OpenAPI_callee_info_create(
    char *called_party_addr,
    OpenAPI_list_t *request_party_addrs,
    OpenAPI_list_t *called_assert_ids
);
void OpenAPI_callee_info_free(OpenAPI_callee_info_t *callee_info);
OpenAPI_callee_info_t *OpenAPI_callee_info_parseFromJSON(cJSON *callee_infoJSON);
cJSON *OpenAPI_callee_info_convertToJSON(OpenAPI_callee_info_t *callee_info);
OpenAPI_callee_info_t *OpenAPI_callee_info_copy(OpenAPI_callee_info_t *dst, OpenAPI_callee_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_callee_info_H_ */


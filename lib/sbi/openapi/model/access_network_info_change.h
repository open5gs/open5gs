/*
 * access_network_info_change.h
 *
 * 
 */

#ifndef _OpenAPI_access_network_info_change_H_
#define _OpenAPI_access_network_info_change_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_access_network_info_change_s OpenAPI_access_network_info_change_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_access_network_info_change_s {
    OpenAPI_list_t *access_network_information;
    char *cellular_network_information;
    char *change_time;
};

OpenAPI_access_network_info_change_t *OpenAPI_access_network_info_change_create(
    OpenAPI_list_t *access_network_information,
    char *cellular_network_information,
    char *change_time
);
void OpenAPI_access_network_info_change_free(OpenAPI_access_network_info_change_t *access_network_info_change);
OpenAPI_access_network_info_change_t *OpenAPI_access_network_info_change_parseFromJSON(cJSON *access_network_info_changeJSON);
cJSON *OpenAPI_access_network_info_change_convertToJSON(OpenAPI_access_network_info_change_t *access_network_info_change);
OpenAPI_access_network_info_change_t *OpenAPI_access_network_info_change_copy(OpenAPI_access_network_info_change_t *dst, OpenAPI_access_network_info_change_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_network_info_change_H_ */


/*
 * last_known_aiotf_info.h
 *
 * Contains the Last Known AIoTF Info.
 */

#ifndef _OpenAPI_last_known_aiotf_info_H_
#define _OpenAPI_last_known_aiotf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_last_known_aiotf_info_s OpenAPI_last_known_aiotf_info_t;
#include "ip_addr_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_last_known_aiotf_info_s {
    int last_known_aiotf_info_ind;
    char *last_known_aiotf_id;
    struct OpenAPI_ip_addr_1_s *last_known_aiotf_address;
    char *last_known_aiotf_fqdn;
};

OpenAPI_last_known_aiotf_info_t *OpenAPI_last_known_aiotf_info_create(
    int last_known_aiotf_info_ind,
    char *last_known_aiotf_id,
    OpenAPI_ip_addr_1_t *last_known_aiotf_address,
    char *last_known_aiotf_fqdn
);
void OpenAPI_last_known_aiotf_info_free(OpenAPI_last_known_aiotf_info_t *last_known_aiotf_info);
OpenAPI_last_known_aiotf_info_t *OpenAPI_last_known_aiotf_info_parseFromJSON(cJSON *last_known_aiotf_infoJSON);
cJSON *OpenAPI_last_known_aiotf_info_convertToJSON(OpenAPI_last_known_aiotf_info_t *last_known_aiotf_info);
OpenAPI_last_known_aiotf_info_t *OpenAPI_last_known_aiotf_info_copy(OpenAPI_last_known_aiotf_info_t *dst, OpenAPI_last_known_aiotf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_last_known_aiotf_info_H_ */


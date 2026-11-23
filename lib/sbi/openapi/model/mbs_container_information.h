/*
 * mbs_container_information.h
 *
 * 
 */

#ifndef _OpenAPI_mbs_container_information_H_
#define _OpenAPI_mbs_container_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_mbs_container_information_s OpenAPI_mbs_container_information_t;
#include "established_connection_info.h"
#include "qos_data.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_mbs_container_information_s {
    char *timeof_first_usage;
    char *timeof_last_usage;
    bool is_qo_s_information_null;
    struct OpenAPI_qos_data_s *qo_s_information;
    struct OpenAPI_established_connection_info_s *established_connection_info;
};

OpenAPI_mbs_container_information_t *OpenAPI_mbs_container_information_create(
    char *timeof_first_usage,
    char *timeof_last_usage,
    bool is_qo_s_information_null,
    OpenAPI_qos_data_t *qo_s_information,
    OpenAPI_established_connection_info_t *established_connection_info
);
void OpenAPI_mbs_container_information_free(OpenAPI_mbs_container_information_t *mbs_container_information);
OpenAPI_mbs_container_information_t *OpenAPI_mbs_container_information_parseFromJSON(cJSON *mbs_container_informationJSON);
cJSON *OpenAPI_mbs_container_information_convertToJSON(OpenAPI_mbs_container_information_t *mbs_container_information);
OpenAPI_mbs_container_information_t *OpenAPI_mbs_container_information_copy(OpenAPI_mbs_container_information_t *dst, OpenAPI_mbs_container_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_container_information_H_ */


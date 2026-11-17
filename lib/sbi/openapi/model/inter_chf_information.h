/*
 * inter_chf_information.h
 *
 * 
 */

#ifndef _OpenAPI_inter_chf_information_H_
#define _OpenAPI_inter_chf_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_inter_chf_information_s OpenAPI_inter_chf_information_t;
#include "nf_identification.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_inter_chf_information_s {
    char *remote_chf_resource;
    struct OpenAPI_nf_identification_s *original_nf_consumer_id;
};

OpenAPI_inter_chf_information_t *OpenAPI_inter_chf_information_create(
    char *remote_chf_resource,
    OpenAPI_nf_identification_t *original_nf_consumer_id
);
void OpenAPI_inter_chf_information_free(OpenAPI_inter_chf_information_t *inter_chf_information);
OpenAPI_inter_chf_information_t *OpenAPI_inter_chf_information_parseFromJSON(cJSON *inter_chf_informationJSON);
cJSON *OpenAPI_inter_chf_information_convertToJSON(OpenAPI_inter_chf_information_t *inter_chf_information);
OpenAPI_inter_chf_information_t *OpenAPI_inter_chf_information_copy(OpenAPI_inter_chf_information_t *dst, OpenAPI_inter_chf_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_inter_chf_information_H_ */


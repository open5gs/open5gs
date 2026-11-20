/*
 * nsac_container_information.h
 *
 * 
 */

#ifndef _OpenAPI_nsac_container_information_H_
#define _OpenAPI_nsac_container_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nsac_container_information_s OpenAPI_nsac_container_information_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nsac_container_information_s {
    bool is_number_of_ues;
    int number_of_ues;
    bool is_number_of_pdus;
    int number_of_pdus;
};

OpenAPI_nsac_container_information_t *OpenAPI_nsac_container_information_create(
    bool is_number_of_ues,
    int number_of_ues,
    bool is_number_of_pdus,
    int number_of_pdus
);
void OpenAPI_nsac_container_information_free(OpenAPI_nsac_container_information_t *nsac_container_information);
OpenAPI_nsac_container_information_t *OpenAPI_nsac_container_information_parseFromJSON(cJSON *nsac_container_informationJSON);
cJSON *OpenAPI_nsac_container_information_convertToJSON(OpenAPI_nsac_container_information_t *nsac_container_information);
OpenAPI_nsac_container_information_t *OpenAPI_nsac_container_information_copy(OpenAPI_nsac_container_information_t *dst, OpenAPI_nsac_container_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsac_container_information_H_ */


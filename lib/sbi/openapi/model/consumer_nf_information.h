/*
 * consumer_nf_information.h
 *
 * Represents the analytics consumer NF Information.
 */

#ifndef _OpenAPI_consumer_nf_information_H_
#define _OpenAPI_consumer_nf_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_consumer_nf_information_s OpenAPI_consumer_nf_information_t;
typedef struct OpenAPI_consumer_nf_information_s {
    char *nf_id;
    char *nf_set_id;
    OpenAPI_list_t *tai_list;
} OpenAPI_consumer_nf_information_t;

OpenAPI_consumer_nf_information_t *OpenAPI_consumer_nf_information_create(
    char *nf_id,
    char *nf_set_id,
    OpenAPI_list_t *tai_list
);
void OpenAPI_consumer_nf_information_free(OpenAPI_consumer_nf_information_t *consumer_nf_information);
OpenAPI_consumer_nf_information_t *OpenAPI_consumer_nf_information_parseFromJSON(cJSON *consumer_nf_informationJSON);
cJSON *OpenAPI_consumer_nf_information_convertToJSON(OpenAPI_consumer_nf_information_t *consumer_nf_information);
OpenAPI_consumer_nf_information_t *OpenAPI_consumer_nf_information_copy(OpenAPI_consumer_nf_information_t *dst, OpenAPI_consumer_nf_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_consumer_nf_information_H_ */


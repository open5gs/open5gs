/*
 * nidd_information_1.h
 *
 * 
 */

#ifndef _OpenAPI_nidd_information_1_H_
#define _OpenAPI_nidd_information_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nidd_information_1_s OpenAPI_nidd_information_1_t;
typedef struct OpenAPI_nidd_information_1_s {
    char *af_id;
    char *gpsi;
    char *ext_group_id;
} OpenAPI_nidd_information_1_t;

OpenAPI_nidd_information_1_t *OpenAPI_nidd_information_1_create(
    char *af_id,
    char *gpsi,
    char *ext_group_id
);
void OpenAPI_nidd_information_1_free(OpenAPI_nidd_information_1_t *nidd_information_1);
OpenAPI_nidd_information_1_t *OpenAPI_nidd_information_1_parseFromJSON(cJSON *nidd_information_1JSON);
cJSON *OpenAPI_nidd_information_1_convertToJSON(OpenAPI_nidd_information_1_t *nidd_information_1);
OpenAPI_nidd_information_1_t *OpenAPI_nidd_information_1_copy(OpenAPI_nidd_information_1_t *dst, OpenAPI_nidd_information_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nidd_information_1_H_ */


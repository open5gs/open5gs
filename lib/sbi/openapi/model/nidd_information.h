/*
 * nidd_information.h
 *
 *
 */

#ifndef _OpenAPI_nidd_information_H_
#define _OpenAPI_nidd_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nidd_information_s OpenAPI_nidd_information_t;
typedef struct OpenAPI_nidd_information_s {
    char *af_id;
    char *gpsi;
    char *ext_group_id;
} OpenAPI_nidd_information_t;

OpenAPI_nidd_information_t *OpenAPI_nidd_information_create(
    char *af_id,
    char *gpsi,
    char *ext_group_id
    );
void OpenAPI_nidd_information_free(OpenAPI_nidd_information_t *nidd_information);
OpenAPI_nidd_information_t *OpenAPI_nidd_information_parseFromJSON(cJSON *nidd_informationJSON);
cJSON *OpenAPI_nidd_information_convertToJSON(OpenAPI_nidd_information_t *nidd_information);
OpenAPI_nidd_information_t *OpenAPI_nidd_information_copy(OpenAPI_nidd_information_t *dst, OpenAPI_nidd_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nidd_information_H_ */


/*
 * dnai_information.h
 *
 *
 */

#ifndef _OpenAPI_dnai_information_H_
#define _OpenAPI_dnai_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnai_information_s OpenAPI_dnai_information_t;
typedef struct OpenAPI_dnai_information_s {
    char *dnai;
    int no_dnai_change_ind;
    int no_local_psa_change_ind;
} OpenAPI_dnai_information_t;

OpenAPI_dnai_information_t *OpenAPI_dnai_information_create(
    char *dnai,
    int no_dnai_change_ind,
    int no_local_psa_change_ind
    );
void OpenAPI_dnai_information_free(OpenAPI_dnai_information_t *dnai_information);
OpenAPI_dnai_information_t *OpenAPI_dnai_information_parseFromJSON(cJSON *dnai_informationJSON);
cJSON *OpenAPI_dnai_information_convertToJSON(OpenAPI_dnai_information_t *dnai_information);
OpenAPI_dnai_information_t *OpenAPI_dnai_information_copy(OpenAPI_dnai_information_t *dst, OpenAPI_dnai_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnai_information_H_ */


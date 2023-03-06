/*
 * n2_ran_information.h
 *
 * Represents the RAN related N2 information data part
 */

#ifndef _OpenAPI_n2_ran_information_H_
#define _OpenAPI_n2_ran_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_ran_information_s OpenAPI_n2_ran_information_t;
typedef struct OpenAPI_n2_ran_information_s {
    struct OpenAPI_n2_info_content_s *n2_info_content;
} OpenAPI_n2_ran_information_t;

OpenAPI_n2_ran_information_t *OpenAPI_n2_ran_information_create(
    OpenAPI_n2_info_content_t *n2_info_content
);
void OpenAPI_n2_ran_information_free(OpenAPI_n2_ran_information_t *n2_ran_information);
OpenAPI_n2_ran_information_t *OpenAPI_n2_ran_information_parseFromJSON(cJSON *n2_ran_informationJSON);
cJSON *OpenAPI_n2_ran_information_convertToJSON(OpenAPI_n2_ran_information_t *n2_ran_information);
OpenAPI_n2_ran_information_t *OpenAPI_n2_ran_information_copy(OpenAPI_n2_ran_information_t *dst, OpenAPI_n2_ran_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_ran_information_H_ */


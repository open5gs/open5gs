/*
 * v2x_information.h
 *
 * V2X related N2 information
 */

#ifndef _OpenAPI_v2x_information_H_
#define _OpenAPI_v2x_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_v2x_information_s OpenAPI_v2x_information_t;
typedef struct OpenAPI_v2x_information_s {
    struct OpenAPI_n2_info_content_s *n2_pc5_pol;
} OpenAPI_v2x_information_t;

OpenAPI_v2x_information_t *OpenAPI_v2x_information_create(
    OpenAPI_n2_info_content_t *n2_pc5_pol
);
void OpenAPI_v2x_information_free(OpenAPI_v2x_information_t *v2x_information);
OpenAPI_v2x_information_t *OpenAPI_v2x_information_parseFromJSON(cJSON *v2x_informationJSON);
cJSON *OpenAPI_v2x_information_convertToJSON(OpenAPI_v2x_information_t *v2x_information);
OpenAPI_v2x_information_t *OpenAPI_v2x_information_copy(OpenAPI_v2x_information_t *dst, OpenAPI_v2x_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_v2x_information_H_ */


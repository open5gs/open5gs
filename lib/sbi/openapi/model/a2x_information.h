/*
 * a2x_information.h
 *
 * A2X related N2 information
 */

#ifndef _OpenAPI_a2x_information_H_
#define _OpenAPI_a2x_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_a2x_information_s OpenAPI_a2x_information_t;
#include "n2_info_content.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_a2x_information_s {
    struct OpenAPI_n2_info_content_s *n2_pc5_pol;
};

OpenAPI_a2x_information_t *OpenAPI_a2x_information_create(
    OpenAPI_n2_info_content_t *n2_pc5_pol
);
void OpenAPI_a2x_information_free(OpenAPI_a2x_information_t *a2x_information);
OpenAPI_a2x_information_t *OpenAPI_a2x_information_parseFromJSON(cJSON *a2x_informationJSON);
cJSON *OpenAPI_a2x_information_convertToJSON(OpenAPI_a2x_information_t *a2x_information);
OpenAPI_a2x_information_t *OpenAPI_a2x_information_copy(OpenAPI_a2x_information_t *dst, OpenAPI_a2x_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_a2x_information_H_ */


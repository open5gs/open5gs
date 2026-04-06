/*
 * rslp_information.h
 *
 * Represents Ranging/SL positioning related N2 information.
 */

#ifndef _OpenAPI_rslp_information_H_
#define _OpenAPI_rslp_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rslp_information_s OpenAPI_rslp_information_t;
#include "n2_info_content.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rslp_information_s {
    struct OpenAPI_n2_info_content_s *n2_pc5_rslp_pol;
};

OpenAPI_rslp_information_t *OpenAPI_rslp_information_create(
    OpenAPI_n2_info_content_t *n2_pc5_rslp_pol
);
void OpenAPI_rslp_information_free(OpenAPI_rslp_information_t *rslp_information);
OpenAPI_rslp_information_t *OpenAPI_rslp_information_parseFromJSON(cJSON *rslp_informationJSON);
cJSON *OpenAPI_rslp_information_convertToJSON(OpenAPI_rslp_information_t *rslp_information);
OpenAPI_rslp_information_t *OpenAPI_rslp_information_copy(OpenAPI_rslp_information_t *dst, OpenAPI_rslp_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rslp_information_H_ */


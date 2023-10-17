/*
 * pro_se_information.h
 *
 * Represents 5G ProSe related N2 information.
 */

#ifndef _OpenAPI_pro_se_information_H_
#define _OpenAPI_pro_se_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "n2_info_content.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pro_se_information_s OpenAPI_pro_se_information_t;
typedef struct OpenAPI_pro_se_information_s {
    struct OpenAPI_n2_info_content_s *n2_pc5_pro_se_pol;
} OpenAPI_pro_se_information_t;

OpenAPI_pro_se_information_t *OpenAPI_pro_se_information_create(
    OpenAPI_n2_info_content_t *n2_pc5_pro_se_pol
);
void OpenAPI_pro_se_information_free(OpenAPI_pro_se_information_t *pro_se_information);
OpenAPI_pro_se_information_t *OpenAPI_pro_se_information_parseFromJSON(cJSON *pro_se_informationJSON);
cJSON *OpenAPI_pro_se_information_convertToJSON(OpenAPI_pro_se_information_t *pro_se_information);
OpenAPI_pro_se_information_t *OpenAPI_pro_se_information_copy(OpenAPI_pro_se_information_t *dst, OpenAPI_pro_se_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pro_se_information_H_ */


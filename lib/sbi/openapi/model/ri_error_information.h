/*
 * ri_error_information.h
 *
 * RI error information
 */

#ifndef _OpenAPI_ri_error_information_H_
#define _OpenAPI_ri_error_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ri_error_information_s OpenAPI_ri_error_information_t;
#include "n32_release_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ri_error_information_s {
    OpenAPI_n32_release_indication_e n32f_connection_rel_ind;
    OpenAPI_n32_release_indication_e n32f_context_rel_ind;
    char *alternative_ri;
};

OpenAPI_ri_error_information_t *OpenAPI_ri_error_information_create(
    OpenAPI_n32_release_indication_e n32f_connection_rel_ind,
    OpenAPI_n32_release_indication_e n32f_context_rel_ind,
    char *alternative_ri
);
void OpenAPI_ri_error_information_free(OpenAPI_ri_error_information_t *ri_error_information);
OpenAPI_ri_error_information_t *OpenAPI_ri_error_information_parseFromJSON(cJSON *ri_error_informationJSON);
cJSON *OpenAPI_ri_error_information_convertToJSON(OpenAPI_ri_error_information_t *ri_error_information);
OpenAPI_ri_error_information_t *OpenAPI_ri_error_information_copy(OpenAPI_ri_error_information_t *dst, OpenAPI_ri_error_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ri_error_information_H_ */


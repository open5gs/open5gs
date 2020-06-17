/*
 * n2_information_transfer_error.h
 *
 *
 */

#ifndef _OpenAPI_n2_information_transfer_error_H_
#define _OpenAPI_n2_information_transfer_error_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "problem_details.h"
#include "pws_error_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n2_information_transfer_error_s OpenAPI_n2_information_transfer_error_t;
typedef struct OpenAPI_n2_information_transfer_error_s {
    struct OpenAPI_problem_details_s *error;
    struct OpenAPI_pws_error_data_s *pwd_error_info;
} OpenAPI_n2_information_transfer_error_t;

OpenAPI_n2_information_transfer_error_t *OpenAPI_n2_information_transfer_error_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_pws_error_data_t *pwd_error_info
    );
void OpenAPI_n2_information_transfer_error_free(OpenAPI_n2_information_transfer_error_t *n2_information_transfer_error);
OpenAPI_n2_information_transfer_error_t *OpenAPI_n2_information_transfer_error_parseFromJSON(cJSON *n2_information_transfer_errorJSON);
cJSON *OpenAPI_n2_information_transfer_error_convertToJSON(OpenAPI_n2_information_transfer_error_t *n2_information_transfer_error);
OpenAPI_n2_information_transfer_error_t *OpenAPI_n2_information_transfer_error_copy(OpenAPI_n2_information_transfer_error_t *dst, OpenAPI_n2_information_transfer_error_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_information_transfer_error_H_ */


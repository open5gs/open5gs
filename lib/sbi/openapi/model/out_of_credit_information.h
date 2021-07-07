/*
 * out_of_credit_information.h
 *
 * Indicates the SDFs without available credit and the corresponding termination action.
 */

#ifndef _OpenAPI_out_of_credit_information_H_
#define _OpenAPI_out_of_credit_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "final_unit_action.h"
#include "flows.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_out_of_credit_information_s OpenAPI_out_of_credit_information_t;
typedef struct OpenAPI_out_of_credit_information_s {
    struct OpenAPI_final_unit_action_s *fin_unit_act;
    OpenAPI_list_t *flows;
} OpenAPI_out_of_credit_information_t;

OpenAPI_out_of_credit_information_t *OpenAPI_out_of_credit_information_create(
    OpenAPI_final_unit_action_t *fin_unit_act,
    OpenAPI_list_t *flows
);
void OpenAPI_out_of_credit_information_free(OpenAPI_out_of_credit_information_t *out_of_credit_information);
OpenAPI_out_of_credit_information_t *OpenAPI_out_of_credit_information_parseFromJSON(cJSON *out_of_credit_informationJSON);
cJSON *OpenAPI_out_of_credit_information_convertToJSON(OpenAPI_out_of_credit_information_t *out_of_credit_information);
OpenAPI_out_of_credit_information_t *OpenAPI_out_of_credit_information_copy(OpenAPI_out_of_credit_information_t *dst, OpenAPI_out_of_credit_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_out_of_credit_information_H_ */


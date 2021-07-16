/*
 * sequence_number.h
 *
 * 
 */

#ifndef _OpenAPI_sequence_number_H_
#define _OpenAPI_sequence_number_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "sign.h"
#include "sqn_scheme.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sequence_number_s OpenAPI_sequence_number_t;
typedef struct OpenAPI_sequence_number_s {
    OpenAPI_sqn_scheme_e sqn_scheme;
    char *sqn;
    OpenAPI_list_t* last_indexes;
    bool is_ind_length;
    int ind_length;
    OpenAPI_sign_e dif_sign;
} OpenAPI_sequence_number_t;

OpenAPI_sequence_number_t *OpenAPI_sequence_number_create(
    OpenAPI_sqn_scheme_e sqn_scheme,
    char *sqn,
    OpenAPI_list_t* last_indexes,
    bool is_ind_length,
    int ind_length,
    OpenAPI_sign_e dif_sign
);
void OpenAPI_sequence_number_free(OpenAPI_sequence_number_t *sequence_number);
OpenAPI_sequence_number_t *OpenAPI_sequence_number_parseFromJSON(cJSON *sequence_numberJSON);
cJSON *OpenAPI_sequence_number_convertToJSON(OpenAPI_sequence_number_t *sequence_number);
OpenAPI_sequence_number_t *OpenAPI_sequence_number_copy(OpenAPI_sequence_number_t *dst, OpenAPI_sequence_number_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sequence_number_H_ */


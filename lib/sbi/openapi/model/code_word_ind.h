/*
 * code_word_ind.h
 *
 *
 */

#ifndef _OpenAPI_code_word_ind_H_
#define _OpenAPI_code_word_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_code_word_ind_NULL = 0, OpenAPI_code_word_ind_CODEWORD_CHECK_IN_UE, OpenAPI_code_word_ind_CODEWORD_CHECK_IN_GMLC } OpenAPI_code_word_ind_e;

char* OpenAPI_code_word_ind_ToString(OpenAPI_code_word_ind_e code_word_ind);

OpenAPI_code_word_ind_e OpenAPI_code_word_ind_FromString(char* code_word_ind);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_code_word_ind_H_ */


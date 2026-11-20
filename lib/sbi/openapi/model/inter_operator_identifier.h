/*
 * inter_operator_identifier.h
 *
 * 
 */

#ifndef _OpenAPI_inter_operator_identifier_H_
#define _OpenAPI_inter_operator_identifier_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_inter_operator_identifier_s OpenAPI_inter_operator_identifier_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_inter_operator_identifier_s {
    char *originating_ioi;
    char *terminating_ioi;
};

OpenAPI_inter_operator_identifier_t *OpenAPI_inter_operator_identifier_create(
    char *originating_ioi,
    char *terminating_ioi
);
void OpenAPI_inter_operator_identifier_free(OpenAPI_inter_operator_identifier_t *inter_operator_identifier);
OpenAPI_inter_operator_identifier_t *OpenAPI_inter_operator_identifier_parseFromJSON(cJSON *inter_operator_identifierJSON);
cJSON *OpenAPI_inter_operator_identifier_convertToJSON(OpenAPI_inter_operator_identifier_t *inter_operator_identifier);
OpenAPI_inter_operator_identifier_t *OpenAPI_inter_operator_identifier_copy(OpenAPI_inter_operator_identifier_t *dst, OpenAPI_inter_operator_identifier_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_inter_operator_identifier_H_ */


/*
 * variable_part.h
 *
 * 
 */

#ifndef _OpenAPI_variable_part_H_
#define _OpenAPI_variable_part_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_variable_part_s OpenAPI_variable_part_t;
#include "variable_part_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_variable_part_s {
    OpenAPI_variable_part_type_e variable_part_type;
    OpenAPI_list_t *variable_part_value;
    bool is_variable_part_order;
    int variable_part_order;
};

OpenAPI_variable_part_t *OpenAPI_variable_part_create(
    OpenAPI_variable_part_type_e variable_part_type,
    OpenAPI_list_t *variable_part_value,
    bool is_variable_part_order,
    int variable_part_order
);
void OpenAPI_variable_part_free(OpenAPI_variable_part_t *variable_part);
OpenAPI_variable_part_t *OpenAPI_variable_part_parseFromJSON(cJSON *variable_partJSON);
cJSON *OpenAPI_variable_part_convertToJSON(OpenAPI_variable_part_t *variable_part);
OpenAPI_variable_part_t *OpenAPI_variable_part_copy(OpenAPI_variable_part_t *dst, OpenAPI_variable_part_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_variable_part_H_ */


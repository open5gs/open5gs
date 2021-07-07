/*
 * wireline_area_1.h
 *
 * 
 */

#ifndef _OpenAPI_wireline_area_1_H_
#define _OpenAPI_wireline_area_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_wireline_area_1_s OpenAPI_wireline_area_1_t;
typedef struct OpenAPI_wireline_area_1_s {
    OpenAPI_list_t *global_line_ids;
    OpenAPI_list_t *hfc_n_ids;
    char *area_code_b;
    char *area_code_c;
} OpenAPI_wireline_area_1_t;

OpenAPI_wireline_area_1_t *OpenAPI_wireline_area_1_create(
    OpenAPI_list_t *global_line_ids,
    OpenAPI_list_t *hfc_n_ids,
    char *area_code_b,
    char *area_code_c
);
void OpenAPI_wireline_area_1_free(OpenAPI_wireline_area_1_t *wireline_area_1);
OpenAPI_wireline_area_1_t *OpenAPI_wireline_area_1_parseFromJSON(cJSON *wireline_area_1JSON);
cJSON *OpenAPI_wireline_area_1_convertToJSON(OpenAPI_wireline_area_1_t *wireline_area_1);
OpenAPI_wireline_area_1_t *OpenAPI_wireline_area_1_copy(OpenAPI_wireline_area_1_t *dst, OpenAPI_wireline_area_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wireline_area_1_H_ */


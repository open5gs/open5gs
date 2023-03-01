/*
 * wireline_area.h
 *
 * One and only one of the \&quot;globLineIds\&quot;, \&quot;hfcNIds\&quot;, \&quot;areaCodeB\&quot; and \&quot;areaCodeC\&quot; attributes shall be included in a WirelineArea data structure 
 */

#ifndef _OpenAPI_wireline_area_H_
#define _OpenAPI_wireline_area_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_wireline_area_s OpenAPI_wireline_area_t;
typedef struct OpenAPI_wireline_area_s {
    OpenAPI_list_t *global_line_ids;
    OpenAPI_list_t *hfc_n_ids;
    char *area_code_b;
    char *area_code_c;
} OpenAPI_wireline_area_t;

OpenAPI_wireline_area_t *OpenAPI_wireline_area_create(
    OpenAPI_list_t *global_line_ids,
    OpenAPI_list_t *hfc_n_ids,
    char *area_code_b,
    char *area_code_c
);
void OpenAPI_wireline_area_free(OpenAPI_wireline_area_t *wireline_area);
OpenAPI_wireline_area_t *OpenAPI_wireline_area_parseFromJSON(cJSON *wireline_areaJSON);
cJSON *OpenAPI_wireline_area_convertToJSON(OpenAPI_wireline_area_t *wireline_area);
OpenAPI_wireline_area_t *OpenAPI_wireline_area_copy(OpenAPI_wireline_area_t *dst, OpenAPI_wireline_area_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_wireline_area_H_ */


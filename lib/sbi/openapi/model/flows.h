/*
 * flows.h
 *
 * Identifies the flows
 */

#ifndef _OpenAPI_flows_H_
#define _OpenAPI_flows_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_flows_s OpenAPI_flows_t;
typedef struct OpenAPI_flows_s {
    OpenAPI_list_t *cont_vers;
    OpenAPI_list_t *f_nums;
    int med_comp_n;
} OpenAPI_flows_t;

OpenAPI_flows_t *OpenAPI_flows_create(
    OpenAPI_list_t *cont_vers,
    OpenAPI_list_t *f_nums,
    int med_comp_n
);
void OpenAPI_flows_free(OpenAPI_flows_t *flows);
OpenAPI_flows_t *OpenAPI_flows_parseFromJSON(cJSON *flowsJSON);
cJSON *OpenAPI_flows_convertToJSON(OpenAPI_flows_t *flows);
OpenAPI_flows_t *OpenAPI_flows_copy(OpenAPI_flows_t *dst, OpenAPI_flows_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_flows_H_ */


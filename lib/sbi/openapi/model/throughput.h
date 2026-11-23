/*
 * throughput.h
 *
 * 
 */

#ifndef _OpenAPI_throughput_H_
#define _OpenAPI_throughput_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_throughput_s OpenAPI_throughput_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_throughput_s {
    bool is_guaranteed_thpt;
    float guaranteed_thpt;
    bool is_maximum_thpt;
    float maximum_thpt;
};

OpenAPI_throughput_t *OpenAPI_throughput_create(
    bool is_guaranteed_thpt,
    float guaranteed_thpt,
    bool is_maximum_thpt,
    float maximum_thpt
);
void OpenAPI_throughput_free(OpenAPI_throughput_t *throughput);
OpenAPI_throughput_t *OpenAPI_throughput_parseFromJSON(cJSON *throughputJSON);
cJSON *OpenAPI_throughput_convertToJSON(OpenAPI_throughput_t *throughput);
OpenAPI_throughput_t *OpenAPI_throughput_copy(OpenAPI_throughput_t *dst, OpenAPI_throughput_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_throughput_H_ */


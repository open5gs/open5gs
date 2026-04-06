/*
 * n6_jitter_information.h
 *
 * Jitter information associated with the Periodicity in downlink
 */

#ifndef _OpenAPI_n6_jitter_information_H_
#define _OpenAPI_n6_jitter_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_n6_jitter_information_s OpenAPI_n6_jitter_information_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_n6_jitter_information_s {
    bool is_lower_jitter_info;
    int lower_jitter_info;
    bool is_higher_jitter_info;
    int higher_jitter_info;
};

OpenAPI_n6_jitter_information_t *OpenAPI_n6_jitter_information_create(
    bool is_lower_jitter_info,
    int lower_jitter_info,
    bool is_higher_jitter_info,
    int higher_jitter_info
);
void OpenAPI_n6_jitter_information_free(OpenAPI_n6_jitter_information_t *n6_jitter_information);
OpenAPI_n6_jitter_information_t *OpenAPI_n6_jitter_information_parseFromJSON(cJSON *n6_jitter_informationJSON);
cJSON *OpenAPI_n6_jitter_information_convertToJSON(OpenAPI_n6_jitter_information_t *n6_jitter_information);
OpenAPI_n6_jitter_information_t *OpenAPI_n6_jitter_information_copy(OpenAPI_n6_jitter_information_t *dst, OpenAPI_n6_jitter_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n6_jitter_information_H_ */


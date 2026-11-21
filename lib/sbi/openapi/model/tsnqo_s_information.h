/*
 * tsnqo_s_information.h
 *
 * 
 */

#ifndef _OpenAPI_tsnqo_s_information_H_
#define _OpenAPI_tsnqo_s_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_tsnqo_s_information_s OpenAPI_tsnqo_s_information_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_tsnqo_s_information_s {
    bool is_priority;
    int priority;
    OpenAPI_list_t *bridge_delay;
};

OpenAPI_tsnqo_s_information_t *OpenAPI_tsnqo_s_information_create(
    bool is_priority,
    int priority,
    OpenAPI_list_t *bridge_delay
);
void OpenAPI_tsnqo_s_information_free(OpenAPI_tsnqo_s_information_t *tsnqo_s_information);
OpenAPI_tsnqo_s_information_t *OpenAPI_tsnqo_s_information_parseFromJSON(cJSON *tsnqo_s_informationJSON);
cJSON *OpenAPI_tsnqo_s_information_convertToJSON(OpenAPI_tsnqo_s_information_t *tsnqo_s_information);
OpenAPI_tsnqo_s_information_t *OpenAPI_tsnqo_s_information_copy(OpenAPI_tsnqo_s_information_t *dst, OpenAPI_tsnqo_s_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tsnqo_s_information_H_ */


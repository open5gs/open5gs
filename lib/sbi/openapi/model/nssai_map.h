/*
 * nssai_map.h
 *
 * 
 */

#ifndef _OpenAPI_nssai_map_H_
#define _OpenAPI_nssai_map_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nssai_map_s OpenAPI_nssai_map_t;
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nssai_map_s {
    struct OpenAPI_snssai_s *serving_snssai;
    struct OpenAPI_snssai_s *home_snssai;
};

OpenAPI_nssai_map_t *OpenAPI_nssai_map_create(
    OpenAPI_snssai_t *serving_snssai,
    OpenAPI_snssai_t *home_snssai
);
void OpenAPI_nssai_map_free(OpenAPI_nssai_map_t *nssai_map);
OpenAPI_nssai_map_t *OpenAPI_nssai_map_parseFromJSON(cJSON *nssai_mapJSON);
cJSON *OpenAPI_nssai_map_convertToJSON(OpenAPI_nssai_map_t *nssai_map);
OpenAPI_nssai_map_t *OpenAPI_nssai_map_copy(OpenAPI_nssai_map_t *dst, OpenAPI_nssai_map_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssai_map_H_ */


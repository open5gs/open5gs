/*
 * alternative_nssai_map.h
 *
 * 
 */

#ifndef _OpenAPI_alternative_nssai_map_H_
#define _OpenAPI_alternative_nssai_map_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_alternative_nssai_map_s OpenAPI_alternative_nssai_map_t;
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_alternative_nssai_map_s {
    struct OpenAPI_snssai_s *snssai;
    struct OpenAPI_snssai_s *alternative_snssai;
};

OpenAPI_alternative_nssai_map_t *OpenAPI_alternative_nssai_map_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_snssai_t *alternative_snssai
);
void OpenAPI_alternative_nssai_map_free(OpenAPI_alternative_nssai_map_t *alternative_nssai_map);
OpenAPI_alternative_nssai_map_t *OpenAPI_alternative_nssai_map_parseFromJSON(cJSON *alternative_nssai_mapJSON);
cJSON *OpenAPI_alternative_nssai_map_convertToJSON(OpenAPI_alternative_nssai_map_t *alternative_nssai_map);
OpenAPI_alternative_nssai_map_t *OpenAPI_alternative_nssai_map_copy(OpenAPI_alternative_nssai_map_t *dst, OpenAPI_alternative_nssai_map_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_alternative_nssai_map_H_ */


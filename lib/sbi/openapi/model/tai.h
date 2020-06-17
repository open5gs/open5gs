/*
 * tai.h
 *
 *
 */

#ifndef _OpenAPI_tai_H_
#define _OpenAPI_tai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tai_s OpenAPI_tai_t;
typedef struct OpenAPI_tai_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    char *tac;
    char *nid;
} OpenAPI_tai_t;

OpenAPI_tai_t *OpenAPI_tai_create(
    OpenAPI_plmn_id_t *plmn_id,
    char *tac,
    char *nid
    );
void OpenAPI_tai_free(OpenAPI_tai_t *tai);
OpenAPI_tai_t *OpenAPI_tai_parseFromJSON(cJSON *taiJSON);
cJSON *OpenAPI_tai_convertToJSON(OpenAPI_tai_t *tai);
OpenAPI_tai_t *OpenAPI_tai_copy(OpenAPI_tai_t *dst, OpenAPI_tai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tai_H_ */


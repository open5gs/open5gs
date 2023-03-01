/*
 * ncgi_tai.h
 *
 * List of NR cell ids, with their pertaining TAIs
 */

#ifndef _OpenAPI_ncgi_tai_H_
#define _OpenAPI_ncgi_tai_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ncgi.h"
#include "tai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ncgi_tai_s OpenAPI_ncgi_tai_t;
typedef struct OpenAPI_ncgi_tai_s {
    struct OpenAPI_tai_s *tai;
    OpenAPI_list_t *cell_list;
} OpenAPI_ncgi_tai_t;

OpenAPI_ncgi_tai_t *OpenAPI_ncgi_tai_create(
    OpenAPI_tai_t *tai,
    OpenAPI_list_t *cell_list
);
void OpenAPI_ncgi_tai_free(OpenAPI_ncgi_tai_t *ncgi_tai);
OpenAPI_ncgi_tai_t *OpenAPI_ncgi_tai_parseFromJSON(cJSON *ncgi_taiJSON);
cJSON *OpenAPI_ncgi_tai_convertToJSON(OpenAPI_ncgi_tai_t *ncgi_tai);
OpenAPI_ncgi_tai_t *OpenAPI_ncgi_tai_copy(OpenAPI_ncgi_tai_t *dst, OpenAPI_ncgi_tai_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ncgi_tai_H_ */


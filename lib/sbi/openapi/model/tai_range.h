/*
 * tai_range.h
 *
 * Range of TAIs (Tracking Area Identities)
 */

#ifndef _OpenAPI_tai_range_H_
#define _OpenAPI_tai_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"
#include "tac_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tai_range_s OpenAPI_tai_range_t;
typedef struct OpenAPI_tai_range_s {
    struct OpenAPI_plmn_id_s *plmn_id;
    OpenAPI_list_t *tac_range_list;
    char *nid;
} OpenAPI_tai_range_t;

OpenAPI_tai_range_t *OpenAPI_tai_range_create(
    OpenAPI_plmn_id_t *plmn_id,
    OpenAPI_list_t *tac_range_list,
    char *nid
);
void OpenAPI_tai_range_free(OpenAPI_tai_range_t *tai_range);
OpenAPI_tai_range_t *OpenAPI_tai_range_parseFromJSON(cJSON *tai_rangeJSON);
cJSON *OpenAPI_tai_range_convertToJSON(OpenAPI_tai_range_t *tai_range);
OpenAPI_tai_range_t *OpenAPI_tai_range_copy(OpenAPI_tai_range_t *dst, OpenAPI_tai_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tai_range_H_ */


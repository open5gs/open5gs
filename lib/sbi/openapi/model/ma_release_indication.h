/*
 * ma_release_indication.h
 *
 * Possible values are   - REL_MAPDU_OVER_3GPP   - REL_MAPDU_OVER_N3GPP
 */

#ifndef _OpenAPI_ma_release_indication_H_
#define _OpenAPI_ma_release_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ma_release_indication_s OpenAPI_ma_release_indication_t;
typedef struct OpenAPI_ma_release_indication_s {
} OpenAPI_ma_release_indication_t;

OpenAPI_ma_release_indication_t *OpenAPI_ma_release_indication_create(
    );
void OpenAPI_ma_release_indication_free(OpenAPI_ma_release_indication_t *ma_release_indication);
OpenAPI_ma_release_indication_t *OpenAPI_ma_release_indication_parseFromJSON(cJSON *ma_release_indicationJSON);
cJSON *OpenAPI_ma_release_indication_convertToJSON(OpenAPI_ma_release_indication_t *ma_release_indication);
OpenAPI_ma_release_indication_t *OpenAPI_ma_release_indication_copy(OpenAPI_ma_release_indication_t *dst, OpenAPI_ma_release_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ma_release_indication_H_ */


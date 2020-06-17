/*
 * psa_indication.h
 *
 * Possible values are   - PSA_INSERTED   - PSA_REMOVED
 */

#ifndef _OpenAPI_psa_indication_H_
#define _OpenAPI_psa_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_psa_indication_s OpenAPI_psa_indication_t;
typedef struct OpenAPI_psa_indication_s {
} OpenAPI_psa_indication_t;

OpenAPI_psa_indication_t *OpenAPI_psa_indication_create(
    );
void OpenAPI_psa_indication_free(OpenAPI_psa_indication_t *psa_indication);
OpenAPI_psa_indication_t *OpenAPI_psa_indication_parseFromJSON(cJSON *psa_indicationJSON);
cJSON *OpenAPI_psa_indication_convertToJSON(OpenAPI_psa_indication_t *psa_indication);
OpenAPI_psa_indication_t *OpenAPI_psa_indication_copy(OpenAPI_psa_indication_t *dst, OpenAPI_psa_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_psa_indication_H_ */


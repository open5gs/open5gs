/*
 * eps_interworking_indication.h
 *
 * Possible values are - NONE - WITH_N26 - WITHOUT_N26 - IWK_NON_3GPP
 */

#ifndef _OpenAPI_eps_interworking_indication_H_
#define _OpenAPI_eps_interworking_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_eps_interworking_indication_s OpenAPI_eps_interworking_indication_t;
typedef struct OpenAPI_eps_interworking_indication_s {
} OpenAPI_eps_interworking_indication_t;

OpenAPI_eps_interworking_indication_t *OpenAPI_eps_interworking_indication_create(
    );
void OpenAPI_eps_interworking_indication_free(OpenAPI_eps_interworking_indication_t *eps_interworking_indication);
OpenAPI_eps_interworking_indication_t *OpenAPI_eps_interworking_indication_parseFromJSON(cJSON *eps_interworking_indicationJSON);
cJSON *OpenAPI_eps_interworking_indication_convertToJSON(OpenAPI_eps_interworking_indication_t *eps_interworking_indication);
OpenAPI_eps_interworking_indication_t *OpenAPI_eps_interworking_indication_copy(OpenAPI_eps_interworking_indication_t *dst, OpenAPI_eps_interworking_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_interworking_indication_H_ */


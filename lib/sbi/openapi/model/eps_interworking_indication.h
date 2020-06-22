/*
 * eps_interworking_indication.h
 *
 *
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

typedef enum { OpenAPI_eps_interworking_indication_NULL = 0, OpenAPI_eps_interworking_indication_NONE, OpenAPI_eps_interworking_indication_WITH_N26, OpenAPI_eps_interworking_indication_WITHOUT_N26, OpenAPI_eps_interworking_indication_IWK_NON_3GPP } OpenAPI_eps_interworking_indication_e;

char* OpenAPI_eps_interworking_indication_ToString(OpenAPI_eps_interworking_indication_e eps_interworking_indication);

OpenAPI_eps_interworking_indication_e OpenAPI_eps_interworking_indication_FromString(char* eps_interworking_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_eps_interworking_indication_H_ */


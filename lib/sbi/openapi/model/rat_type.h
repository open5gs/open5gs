/*
 * rat_type.h
 *
 * 
 */

#ifndef _OpenAPI_rat_type_H_
#define _OpenAPI_rat_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_rat_type_NULL = 0, OpenAPI_rat_type_NR, OpenAPI_rat_type_EUTRA, OpenAPI_rat_type_WLAN, OpenAPI_rat_type__VIRTUAL, OpenAPI_rat_type_NBIOT, OpenAPI_rat_type_WIRELINE, OpenAPI_rat_type_WIRELINE_CABLE, OpenAPI_rat_type_WIRELINE_BBF, OpenAPI_rat_type_LTE_M, OpenAPI_rat_type_NR_U, OpenAPI_rat_type_EUTRA_U, OpenAPI_rat_type_TRUSTED_N3GA, OpenAPI_rat_type_TRUSTED_WLAN, OpenAPI_rat_type_UTRA, OpenAPI_rat_type_GERA, OpenAPI_rat_type_NR_LEO, OpenAPI_rat_type_NR_MEO, OpenAPI_rat_type_NR_GEO, OpenAPI_rat_type_NR_OTHER_SAT, OpenAPI_rat_type_NR_REDCAP } OpenAPI_rat_type_e;

char* OpenAPI_rat_type_ToString(OpenAPI_rat_type_e rat_type);

OpenAPI_rat_type_e OpenAPI_rat_type_FromString(char* rat_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rat_type_H_ */


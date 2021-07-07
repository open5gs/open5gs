/*
 * access_tech_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_access_tech_any_of_H_
#define _OpenAPI_access_tech_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_access_tech_any_of_NULL = 0, OpenAPI_access_tech_any_of_NR, OpenAPI_access_tech_any_of_EUTRAN_IN_WBS1_MODE_AND_NBS1_MODE, OpenAPI_access_tech_any_of_EUTRAN_IN_NBS1_MODE_ONLY, OpenAPI_access_tech_any_of_EUTRAN_IN_WBS1_MODE_ONLY, OpenAPI_access_tech_any_of_UTRAN, OpenAPI_access_tech_any_of_GSM_AND_ECGSM_IoT, OpenAPI_access_tech_any_of_GSM_WITHOUT_ECGSM_IoT, OpenAPI_access_tech_any_of_ECGSM_IoT_ONLY, OpenAPI_access_tech_any_of_CDMA_1xRTT, OpenAPI_access_tech_any_of_CDMA_HRPD, OpenAPI_access_tech_any_of_GSM_COMPACT } OpenAPI_access_tech_any_of_e;

char* OpenAPI_access_tech_any_of_ToString(OpenAPI_access_tech_any_of_e access_tech_any_of);

OpenAPI_access_tech_any_of_e OpenAPI_access_tech_any_of_FromString(char* access_tech_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_tech_any_of_H_ */


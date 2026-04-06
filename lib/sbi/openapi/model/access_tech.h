/*
 * access_tech.h
 *
 * Represents the access technology
 */

#ifndef _OpenAPI_access_tech_H_
#define _OpenAPI_access_tech_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_access_tech_NULL = 0, OpenAPI_access_tech_NR, OpenAPI_access_tech_EUTRAN_IN_WBS1_MODE_AND_NBS1_MODE, OpenAPI_access_tech_EUTRAN_IN_NBS1_MODE_ONLY, OpenAPI_access_tech_EUTRAN_IN_WBS1_MODE_ONLY, OpenAPI_access_tech_UTRAN, OpenAPI_access_tech_GSM_AND_ECGSM_IoT, OpenAPI_access_tech_GSM_WITHOUT_ECGSM_IoT, OpenAPI_access_tech_ECGSM_IoT_ONLY, OpenAPI_access_tech_CDMA_1xRTT, OpenAPI_access_tech_CDMA_HRPD, OpenAPI_access_tech_GSM_COMPACT, OpenAPI_access_tech_SATELLITE_NG_RAN, OpenAPI_access_tech_SATELLITE_EUTRAN_IN_NBS1_MODE, OpenAPI_access_tech_SATELLITE_EUTRAN_IN_WBS1_MODE } OpenAPI_access_tech_e;

char* OpenAPI_access_tech_ToString(OpenAPI_access_tech_e access_tech);

OpenAPI_access_tech_e OpenAPI_access_tech_FromString(char* access_tech);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_tech_H_ */


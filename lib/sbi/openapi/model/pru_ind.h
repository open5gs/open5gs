/*
 * pru_ind.h
 *
 * Indicates whether the UE can act as a PRU or the type of PRU. 
 */

#ifndef _OpenAPI_pru_ind_H_
#define _OpenAPI_pru_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pru_ind_NULL = 0, OpenAPI_pru_ind_NON_PRU, OpenAPI_pru_ind_STATIONARY_PRU, OpenAPI_pru_ind_NON_STATIONARY_PRU } OpenAPI_pru_ind_e;

char* OpenAPI_pru_ind_ToString(OpenAPI_pru_ind_e pru_ind);

OpenAPI_pru_ind_e OpenAPI_pru_ind_FromString(char* pru_ind);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pru_ind_H_ */


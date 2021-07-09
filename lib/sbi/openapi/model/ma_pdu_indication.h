/*
 * ma_pdu_indication.h
 *
 * 
 */

#ifndef _OpenAPI_ma_pdu_indication_H_
#define _OpenAPI_ma_pdu_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ma_pdu_indication_NULL = 0, OpenAPI_ma_pdu_indication_MA_PDU_REQUEST, OpenAPI_ma_pdu_indication_MA_PDU_NETWORK_UPGRADE_ALLOWED } OpenAPI_ma_pdu_indication_e;

char* OpenAPI_ma_pdu_indication_ToString(OpenAPI_ma_pdu_indication_e ma_pdu_indication);

OpenAPI_ma_pdu_indication_e OpenAPI_ma_pdu_indication_FromString(char* ma_pdu_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ma_pdu_indication_H_ */


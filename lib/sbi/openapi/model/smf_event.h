/*
 * smf_event.h
 *
 * 
 */

#ifndef _OpenAPI_smf_event_H_
#define _OpenAPI_smf_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_smf_event_NULL = 0, OpenAPI_smf_event_AC_TY_CH, OpenAPI_smf_event_UP_PATH_CH, OpenAPI_smf_event_PDU_SES_REL, OpenAPI_smf_event_PLMN_CH, OpenAPI_smf_event_UE_IP_CH, OpenAPI_smf_event_RAT_TY_CH, OpenAPI_smf_event_DDDS, OpenAPI_smf_event_COMM_FAIL, OpenAPI_smf_event_PDU_SES_EST, OpenAPI_smf_event_QFI_ALLOC, OpenAPI_smf_event_QOS_MON, OpenAPI_smf_event_SMCC_EXP, OpenAPI_smf_event_DISPERSION, OpenAPI_smf_event_RED_TRANS_EXP, OpenAPI_smf_event_WLAN_INFO, OpenAPI_smf_event_UPF_INFO, OpenAPI_smf_event_UP_STATUS_INFO, OpenAPI_smf_event_UPF_EVENT, OpenAPI_smf_event_SATB_CH, OpenAPI_smf_event_TRAFFIC_CORRELATION, OpenAPI_smf_event_TRAFF_ROUTE_REQ_OUTCOME, OpenAPI_smf_event_SIM_CONN_FAIL, OpenAPI_smf_event_QFI_DEALLOCATION, OpenAPI_smf_event_QOS_FLOW_CHANGE, OpenAPI_smf_event_ENERGY_USAGE_DATA, OpenAPI_smf_event_SIGNALLING_INFO } OpenAPI_smf_event_e;

char* OpenAPI_smf_event_ToString(OpenAPI_smf_event_e smf_event);

OpenAPI_smf_event_e OpenAPI_smf_event_FromString(char* smf_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_event_H_ */


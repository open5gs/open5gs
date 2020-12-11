/*
 * policy_control_request_trigger.h
 *
 * Possible values are - PLMN_CH: PLMN Change - RES_MO_RE: A request for resource modification has been received by the SMF. The SMF always reports to the PCF. - AC_TY_CH: Access Type Change - UE_IP_CH: UE IP address change. The SMF always reports to the PCF. - UE_MAC_CH: A new UE MAC address is detected or a used UE MAC address is inactive for a specific period - AN_CH_COR: Access Network Charging Correlation Information - US_RE: The PDU Session or the Monitoring key specific resources consumed by a UE either reached the threshold or needs to be reported for other reasons. - APP_STA: The start of application traffic has been detected. - APP_STO: The stop of application traffic has been detected. - AN_INFO: Access Network Information report - CM_SES_FAIL: Credit management session failure - PS_DA_OFF: The SMF reports when the 3GPP PS Data Off status changes. The SMF always reports to the PCF. - DEF_QOS_CH: Default QoS Change. The SMF always reports to the PCF. - SE_AMBR_CH: Session AMBR Change. The SMF always reports to the PCF. - QOS_NOTIF: The SMF notify the PCF when receiving notification from RAN that QoS targets of the QoS Flow cannot be guranteed or gurateed again. - NO_CREDIT: Out of credit - REALLO_OF_CREDIT: Reallocation of credit - PRA_CH: Change of UE presence in Presence Reporting Area - SAREA_CH: Location Change with respect to the Serving Area - SCNN_CH: Location Change with respect to the Serving CN node - RE_TIMEOUT: Indicates the SMF generated the request because there has been a PCC revalidation timeout - RES_RELEASE: Indicate that the SMF can inform the PCF of the outcome of the release of resources for those rules that require so. - SUCC_RES_ALLO: Indicates that the requested rule data is the successful resource allocation. - RAT_TY_CH: RAT Type Change. - REF_QOS_IND_CH: Reflective QoS indication Change - NUM_OF_PACKET_FILTER: Indicates that the SMF shall report the number of supported packet filter for signalled QoS rules - UE_STATUS_RESUME: Indicates that the UE’s status is resumed. - UE_TZ_CH: UE Time Zone Change - AUTH_PROF_CH: The DN-AAA authorization profile index has changed - QOS_MONITORING: Indicate that the SMF notifies the PCF of the QoS Monitoring information. - SCELL_CH: Location Change with respect to the Serving Cell. Only applicable to the interworking scenario as defined in Annex B. - EPS_FALLBACK: EPS Fallback report is enabled in the SMF. - MA_PDU: UE Indicates that the SMF notifies the PCF of the MA PDU session request - TSN_ETHER_PORT: Manageable Ethernet port detected - TSN_CONTAINER: Port management container detected. - 5G_RG_JOIN: The 5G-RG has joined to an IP Multicast Group. - 5G_RG_LEAVE: The 5G-RG has left an IP Multicast Group.
 */

#ifndef _OpenAPI_policy_control_request_trigger_H_
#define _OpenAPI_policy_control_request_trigger_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_policy_control_request_trigger_s OpenAPI_policy_control_request_trigger_t;
typedef struct OpenAPI_policy_control_request_trigger_s {
} OpenAPI_policy_control_request_trigger_t;

OpenAPI_policy_control_request_trigger_t *OpenAPI_policy_control_request_trigger_create(
    );
void OpenAPI_policy_control_request_trigger_free(OpenAPI_policy_control_request_trigger_t *policy_control_request_trigger);
OpenAPI_policy_control_request_trigger_t *OpenAPI_policy_control_request_trigger_parseFromJSON(cJSON *policy_control_request_triggerJSON);
cJSON *OpenAPI_policy_control_request_trigger_convertToJSON(OpenAPI_policy_control_request_trigger_t *policy_control_request_trigger);
OpenAPI_policy_control_request_trigger_t *OpenAPI_policy_control_request_trigger_copy(OpenAPI_policy_control_request_trigger_t *dst, OpenAPI_policy_control_request_trigger_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_policy_control_request_trigger_H_ */


/*
 * failure_code.h
 *
 * Possible values are   - UNK_RULE_ID: Indicates that the pre-provisioned PCC rule could not be successfully activated because the PCC rule identifier is unknown to the SMF.   - RA_GR_ERR: Indicate that the PCC rule could not be successfully installed or enforced because the Rating Group specified within the Charging Data policy decision which the PCC rule refers to is unknown or, invalid.   - SER_ID_ERR: Indicate that the PCC rule could not be successfully installed or enforced because the Service Identifier specified within the Charging Data policy decision which the PCC rule refers to is invalid, unknown, or not applicable to the service being charged.   - NF_MAL: Indicate that the PCC rule could not be successfully installed (for those provisioned from the PCF) or activated (for those pre-defined in SMF) or enforced (for those already successfully installed) due to SMF/UPF malfunction.   - RES_LIM: Indicate that the PCC rule could not be successfully installed (for those provisioned from PCF) or activated (for those pre-defined in SMF) or enforced (for those already successfully installed) due to a limitation of resources at the SMF/UPF.   - MAX_NR_QoS_FLOW: Indicate that the PCC rule could not be successfully installed (for those provisioned from PCF) or activated (for those pre-defined in SMF) or enforced (for those already successfully installed) due to the fact that the maximum number of QoS flows has been reached for the PDU session.   - MISS_FLOW_INFO: Indicate that the PCC rule could not be successfully installed or enforced because neither the \&quot;flowInfos\&quot; attribute nor the \&quot;appId\&quot; attribute is specified within the PccRule data structure by the PCF during the first install request of the PCC rule.   - RES_ALLO_FAIL: Indicate that the PCC rule could not be successfully installed or maintained since the QoS flow establishment/modification failed, or the QoS flow was released.   - UNSUCC_QOS_VAL: indicate that the QoS validation has failed or when Guaranteed Bandwidth &gt; Max-Requested-Bandwidth.   - INCOR_FLOW_INFO: Indicate that the PCC rule could not be successfully installed or modified at the SMF because the provided flow information is not supported by the network (e.g. the provided IP address(es) or Ipv6 prefix(es) do not correspond to an IP version applicable for the PDU session).   - PS_TO_CS_HAN: Indicate that the PCC rule could not be maintained because of PS to CS handover.   - APP_ID_ERR: Indicate that the rule could not be successfully installed or enforced because the Application Identifier is invalid, unknown, or not applicable to the application required for detection.   - NO_QOS_FLOW_BOUND: Indicate that there is no QoS flow which the SMF can bind the PCC rule(s) to.   - FILTER_RES: Indicate that the Flow Information within the \&quot;flowInfos\&quot; attribute cannot be handled by the SMF because any of the restrictions defined in subclause 5.4.2 of 3GPP TS 29.212 was not met.   - MISS_REDI_SER_ADDR: Indicate that the PCC rule could not be successfully installed or enforced at the SMF because there is no valid Redirect Server Address within the Traffic Control Data policy decision which the PCC rule refers to provided by the PCF and no preconfigured redirection address for this PCC rule at the SMF.   - CM_END_USER_SER_DENIED: Indicate that the charging system denied the service request due to service restrictions (e.g. terminate rating group) or limitations related to the end-user, for example the end-user&#39;s account could not cover the requested service.   - CM_CREDIT_CON_NOT_APP: Indicate that the charging system determined that the service can be granted to the end user but no further credit control is needed for the service (e.g. service is free of charge or is treated for offline charging).   - CM_AUTH_REJ: Indicate that the charging system denied the service request in order to terminate the service for which credit is requested.   - CM_USER_UNK: Indicate that the specified end user could not be found in the charging system.   - CM_RAT_FAILED: Indicate that the charging system cannot rate the service request due to insufficient rating input, incorrect AVP combination or due to an attribute or an attribute value that is not recognized or supported in the rating.   - UE_STA_SUSP: Indicates that the UE is in suspend state.
 */

#ifndef _OpenAPI_failure_code_H_
#define _OpenAPI_failure_code_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_failure_code_s OpenAPI_failure_code_t;
typedef struct OpenAPI_failure_code_s {
} OpenAPI_failure_code_t;

OpenAPI_failure_code_t *OpenAPI_failure_code_create(
    );
void OpenAPI_failure_code_free(OpenAPI_failure_code_t *failure_code);
OpenAPI_failure_code_t *OpenAPI_failure_code_parseFromJSON(cJSON *failure_codeJSON);
cJSON *OpenAPI_failure_code_convertToJSON(OpenAPI_failure_code_t *failure_code);
OpenAPI_failure_code_t *OpenAPI_failure_code_copy(OpenAPI_failure_code_t *dst, OpenAPI_failure_code_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_failure_code_H_ */


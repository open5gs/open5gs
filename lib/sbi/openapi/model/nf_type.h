/*
 * nf_type.h
 *
 * 
 */

#ifndef _OpenAPI_nf_type_H_
#define _OpenAPI_nf_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nf_type_NULL = 0, OpenAPI_nf_type_NRF, OpenAPI_nf_type_UDM, OpenAPI_nf_type_AMF, OpenAPI_nf_type_SMF, OpenAPI_nf_type_AUSF, OpenAPI_nf_type_NEF, OpenAPI_nf_type_PCF, OpenAPI_nf_type_SMSF, OpenAPI_nf_type_NSSF, OpenAPI_nf_type_UDR, OpenAPI_nf_type_LMF, OpenAPI_nf_type_GMLC, OpenAPI_nf_type_5G_EIR, OpenAPI_nf_type_SEPP, OpenAPI_nf_type_UPF, OpenAPI_nf_type_N3IWF, OpenAPI_nf_type_AF, OpenAPI_nf_type_UDSF, OpenAPI_nf_type_BSF, OpenAPI_nf_type_CHF, OpenAPI_nf_type_NWDAF, OpenAPI_nf_type_PCSCF, OpenAPI_nf_type_CBCF, OpenAPI_nf_type_HSS, OpenAPI_nf_type_UCMF, OpenAPI_nf_type_SOR_AF, OpenAPI_nf_type_SPAF, OpenAPI_nf_type_MME, OpenAPI_nf_type_SCSAS, OpenAPI_nf_type_SCEF, OpenAPI_nf_type_SCP, OpenAPI_nf_type_NSSAAF, OpenAPI_nf_type_ICSCF, OpenAPI_nf_type_SCSCF, OpenAPI_nf_type_DRA, OpenAPI_nf_type_IMS_AS, OpenAPI_nf_type_AANF, OpenAPI_nf_type__5G_DDNMF, OpenAPI_nf_type_NSACF, OpenAPI_nf_type_MFAF, OpenAPI_nf_type_EASDF, OpenAPI_nf_type_DCCF, OpenAPI_nf_type_MB_SMF, OpenAPI_nf_type_TSCTSF, OpenAPI_nf_type_ADRF, OpenAPI_nf_type_GBA_BSF, OpenAPI_nf_type_CEF, OpenAPI_nf_type_MB_UPF, OpenAPI_nf_type_NSWOF, OpenAPI_nf_type_PKMF, OpenAPI_nf_type_MNPF, OpenAPI_nf_type_SMS_GMSC, OpenAPI_nf_type_SMS_IWMSC, OpenAPI_nf_type_MBSF, OpenAPI_nf_type_MBSTF, OpenAPI_nf_type_PANF } OpenAPI_nf_type_e;

char* OpenAPI_nf_type_ToString(OpenAPI_nf_type_e nf_type);

OpenAPI_nf_type_e OpenAPI_nf_type_FromString(char* nf_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_type_H_ */


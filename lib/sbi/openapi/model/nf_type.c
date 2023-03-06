
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_type.h"

char* OpenAPI_nf_type_ToString(OpenAPI_nf_type_e nf_type)
{
    const char *nf_typeArray[] =  { "NULL", "NRF", "UDM", "AMF", "SMF", "AUSF", "NEF", "PCF", "SMSF", "NSSF", "UDR", "LMF", "GMLC", "5G_EIR", "SEPP", "UPF", "N3IWF", "AF", "UDSF", "BSF", "CHF", "NWDAF", "PCSCF", "CBCF", "HSS", "UCMF", "SOR_AF", "SPAF", "MME", "SCSAS", "SCEF", "SCP", "NSSAAF", "ICSCF", "SCSCF", "DRA", "IMS_AS", "AANF", "5G_DDNMF", "NSACF", "MFAF", "EASDF", "DCCF", "MB_SMF", "TSCTSF", "ADRF", "GBA_BSF", "CEF", "MB_UPF", "NSWOF", "PKMF", "MNPF", "SMS_GMSC", "SMS_IWMSC", "MBSF", "MBSTF", "PANF" };
    size_t sizeofArray = sizeof(nf_typeArray) / sizeof(nf_typeArray[0]);
    if (nf_type < sizeofArray)
        return (char *)nf_typeArray[nf_type];
    else
        return (char *)"Unknown";
}

OpenAPI_nf_type_e OpenAPI_nf_type_FromString(char* nf_type)
{
    int stringToReturn = 0;
    const char *nf_typeArray[] =  { "NULL", "NRF", "UDM", "AMF", "SMF", "AUSF", "NEF", "PCF", "SMSF", "NSSF", "UDR", "LMF", "GMLC", "5G_EIR", "SEPP", "UPF", "N3IWF", "AF", "UDSF", "BSF", "CHF", "NWDAF", "PCSCF", "CBCF", "HSS", "UCMF", "SOR_AF", "SPAF", "MME", "SCSAS", "SCEF", "SCP", "NSSAAF", "ICSCF", "SCSCF", "DRA", "IMS_AS", "AANF", "5G_DDNMF", "NSACF", "MFAF", "EASDF", "DCCF", "MB_SMF", "TSCTSF", "ADRF", "GBA_BSF", "CEF", "MB_UPF", "NSWOF", "PKMF", "MNPF", "SMS_GMSC", "SMS_IWMSC", "MBSF", "MBSTF", "PANF" };
    size_t sizeofArray = sizeof(nf_typeArray) / sizeof(nf_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(nf_type, nf_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}


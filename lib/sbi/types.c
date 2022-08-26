/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ogs-sbi.h"

struct service_desc_s {
    OpenAPI_nf_type_e nf_type;
    const char *service_name;
};

static struct service_desc_s service_desc[] = {
    { OpenAPI_nf_type_NULL, NULL },
    { OpenAPI_nf_type_NRF, OGS_SBI_SERVICE_NAME_NNRF_NFM },
    { OpenAPI_nf_type_NRF, OGS_SBI_SERVICE_NAME_NNRF_DISC },
    { OpenAPI_nf_type_NRF, OGS_SBI_SERVICE_NAME_NNRF_OAUTH2 },
    { OpenAPI_nf_type_UDM, OGS_SBI_SERVICE_NAME_NUDM_SDM },
    { OpenAPI_nf_type_UDM, OGS_SBI_SERVICE_NAME_NUDM_UECM },
    { OpenAPI_nf_type_UDM, OGS_SBI_SERVICE_NAME_NUDM_UEAU },
    { OpenAPI_nf_type_UDM, OGS_SBI_SERVICE_NAME_NUDM_EE },
    { OpenAPI_nf_type_UDM, OGS_SBI_SERVICE_NAME_NUDM_PP },
    { OpenAPI_nf_type_UDM, OGS_SBI_SERVICE_NAME_NUDM_NIDDAU },
    { OpenAPI_nf_type_UDM, OGS_SBI_SERVICE_NAME_NUDM_MT },
    { OpenAPI_nf_type_AMF, OGS_SBI_SERVICE_NAME_NAMF_COMM },
    { OpenAPI_nf_type_AMF, OGS_SBI_SERVICE_NAME_NAMF_EVTS },
    { OpenAPI_nf_type_AMF, OGS_SBI_SERVICE_NAME_NAMF_MT },
    { OpenAPI_nf_type_AMF, OGS_SBI_SERVICE_NAME_NAMF_LOC },
    { OpenAPI_nf_type_SMF, OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION },
    { OpenAPI_nf_type_SMF, OGS_SBI_SERVICE_NAME_NSMF_EVENT_EXPOSURE },
    { OpenAPI_nf_type_SMF, OGS_SBI_SERVICE_NAME_NSMF_NIDD },
    { OpenAPI_nf_type_AUSF, OGS_SBI_SERVICE_NAME_NAUSF_AUTH },
    { OpenAPI_nf_type_AUSF, OGS_SBI_SERVICE_NAME_NAUSF_SORPROTECTION },
    { OpenAPI_nf_type_AUSF, OGS_SBI_SERVICE_NAME_NAUSF_UPUPROTECTION },
    { OpenAPI_nf_type_NEF, OGS_SBI_SERVICE_NAME_NNEF_PFDMANAGEMENT },
    { OpenAPI_nf_type_NEF, OGS_SBI_SERVICE_NAME_NNEF_SMCONTEXT },
    { OpenAPI_nf_type_NEF, OGS_SBI_SERVICE_NAME_NNEF_EVENTEXPOSURE },
    { OpenAPI_nf_type_PCF, OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL },
    { OpenAPI_nf_type_PCF, OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL },
    { OpenAPI_nf_type_PCF, OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION },
    { OpenAPI_nf_type_PCF, OGS_SBI_SERVICE_NAME_NPCF_BDTPOLICYCONTROL },
    { OpenAPI_nf_type_PCF, OGS_SBI_SERVICE_NAME_NPCF_EVENTEXPOSURE },
    { OpenAPI_nf_type_PCF, OGS_SBI_SERVICE_NAME_NPCF_UE_POLICY_CONTROL },
    { OpenAPI_nf_type_SMSF, OGS_SBI_SERVICE_NAME_NSMSF_SMS },
    { OpenAPI_nf_type_NSSF, OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION },
    { OpenAPI_nf_type_NSSF, OGS_SBI_SERVICE_NAME_NNSSF_NSSAIAVAILABILITY },
    { OpenAPI_nf_type_UDR, OGS_SBI_SERVICE_NAME_NUDR_DR },
    { OpenAPI_nf_type_UDR, OGS_SBI_SERVICE_NAME_NUDR_GROUP_ID_MAP },
    { OpenAPI_nf_type_LMF, OGS_SBI_SERVICE_NAME_NLMF_LOC },
    { OpenAPI_nf_type_5G_EIR, OGS_SBI_SERVICE_NAME_N5G_EIR_EIC },
    { OpenAPI_nf_type_BSF, OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT },
    { OpenAPI_nf_type_CHF, OGS_SBI_SERVICE_NAME_NCHF_SPENDINGLIMITCONTROL },
    { OpenAPI_nf_type_CHF, OGS_SBI_SERVICE_NAME_NCHF_CONVERGEDCHARGING },
    { OpenAPI_nf_type_CHF, OGS_SBI_SERVICE_NAME_NCHF_OFFLINEONLYCHARGING },
    { OpenAPI_nf_type_NWDAF, OGS_SBI_SERVICE_NAME_NNWDAF_EVENTSSUBSCRIPTION },
    { OpenAPI_nf_type_NWDAF, OGS_SBI_SERVICE_NAME_NNWDAF_ANALYTICSINFO },
    { OpenAPI_nf_type_GMLC, OGS_SBI_SERVICE_NAME_NGMLC_LOC },
    { OpenAPI_nf_type_UCMF, OGS_SBI_SERVICE_NAME_NUCMF_PROVISIONING },
    { OpenAPI_nf_type_UCMF, OGS_SBI_SERVICE_NAME_NUCMF_UECAPABILITYMANAGEMENT },
    { OpenAPI_nf_type_HSS, OGS_SBI_SERVICE_NAME_NHSS_SDM },
    { OpenAPI_nf_type_HSS, OGS_SBI_SERVICE_NAME_NHSS_UECM },
    { OpenAPI_nf_type_HSS, OGS_SBI_SERVICE_NAME_NHSS_UEAU },
    { OpenAPI_nf_type_HSS, OGS_SBI_SERVICE_NAME_NHSS_EE },
    { OpenAPI_nf_type_HSS, OGS_SBI_SERVICE_NAME_NHSS_IMS_SDM },
    { OpenAPI_nf_type_HSS, OGS_SBI_SERVICE_NAME_NHSS_IMS_UECM },
    { OpenAPI_nf_type_HSS, OGS_SBI_SERVICE_NAME_NHSS_IMS_UEAU },
    { OpenAPI_nf_type_SEPP, OGS_SBI_SERVICE_NAME_NSEPP_TELESCOPIC },
    { OpenAPI_nf_type_SOR_AF, OGS_SBI_SERVICE_NAME_NSORAF_SOR },
    { OpenAPI_nf_type_SPAF, OGS_SBI_SERVICE_NAME_NSPAF_SECURED_PACKET },
    { OpenAPI_nf_type_UDSF, OGS_SBI_SERVICE_NAME_NUDSF_DR },
    { OpenAPI_nf_type_NSSAAF, OGS_SBI_SERVICE_NAME_NNSSAAF_NSSAA },
};



OpenAPI_nf_type_e ogs_sbi_service_type_to_nf_type(ogs_sbi_service_type_e type)
{
    ogs_assert(type > OGS_SBI_SERVICE_TYPE_NULL &&
                type < OGS_SBI_MAX_NUM_OF_SERVICE_TYPE);
    return service_desc[type].nf_type;
}

const char *ogs_sbi_service_type_to_name(ogs_sbi_service_type_e type)
{
    ogs_assert(type > OGS_SBI_SERVICE_TYPE_NULL &&
                type < OGS_SBI_MAX_NUM_OF_SERVICE_TYPE);
    ogs_assert(service_desc[type].service_name);
    return service_desc[type].service_name;
}

ogs_sbi_service_type_e ogs_sbi_service_type_from_name(const char *name)
{
    int i;

    ogs_assert(name);

    for (i = 0; i < OGS_SBI_MAX_NUM_OF_SERVICE_TYPE; i++) {
        if (service_desc[i].service_name &&
            strcmp(name, service_desc[i].service_name) == 0)
            return (ogs_sbi_service_type_e)i;
    }

    return OGS_SBI_SERVICE_TYPE_NULL;
}

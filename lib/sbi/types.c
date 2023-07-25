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

struct app_error_desc_s {
    const char *app_strerror;
};

static struct app_error_desc_s app_error_desc[] = {
    { NULL },
    { OGS_SBI_APP_STRERROR_N1_SM_ERROR },
    { OGS_SBI_APP_STRERROR_N2_SM_ERROR },
    { OGS_SBI_APP_STRERROR_SNSSAI_DENIED },
    { OGS_SBI_APP_STRERROR_DNN_DENIED },
    { OGS_SBI_APP_STRERROR_PDUTYPE_DENIED },
    { OGS_SBI_APP_STRERROR_SSC_DENIED },
    { OGS_SBI_APP_STRERROR_SUBSCRIPTION_DENIED },
    { OGS_SBI_APP_STRERROR_DNN_NOT_SUPPORTED },
    { OGS_SBI_APP_STRERROR_PDUTYPE_NOT_SUPPORTED },
    { OGS_SBI_APP_STRERROR_SSC_NOT_SUPPORTED },
    { OGS_SBI_APP_STRERROR_HOME_ROUTED_ROAMING_REQUIRED },
    { OGS_SBI_APP_STRERROR_OUT_OF_LADN_SERVICE_AREA },
    { OGS_SBI_APP_STRERROR_PRIORITIZED_SERVICES_ONLY },
    { OGS_SBI_APP_STRERROR_PDU_SESSION_ANCHOR_CHANGE },
    { OGS_SBI_APP_STRERROR_TARGET_MME_CAPABILITY },
    { OGS_SBI_APP_STRERROR_NO_EPS_5GS_CONTINUITY },
    { OGS_SBI_APP_STRERROR_UNABLE_TO_PAGE_UE },
    { OGS_SBI_APP_STRERROR_UE_NOT_RESPONDING },
    { OGS_SBI_APP_STRERROR_REJECTED_BY_UE },
    { OGS_SBI_APP_STRERROR_REJECTED_DUE_VPLMN_POLICY },
    { OGS_SBI_APP_STRERROR_HO_TAU_IN_PROGRESS },
    { OGS_SBI_APP_STRERROR_INTEGRITY_PROTECTED_MDR_NOT_ACCEPTABLE },
    { OGS_SBI_APP_STRERROR_EBI_EXHAUSTED },
    { OGS_SBI_APP_STRERROR_EBI_REJECTED_LOCAL_POLICY },
    { OGS_SBI_APP_STRERROR_EBI_REJECTED_NO_N26 },
    { OGS_SBI_APP_STRERROR_DEFAULT_EPS_BEARER_INACTIVE },
    { OGS_SBI_APP_STRERROR_HANDOVER_RESOURCE_ALLOCATION_FAILURE },
    { OGS_SBI_APP_STRERROR_LATE_OVERLAPPING_REQUEST },
    { OGS_SBI_APP_STRERROR_DEFAULT_EBI_NOT_TRANSFERRED },
    { OGS_SBI_APP_STRERROR_NOT_SUPPORTED_WITH_ISMF },
    { OGS_SBI_APP_STRERROR_SERVICE_NOT_AUTHORIZED_BY_NEXT_HOP },
    { OGS_SBI_APP_STRERROR_NO_DATA_FORWARDING },
    { OGS_SBI_APP_STRERROR_S_NSSAI_UNAVAILABLE_DUE_TO_NSAC },
    { OGS_SBI_APP_STRERROR_EXCEEDED_UE_SLICE_DATA_RATE },
    { OGS_SBI_APP_STRERROR_EXCEEDED_SLICE_DATA_RATE },
    { OGS_SBI_APP_STRERROR_CONTEXT_NOT_FOUND },
    { OGS_SBI_APP_STRERROR_HIGHER_PRIORITY_REQUEST_ONGOING },
    { OGS_SBI_APP_STRERROR_UE_IN_CM_IDLE_STATE },
    { OGS_SBI_APP_STRERROR_INSUFFICIENT_RESOURCES_SLICE },
    { OGS_SBI_APP_STRERROR_INSUFFICIENT_RESOURCES_SLICE_DNN },
    { OGS_SBI_APP_STRERROR_DNN_CONGESTION },
    { OGS_SBI_APP_STRERROR_S_NSSAI_CONGESTION },
    { OGS_SBI_APP_STRERROR_PEER_NOT_RESPONDING },
    { OGS_SBI_APP_STRERROR_NETWORK_FAILURE },
    { OGS_SBI_APP_STRERROR_UPF_NOT_RESPONDING },
    { OGS_SBI_APP_STRERROR_UE_NOT_REACHABLE },
};

const char *ogs_sbi_app_strerror(ogs_sbi_app_errno_e err)
{
    ogs_assert(err > OGS_SBI_APP_ERRNO_NULL &&
                err < OGS_SBI_MAX_NUM_OF_APP_ERRNO);
    ogs_assert(app_error_desc[err].app_strerror);
    return app_error_desc[err].app_strerror;
}

ogs_sbi_app_errno_e ogs_sbi_app_errno(const char *str)
{
    int i;

    ogs_assert(str);

    for (i = 0; i < OGS_SBI_MAX_NUM_OF_APP_ERRNO; i++) {
        if (app_error_desc[i].app_strerror &&
            strcmp(str, app_error_desc[i].app_strerror) == 0)
            return (ogs_sbi_app_errno_e)i;
    }

    return OGS_SBI_APP_ERRNO_NULL;
}

/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

static const struct {
    const char *name;
    int service_name_id;
} service_name_map[] = {
    { OGS_SBI_SERVICE_NAME_NAMF_CALLBACK,
        OGS_SBI_SERVICE_NAME_ID_NAMF_CALLBACK },
    { OGS_SBI_SERVICE_NAME_NSMF_CALLBACK,
        OGS_SBI_SERVICE_NAME_ID_NSMF_CALLBACK },
    { OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE,
        OGS_SBI_SERVICE_NAME_ID_N32C_HANDSHAKE },
};

int ogs_sbi_service_name_id_from_string(const char *service_name)
{
    OpenAPI_service_name_e parsed = OpenAPI_service_name_NULL;
    const char *canonical = NULL;
    int i;

    ogs_assert(service_name);

    for (i = 0; i < OGS_ARRAY_SIZE(service_name_map); i++) {
        if (!strcmp(service_name, service_name_map[i].name))
            return service_name_map[i].service_name_id;
    }

    parsed = OpenAPI_service_name_FromString((char *)service_name);
    canonical = OpenAPI_service_name_ToString(parsed);

    if (!canonical || strcmp(service_name, canonical) != 0)
        return OpenAPI_service_name_NULL;

    return parsed;
}

static OpenAPI_nf_type_e nf_type_from_special_prefix(const char *prefix)
{
    ogs_assert(prefix);

    if (strcmp(prefix, "nsoraf") == 0)
        return OpenAPI_nf_type_SOR_AF;
    if (strcmp(prefix, "n5gddnmf") == 0)
        return OpenAPI_nf_type__5G_DDNMF;
    if (strcmp(prefix, "nmbsmf") == 0)
        return OpenAPI_nf_type_MB_SMF;
    if (strcmp(prefix, "nbsp") == 0)
        return OpenAPI_nf_type_GBA_BSF;
    if (strcmp(prefix, "niwmsc") == 0)
        return OpenAPI_nf_type_SMS_IWMSC;

    return OpenAPI_nf_type_NULL;
}

static OpenAPI_nf_type_e nf_type_from_prefix(const char *prefix)
{
    OpenAPI_nf_type_e nf_type;
    char normalized[32];
    int i, j = 0;

    ogs_assert(prefix);

    /*
     * Generic normalization:
     *
     *   nnrf      -> NRF
     *   nudm      -> UDM
     *   namf      -> AMF
     *   n5g-eir   -> 5G_EIR
     *   nnssaaf   -> NSSAAF
     *
     * Rules:
     *  - remove leading 'n'
     *  - convert to upper-case
     *  - convert '-' to '_'
     */
    i = 0;
    if (prefix[0] == 'n' || prefix[0] == 'N')
        i++;

    while (prefix[i] && j < (int)sizeof(normalized) - 1) {
        char c = prefix[i++];

        if (c == '-')
            normalized[j++] = '_';
        else
            normalized[j++] = toupper((unsigned char)c);
    }
    normalized[j] = '\0';

    for (nf_type = OpenAPI_nf_type_NULL + 1;
            nf_type <= OpenAPI_nf_type_PANF;
            nf_type++) {
        const char *name = OpenAPI_nf_type_ToString(nf_type);

        if (!name)
            continue;

        if (strcmp(normalized, name) == 0)
            return nf_type;
    }

    return nf_type_from_special_prefix(prefix);
}

OpenAPI_nf_type_e ogs_sbi_service_name_to_nf_type(
        OpenAPI_service_name_e service_name)
{
    const char *name;
    const char *p;
    char prefix[32];
    size_t len;
    OpenAPI_nf_type_e nf_type;

    name = OpenAPI_service_name_ToString(service_name);
    if (!name)
        return OpenAPI_nf_type_NULL;

    if (strcmp(name, "NULL") == 0 || strcmp(name, "Unknown") == 0)
        return OpenAPI_nf_type_NULL;

    /*
     * Try each hyphen-delimited prefix progressively.
     *
     * Examples:
     *   nnrf-nfm          -> "nnrf"
     *   namf-comm         -> "namf"
     *   n5g-eir-eic       -> "n5g", then "n5g-eir"
     *   niwmsc-smservice  -> "niwmsc"
     */
    for (p = name; *p; p++) {
        if (*p != '-')
            continue;

        len = p - name;
        if (len == 0 || len >= sizeof(prefix))
            continue;

        memcpy(prefix, name, len);
        prefix[len] = '\0';

        nf_type = nf_type_from_prefix(prefix);
        if (nf_type != OpenAPI_nf_type_NULL)
            return nf_type;
    }

    return OpenAPI_nf_type_NULL;
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

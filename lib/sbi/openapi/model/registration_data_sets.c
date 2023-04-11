
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "registration_data_sets.h"

OpenAPI_registration_data_sets_t *OpenAPI_registration_data_sets_create(
    OpenAPI_amf3_gpp_access_registration_t *amf3_gpp,
    OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp,
    OpenAPI_smf_registration_info_t *smf_registration,
    OpenAPI_smsf_registration_t *smsf3_gpp,
    OpenAPI_smsf_registration_t *smsf_non3_gpp,
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw,
    OpenAPI_nwdaf_registration_info_t *nwdaf_registration
)
{
    OpenAPI_registration_data_sets_t *registration_data_sets_local_var = ogs_malloc(sizeof(OpenAPI_registration_data_sets_t));
    ogs_assert(registration_data_sets_local_var);

    registration_data_sets_local_var->amf3_gpp = amf3_gpp;
    registration_data_sets_local_var->amf_non3_gpp = amf_non3_gpp;
    registration_data_sets_local_var->smf_registration = smf_registration;
    registration_data_sets_local_var->smsf3_gpp = smsf3_gpp;
    registration_data_sets_local_var->smsf_non3_gpp = smsf_non3_gpp;
    registration_data_sets_local_var->ip_sm_gw = ip_sm_gw;
    registration_data_sets_local_var->nwdaf_registration = nwdaf_registration;

    return registration_data_sets_local_var;
}

void OpenAPI_registration_data_sets_free(OpenAPI_registration_data_sets_t *registration_data_sets)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == registration_data_sets) {
        return;
    }
    if (registration_data_sets->amf3_gpp) {
        OpenAPI_amf3_gpp_access_registration_free(registration_data_sets->amf3_gpp);
        registration_data_sets->amf3_gpp = NULL;
    }
    if (registration_data_sets->amf_non3_gpp) {
        OpenAPI_amf_non3_gpp_access_registration_free(registration_data_sets->amf_non3_gpp);
        registration_data_sets->amf_non3_gpp = NULL;
    }
    if (registration_data_sets->smf_registration) {
        OpenAPI_smf_registration_info_free(registration_data_sets->smf_registration);
        registration_data_sets->smf_registration = NULL;
    }
    if (registration_data_sets->smsf3_gpp) {
        OpenAPI_smsf_registration_free(registration_data_sets->smsf3_gpp);
        registration_data_sets->smsf3_gpp = NULL;
    }
    if (registration_data_sets->smsf_non3_gpp) {
        OpenAPI_smsf_registration_free(registration_data_sets->smsf_non3_gpp);
        registration_data_sets->smsf_non3_gpp = NULL;
    }
    if (registration_data_sets->ip_sm_gw) {
        OpenAPI_ip_sm_gw_registration_free(registration_data_sets->ip_sm_gw);
        registration_data_sets->ip_sm_gw = NULL;
    }
    if (registration_data_sets->nwdaf_registration) {
        OpenAPI_nwdaf_registration_info_free(registration_data_sets->nwdaf_registration);
        registration_data_sets->nwdaf_registration = NULL;
    }
    ogs_free(registration_data_sets);
}

cJSON *OpenAPI_registration_data_sets_convertToJSON(OpenAPI_registration_data_sets_t *registration_data_sets)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (registration_data_sets == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [RegistrationDataSets]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (registration_data_sets->amf3_gpp) {
    cJSON *amf3_gpp_local_JSON = OpenAPI_amf3_gpp_access_registration_convertToJSON(registration_data_sets->amf3_gpp);
    if (amf3_gpp_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [amf3_gpp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "amf3Gpp", amf3_gpp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [amf3_gpp]");
        goto end;
    }
    }

    if (registration_data_sets->amf_non3_gpp) {
    cJSON *amf_non3_gpp_local_JSON = OpenAPI_amf_non3_gpp_access_registration_convertToJSON(registration_data_sets->amf_non3_gpp);
    if (amf_non3_gpp_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [amf_non3_gpp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "amfNon3Gpp", amf_non3_gpp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [amf_non3_gpp]");
        goto end;
    }
    }

    if (registration_data_sets->smf_registration) {
    cJSON *smf_registration_local_JSON = OpenAPI_smf_registration_info_convertToJSON(registration_data_sets->smf_registration);
    if (smf_registration_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [smf_registration]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smfRegistration", smf_registration_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [smf_registration]");
        goto end;
    }
    }

    if (registration_data_sets->smsf3_gpp) {
    cJSON *smsf3_gpp_local_JSON = OpenAPI_smsf_registration_convertToJSON(registration_data_sets->smsf3_gpp);
    if (smsf3_gpp_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [smsf3_gpp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsf3Gpp", smsf3_gpp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [smsf3_gpp]");
        goto end;
    }
    }

    if (registration_data_sets->smsf_non3_gpp) {
    cJSON *smsf_non3_gpp_local_JSON = OpenAPI_smsf_registration_convertToJSON(registration_data_sets->smsf_non3_gpp);
    if (smsf_non3_gpp_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [smsf_non3_gpp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsfNon3Gpp", smsf_non3_gpp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [smsf_non3_gpp]");
        goto end;
    }
    }

    if (registration_data_sets->ip_sm_gw) {
    cJSON *ip_sm_gw_local_JSON = OpenAPI_ip_sm_gw_registration_convertToJSON(registration_data_sets->ip_sm_gw);
    if (ip_sm_gw_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [ip_sm_gw]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ipSmGw", ip_sm_gw_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [ip_sm_gw]");
        goto end;
    }
    }

    if (registration_data_sets->nwdaf_registration) {
    cJSON *nwdaf_registration_local_JSON = OpenAPI_nwdaf_registration_info_convertToJSON(registration_data_sets->nwdaf_registration);
    if (nwdaf_registration_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [nwdaf_registration]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nwdafRegistration", nwdaf_registration_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed [nwdaf_registration]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_registration_data_sets_t *OpenAPI_registration_data_sets_parseFromJSON(cJSON *registration_data_setsJSON)
{
    OpenAPI_registration_data_sets_t *registration_data_sets_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *amf3_gpp = NULL;
    OpenAPI_amf3_gpp_access_registration_t *amf3_gpp_local_nonprim = NULL;
    cJSON *amf_non3_gpp = NULL;
    OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp_local_nonprim = NULL;
    cJSON *smf_registration = NULL;
    OpenAPI_smf_registration_info_t *smf_registration_local_nonprim = NULL;
    cJSON *smsf3_gpp = NULL;
    OpenAPI_smsf_registration_t *smsf3_gpp_local_nonprim = NULL;
    cJSON *smsf_non3_gpp = NULL;
    OpenAPI_smsf_registration_t *smsf_non3_gpp_local_nonprim = NULL;
    cJSON *ip_sm_gw = NULL;
    OpenAPI_ip_sm_gw_registration_t *ip_sm_gw_local_nonprim = NULL;
    cJSON *nwdaf_registration = NULL;
    OpenAPI_nwdaf_registration_info_t *nwdaf_registration_local_nonprim = NULL;
    amf3_gpp = cJSON_GetObjectItemCaseSensitive(registration_data_setsJSON, "amf3Gpp");
    if (amf3_gpp) {
    amf3_gpp_local_nonprim = OpenAPI_amf3_gpp_access_registration_parseFromJSON(amf3_gpp);
    if (!amf3_gpp_local_nonprim) {
        ogs_error("OpenAPI_amf3_gpp_access_registration_parseFromJSON failed [amf3_gpp]");
        goto end;
    }
    }

    amf_non3_gpp = cJSON_GetObjectItemCaseSensitive(registration_data_setsJSON, "amfNon3Gpp");
    if (amf_non3_gpp) {
    amf_non3_gpp_local_nonprim = OpenAPI_amf_non3_gpp_access_registration_parseFromJSON(amf_non3_gpp);
    if (!amf_non3_gpp_local_nonprim) {
        ogs_error("OpenAPI_amf_non3_gpp_access_registration_parseFromJSON failed [amf_non3_gpp]");
        goto end;
    }
    }

    smf_registration = cJSON_GetObjectItemCaseSensitive(registration_data_setsJSON, "smfRegistration");
    if (smf_registration) {
    smf_registration_local_nonprim = OpenAPI_smf_registration_info_parseFromJSON(smf_registration);
    if (!smf_registration_local_nonprim) {
        ogs_error("OpenAPI_smf_registration_info_parseFromJSON failed [smf_registration]");
        goto end;
    }
    }

    smsf3_gpp = cJSON_GetObjectItemCaseSensitive(registration_data_setsJSON, "smsf3Gpp");
    if (smsf3_gpp) {
    smsf3_gpp_local_nonprim = OpenAPI_smsf_registration_parseFromJSON(smsf3_gpp);
    if (!smsf3_gpp_local_nonprim) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON failed [smsf3_gpp]");
        goto end;
    }
    }

    smsf_non3_gpp = cJSON_GetObjectItemCaseSensitive(registration_data_setsJSON, "smsfNon3Gpp");
    if (smsf_non3_gpp) {
    smsf_non3_gpp_local_nonprim = OpenAPI_smsf_registration_parseFromJSON(smsf_non3_gpp);
    if (!smsf_non3_gpp_local_nonprim) {
        ogs_error("OpenAPI_smsf_registration_parseFromJSON failed [smsf_non3_gpp]");
        goto end;
    }
    }

    ip_sm_gw = cJSON_GetObjectItemCaseSensitive(registration_data_setsJSON, "ipSmGw");
    if (ip_sm_gw) {
    ip_sm_gw_local_nonprim = OpenAPI_ip_sm_gw_registration_parseFromJSON(ip_sm_gw);
    if (!ip_sm_gw_local_nonprim) {
        ogs_error("OpenAPI_ip_sm_gw_registration_parseFromJSON failed [ip_sm_gw]");
        goto end;
    }
    }

    nwdaf_registration = cJSON_GetObjectItemCaseSensitive(registration_data_setsJSON, "nwdafRegistration");
    if (nwdaf_registration) {
    nwdaf_registration_local_nonprim = OpenAPI_nwdaf_registration_info_parseFromJSON(nwdaf_registration);
    if (!nwdaf_registration_local_nonprim) {
        ogs_error("OpenAPI_nwdaf_registration_info_parseFromJSON failed [nwdaf_registration]");
        goto end;
    }
    }

    registration_data_sets_local_var = OpenAPI_registration_data_sets_create (
        amf3_gpp ? amf3_gpp_local_nonprim : NULL,
        amf_non3_gpp ? amf_non3_gpp_local_nonprim : NULL,
        smf_registration ? smf_registration_local_nonprim : NULL,
        smsf3_gpp ? smsf3_gpp_local_nonprim : NULL,
        smsf_non3_gpp ? smsf_non3_gpp_local_nonprim : NULL,
        ip_sm_gw ? ip_sm_gw_local_nonprim : NULL,
        nwdaf_registration ? nwdaf_registration_local_nonprim : NULL
    );

    return registration_data_sets_local_var;
end:
    if (amf3_gpp_local_nonprim) {
        OpenAPI_amf3_gpp_access_registration_free(amf3_gpp_local_nonprim);
        amf3_gpp_local_nonprim = NULL;
    }
    if (amf_non3_gpp_local_nonprim) {
        OpenAPI_amf_non3_gpp_access_registration_free(amf_non3_gpp_local_nonprim);
        amf_non3_gpp_local_nonprim = NULL;
    }
    if (smf_registration_local_nonprim) {
        OpenAPI_smf_registration_info_free(smf_registration_local_nonprim);
        smf_registration_local_nonprim = NULL;
    }
    if (smsf3_gpp_local_nonprim) {
        OpenAPI_smsf_registration_free(smsf3_gpp_local_nonprim);
        smsf3_gpp_local_nonprim = NULL;
    }
    if (smsf_non3_gpp_local_nonprim) {
        OpenAPI_smsf_registration_free(smsf_non3_gpp_local_nonprim);
        smsf_non3_gpp_local_nonprim = NULL;
    }
    if (ip_sm_gw_local_nonprim) {
        OpenAPI_ip_sm_gw_registration_free(ip_sm_gw_local_nonprim);
        ip_sm_gw_local_nonprim = NULL;
    }
    if (nwdaf_registration_local_nonprim) {
        OpenAPI_nwdaf_registration_info_free(nwdaf_registration_local_nonprim);
        nwdaf_registration_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_registration_data_sets_t *OpenAPI_registration_data_sets_copy(OpenAPI_registration_data_sets_t *dst, OpenAPI_registration_data_sets_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_registration_data_sets_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_registration_data_sets_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_registration_data_sets_free(dst);
    dst = OpenAPI_registration_data_sets_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


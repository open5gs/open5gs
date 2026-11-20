
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsn_charging_information.h"

OpenAPI_tsn_charging_information_t *OpenAPI_tsn_charging_information_create(
    char *d_nn,
    OpenAPI_snssai_t *s_nssai,
    char *internal_group_identifier,
    OpenAPI_list_t *external_individual_id_list,
    OpenAPI_model_5_gs_bridge_information_t *_5_gs_bridge_information,
    OpenAPI_tsnqo_s_information_t *t_snqo_s_information,
    OpenAPI_charging_tsc_assistance_information_t *t_sc_assistance_information,
    OpenAPI_time_synchronization_information_t *time_synchronization_information
)
{
    OpenAPI_tsn_charging_information_t *tsn_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_tsn_charging_information_t));
    ogs_assert(tsn_charging_information_local_var);

    tsn_charging_information_local_var->d_nn = d_nn;
    tsn_charging_information_local_var->s_nssai = s_nssai;
    tsn_charging_information_local_var->internal_group_identifier = internal_group_identifier;
    tsn_charging_information_local_var->external_individual_id_list = external_individual_id_list;
    tsn_charging_information_local_var->_5_gs_bridge_information = _5_gs_bridge_information;
    tsn_charging_information_local_var->t_snqo_s_information = t_snqo_s_information;
    tsn_charging_information_local_var->t_sc_assistance_information = t_sc_assistance_information;
    tsn_charging_information_local_var->time_synchronization_information = time_synchronization_information;

    return tsn_charging_information_local_var;
}

void OpenAPI_tsn_charging_information_free(OpenAPI_tsn_charging_information_t *tsn_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tsn_charging_information) {
        return;
    }
    if (tsn_charging_information->d_nn) {
        ogs_free(tsn_charging_information->d_nn);
        tsn_charging_information->d_nn = NULL;
    }
    if (tsn_charging_information->s_nssai) {
        OpenAPI_snssai_free(tsn_charging_information->s_nssai);
        tsn_charging_information->s_nssai = NULL;
    }
    if (tsn_charging_information->internal_group_identifier) {
        ogs_free(tsn_charging_information->internal_group_identifier);
        tsn_charging_information->internal_group_identifier = NULL;
    }
    if (tsn_charging_information->external_individual_id_list) {
        OpenAPI_list_for_each(tsn_charging_information->external_individual_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(tsn_charging_information->external_individual_id_list);
        tsn_charging_information->external_individual_id_list = NULL;
    }
    if (tsn_charging_information->_5_gs_bridge_information) {
        OpenAPI_model_5_gs_bridge_information_free(tsn_charging_information->_5_gs_bridge_information);
        tsn_charging_information->_5_gs_bridge_information = NULL;
    }
    if (tsn_charging_information->t_snqo_s_information) {
        OpenAPI_tsnqo_s_information_free(tsn_charging_information->t_snqo_s_information);
        tsn_charging_information->t_snqo_s_information = NULL;
    }
    if (tsn_charging_information->t_sc_assistance_information) {
        OpenAPI_charging_tsc_assistance_information_free(tsn_charging_information->t_sc_assistance_information);
        tsn_charging_information->t_sc_assistance_information = NULL;
    }
    if (tsn_charging_information->time_synchronization_information) {
        OpenAPI_time_synchronization_information_free(tsn_charging_information->time_synchronization_information);
        tsn_charging_information->time_synchronization_information = NULL;
    }
    ogs_free(tsn_charging_information);
}

cJSON *OpenAPI_tsn_charging_information_convertToJSON(OpenAPI_tsn_charging_information_t *tsn_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tsn_charging_information == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [TSNChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tsn_charging_information->d_nn) {
    if (cJSON_AddStringToObject(item, "dNN", tsn_charging_information->d_nn) == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [d_nn]");
        goto end;
    }
    }

    if (tsn_charging_information->s_nssai) {
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(tsn_charging_information->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNSSAI", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [s_nssai]");
        goto end;
    }
    }

    if (tsn_charging_information->internal_group_identifier) {
    if (cJSON_AddStringToObject(item, "internalGroupIdentifier", tsn_charging_information->internal_group_identifier) == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [internal_group_identifier]");
        goto end;
    }
    }

    if (tsn_charging_information->external_individual_id_list) {
    cJSON *external_individual_id_listList = cJSON_AddArrayToObject(item, "externalIndividualIdList");
    if (external_individual_id_listList == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [external_individual_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(tsn_charging_information->external_individual_id_list, node) {
        if (cJSON_AddStringToObject(external_individual_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [external_individual_id_list]");
            goto end;
        }
    }
    }

    if (tsn_charging_information->_5_gs_bridge_information) {
    cJSON *_5_gs_bridge_information_local_JSON = OpenAPI_model_5_gs_bridge_information_convertToJSON(tsn_charging_information->_5_gs_bridge_information);
    if (_5_gs_bridge_information_local_JSON == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [_5_gs_bridge_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "5GSBridgeInformation", _5_gs_bridge_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [_5_gs_bridge_information]");
        goto end;
    }
    }

    if (tsn_charging_information->t_snqo_s_information) {
    cJSON *t_snqo_s_information_local_JSON = OpenAPI_tsnqo_s_information_convertToJSON(tsn_charging_information->t_snqo_s_information);
    if (t_snqo_s_information_local_JSON == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [t_snqo_s_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tSNQoSInformation", t_snqo_s_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [t_snqo_s_information]");
        goto end;
    }
    }

    if (tsn_charging_information->t_sc_assistance_information) {
    cJSON *t_sc_assistance_information_local_JSON = OpenAPI_charging_tsc_assistance_information_convertToJSON(tsn_charging_information->t_sc_assistance_information);
    if (t_sc_assistance_information_local_JSON == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [t_sc_assistance_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tSCAssistanceInformation", t_sc_assistance_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [t_sc_assistance_information]");
        goto end;
    }
    }

    if (tsn_charging_information->time_synchronization_information) {
    cJSON *time_synchronization_information_local_JSON = OpenAPI_time_synchronization_information_convertToJSON(tsn_charging_information->time_synchronization_information);
    if (time_synchronization_information_local_JSON == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [time_synchronization_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "timeSynchronizationInformation", time_synchronization_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed [time_synchronization_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tsn_charging_information_t *OpenAPI_tsn_charging_information_parseFromJSON(cJSON *tsn_charging_informationJSON)
{
    OpenAPI_tsn_charging_information_t *tsn_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *d_nn = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *internal_group_identifier = NULL;
    cJSON *external_individual_id_list = NULL;
    OpenAPI_list_t *external_individual_id_listList = NULL;
    cJSON *_5_gs_bridge_information = NULL;
    OpenAPI_model_5_gs_bridge_information_t *_5_gs_bridge_information_local_nonprim = NULL;
    cJSON *t_snqo_s_information = NULL;
    OpenAPI_tsnqo_s_information_t *t_snqo_s_information_local_nonprim = NULL;
    cJSON *t_sc_assistance_information = NULL;
    OpenAPI_charging_tsc_assistance_information_t *t_sc_assistance_information_local_nonprim = NULL;
    cJSON *time_synchronization_information = NULL;
    OpenAPI_time_synchronization_information_t *time_synchronization_information_local_nonprim = NULL;
    d_nn = cJSON_GetObjectItemCaseSensitive(tsn_charging_informationJSON, "dNN");
    if (d_nn) {
    if (!cJSON_IsString(d_nn) && !cJSON_IsNull(d_nn)) {
        ogs_error("OpenAPI_tsn_charging_information_parseFromJSON() failed [d_nn]");
        goto end;
    }
    }

    s_nssai = cJSON_GetObjectItemCaseSensitive(tsn_charging_informationJSON, "sNSSAI");
    if (s_nssai) {
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }
    }

    internal_group_identifier = cJSON_GetObjectItemCaseSensitive(tsn_charging_informationJSON, "internalGroupIdentifier");
    if (internal_group_identifier) {
    if (!cJSON_IsString(internal_group_identifier) && !cJSON_IsNull(internal_group_identifier)) {
        ogs_error("OpenAPI_tsn_charging_information_parseFromJSON() failed [internal_group_identifier]");
        goto end;
    }
    }

    external_individual_id_list = cJSON_GetObjectItemCaseSensitive(tsn_charging_informationJSON, "externalIndividualIdList");
    if (external_individual_id_list) {
        cJSON *external_individual_id_list_local = NULL;
        if (!cJSON_IsArray(external_individual_id_list)) {
            ogs_error("OpenAPI_tsn_charging_information_parseFromJSON() failed [external_individual_id_list]");
            goto end;
        }

        external_individual_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(external_individual_id_list_local, external_individual_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(external_individual_id_list_local)) {
                ogs_error("OpenAPI_tsn_charging_information_parseFromJSON() failed [external_individual_id_list]");
                goto end;
            }
            OpenAPI_list_add(external_individual_id_listList, ogs_strdup(external_individual_id_list_local->valuestring));
        }
    }

    _5_gs_bridge_information = cJSON_GetObjectItemCaseSensitive(tsn_charging_informationJSON, "5GSBridgeInformation");
    if (_5_gs_bridge_information) {
    _5_gs_bridge_information_local_nonprim = OpenAPI_model_5_gs_bridge_information_parseFromJSON(_5_gs_bridge_information);
    if (!_5_gs_bridge_information_local_nonprim) {
        ogs_error("OpenAPI_model_5_gs_bridge_information_parseFromJSON failed [_5_gs_bridge_information]");
        goto end;
    }
    }

    t_snqo_s_information = cJSON_GetObjectItemCaseSensitive(tsn_charging_informationJSON, "tSNQoSInformation");
    if (t_snqo_s_information) {
    t_snqo_s_information_local_nonprim = OpenAPI_tsnqo_s_information_parseFromJSON(t_snqo_s_information);
    if (!t_snqo_s_information_local_nonprim) {
        ogs_error("OpenAPI_tsnqo_s_information_parseFromJSON failed [t_snqo_s_information]");
        goto end;
    }
    }

    t_sc_assistance_information = cJSON_GetObjectItemCaseSensitive(tsn_charging_informationJSON, "tSCAssistanceInformation");
    if (t_sc_assistance_information) {
    t_sc_assistance_information_local_nonprim = OpenAPI_charging_tsc_assistance_information_parseFromJSON(t_sc_assistance_information);
    if (!t_sc_assistance_information_local_nonprim) {
        ogs_error("OpenAPI_charging_tsc_assistance_information_parseFromJSON failed [t_sc_assistance_information]");
        goto end;
    }
    }

    time_synchronization_information = cJSON_GetObjectItemCaseSensitive(tsn_charging_informationJSON, "timeSynchronizationInformation");
    if (time_synchronization_information) {
    time_synchronization_information_local_nonprim = OpenAPI_time_synchronization_information_parseFromJSON(time_synchronization_information);
    if (!time_synchronization_information_local_nonprim) {
        ogs_error("OpenAPI_time_synchronization_information_parseFromJSON failed [time_synchronization_information]");
        goto end;
    }
    }

    tsn_charging_information_local_var = OpenAPI_tsn_charging_information_create (
        d_nn && !cJSON_IsNull(d_nn) ? ogs_strdup(d_nn->valuestring) : NULL,
        s_nssai ? s_nssai_local_nonprim : NULL,
        internal_group_identifier && !cJSON_IsNull(internal_group_identifier) ? ogs_strdup(internal_group_identifier->valuestring) : NULL,
        external_individual_id_list ? external_individual_id_listList : NULL,
        _5_gs_bridge_information ? _5_gs_bridge_information_local_nonprim : NULL,
        t_snqo_s_information ? t_snqo_s_information_local_nonprim : NULL,
        t_sc_assistance_information ? t_sc_assistance_information_local_nonprim : NULL,
        time_synchronization_information ? time_synchronization_information_local_nonprim : NULL
    );

    return tsn_charging_information_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    if (external_individual_id_listList) {
        OpenAPI_list_for_each(external_individual_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(external_individual_id_listList);
        external_individual_id_listList = NULL;
    }
    if (_5_gs_bridge_information_local_nonprim) {
        OpenAPI_model_5_gs_bridge_information_free(_5_gs_bridge_information_local_nonprim);
        _5_gs_bridge_information_local_nonprim = NULL;
    }
    if (t_snqo_s_information_local_nonprim) {
        OpenAPI_tsnqo_s_information_free(t_snqo_s_information_local_nonprim);
        t_snqo_s_information_local_nonprim = NULL;
    }
    if (t_sc_assistance_information_local_nonprim) {
        OpenAPI_charging_tsc_assistance_information_free(t_sc_assistance_information_local_nonprim);
        t_sc_assistance_information_local_nonprim = NULL;
    }
    if (time_synchronization_information_local_nonprim) {
        OpenAPI_time_synchronization_information_free(time_synchronization_information_local_nonprim);
        time_synchronization_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_tsn_charging_information_t *OpenAPI_tsn_charging_information_copy(OpenAPI_tsn_charging_information_t *dst, OpenAPI_tsn_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tsn_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tsn_charging_information_convertToJSON() failed");
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

    OpenAPI_tsn_charging_information_free(dst);
    dst = OpenAPI_tsn_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


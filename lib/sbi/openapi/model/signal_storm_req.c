
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "signal_storm_req.h"

OpenAPI_signal_storm_req_t *OpenAPI_signal_storm_req_create(
    OpenAPI_list_t *tgt_nf_instance_ids,
    OpenAPI_list_t *tgt_nf_set_ids,
    OpenAPI_list_t *int_group_ids,
    OpenAPI_list_t *exter_group_ids,
    OpenAPI_list_t *supis,
    OpenAPI_list_t *gpsis,
    OpenAPI_list_t *tgt_cause_ids,
    bool is_thr_period,
    int thr_period,
    bool is_sig_freq_thr,
    int sig_freq_thr,
    bool is_ue_req_thr,
    int ue_req_thr,
    bool is_ue_num_thr,
    int ue_num_thr
)
{
    OpenAPI_signal_storm_req_t *signal_storm_req_local_var = ogs_malloc(sizeof(OpenAPI_signal_storm_req_t));
    ogs_assert(signal_storm_req_local_var);

    signal_storm_req_local_var->tgt_nf_instance_ids = tgt_nf_instance_ids;
    signal_storm_req_local_var->tgt_nf_set_ids = tgt_nf_set_ids;
    signal_storm_req_local_var->int_group_ids = int_group_ids;
    signal_storm_req_local_var->exter_group_ids = exter_group_ids;
    signal_storm_req_local_var->supis = supis;
    signal_storm_req_local_var->gpsis = gpsis;
    signal_storm_req_local_var->tgt_cause_ids = tgt_cause_ids;
    signal_storm_req_local_var->is_thr_period = is_thr_period;
    signal_storm_req_local_var->thr_period = thr_period;
    signal_storm_req_local_var->is_sig_freq_thr = is_sig_freq_thr;
    signal_storm_req_local_var->sig_freq_thr = sig_freq_thr;
    signal_storm_req_local_var->is_ue_req_thr = is_ue_req_thr;
    signal_storm_req_local_var->ue_req_thr = ue_req_thr;
    signal_storm_req_local_var->is_ue_num_thr = is_ue_num_thr;
    signal_storm_req_local_var->ue_num_thr = ue_num_thr;

    return signal_storm_req_local_var;
}

void OpenAPI_signal_storm_req_free(OpenAPI_signal_storm_req_t *signal_storm_req)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == signal_storm_req) {
        return;
    }
    if (signal_storm_req->tgt_nf_instance_ids) {
        OpenAPI_list_for_each(signal_storm_req->tgt_nf_instance_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_req->tgt_nf_instance_ids);
        signal_storm_req->tgt_nf_instance_ids = NULL;
    }
    if (signal_storm_req->tgt_nf_set_ids) {
        OpenAPI_list_for_each(signal_storm_req->tgt_nf_set_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_req->tgt_nf_set_ids);
        signal_storm_req->tgt_nf_set_ids = NULL;
    }
    if (signal_storm_req->int_group_ids) {
        OpenAPI_list_for_each(signal_storm_req->int_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_req->int_group_ids);
        signal_storm_req->int_group_ids = NULL;
    }
    if (signal_storm_req->exter_group_ids) {
        OpenAPI_list_for_each(signal_storm_req->exter_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_req->exter_group_ids);
        signal_storm_req->exter_group_ids = NULL;
    }
    if (signal_storm_req->supis) {
        OpenAPI_list_for_each(signal_storm_req->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_req->supis);
        signal_storm_req->supis = NULL;
    }
    if (signal_storm_req->gpsis) {
        OpenAPI_list_for_each(signal_storm_req->gpsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_req->gpsis);
        signal_storm_req->gpsis = NULL;
    }
    if (signal_storm_req->tgt_cause_ids) {
        OpenAPI_list_free(signal_storm_req->tgt_cause_ids);
        signal_storm_req->tgt_cause_ids = NULL;
    }
    ogs_free(signal_storm_req);
}

cJSON *OpenAPI_signal_storm_req_convertToJSON(OpenAPI_signal_storm_req_t *signal_storm_req)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (signal_storm_req == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [SignalStormReq]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (signal_storm_req->tgt_nf_instance_ids) {
    cJSON *tgt_nf_instance_idsList = cJSON_AddArrayToObject(item, "tgtNfInstanceIds");
    if (tgt_nf_instance_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [tgt_nf_instance_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_req->tgt_nf_instance_ids, node) {
        if (cJSON_AddStringToObject(tgt_nf_instance_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [tgt_nf_instance_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_req->tgt_nf_set_ids) {
    cJSON *tgt_nf_set_idsList = cJSON_AddArrayToObject(item, "tgtNfSetIds");
    if (tgt_nf_set_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [tgt_nf_set_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_req->tgt_nf_set_ids, node) {
        if (cJSON_AddStringToObject(tgt_nf_set_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [tgt_nf_set_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_req->int_group_ids) {
    cJSON *int_group_idsList = cJSON_AddArrayToObject(item, "intGroupIds");
    if (int_group_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [int_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_req->int_group_ids, node) {
        if (cJSON_AddStringToObject(int_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [int_group_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_req->exter_group_ids) {
    cJSON *exter_group_idsList = cJSON_AddArrayToObject(item, "exterGroupIds");
    if (exter_group_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [exter_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_req->exter_group_ids, node) {
        if (cJSON_AddStringToObject(exter_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [exter_group_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_req->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_req->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (signal_storm_req->gpsis) {
    cJSON *gpsisList = cJSON_AddArrayToObject(item, "gpsis");
    if (gpsisList == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [gpsis]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_req->gpsis, node) {
        if (cJSON_AddStringToObject(gpsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [gpsis]");
            goto end;
        }
    }
    }

    if (signal_storm_req->tgt_cause_ids != OpenAPI_target_cause_id_NULL) {
    cJSON *tgt_cause_idsList = cJSON_AddArrayToObject(item, "tgtCauseIds");
    if (tgt_cause_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [tgt_cause_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_req->tgt_cause_ids, node) {
        if (cJSON_AddStringToObject(tgt_cause_idsList, "", OpenAPI_target_cause_id_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [tgt_cause_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_req->is_thr_period) {
    if (cJSON_AddNumberToObject(item, "thrPeriod", signal_storm_req->thr_period) == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [thr_period]");
        goto end;
    }
    }

    if (signal_storm_req->is_sig_freq_thr) {
    if (cJSON_AddNumberToObject(item, "sigFreqThr", signal_storm_req->sig_freq_thr) == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [sig_freq_thr]");
        goto end;
    }
    }

    if (signal_storm_req->is_ue_req_thr) {
    if (cJSON_AddNumberToObject(item, "ueReqThr", signal_storm_req->ue_req_thr) == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [ue_req_thr]");
        goto end;
    }
    }

    if (signal_storm_req->is_ue_num_thr) {
    if (cJSON_AddNumberToObject(item, "ueNumThr", signal_storm_req->ue_num_thr) == NULL) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed [ue_num_thr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_signal_storm_req_t *OpenAPI_signal_storm_req_parseFromJSON(cJSON *signal_storm_reqJSON)
{
    OpenAPI_signal_storm_req_t *signal_storm_req_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tgt_nf_instance_ids = NULL;
    OpenAPI_list_t *tgt_nf_instance_idsList = NULL;
    cJSON *tgt_nf_set_ids = NULL;
    OpenAPI_list_t *tgt_nf_set_idsList = NULL;
    cJSON *int_group_ids = NULL;
    OpenAPI_list_t *int_group_idsList = NULL;
    cJSON *exter_group_ids = NULL;
    OpenAPI_list_t *exter_group_idsList = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *gpsis = NULL;
    OpenAPI_list_t *gpsisList = NULL;
    cJSON *tgt_cause_ids = NULL;
    OpenAPI_list_t *tgt_cause_idsList = NULL;
    cJSON *thr_period = NULL;
    cJSON *sig_freq_thr = NULL;
    cJSON *ue_req_thr = NULL;
    cJSON *ue_num_thr = NULL;
    tgt_nf_instance_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "tgtNfInstanceIds");
    if (tgt_nf_instance_ids) {
        cJSON *tgt_nf_instance_ids_local = NULL;
        if (!cJSON_IsArray(tgt_nf_instance_ids)) {
            ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [tgt_nf_instance_ids]");
            goto end;
        }

        tgt_nf_instance_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tgt_nf_instance_ids_local, tgt_nf_instance_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(tgt_nf_instance_ids_local)) {
                ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [tgt_nf_instance_ids]");
                goto end;
            }
            OpenAPI_list_add(tgt_nf_instance_idsList, ogs_strdup(tgt_nf_instance_ids_local->valuestring));
        }
    }

    tgt_nf_set_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "tgtNfSetIds");
    if (tgt_nf_set_ids) {
        cJSON *tgt_nf_set_ids_local = NULL;
        if (!cJSON_IsArray(tgt_nf_set_ids)) {
            ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [tgt_nf_set_ids]");
            goto end;
        }

        tgt_nf_set_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tgt_nf_set_ids_local, tgt_nf_set_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(tgt_nf_set_ids_local)) {
                ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [tgt_nf_set_ids]");
                goto end;
            }
            OpenAPI_list_add(tgt_nf_set_idsList, ogs_strdup(tgt_nf_set_ids_local->valuestring));
        }
    }

    int_group_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "intGroupIds");
    if (int_group_ids) {
        cJSON *int_group_ids_local = NULL;
        if (!cJSON_IsArray(int_group_ids)) {
            ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [int_group_ids]");
            goto end;
        }

        int_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(int_group_ids_local, int_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(int_group_ids_local)) {
                ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [int_group_ids]");
                goto end;
            }
            OpenAPI_list_add(int_group_idsList, ogs_strdup(int_group_ids_local->valuestring));
        }
    }

    exter_group_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "exterGroupIds");
    if (exter_group_ids) {
        cJSON *exter_group_ids_local = NULL;
        if (!cJSON_IsArray(exter_group_ids)) {
            ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [exter_group_ids]");
            goto end;
        }

        exter_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(exter_group_ids_local, exter_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(exter_group_ids_local)) {
                ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [exter_group_ids]");
                goto end;
            }
            OpenAPI_list_add(exter_group_idsList, ogs_strdup(exter_group_ids_local->valuestring));
        }
    }

    supis = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    gpsis = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "gpsis");
    if (gpsis) {
        cJSON *gpsis_local = NULL;
        if (!cJSON_IsArray(gpsis)) {
            ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [gpsis]");
            goto end;
        }

        gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsis_local, gpsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gpsis_local)) {
                ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [gpsis]");
                goto end;
            }
            OpenAPI_list_add(gpsisList, ogs_strdup(gpsis_local->valuestring));
        }
    }

    tgt_cause_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "tgtCauseIds");
    if (tgt_cause_ids) {
        cJSON *tgt_cause_ids_local = NULL;
        if (!cJSON_IsArray(tgt_cause_ids)) {
            ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [tgt_cause_ids]");
            goto end;
        }

        tgt_cause_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tgt_cause_ids_local, tgt_cause_ids) {
            OpenAPI_target_cause_id_e localEnum = OpenAPI_target_cause_id_NULL;
            if (!cJSON_IsString(tgt_cause_ids_local)) {
                ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [tgt_cause_ids]");
                goto end;
            }
            localEnum = OpenAPI_target_cause_id_FromString(tgt_cause_ids_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"tgt_cause_ids\" is not supported. Ignoring it ...",
                         tgt_cause_ids_local->valuestring);
            } else {
                OpenAPI_list_add(tgt_cause_idsList, (void *)localEnum);
            }
        }
        if (tgt_cause_idsList->count == 0) {
            ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed: Expected tgt_cause_idsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    thr_period = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "thrPeriod");
    if (thr_period) {
    if (!cJSON_IsNumber(thr_period)) {
        ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [thr_period]");
        goto end;
    }
    }

    sig_freq_thr = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "sigFreqThr");
    if (sig_freq_thr) {
    if (!cJSON_IsNumber(sig_freq_thr)) {
        ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [sig_freq_thr]");
        goto end;
    }
    }

    ue_req_thr = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "ueReqThr");
    if (ue_req_thr) {
    if (!cJSON_IsNumber(ue_req_thr)) {
        ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [ue_req_thr]");
        goto end;
    }
    }

    ue_num_thr = cJSON_GetObjectItemCaseSensitive(signal_storm_reqJSON, "ueNumThr");
    if (ue_num_thr) {
    if (!cJSON_IsNumber(ue_num_thr)) {
        ogs_error("OpenAPI_signal_storm_req_parseFromJSON() failed [ue_num_thr]");
        goto end;
    }
    }

    signal_storm_req_local_var = OpenAPI_signal_storm_req_create (
        tgt_nf_instance_ids ? tgt_nf_instance_idsList : NULL,
        tgt_nf_set_ids ? tgt_nf_set_idsList : NULL,
        int_group_ids ? int_group_idsList : NULL,
        exter_group_ids ? exter_group_idsList : NULL,
        supis ? supisList : NULL,
        gpsis ? gpsisList : NULL,
        tgt_cause_ids ? tgt_cause_idsList : NULL,
        thr_period ? true : false,
        thr_period ? thr_period->valuedouble : 0,
        sig_freq_thr ? true : false,
        sig_freq_thr ? sig_freq_thr->valuedouble : 0,
        ue_req_thr ? true : false,
        ue_req_thr ? ue_req_thr->valuedouble : 0,
        ue_num_thr ? true : false,
        ue_num_thr ? ue_num_thr->valuedouble : 0
    );

    return signal_storm_req_local_var;
end:
    if (tgt_nf_instance_idsList) {
        OpenAPI_list_for_each(tgt_nf_instance_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(tgt_nf_instance_idsList);
        tgt_nf_instance_idsList = NULL;
    }
    if (tgt_nf_set_idsList) {
        OpenAPI_list_for_each(tgt_nf_set_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(tgt_nf_set_idsList);
        tgt_nf_set_idsList = NULL;
    }
    if (int_group_idsList) {
        OpenAPI_list_for_each(int_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(int_group_idsList);
        int_group_idsList = NULL;
    }
    if (exter_group_idsList) {
        OpenAPI_list_for_each(exter_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(exter_group_idsList);
        exter_group_idsList = NULL;
    }
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (gpsisList) {
        OpenAPI_list_for_each(gpsisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gpsisList);
        gpsisList = NULL;
    }
    if (tgt_cause_idsList) {
        OpenAPI_list_free(tgt_cause_idsList);
        tgt_cause_idsList = NULL;
    }
    return NULL;
}

OpenAPI_signal_storm_req_t *OpenAPI_signal_storm_req_copy(OpenAPI_signal_storm_req_t *dst, OpenAPI_signal_storm_req_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_signal_storm_req_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_signal_storm_req_convertToJSON() failed");
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

    OpenAPI_signal_storm_req_free(dst);
    dst = OpenAPI_signal_storm_req_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


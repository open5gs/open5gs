
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "signal_storm_info.h"

OpenAPI_signal_storm_info_t *OpenAPI_signal_storm_info_create(
    OpenAPI_list_t *tgt_nf_instance_ids,
    OpenAPI_list_t *tgt_nf_set_ids,
    OpenAPI_list_t *src_int_group_ids,
    OpenAPI_list_t *src_exter_group_ids,
    OpenAPI_list_t *src_supis,
    OpenAPI_list_t *src_gpsis,
    OpenAPI_list_t *sig_storm_causes,
    OpenAPI_list_t *src_nf_instance_ids,
    OpenAPI_list_t *src_nf_set_ids,
    OpenAPI_list_t *sig_info,
    OpenAPI_list_t *timer_info,
    bool is_priority,
    int priority,
    bool is_capacity,
    int capacity,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_signal_storm_info_t *signal_storm_info_local_var = ogs_malloc(sizeof(OpenAPI_signal_storm_info_t));
    ogs_assert(signal_storm_info_local_var);

    signal_storm_info_local_var->tgt_nf_instance_ids = tgt_nf_instance_ids;
    signal_storm_info_local_var->tgt_nf_set_ids = tgt_nf_set_ids;
    signal_storm_info_local_var->src_int_group_ids = src_int_group_ids;
    signal_storm_info_local_var->src_exter_group_ids = src_exter_group_ids;
    signal_storm_info_local_var->src_supis = src_supis;
    signal_storm_info_local_var->src_gpsis = src_gpsis;
    signal_storm_info_local_var->sig_storm_causes = sig_storm_causes;
    signal_storm_info_local_var->src_nf_instance_ids = src_nf_instance_ids;
    signal_storm_info_local_var->src_nf_set_ids = src_nf_set_ids;
    signal_storm_info_local_var->sig_info = sig_info;
    signal_storm_info_local_var->timer_info = timer_info;
    signal_storm_info_local_var->is_priority = is_priority;
    signal_storm_info_local_var->priority = priority;
    signal_storm_info_local_var->is_capacity = is_capacity;
    signal_storm_info_local_var->capacity = capacity;
    signal_storm_info_local_var->is_confidence = is_confidence;
    signal_storm_info_local_var->confidence = confidence;

    return signal_storm_info_local_var;
}

void OpenAPI_signal_storm_info_free(OpenAPI_signal_storm_info_t *signal_storm_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == signal_storm_info) {
        return;
    }
    if (signal_storm_info->tgt_nf_instance_ids) {
        OpenAPI_list_for_each(signal_storm_info->tgt_nf_instance_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->tgt_nf_instance_ids);
        signal_storm_info->tgt_nf_instance_ids = NULL;
    }
    if (signal_storm_info->tgt_nf_set_ids) {
        OpenAPI_list_for_each(signal_storm_info->tgt_nf_set_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->tgt_nf_set_ids);
        signal_storm_info->tgt_nf_set_ids = NULL;
    }
    if (signal_storm_info->src_int_group_ids) {
        OpenAPI_list_for_each(signal_storm_info->src_int_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->src_int_group_ids);
        signal_storm_info->src_int_group_ids = NULL;
    }
    if (signal_storm_info->src_exter_group_ids) {
        OpenAPI_list_for_each(signal_storm_info->src_exter_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->src_exter_group_ids);
        signal_storm_info->src_exter_group_ids = NULL;
    }
    if (signal_storm_info->src_supis) {
        OpenAPI_list_for_each(signal_storm_info->src_supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->src_supis);
        signal_storm_info->src_supis = NULL;
    }
    if (signal_storm_info->src_gpsis) {
        OpenAPI_list_for_each(signal_storm_info->src_gpsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->src_gpsis);
        signal_storm_info->src_gpsis = NULL;
    }
    if (signal_storm_info->sig_storm_causes) {
        OpenAPI_list_free(signal_storm_info->sig_storm_causes);
        signal_storm_info->sig_storm_causes = NULL;
    }
    if (signal_storm_info->src_nf_instance_ids) {
        OpenAPI_list_for_each(signal_storm_info->src_nf_instance_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->src_nf_instance_ids);
        signal_storm_info->src_nf_instance_ids = NULL;
    }
    if (signal_storm_info->src_nf_set_ids) {
        OpenAPI_list_for_each(signal_storm_info->src_nf_set_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->src_nf_set_ids);
        signal_storm_info->src_nf_set_ids = NULL;
    }
    if (signal_storm_info->sig_info) {
        OpenAPI_list_for_each(signal_storm_info->sig_info, node) {
            OpenAPI_signal_info_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->sig_info);
        signal_storm_info->sig_info = NULL;
    }
    if (signal_storm_info->timer_info) {
        OpenAPI_list_for_each(signal_storm_info->timer_info, node) {
            OpenAPI_timer_info_free(node->data);
        }
        OpenAPI_list_free(signal_storm_info->timer_info);
        signal_storm_info->timer_info = NULL;
    }
    ogs_free(signal_storm_info);
}

cJSON *OpenAPI_signal_storm_info_convertToJSON(OpenAPI_signal_storm_info_t *signal_storm_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (signal_storm_info == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [SignalStormInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (signal_storm_info->tgt_nf_instance_ids) {
    cJSON *tgt_nf_instance_idsList = cJSON_AddArrayToObject(item, "tgtNfInstanceIds");
    if (tgt_nf_instance_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [tgt_nf_instance_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->tgt_nf_instance_ids, node) {
        if (cJSON_AddStringToObject(tgt_nf_instance_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [tgt_nf_instance_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_info->tgt_nf_set_ids) {
    cJSON *tgt_nf_set_idsList = cJSON_AddArrayToObject(item, "tgtNfSetIds");
    if (tgt_nf_set_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [tgt_nf_set_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->tgt_nf_set_ids, node) {
        if (cJSON_AddStringToObject(tgt_nf_set_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [tgt_nf_set_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_info->src_int_group_ids) {
    cJSON *src_int_group_idsList = cJSON_AddArrayToObject(item, "srcIntGroupIds");
    if (src_int_group_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_int_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->src_int_group_ids, node) {
        if (cJSON_AddStringToObject(src_int_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_int_group_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_info->src_exter_group_ids) {
    cJSON *src_exter_group_idsList = cJSON_AddArrayToObject(item, "srcExterGroupIds");
    if (src_exter_group_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_exter_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->src_exter_group_ids, node) {
        if (cJSON_AddStringToObject(src_exter_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_exter_group_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_info->src_supis) {
    cJSON *src_supisList = cJSON_AddArrayToObject(item, "srcSupis");
    if (src_supisList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_supis]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->src_supis, node) {
        if (cJSON_AddStringToObject(src_supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_supis]");
            goto end;
        }
    }
    }

    if (signal_storm_info->src_gpsis) {
    cJSON *src_gpsisList = cJSON_AddArrayToObject(item, "srcGpsis");
    if (src_gpsisList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_gpsis]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->src_gpsis, node) {
        if (cJSON_AddStringToObject(src_gpsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_gpsis]");
            goto end;
        }
    }
    }

    if (signal_storm_info->sig_storm_causes == OpenAPI_target_cause_id_NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [sig_storm_causes]");
        return NULL;
    }
    cJSON *sig_storm_causesList = cJSON_AddArrayToObject(item, "sigStormCauses");
    if (sig_storm_causesList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [sig_storm_causes]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->sig_storm_causes, node) {
        if (cJSON_AddStringToObject(sig_storm_causesList, "", OpenAPI_target_cause_id_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [sig_storm_causes]");
            goto end;
        }
    }

    if (signal_storm_info->src_nf_instance_ids) {
    cJSON *src_nf_instance_idsList = cJSON_AddArrayToObject(item, "srcNfInstanceIds");
    if (src_nf_instance_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_nf_instance_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->src_nf_instance_ids, node) {
        if (cJSON_AddStringToObject(src_nf_instance_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_nf_instance_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_info->src_nf_set_ids) {
    cJSON *src_nf_set_idsList = cJSON_AddArrayToObject(item, "srcNfSetIds");
    if (src_nf_set_idsList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_nf_set_ids]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->src_nf_set_ids, node) {
        if (cJSON_AddStringToObject(src_nf_set_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [src_nf_set_ids]");
            goto end;
        }
    }
    }

    if (signal_storm_info->sig_info) {
    cJSON *sig_infoList = cJSON_AddArrayToObject(item, "sigInfo");
    if (sig_infoList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [sig_info]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->sig_info, node) {
        cJSON *itemLocal = OpenAPI_signal_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [sig_info]");
            goto end;
        }
        cJSON_AddItemToArray(sig_infoList, itemLocal);
    }
    }

    if (signal_storm_info->timer_info) {
    cJSON *timer_infoList = cJSON_AddArrayToObject(item, "timerInfo");
    if (timer_infoList == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [timer_info]");
        goto end;
    }
    OpenAPI_list_for_each(signal_storm_info->timer_info, node) {
        cJSON *itemLocal = OpenAPI_timer_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [timer_info]");
            goto end;
        }
        cJSON_AddItemToArray(timer_infoList, itemLocal);
    }
    }

    if (signal_storm_info->is_priority) {
    if (cJSON_AddNumberToObject(item, "priority", signal_storm_info->priority) == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [priority]");
        goto end;
    }
    }

    if (signal_storm_info->is_capacity) {
    if (cJSON_AddNumberToObject(item, "capacity", signal_storm_info->capacity) == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [capacity]");
        goto end;
    }
    }

    if (signal_storm_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", signal_storm_info->confidence) == NULL) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_signal_storm_info_t *OpenAPI_signal_storm_info_parseFromJSON(cJSON *signal_storm_infoJSON)
{
    OpenAPI_signal_storm_info_t *signal_storm_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *tgt_nf_instance_ids = NULL;
    OpenAPI_list_t *tgt_nf_instance_idsList = NULL;
    cJSON *tgt_nf_set_ids = NULL;
    OpenAPI_list_t *tgt_nf_set_idsList = NULL;
    cJSON *src_int_group_ids = NULL;
    OpenAPI_list_t *src_int_group_idsList = NULL;
    cJSON *src_exter_group_ids = NULL;
    OpenAPI_list_t *src_exter_group_idsList = NULL;
    cJSON *src_supis = NULL;
    OpenAPI_list_t *src_supisList = NULL;
    cJSON *src_gpsis = NULL;
    OpenAPI_list_t *src_gpsisList = NULL;
    cJSON *sig_storm_causes = NULL;
    OpenAPI_list_t *sig_storm_causesList = NULL;
    cJSON *src_nf_instance_ids = NULL;
    OpenAPI_list_t *src_nf_instance_idsList = NULL;
    cJSON *src_nf_set_ids = NULL;
    OpenAPI_list_t *src_nf_set_idsList = NULL;
    cJSON *sig_info = NULL;
    OpenAPI_list_t *sig_infoList = NULL;
    cJSON *timer_info = NULL;
    OpenAPI_list_t *timer_infoList = NULL;
    cJSON *priority = NULL;
    cJSON *capacity = NULL;
    cJSON *confidence = NULL;
    tgt_nf_instance_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "tgtNfInstanceIds");
    if (tgt_nf_instance_ids) {
        cJSON *tgt_nf_instance_ids_local = NULL;
        if (!cJSON_IsArray(tgt_nf_instance_ids)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [tgt_nf_instance_ids]");
            goto end;
        }

        tgt_nf_instance_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tgt_nf_instance_ids_local, tgt_nf_instance_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(tgt_nf_instance_ids_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [tgt_nf_instance_ids]");
                goto end;
            }
            OpenAPI_list_add(tgt_nf_instance_idsList, ogs_strdup(tgt_nf_instance_ids_local->valuestring));
        }
    }

    tgt_nf_set_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "tgtNfSetIds");
    if (tgt_nf_set_ids) {
        cJSON *tgt_nf_set_ids_local = NULL;
        if (!cJSON_IsArray(tgt_nf_set_ids)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [tgt_nf_set_ids]");
            goto end;
        }

        tgt_nf_set_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(tgt_nf_set_ids_local, tgt_nf_set_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(tgt_nf_set_ids_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [tgt_nf_set_ids]");
                goto end;
            }
            OpenAPI_list_add(tgt_nf_set_idsList, ogs_strdup(tgt_nf_set_ids_local->valuestring));
        }
    }

    src_int_group_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "srcIntGroupIds");
    if (src_int_group_ids) {
        cJSON *src_int_group_ids_local = NULL;
        if (!cJSON_IsArray(src_int_group_ids)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_int_group_ids]");
            goto end;
        }

        src_int_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(src_int_group_ids_local, src_int_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(src_int_group_ids_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_int_group_ids]");
                goto end;
            }
            OpenAPI_list_add(src_int_group_idsList, ogs_strdup(src_int_group_ids_local->valuestring));
        }
    }

    src_exter_group_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "srcExterGroupIds");
    if (src_exter_group_ids) {
        cJSON *src_exter_group_ids_local = NULL;
        if (!cJSON_IsArray(src_exter_group_ids)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_exter_group_ids]");
            goto end;
        }

        src_exter_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(src_exter_group_ids_local, src_exter_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(src_exter_group_ids_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_exter_group_ids]");
                goto end;
            }
            OpenAPI_list_add(src_exter_group_idsList, ogs_strdup(src_exter_group_ids_local->valuestring));
        }
    }

    src_supis = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "srcSupis");
    if (src_supis) {
        cJSON *src_supis_local = NULL;
        if (!cJSON_IsArray(src_supis)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_supis]");
            goto end;
        }

        src_supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(src_supis_local, src_supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(src_supis_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_supis]");
                goto end;
            }
            OpenAPI_list_add(src_supisList, ogs_strdup(src_supis_local->valuestring));
        }
    }

    src_gpsis = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "srcGpsis");
    if (src_gpsis) {
        cJSON *src_gpsis_local = NULL;
        if (!cJSON_IsArray(src_gpsis)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_gpsis]");
            goto end;
        }

        src_gpsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(src_gpsis_local, src_gpsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(src_gpsis_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_gpsis]");
                goto end;
            }
            OpenAPI_list_add(src_gpsisList, ogs_strdup(src_gpsis_local->valuestring));
        }
    }

    sig_storm_causes = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "sigStormCauses");
    if (!sig_storm_causes) {
        ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [sig_storm_causes]");
        goto end;
    }
        cJSON *sig_storm_causes_local = NULL;
        if (!cJSON_IsArray(sig_storm_causes)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [sig_storm_causes]");
            goto end;
        }

        sig_storm_causesList = OpenAPI_list_create();

        cJSON_ArrayForEach(sig_storm_causes_local, sig_storm_causes) {
            OpenAPI_target_cause_id_e localEnum = OpenAPI_target_cause_id_NULL;
            if (!cJSON_IsString(sig_storm_causes_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [sig_storm_causes]");
                goto end;
            }
            localEnum = OpenAPI_target_cause_id_FromString(sig_storm_causes_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"sig_storm_causes\" is not supported. Ignoring it ...",
                         sig_storm_causes_local->valuestring);
            } else {
                OpenAPI_list_add(sig_storm_causesList, (void *)localEnum);
            }
        }
        if (sig_storm_causesList->count == 0) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed: Expected sig_storm_causesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }

    src_nf_instance_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "srcNfInstanceIds");
    if (src_nf_instance_ids) {
        cJSON *src_nf_instance_ids_local = NULL;
        if (!cJSON_IsArray(src_nf_instance_ids)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_nf_instance_ids]");
            goto end;
        }

        src_nf_instance_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(src_nf_instance_ids_local, src_nf_instance_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(src_nf_instance_ids_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_nf_instance_ids]");
                goto end;
            }
            OpenAPI_list_add(src_nf_instance_idsList, ogs_strdup(src_nf_instance_ids_local->valuestring));
        }
    }

    src_nf_set_ids = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "srcNfSetIds");
    if (src_nf_set_ids) {
        cJSON *src_nf_set_ids_local = NULL;
        if (!cJSON_IsArray(src_nf_set_ids)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_nf_set_ids]");
            goto end;
        }

        src_nf_set_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(src_nf_set_ids_local, src_nf_set_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(src_nf_set_ids_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [src_nf_set_ids]");
                goto end;
            }
            OpenAPI_list_add(src_nf_set_idsList, ogs_strdup(src_nf_set_ids_local->valuestring));
        }
    }

    sig_info = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "sigInfo");
    if (sig_info) {
        cJSON *sig_info_local = NULL;
        if (!cJSON_IsArray(sig_info)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [sig_info]");
            goto end;
        }

        sig_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(sig_info_local, sig_info) {
            if (!cJSON_IsObject(sig_info_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [sig_info]");
                goto end;
            }
            OpenAPI_signal_info_t *sig_infoItem = OpenAPI_signal_info_parseFromJSON(sig_info_local);
            if (!sig_infoItem) {
                ogs_error("No sig_infoItem");
                goto end;
            }
            OpenAPI_list_add(sig_infoList, sig_infoItem);
        }
    }

    timer_info = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "timerInfo");
    if (timer_info) {
        cJSON *timer_info_local = NULL;
        if (!cJSON_IsArray(timer_info)) {
            ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [timer_info]");
            goto end;
        }

        timer_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(timer_info_local, timer_info) {
            if (!cJSON_IsObject(timer_info_local)) {
                ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [timer_info]");
                goto end;
            }
            OpenAPI_timer_info_t *timer_infoItem = OpenAPI_timer_info_parseFromJSON(timer_info_local);
            if (!timer_infoItem) {
                ogs_error("No timer_infoItem");
                goto end;
            }
            OpenAPI_list_add(timer_infoList, timer_infoItem);
        }
    }

    priority = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "priority");
    if (priority) {
    if (!cJSON_IsNumber(priority)) {
        ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [priority]");
        goto end;
    }
    }

    capacity = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "capacity");
    if (capacity) {
    if (!cJSON_IsNumber(capacity)) {
        ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [capacity]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(signal_storm_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_signal_storm_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    signal_storm_info_local_var = OpenAPI_signal_storm_info_create (
        tgt_nf_instance_ids ? tgt_nf_instance_idsList : NULL,
        tgt_nf_set_ids ? tgt_nf_set_idsList : NULL,
        src_int_group_ids ? src_int_group_idsList : NULL,
        src_exter_group_ids ? src_exter_group_idsList : NULL,
        src_supis ? src_supisList : NULL,
        src_gpsis ? src_gpsisList : NULL,
        sig_storm_causesList,
        src_nf_instance_ids ? src_nf_instance_idsList : NULL,
        src_nf_set_ids ? src_nf_set_idsList : NULL,
        sig_info ? sig_infoList : NULL,
        timer_info ? timer_infoList : NULL,
        priority ? true : false,
        priority ? priority->valuedouble : 0,
        capacity ? true : false,
        capacity ? capacity->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return signal_storm_info_local_var;
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
    if (src_int_group_idsList) {
        OpenAPI_list_for_each(src_int_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(src_int_group_idsList);
        src_int_group_idsList = NULL;
    }
    if (src_exter_group_idsList) {
        OpenAPI_list_for_each(src_exter_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(src_exter_group_idsList);
        src_exter_group_idsList = NULL;
    }
    if (src_supisList) {
        OpenAPI_list_for_each(src_supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(src_supisList);
        src_supisList = NULL;
    }
    if (src_gpsisList) {
        OpenAPI_list_for_each(src_gpsisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(src_gpsisList);
        src_gpsisList = NULL;
    }
    if (sig_storm_causesList) {
        OpenAPI_list_free(sig_storm_causesList);
        sig_storm_causesList = NULL;
    }
    if (src_nf_instance_idsList) {
        OpenAPI_list_for_each(src_nf_instance_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(src_nf_instance_idsList);
        src_nf_instance_idsList = NULL;
    }
    if (src_nf_set_idsList) {
        OpenAPI_list_for_each(src_nf_set_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(src_nf_set_idsList);
        src_nf_set_idsList = NULL;
    }
    if (sig_infoList) {
        OpenAPI_list_for_each(sig_infoList, node) {
            OpenAPI_signal_info_free(node->data);
        }
        OpenAPI_list_free(sig_infoList);
        sig_infoList = NULL;
    }
    if (timer_infoList) {
        OpenAPI_list_for_each(timer_infoList, node) {
            OpenAPI_timer_info_free(node->data);
        }
        OpenAPI_list_free(timer_infoList);
        timer_infoList = NULL;
    }
    return NULL;
}

OpenAPI_signal_storm_info_t *OpenAPI_signal_storm_info_copy(OpenAPI_signal_storm_info_t *dst, OpenAPI_signal_storm_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_signal_storm_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_signal_storm_info_convertToJSON() failed");
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

    OpenAPI_signal_storm_info_free(dst);
    dst = OpenAPI_signal_storm_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


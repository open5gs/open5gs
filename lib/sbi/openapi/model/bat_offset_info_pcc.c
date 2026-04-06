
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bat_offset_info_pcc.h"

OpenAPI_bat_offset_info_pcc_t *OpenAPI_bat_offset_info_pcc_create(
    int ran_bat_offset_notif,
    bool is_adj_period,
    int adj_period,
    OpenAPI_list_t *ref_pcc_rule_ids
)
{
    OpenAPI_bat_offset_info_pcc_t *bat_offset_info_pcc_local_var = ogs_malloc(sizeof(OpenAPI_bat_offset_info_pcc_t));
    ogs_assert(bat_offset_info_pcc_local_var);

    bat_offset_info_pcc_local_var->ran_bat_offset_notif = ran_bat_offset_notif;
    bat_offset_info_pcc_local_var->is_adj_period = is_adj_period;
    bat_offset_info_pcc_local_var->adj_period = adj_period;
    bat_offset_info_pcc_local_var->ref_pcc_rule_ids = ref_pcc_rule_ids;

    return bat_offset_info_pcc_local_var;
}

void OpenAPI_bat_offset_info_pcc_free(OpenAPI_bat_offset_info_pcc_t *bat_offset_info_pcc)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bat_offset_info_pcc) {
        return;
    }
    if (bat_offset_info_pcc->ref_pcc_rule_ids) {
        OpenAPI_list_for_each(bat_offset_info_pcc->ref_pcc_rule_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(bat_offset_info_pcc->ref_pcc_rule_ids);
        bat_offset_info_pcc->ref_pcc_rule_ids = NULL;
    }
    ogs_free(bat_offset_info_pcc);
}

cJSON *OpenAPI_bat_offset_info_pcc_convertToJSON(OpenAPI_bat_offset_info_pcc_t *bat_offset_info_pcc)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bat_offset_info_pcc == NULL) {
        ogs_error("OpenAPI_bat_offset_info_pcc_convertToJSON() failed [BatOffsetInfoPcc]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "ranBatOffsetNotif", bat_offset_info_pcc->ran_bat_offset_notif) == NULL) {
        ogs_error("OpenAPI_bat_offset_info_pcc_convertToJSON() failed [ran_bat_offset_notif]");
        goto end;
    }

    if (bat_offset_info_pcc->is_adj_period) {
    if (cJSON_AddNumberToObject(item, "adjPeriod", bat_offset_info_pcc->adj_period) == NULL) {
        ogs_error("OpenAPI_bat_offset_info_pcc_convertToJSON() failed [adj_period]");
        goto end;
    }
    }

    if (!bat_offset_info_pcc->ref_pcc_rule_ids) {
        ogs_error("OpenAPI_bat_offset_info_pcc_convertToJSON() failed [ref_pcc_rule_ids]");
        return NULL;
    }
    cJSON *ref_pcc_rule_idsList = cJSON_AddArrayToObject(item, "refPccRuleIds");
    if (ref_pcc_rule_idsList == NULL) {
        ogs_error("OpenAPI_bat_offset_info_pcc_convertToJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
    OpenAPI_list_for_each(bat_offset_info_pcc->ref_pcc_rule_ids, node) {
        if (cJSON_AddStringToObject(ref_pcc_rule_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_bat_offset_info_pcc_convertToJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_bat_offset_info_pcc_t *OpenAPI_bat_offset_info_pcc_parseFromJSON(cJSON *bat_offset_info_pccJSON)
{
    OpenAPI_bat_offset_info_pcc_t *bat_offset_info_pcc_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ran_bat_offset_notif = NULL;
    cJSON *adj_period = NULL;
    cJSON *ref_pcc_rule_ids = NULL;
    OpenAPI_list_t *ref_pcc_rule_idsList = NULL;
    ran_bat_offset_notif = cJSON_GetObjectItemCaseSensitive(bat_offset_info_pccJSON, "ranBatOffsetNotif");
    if (!ran_bat_offset_notif) {
        ogs_error("OpenAPI_bat_offset_info_pcc_parseFromJSON() failed [ran_bat_offset_notif]");
        goto end;
    }
    if (!cJSON_IsNumber(ran_bat_offset_notif)) {
        ogs_error("OpenAPI_bat_offset_info_pcc_parseFromJSON() failed [ran_bat_offset_notif]");
        goto end;
    }

    adj_period = cJSON_GetObjectItemCaseSensitive(bat_offset_info_pccJSON, "adjPeriod");
    if (adj_period) {
    if (!cJSON_IsNumber(adj_period)) {
        ogs_error("OpenAPI_bat_offset_info_pcc_parseFromJSON() failed [adj_period]");
        goto end;
    }
    }

    ref_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(bat_offset_info_pccJSON, "refPccRuleIds");
    if (!ref_pcc_rule_ids) {
        ogs_error("OpenAPI_bat_offset_info_pcc_parseFromJSON() failed [ref_pcc_rule_ids]");
        goto end;
    }
        cJSON *ref_pcc_rule_ids_local = NULL;
        if (!cJSON_IsArray(ref_pcc_rule_ids)) {
            ogs_error("OpenAPI_bat_offset_info_pcc_parseFromJSON() failed [ref_pcc_rule_ids]");
            goto end;
        }

        ref_pcc_rule_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ref_pcc_rule_ids_local, ref_pcc_rule_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ref_pcc_rule_ids_local)) {
                ogs_error("OpenAPI_bat_offset_info_pcc_parseFromJSON() failed [ref_pcc_rule_ids]");
                goto end;
            }
            OpenAPI_list_add(ref_pcc_rule_idsList, ogs_strdup(ref_pcc_rule_ids_local->valuestring));
        }

    bat_offset_info_pcc_local_var = OpenAPI_bat_offset_info_pcc_create (
        
        ran_bat_offset_notif->valuedouble,
        adj_period ? true : false,
        adj_period ? adj_period->valuedouble : 0,
        ref_pcc_rule_idsList
    );

    return bat_offset_info_pcc_local_var;
end:
    if (ref_pcc_rule_idsList) {
        OpenAPI_list_for_each(ref_pcc_rule_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ref_pcc_rule_idsList);
        ref_pcc_rule_idsList = NULL;
    }
    return NULL;
}

OpenAPI_bat_offset_info_pcc_t *OpenAPI_bat_offset_info_pcc_copy(OpenAPI_bat_offset_info_pcc_t *dst, OpenAPI_bat_offset_info_pcc_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bat_offset_info_pcc_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bat_offset_info_pcc_convertToJSON() failed");
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

    OpenAPI_bat_offset_info_pcc_free(dst);
    dst = OpenAPI_bat_offset_info_pcc_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


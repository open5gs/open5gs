
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "final_unit_indication.h"

OpenAPI_final_unit_indication_t *OpenAPI_final_unit_indication_create(
    OpenAPI_final_unit_action_e final_unit_action,
    char *restriction_filter_rule,
    OpenAPI_list_t *restriction_filter_rule_list,
    char *filter_id,
    OpenAPI_list_t *filter_id_list,
    OpenAPI_redirect_server_t *redirect_server
)
{
    OpenAPI_final_unit_indication_t *final_unit_indication_local_var = ogs_malloc(sizeof(OpenAPI_final_unit_indication_t));
    ogs_assert(final_unit_indication_local_var);

    final_unit_indication_local_var->final_unit_action = final_unit_action;
    final_unit_indication_local_var->restriction_filter_rule = restriction_filter_rule;
    final_unit_indication_local_var->restriction_filter_rule_list = restriction_filter_rule_list;
    final_unit_indication_local_var->filter_id = filter_id;
    final_unit_indication_local_var->filter_id_list = filter_id_list;
    final_unit_indication_local_var->redirect_server = redirect_server;

    return final_unit_indication_local_var;
}

void OpenAPI_final_unit_indication_free(OpenAPI_final_unit_indication_t *final_unit_indication)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == final_unit_indication) {
        return;
    }
    if (final_unit_indication->restriction_filter_rule) {
        ogs_free(final_unit_indication->restriction_filter_rule);
        final_unit_indication->restriction_filter_rule = NULL;
    }
    if (final_unit_indication->restriction_filter_rule_list) {
        OpenAPI_list_for_each(final_unit_indication->restriction_filter_rule_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(final_unit_indication->restriction_filter_rule_list);
        final_unit_indication->restriction_filter_rule_list = NULL;
    }
    if (final_unit_indication->filter_id) {
        ogs_free(final_unit_indication->filter_id);
        final_unit_indication->filter_id = NULL;
    }
    if (final_unit_indication->filter_id_list) {
        OpenAPI_list_for_each(final_unit_indication->filter_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(final_unit_indication->filter_id_list);
        final_unit_indication->filter_id_list = NULL;
    }
    if (final_unit_indication->redirect_server) {
        OpenAPI_redirect_server_free(final_unit_indication->redirect_server);
        final_unit_indication->redirect_server = NULL;
    }
    ogs_free(final_unit_indication);
}

cJSON *OpenAPI_final_unit_indication_convertToJSON(OpenAPI_final_unit_indication_t *final_unit_indication)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (final_unit_indication == NULL) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [FinalUnitIndication]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (final_unit_indication->final_unit_action == OpenAPI_final_unit_action_NULL) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [final_unit_action]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "finalUnitAction", OpenAPI_final_unit_action_ToString(final_unit_indication->final_unit_action)) == NULL) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [final_unit_action]");
        goto end;
    }

    if (final_unit_indication->restriction_filter_rule) {
    if (cJSON_AddStringToObject(item, "restrictionFilterRule", final_unit_indication->restriction_filter_rule) == NULL) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [restriction_filter_rule]");
        goto end;
    }
    }

    if (final_unit_indication->restriction_filter_rule_list) {
    cJSON *restriction_filter_rule_listList = cJSON_AddArrayToObject(item, "restrictionFilterRuleList");
    if (restriction_filter_rule_listList == NULL) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [restriction_filter_rule_list]");
        goto end;
    }
    OpenAPI_list_for_each(final_unit_indication->restriction_filter_rule_list, node) {
        if (cJSON_AddStringToObject(restriction_filter_rule_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [restriction_filter_rule_list]");
            goto end;
        }
    }
    }

    if (final_unit_indication->filter_id) {
    if (cJSON_AddStringToObject(item, "filterId", final_unit_indication->filter_id) == NULL) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [filter_id]");
        goto end;
    }
    }

    if (final_unit_indication->filter_id_list) {
    cJSON *filter_id_listList = cJSON_AddArrayToObject(item, "filterIdList");
    if (filter_id_listList == NULL) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [filter_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(final_unit_indication->filter_id_list, node) {
        if (cJSON_AddStringToObject(filter_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [filter_id_list]");
            goto end;
        }
    }
    }

    if (final_unit_indication->redirect_server) {
    cJSON *redirect_server_local_JSON = OpenAPI_redirect_server_convertToJSON(final_unit_indication->redirect_server);
    if (redirect_server_local_JSON == NULL) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [redirect_server]");
        goto end;
    }
    cJSON_AddItemToObject(item, "redirectServer", redirect_server_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed [redirect_server]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_final_unit_indication_t *OpenAPI_final_unit_indication_parseFromJSON(cJSON *final_unit_indicationJSON)
{
    OpenAPI_final_unit_indication_t *final_unit_indication_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *final_unit_action = NULL;
    OpenAPI_final_unit_action_e final_unit_actionVariable = 0;
    cJSON *restriction_filter_rule = NULL;
    cJSON *restriction_filter_rule_list = NULL;
    OpenAPI_list_t *restriction_filter_rule_listList = NULL;
    cJSON *filter_id = NULL;
    cJSON *filter_id_list = NULL;
    OpenAPI_list_t *filter_id_listList = NULL;
    cJSON *redirect_server = NULL;
    OpenAPI_redirect_server_t *redirect_server_local_nonprim = NULL;
    final_unit_action = cJSON_GetObjectItemCaseSensitive(final_unit_indicationJSON, "finalUnitAction");
    if (!final_unit_action) {
        ogs_error("OpenAPI_final_unit_indication_parseFromJSON() failed [final_unit_action]");
        goto end;
    }
    if (!cJSON_IsString(final_unit_action)) {
        ogs_error("OpenAPI_final_unit_indication_parseFromJSON() failed [final_unit_action]");
        goto end;
    }
    final_unit_actionVariable = OpenAPI_final_unit_action_FromString(final_unit_action->valuestring);

    restriction_filter_rule = cJSON_GetObjectItemCaseSensitive(final_unit_indicationJSON, "restrictionFilterRule");
    if (restriction_filter_rule) {
    if (!cJSON_IsString(restriction_filter_rule) && !cJSON_IsNull(restriction_filter_rule)) {
        ogs_error("OpenAPI_final_unit_indication_parseFromJSON() failed [restriction_filter_rule]");
        goto end;
    }
    }

    restriction_filter_rule_list = cJSON_GetObjectItemCaseSensitive(final_unit_indicationJSON, "restrictionFilterRuleList");
    if (restriction_filter_rule_list) {
        cJSON *restriction_filter_rule_list_local = NULL;
        if (!cJSON_IsArray(restriction_filter_rule_list)) {
            ogs_error("OpenAPI_final_unit_indication_parseFromJSON() failed [restriction_filter_rule_list]");
            goto end;
        }

        restriction_filter_rule_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restriction_filter_rule_list_local, restriction_filter_rule_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(restriction_filter_rule_list_local)) {
                ogs_error("OpenAPI_final_unit_indication_parseFromJSON() failed [restriction_filter_rule_list]");
                goto end;
            }
            OpenAPI_list_add(restriction_filter_rule_listList, ogs_strdup(restriction_filter_rule_list_local->valuestring));
        }
    }

    filter_id = cJSON_GetObjectItemCaseSensitive(final_unit_indicationJSON, "filterId");
    if (filter_id) {
    if (!cJSON_IsString(filter_id) && !cJSON_IsNull(filter_id)) {
        ogs_error("OpenAPI_final_unit_indication_parseFromJSON() failed [filter_id]");
        goto end;
    }
    }

    filter_id_list = cJSON_GetObjectItemCaseSensitive(final_unit_indicationJSON, "filterIdList");
    if (filter_id_list) {
        cJSON *filter_id_list_local = NULL;
        if (!cJSON_IsArray(filter_id_list)) {
            ogs_error("OpenAPI_final_unit_indication_parseFromJSON() failed [filter_id_list]");
            goto end;
        }

        filter_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(filter_id_list_local, filter_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(filter_id_list_local)) {
                ogs_error("OpenAPI_final_unit_indication_parseFromJSON() failed [filter_id_list]");
                goto end;
            }
            OpenAPI_list_add(filter_id_listList, ogs_strdup(filter_id_list_local->valuestring));
        }
    }

    redirect_server = cJSON_GetObjectItemCaseSensitive(final_unit_indicationJSON, "redirectServer");
    if (redirect_server) {
    redirect_server_local_nonprim = OpenAPI_redirect_server_parseFromJSON(redirect_server);
    if (!redirect_server_local_nonprim) {
        ogs_error("OpenAPI_redirect_server_parseFromJSON failed [redirect_server]");
        goto end;
    }
    }

    final_unit_indication_local_var = OpenAPI_final_unit_indication_create (
        final_unit_actionVariable,
        restriction_filter_rule && !cJSON_IsNull(restriction_filter_rule) ? ogs_strdup(restriction_filter_rule->valuestring) : NULL,
        restriction_filter_rule_list ? restriction_filter_rule_listList : NULL,
        filter_id && !cJSON_IsNull(filter_id) ? ogs_strdup(filter_id->valuestring) : NULL,
        filter_id_list ? filter_id_listList : NULL,
        redirect_server ? redirect_server_local_nonprim : NULL
    );

    return final_unit_indication_local_var;
end:
    if (restriction_filter_rule_listList) {
        OpenAPI_list_for_each(restriction_filter_rule_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(restriction_filter_rule_listList);
        restriction_filter_rule_listList = NULL;
    }
    if (filter_id_listList) {
        OpenAPI_list_for_each(filter_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(filter_id_listList);
        filter_id_listList = NULL;
    }
    if (redirect_server_local_nonprim) {
        OpenAPI_redirect_server_free(redirect_server_local_nonprim);
        redirect_server_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_final_unit_indication_t *OpenAPI_final_unit_indication_copy(OpenAPI_final_unit_indication_t *dst, OpenAPI_final_unit_indication_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_final_unit_indication_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_final_unit_indication_convertToJSON() failed");
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

    OpenAPI_final_unit_indication_free(dst);
    dst = OpenAPI_final_unit_indication_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


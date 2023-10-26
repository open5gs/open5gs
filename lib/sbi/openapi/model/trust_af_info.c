
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trust_af_info.h"

OpenAPI_trust_af_info_t *OpenAPI_trust_af_info_create(
    OpenAPI_list_t *s_nssai_info_list,
    OpenAPI_list_t *af_events,
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *internal_group_id,
    bool is_mapping_ind,
    int mapping_ind
)
{
    OpenAPI_trust_af_info_t *trust_af_info_local_var = ogs_malloc(sizeof(OpenAPI_trust_af_info_t));
    ogs_assert(trust_af_info_local_var);

    trust_af_info_local_var->s_nssai_info_list = s_nssai_info_list;
    trust_af_info_local_var->af_events = af_events;
    trust_af_info_local_var->app_ids = app_ids;
    trust_af_info_local_var->internal_group_id = internal_group_id;
    trust_af_info_local_var->is_mapping_ind = is_mapping_ind;
    trust_af_info_local_var->mapping_ind = mapping_ind;

    return trust_af_info_local_var;
}

void OpenAPI_trust_af_info_free(OpenAPI_trust_af_info_t *trust_af_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == trust_af_info) {
        return;
    }
    if (trust_af_info->s_nssai_info_list) {
        OpenAPI_list_for_each(trust_af_info->s_nssai_info_list, node) {
            OpenAPI_snssai_info_item_free(node->data);
        }
        OpenAPI_list_free(trust_af_info->s_nssai_info_list);
        trust_af_info->s_nssai_info_list = NULL;
    }
    if (trust_af_info->af_events) {
        OpenAPI_list_free(trust_af_info->af_events);
        trust_af_info->af_events = NULL;
    }
    if (trust_af_info->app_ids) {
        OpenAPI_list_for_each(trust_af_info->app_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(trust_af_info->app_ids);
        trust_af_info->app_ids = NULL;
    }
    if (trust_af_info->internal_group_id) {
        OpenAPI_list_for_each(trust_af_info->internal_group_id, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(trust_af_info->internal_group_id);
        trust_af_info->internal_group_id = NULL;
    }
    ogs_free(trust_af_info);
}

cJSON *OpenAPI_trust_af_info_convertToJSON(OpenAPI_trust_af_info_t *trust_af_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (trust_af_info == NULL) {
        ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [TrustAfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (trust_af_info->s_nssai_info_list) {
    cJSON *s_nssai_info_listList = cJSON_AddArrayToObject(item, "sNssaiInfoList");
    if (s_nssai_info_listList == NULL) {
        ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [s_nssai_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(trust_af_info->s_nssai_info_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_info_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [s_nssai_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(s_nssai_info_listList, itemLocal);
    }
    }

    if (trust_af_info->af_events != OpenAPI_af_event_NULL) {
    cJSON *af_eventsList = cJSON_AddArrayToObject(item, "afEvents");
    if (af_eventsList == NULL) {
        ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [af_events]");
        goto end;
    }
    OpenAPI_list_for_each(trust_af_info->af_events, node) {
        if (cJSON_AddStringToObject(af_eventsList, "", OpenAPI_af_event_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [af_events]");
            goto end;
        }
    }
    }

    if (trust_af_info->app_ids) {
    cJSON *app_idsList = cJSON_AddArrayToObject(item, "appIds");
    if (app_idsList == NULL) {
        ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [app_ids]");
        goto end;
    }
    OpenAPI_list_for_each(trust_af_info->app_ids, node) {
        if (cJSON_AddStringToObject(app_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [app_ids]");
            goto end;
        }
    }
    }

    if (trust_af_info->internal_group_id) {
    cJSON *internal_group_idList = cJSON_AddArrayToObject(item, "internalGroupId");
    if (internal_group_idList == NULL) {
        ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [internal_group_id]");
        goto end;
    }
    OpenAPI_list_for_each(trust_af_info->internal_group_id, node) {
        if (cJSON_AddStringToObject(internal_group_idList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [internal_group_id]");
            goto end;
        }
    }
    }

    if (trust_af_info->is_mapping_ind) {
    if (cJSON_AddBoolToObject(item, "mappingInd", trust_af_info->mapping_ind) == NULL) {
        ogs_error("OpenAPI_trust_af_info_convertToJSON() failed [mapping_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_trust_af_info_t *OpenAPI_trust_af_info_parseFromJSON(cJSON *trust_af_infoJSON)
{
    OpenAPI_trust_af_info_t *trust_af_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *s_nssai_info_list = NULL;
    OpenAPI_list_t *s_nssai_info_listList = NULL;
    cJSON *af_events = NULL;
    OpenAPI_list_t *af_eventsList = NULL;
    cJSON *app_ids = NULL;
    OpenAPI_list_t *app_idsList = NULL;
    cJSON *internal_group_id = NULL;
    OpenAPI_list_t *internal_group_idList = NULL;
    cJSON *mapping_ind = NULL;
    s_nssai_info_list = cJSON_GetObjectItemCaseSensitive(trust_af_infoJSON, "sNssaiInfoList");
    if (s_nssai_info_list) {
        cJSON *s_nssai_info_list_local = NULL;
        if (!cJSON_IsArray(s_nssai_info_list)) {
            ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed [s_nssai_info_list]");
            goto end;
        }

        s_nssai_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssai_info_list_local, s_nssai_info_list) {
            if (!cJSON_IsObject(s_nssai_info_list_local)) {
                ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed [s_nssai_info_list]");
                goto end;
            }
            OpenAPI_snssai_info_item_t *s_nssai_info_listItem = OpenAPI_snssai_info_item_parseFromJSON(s_nssai_info_list_local);
            if (!s_nssai_info_listItem) {
                ogs_error("No s_nssai_info_listItem");
                goto end;
            }
            OpenAPI_list_add(s_nssai_info_listList, s_nssai_info_listItem);
        }
    }

    af_events = cJSON_GetObjectItemCaseSensitive(trust_af_infoJSON, "afEvents");
    if (af_events) {
        cJSON *af_events_local = NULL;
        if (!cJSON_IsArray(af_events)) {
            ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed [af_events]");
            goto end;
        }

        af_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_events_local, af_events) {
            OpenAPI_af_event_e localEnum = OpenAPI_af_event_NULL;
            if (!cJSON_IsString(af_events_local)) {
                ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed [af_events]");
                goto end;
            }
            localEnum = OpenAPI_af_event_FromString(af_events_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"af_events\" is not supported. Ignoring it ...",
                         af_events_local->valuestring);
            } else {
                OpenAPI_list_add(af_eventsList, (void *)localEnum);
            }
        }
        if (af_eventsList->count == 0) {
            ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed: Expected af_eventsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    app_ids = cJSON_GetObjectItemCaseSensitive(trust_af_infoJSON, "appIds");
    if (app_ids) {
        cJSON *app_ids_local = NULL;
        if (!cJSON_IsArray(app_ids)) {
            ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed [app_ids]");
            goto end;
        }

        app_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_ids_local, app_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(app_ids_local)) {
                ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed [app_ids]");
                goto end;
            }
            OpenAPI_list_add(app_idsList, ogs_strdup(app_ids_local->valuestring));
        }
    }

    internal_group_id = cJSON_GetObjectItemCaseSensitive(trust_af_infoJSON, "internalGroupId");
    if (internal_group_id) {
        cJSON *internal_group_id_local = NULL;
        if (!cJSON_IsArray(internal_group_id)) {
            ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed [internal_group_id]");
            goto end;
        }

        internal_group_idList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_id_local, internal_group_id) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(internal_group_id_local)) {
                ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed [internal_group_id]");
                goto end;
            }
            OpenAPI_list_add(internal_group_idList, ogs_strdup(internal_group_id_local->valuestring));
        }
    }

    mapping_ind = cJSON_GetObjectItemCaseSensitive(trust_af_infoJSON, "mappingInd");
    if (mapping_ind) {
    if (!cJSON_IsBool(mapping_ind)) {
        ogs_error("OpenAPI_trust_af_info_parseFromJSON() failed [mapping_ind]");
        goto end;
    }
    }

    trust_af_info_local_var = OpenAPI_trust_af_info_create (
        s_nssai_info_list ? s_nssai_info_listList : NULL,
        af_events ? af_eventsList : NULL,
        app_ids ? app_idsList : NULL,
        internal_group_id ? internal_group_idList : NULL,
        mapping_ind ? true : false,
        mapping_ind ? mapping_ind->valueint : 0
    );

    return trust_af_info_local_var;
end:
    if (s_nssai_info_listList) {
        OpenAPI_list_for_each(s_nssai_info_listList, node) {
            OpenAPI_snssai_info_item_free(node->data);
        }
        OpenAPI_list_free(s_nssai_info_listList);
        s_nssai_info_listList = NULL;
    }
    if (af_eventsList) {
        OpenAPI_list_free(af_eventsList);
        af_eventsList = NULL;
    }
    if (app_idsList) {
        OpenAPI_list_for_each(app_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(app_idsList);
        app_idsList = NULL;
    }
    if (internal_group_idList) {
        OpenAPI_list_for_each(internal_group_idList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(internal_group_idList);
        internal_group_idList = NULL;
    }
    return NULL;
}

OpenAPI_trust_af_info_t *OpenAPI_trust_af_info_copy(OpenAPI_trust_af_info_t *dst, OpenAPI_trust_af_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_trust_af_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_trust_af_info_convertToJSON() failed");
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

    OpenAPI_trust_af_info_free(dst);
    dst = OpenAPI_trust_af_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


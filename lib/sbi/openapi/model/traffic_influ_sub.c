
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_influ_sub.h"

OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_create(
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *snssais,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t *internal_group_ids_add,
    OpenAPI_list_t *subscriber_cat_list,
    OpenAPI_list_t *supis,
    OpenAPI_plmn_id_t *plmn_id,
    char *ipv4_addr,
    char *ipv6_addr,
    char *notification_uri,
    char *notif_id,
    char *expiry,
    char *supported_features,
    OpenAPI_list_t *reset_ids,
    bool is_imm_rep,
    int imm_rep,
    OpenAPI_list_t *imm_reports
)
{
    OpenAPI_traffic_influ_sub_t *traffic_influ_sub_local_var = ogs_malloc(sizeof(OpenAPI_traffic_influ_sub_t));
    ogs_assert(traffic_influ_sub_local_var);

    traffic_influ_sub_local_var->dnns = dnns;
    traffic_influ_sub_local_var->snssais = snssais;
    traffic_influ_sub_local_var->internal_group_ids = internal_group_ids;
    traffic_influ_sub_local_var->internal_group_ids_add = internal_group_ids_add;
    traffic_influ_sub_local_var->subscriber_cat_list = subscriber_cat_list;
    traffic_influ_sub_local_var->supis = supis;
    traffic_influ_sub_local_var->plmn_id = plmn_id;
    traffic_influ_sub_local_var->ipv4_addr = ipv4_addr;
    traffic_influ_sub_local_var->ipv6_addr = ipv6_addr;
    traffic_influ_sub_local_var->notification_uri = notification_uri;
    traffic_influ_sub_local_var->notif_id = notif_id;
    traffic_influ_sub_local_var->expiry = expiry;
    traffic_influ_sub_local_var->supported_features = supported_features;
    traffic_influ_sub_local_var->reset_ids = reset_ids;
    traffic_influ_sub_local_var->is_imm_rep = is_imm_rep;
    traffic_influ_sub_local_var->imm_rep = imm_rep;
    traffic_influ_sub_local_var->imm_reports = imm_reports;

    return traffic_influ_sub_local_var;
}

void OpenAPI_traffic_influ_sub_free(OpenAPI_traffic_influ_sub_t *traffic_influ_sub)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_influ_sub) {
        return;
    }
    if (traffic_influ_sub->dnns) {
        OpenAPI_list_for_each(traffic_influ_sub->dnns, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->dnns);
        traffic_influ_sub->dnns = NULL;
    }
    if (traffic_influ_sub->snssais) {
        OpenAPI_list_for_each(traffic_influ_sub->snssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->snssais);
        traffic_influ_sub->snssais = NULL;
    }
    if (traffic_influ_sub->internal_group_ids) {
        OpenAPI_list_for_each(traffic_influ_sub->internal_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->internal_group_ids);
        traffic_influ_sub->internal_group_ids = NULL;
    }
    if (traffic_influ_sub->internal_group_ids_add) {
        OpenAPI_list_for_each(traffic_influ_sub->internal_group_ids_add, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->internal_group_ids_add);
        traffic_influ_sub->internal_group_ids_add = NULL;
    }
    if (traffic_influ_sub->subscriber_cat_list) {
        OpenAPI_list_for_each(traffic_influ_sub->subscriber_cat_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->subscriber_cat_list);
        traffic_influ_sub->subscriber_cat_list = NULL;
    }
    if (traffic_influ_sub->supis) {
        OpenAPI_list_for_each(traffic_influ_sub->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->supis);
        traffic_influ_sub->supis = NULL;
    }
    if (traffic_influ_sub->plmn_id) {
        OpenAPI_plmn_id_free(traffic_influ_sub->plmn_id);
        traffic_influ_sub->plmn_id = NULL;
    }
    if (traffic_influ_sub->ipv4_addr) {
        ogs_free(traffic_influ_sub->ipv4_addr);
        traffic_influ_sub->ipv4_addr = NULL;
    }
    if (traffic_influ_sub->ipv6_addr) {
        ogs_free(traffic_influ_sub->ipv6_addr);
        traffic_influ_sub->ipv6_addr = NULL;
    }
    if (traffic_influ_sub->notification_uri) {
        ogs_free(traffic_influ_sub->notification_uri);
        traffic_influ_sub->notification_uri = NULL;
    }
    if (traffic_influ_sub->notif_id) {
        ogs_free(traffic_influ_sub->notif_id);
        traffic_influ_sub->notif_id = NULL;
    }
    if (traffic_influ_sub->expiry) {
        ogs_free(traffic_influ_sub->expiry);
        traffic_influ_sub->expiry = NULL;
    }
    if (traffic_influ_sub->supported_features) {
        ogs_free(traffic_influ_sub->supported_features);
        traffic_influ_sub->supported_features = NULL;
    }
    if (traffic_influ_sub->reset_ids) {
        OpenAPI_list_for_each(traffic_influ_sub->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->reset_ids);
        traffic_influ_sub->reset_ids = NULL;
    }
    if (traffic_influ_sub->imm_reports) {
        OpenAPI_list_for_each(traffic_influ_sub->imm_reports, node) {
            OpenAPI_traffic_influ_data_notif_free(node->data);
        }
        OpenAPI_list_free(traffic_influ_sub->imm_reports);
        traffic_influ_sub->imm_reports = NULL;
    }
    ogs_free(traffic_influ_sub);
}

cJSON *OpenAPI_traffic_influ_sub_convertToJSON(OpenAPI_traffic_influ_sub_t *traffic_influ_sub)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_influ_sub == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [TrafficInfluSub]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_influ_sub->dnns) {
    cJSON *dnnsList = cJSON_AddArrayToObject(item, "dnns");
    if (dnnsList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [dnns]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->dnns, node) {
        if (cJSON_AddStringToObject(dnnsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [dnns]");
            goto end;
        }
    }
    }

    if (traffic_influ_sub->snssais) {
    cJSON *snssaisList = cJSON_AddArrayToObject(item, "snssais");
    if (snssaisList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [snssais]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->snssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [snssais]");
            goto end;
        }
        cJSON_AddItemToArray(snssaisList, itemLocal);
    }
    }

    if (traffic_influ_sub->internal_group_ids) {
    cJSON *internal_group_idsList = cJSON_AddArrayToObject(item, "internalGroupIds");
    if (internal_group_idsList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [internal_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->internal_group_ids, node) {
        if (cJSON_AddStringToObject(internal_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [internal_group_ids]");
            goto end;
        }
    }
    }

    if (traffic_influ_sub->internal_group_ids_add) {
    cJSON *internal_group_ids_addList = cJSON_AddArrayToObject(item, "internalGroupIdsAdd");
    if (internal_group_ids_addList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [internal_group_ids_add]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->internal_group_ids_add, node) {
        if (cJSON_AddStringToObject(internal_group_ids_addList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [internal_group_ids_add]");
            goto end;
        }
    }
    }

    if (traffic_influ_sub->subscriber_cat_list) {
    cJSON *subscriber_cat_listList = cJSON_AddArrayToObject(item, "subscriberCatList");
    if (subscriber_cat_listList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [subscriber_cat_list]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->subscriber_cat_list, node) {
        if (cJSON_AddStringToObject(subscriber_cat_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [subscriber_cat_list]");
            goto end;
        }
    }
    }

    if (traffic_influ_sub->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (traffic_influ_sub->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(traffic_influ_sub->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (traffic_influ_sub->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", traffic_influ_sub->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (traffic_influ_sub->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", traffic_influ_sub->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (!traffic_influ_sub->notification_uri) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [notification_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notificationUri", traffic_influ_sub->notification_uri) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (traffic_influ_sub->notif_id) {
    if (cJSON_AddStringToObject(item, "notifId", traffic_influ_sub->notif_id) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [notif_id]");
        goto end;
    }
    }

    if (traffic_influ_sub->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", traffic_influ_sub->expiry) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (traffic_influ_sub->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", traffic_influ_sub->supported_features) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (traffic_influ_sub->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

    if (traffic_influ_sub->is_imm_rep) {
    if (cJSON_AddBoolToObject(item, "immRep", traffic_influ_sub->imm_rep) == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [imm_rep]");
        goto end;
    }
    }

    if (traffic_influ_sub->imm_reports) {
    cJSON *imm_reportsList = cJSON_AddArrayToObject(item, "immReports");
    if (imm_reportsList == NULL) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [imm_reports]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_influ_sub->imm_reports, node) {
        cJSON *itemLocal = OpenAPI_traffic_influ_data_notif_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed [imm_reports]");
            goto end;
        }
        cJSON_AddItemToArray(imm_reportsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_parseFromJSON(cJSON *traffic_influ_subJSON)
{
    OpenAPI_traffic_influ_sub_t *traffic_influ_sub_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnns = NULL;
    OpenAPI_list_t *dnnsList = NULL;
    cJSON *snssais = NULL;
    OpenAPI_list_t *snssaisList = NULL;
    cJSON *internal_group_ids = NULL;
    OpenAPI_list_t *internal_group_idsList = NULL;
    cJSON *internal_group_ids_add = NULL;
    OpenAPI_list_t *internal_group_ids_addList = NULL;
    cJSON *subscriber_cat_list = NULL;
    OpenAPI_list_t *subscriber_cat_listList = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_addr = NULL;
    cJSON *notification_uri = NULL;
    cJSON *notif_id = NULL;
    cJSON *expiry = NULL;
    cJSON *supported_features = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    cJSON *imm_rep = NULL;
    cJSON *imm_reports = NULL;
    OpenAPI_list_t *imm_reportsList = NULL;
    dnns = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "dnns");
    if (dnns) {
        cJSON *dnns_local = NULL;
        if (!cJSON_IsArray(dnns)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [dnns]");
            goto end;
        }

        dnnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnns_local, dnns) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnns_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [dnns]");
                goto end;
            }
            OpenAPI_list_add(dnnsList, ogs_strdup(dnns_local->valuestring));
        }
    }

    snssais = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "snssais");
    if (snssais) {
        cJSON *snssais_local = NULL;
        if (!cJSON_IsArray(snssais)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [snssais]");
            goto end;
        }

        snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssais_local, snssais) {
            if (!cJSON_IsObject(snssais_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [snssais]");
                goto end;
            }
            OpenAPI_snssai_t *snssaisItem = OpenAPI_snssai_parseFromJSON(snssais_local);
            if (!snssaisItem) {
                ogs_error("No snssaisItem");
                goto end;
            }
            OpenAPI_list_add(snssaisList, snssaisItem);
        }
    }

    internal_group_ids = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "internalGroupIds");
    if (internal_group_ids) {
        cJSON *internal_group_ids_local = NULL;
        if (!cJSON_IsArray(internal_group_ids)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [internal_group_ids]");
            goto end;
        }

        internal_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(internal_group_ids_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [internal_group_ids]");
                goto end;
            }
            OpenAPI_list_add(internal_group_idsList, ogs_strdup(internal_group_ids_local->valuestring));
        }
    }

    internal_group_ids_add = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "internalGroupIdsAdd");
    if (internal_group_ids_add) {
        cJSON *internal_group_ids_add_local = NULL;
        if (!cJSON_IsArray(internal_group_ids_add)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [internal_group_ids_add]");
            goto end;
        }

        internal_group_ids_addList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_ids_add_local, internal_group_ids_add) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(internal_group_ids_add_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [internal_group_ids_add]");
                goto end;
            }
            OpenAPI_list_add(internal_group_ids_addList, ogs_strdup(internal_group_ids_add_local->valuestring));
        }
    }

    subscriber_cat_list = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "subscriberCatList");
    if (subscriber_cat_list) {
        cJSON *subscriber_cat_list_local = NULL;
        if (!cJSON_IsArray(subscriber_cat_list)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [subscriber_cat_list]");
            goto end;
        }

        subscriber_cat_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscriber_cat_list_local, subscriber_cat_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(subscriber_cat_list_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [subscriber_cat_list]");
                goto end;
            }
            OpenAPI_list_add(subscriber_cat_listList, ogs_strdup(subscriber_cat_list_local->valuestring));
        }
    }

    supis = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    ipv4_addr = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_addr = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "ipv6Addr");
    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr) && !cJSON_IsNull(ipv6_addr)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    notification_uri = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    notif_id = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "notifId");
    if (notif_id) {
    if (!cJSON_IsString(notif_id) && !cJSON_IsNull(notif_id)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [notif_id]");
        goto end;
    }
    }

    expiry = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    imm_rep = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "immRep");
    if (imm_rep) {
    if (!cJSON_IsBool(imm_rep)) {
        ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [imm_rep]");
        goto end;
    }
    }

    imm_reports = cJSON_GetObjectItemCaseSensitive(traffic_influ_subJSON, "immReports");
    if (imm_reports) {
        cJSON *imm_reports_local = NULL;
        if (!cJSON_IsArray(imm_reports)) {
            ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [imm_reports]");
            goto end;
        }

        imm_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(imm_reports_local, imm_reports) {
            if (!cJSON_IsObject(imm_reports_local)) {
                ogs_error("OpenAPI_traffic_influ_sub_parseFromJSON() failed [imm_reports]");
                goto end;
            }
            OpenAPI_traffic_influ_data_notif_t *imm_reportsItem = OpenAPI_traffic_influ_data_notif_parseFromJSON(imm_reports_local);
            if (!imm_reportsItem) {
                ogs_error("No imm_reportsItem");
                goto end;
            }
            OpenAPI_list_add(imm_reportsList, imm_reportsItem);
        }
    }

    traffic_influ_sub_local_var = OpenAPI_traffic_influ_sub_create (
        dnns ? dnnsList : NULL,
        snssais ? snssaisList : NULL,
        internal_group_ids ? internal_group_idsList : NULL,
        internal_group_ids_add ? internal_group_ids_addList : NULL,
        subscriber_cat_list ? subscriber_cat_listList : NULL,
        supis ? supisList : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr && !cJSON_IsNull(ipv6_addr) ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        ogs_strdup(notification_uri->valuestring),
        notif_id && !cJSON_IsNull(notif_id) ? ogs_strdup(notif_id->valuestring) : NULL,
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL,
        imm_rep ? true : false,
        imm_rep ? imm_rep->valueint : 0,
        imm_reports ? imm_reportsList : NULL
    );

    return traffic_influ_sub_local_var;
end:
    if (dnnsList) {
        OpenAPI_list_for_each(dnnsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnnsList);
        dnnsList = NULL;
    }
    if (snssaisList) {
        OpenAPI_list_for_each(snssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssaisList);
        snssaisList = NULL;
    }
    if (internal_group_idsList) {
        OpenAPI_list_for_each(internal_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(internal_group_idsList);
        internal_group_idsList = NULL;
    }
    if (internal_group_ids_addList) {
        OpenAPI_list_for_each(internal_group_ids_addList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(internal_group_ids_addList);
        internal_group_ids_addList = NULL;
    }
    if (subscriber_cat_listList) {
        OpenAPI_list_for_each(subscriber_cat_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscriber_cat_listList);
        subscriber_cat_listList = NULL;
    }
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    if (imm_reportsList) {
        OpenAPI_list_for_each(imm_reportsList, node) {
            OpenAPI_traffic_influ_data_notif_free(node->data);
        }
        OpenAPI_list_free(imm_reportsList);
        imm_reportsList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_influ_sub_t *OpenAPI_traffic_influ_sub_copy(OpenAPI_traffic_influ_sub_t *dst, OpenAPI_traffic_influ_sub_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_influ_sub_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_influ_sub_convertToJSON() failed");
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

    OpenAPI_traffic_influ_sub_free(dst);
    dst = OpenAPI_traffic_influ_sub_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


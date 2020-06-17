
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_service.h"

OpenAPI_nf_service_t *OpenAPI_nf_service_create(
    char *service_instance_id,
    char *service_name,
    OpenAPI_list_t *versions,
    OpenAPI_uri_scheme_e scheme,
    OpenAPI_nf_service_status_e nf_service_status,
    char *fqdn,
    char *inter_plmn_fqdn,
    OpenAPI_list_t *ip_end_points,
    char *api_prefix,
    OpenAPI_list_t *default_notification_subscriptions,
    OpenAPI_list_t *allowed_plmns,
    OpenAPI_list_t *allowed_snpns,
    OpenAPI_list_t *allowed_nf_types,
    OpenAPI_list_t *allowed_nf_domains,
    OpenAPI_list_t *allowed_nssais,
    int priority,
    int capacity,
    int load,
    char *load_time_stamp,
    char *recovery_time,
    OpenAPI_chf_service_info_t *chf_service_info,
    char *supported_features,
    OpenAPI_list_t *nf_service_set_id_list,
    OpenAPI_list_t *s_nssais,
    OpenAPI_list_t *per_plmn_snssai_list,
    char *vendor_id,
    OpenAPI_list_t* supported_vendor_specific_features,
    int oauth2_required
    )
{
    OpenAPI_nf_service_t *nf_service_local_var = OpenAPI_malloc(sizeof(OpenAPI_nf_service_t));
    if (!nf_service_local_var) {
        return NULL;
    }
    nf_service_local_var->service_instance_id = service_instance_id;
    nf_service_local_var->service_name = service_name;
    nf_service_local_var->versions = versions;
    nf_service_local_var->scheme = scheme;
    nf_service_local_var->nf_service_status = nf_service_status;
    nf_service_local_var->fqdn = fqdn;
    nf_service_local_var->inter_plmn_fqdn = inter_plmn_fqdn;
    nf_service_local_var->ip_end_points = ip_end_points;
    nf_service_local_var->api_prefix = api_prefix;
    nf_service_local_var->default_notification_subscriptions = default_notification_subscriptions;
    nf_service_local_var->allowed_plmns = allowed_plmns;
    nf_service_local_var->allowed_snpns = allowed_snpns;
    nf_service_local_var->allowed_nf_types = allowed_nf_types;
    nf_service_local_var->allowed_nf_domains = allowed_nf_domains;
    nf_service_local_var->allowed_nssais = allowed_nssais;
    nf_service_local_var->priority = priority;
    nf_service_local_var->capacity = capacity;
    nf_service_local_var->load = load;
    nf_service_local_var->load_time_stamp = load_time_stamp;
    nf_service_local_var->recovery_time = recovery_time;
    nf_service_local_var->chf_service_info = chf_service_info;
    nf_service_local_var->supported_features = supported_features;
    nf_service_local_var->nf_service_set_id_list = nf_service_set_id_list;
    nf_service_local_var->s_nssais = s_nssais;
    nf_service_local_var->per_plmn_snssai_list = per_plmn_snssai_list;
    nf_service_local_var->vendor_id = vendor_id;
    nf_service_local_var->supported_vendor_specific_features = supported_vendor_specific_features;
    nf_service_local_var->oauth2_required = oauth2_required;

    return nf_service_local_var;
}

void OpenAPI_nf_service_free(OpenAPI_nf_service_t *nf_service)
{
    if (NULL == nf_service) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nf_service->service_instance_id);
    ogs_free(nf_service->service_name);
    OpenAPI_list_for_each(nf_service->versions, node) {
        OpenAPI_nf_service_version_free(node->data);
    }
    OpenAPI_list_free(nf_service->versions);
    ogs_free(nf_service->fqdn);
    ogs_free(nf_service->inter_plmn_fqdn);
    OpenAPI_list_for_each(nf_service->ip_end_points, node) {
        OpenAPI_ip_end_point_free(node->data);
    }
    OpenAPI_list_free(nf_service->ip_end_points);
    ogs_free(nf_service->api_prefix);
    OpenAPI_list_for_each(nf_service->default_notification_subscriptions, node) {
        OpenAPI_default_notification_subscription_free(node->data);
    }
    OpenAPI_list_free(nf_service->default_notification_subscriptions);
    OpenAPI_list_for_each(nf_service->allowed_plmns, node) {
        OpenAPI_plmn_id_free(node->data);
    }
    OpenAPI_list_free(nf_service->allowed_plmns);
    OpenAPI_list_for_each(nf_service->allowed_snpns, node) {
        OpenAPI_plmn_id_nid_free(node->data);
    }
    OpenAPI_list_free(nf_service->allowed_snpns);
    OpenAPI_list_free(nf_service->allowed_nf_types);
    OpenAPI_list_for_each(nf_service->allowed_nf_domains, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(nf_service->allowed_nf_domains);
    OpenAPI_list_for_each(nf_service->allowed_nssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(nf_service->allowed_nssais);
    ogs_free(nf_service->load_time_stamp);
    ogs_free(nf_service->recovery_time);
    OpenAPI_chf_service_info_free(nf_service->chf_service_info);
    ogs_free(nf_service->supported_features);
    OpenAPI_list_for_each(nf_service->nf_service_set_id_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(nf_service->nf_service_set_id_list);
    OpenAPI_list_for_each(nf_service->s_nssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(nf_service->s_nssais);
    OpenAPI_list_for_each(nf_service->per_plmn_snssai_list, node) {
        OpenAPI_plmn_snssai_free(node->data);
    }
    OpenAPI_list_free(nf_service->per_plmn_snssai_list);
    ogs_free(nf_service->vendor_id);
    OpenAPI_list_for_each(nf_service->supported_vendor_specific_features, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(nf_service->supported_vendor_specific_features);
    ogs_free(nf_service);
}

cJSON *OpenAPI_nf_service_convertToJSON(OpenAPI_nf_service_t *nf_service)
{
    cJSON *item = NULL;

    if (nf_service == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [NFService]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nf_service->service_instance_id) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [service_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "serviceInstanceId", nf_service->service_instance_id) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [service_instance_id]");
        goto end;
    }

    if (!nf_service->service_name) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [service_name]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "serviceName", nf_service->service_name) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [service_name]");
        goto end;
    }

    if (!nf_service->versions) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [versions]");
        goto end;
    }
    cJSON *versionsList = cJSON_AddArrayToObject(item, "versions");
    if (versionsList == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [versions]");
        goto end;
    }

    OpenAPI_lnode_t *versions_node;
    if (nf_service->versions) {
        OpenAPI_list_for_each(nf_service->versions, versions_node) {
            cJSON *itemLocal = OpenAPI_nf_service_version_convertToJSON(versions_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [versions]");
                goto end;
            }
            cJSON_AddItemToArray(versionsList, itemLocal);
        }
    }

    if (!nf_service->scheme) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [scheme]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "scheme", OpenAPI_uri_scheme_ToString(nf_service->scheme)) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [scheme]");
        goto end;
    }

    if (!nf_service->nf_service_status) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [nf_service_status]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfServiceStatus", OpenAPI_nf_service_status_ToString(nf_service->nf_service_status)) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [nf_service_status]");
        goto end;
    }

    if (nf_service->fqdn) {
        if (cJSON_AddStringToObject(item, "fqdn", nf_service->fqdn) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [fqdn]");
            goto end;
        }
    }

    if (nf_service->inter_plmn_fqdn) {
        if (cJSON_AddStringToObject(item, "interPlmnFqdn", nf_service->inter_plmn_fqdn) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [inter_plmn_fqdn]");
            goto end;
        }
    }

    if (nf_service->ip_end_points) {
        cJSON *ip_end_pointsList = cJSON_AddArrayToObject(item, "ipEndPoints");
        if (ip_end_pointsList == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [ip_end_points]");
            goto end;
        }

        OpenAPI_lnode_t *ip_end_points_node;
        if (nf_service->ip_end_points) {
            OpenAPI_list_for_each(nf_service->ip_end_points, ip_end_points_node) {
                cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(ip_end_points_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_service_convertToJSON() failed [ip_end_points]");
                    goto end;
                }
                cJSON_AddItemToArray(ip_end_pointsList, itemLocal);
            }
        }
    }

    if (nf_service->api_prefix) {
        if (cJSON_AddStringToObject(item, "apiPrefix", nf_service->api_prefix) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [api_prefix]");
            goto end;
        }
    }

    if (nf_service->default_notification_subscriptions) {
        cJSON *default_notification_subscriptionsList = cJSON_AddArrayToObject(item, "defaultNotificationSubscriptions");
        if (default_notification_subscriptionsList == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [default_notification_subscriptions]");
            goto end;
        }

        OpenAPI_lnode_t *default_notification_subscriptions_node;
        if (nf_service->default_notification_subscriptions) {
            OpenAPI_list_for_each(nf_service->default_notification_subscriptions, default_notification_subscriptions_node) {
                cJSON *itemLocal = OpenAPI_default_notification_subscription_convertToJSON(default_notification_subscriptions_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_service_convertToJSON() failed [default_notification_subscriptions]");
                    goto end;
                }
                cJSON_AddItemToArray(default_notification_subscriptionsList, itemLocal);
            }
        }
    }

    if (nf_service->allowed_plmns) {
        cJSON *allowed_plmnsList = cJSON_AddArrayToObject(item, "allowedPlmns");
        if (allowed_plmnsList == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_plmns]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_plmns_node;
        if (nf_service->allowed_plmns) {
            OpenAPI_list_for_each(nf_service->allowed_plmns, allowed_plmns_node) {
                cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(allowed_plmns_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_plmns]");
                    goto end;
                }
                cJSON_AddItemToArray(allowed_plmnsList, itemLocal);
            }
        }
    }

    if (nf_service->allowed_snpns) {
        cJSON *allowed_snpnsList = cJSON_AddArrayToObject(item, "allowedSnpns");
        if (allowed_snpnsList == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_snpns]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_snpns_node;
        if (nf_service->allowed_snpns) {
            OpenAPI_list_for_each(nf_service->allowed_snpns, allowed_snpns_node) {
                cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(allowed_snpns_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_snpns]");
                    goto end;
                }
                cJSON_AddItemToArray(allowed_snpnsList, itemLocal);
            }
        }
    }

    if (nf_service->allowed_nf_types) {
        cJSON *allowed_nf_types = cJSON_AddArrayToObject(item, "allowedNfTypes");
        if (allowed_nf_types == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nf_types]");
            goto end;
        }
        OpenAPI_lnode_t *allowed_nf_types_node;
        OpenAPI_list_for_each(nf_service->allowed_nf_types, allowed_nf_types_node) {
            if (cJSON_AddStringToObject(allowed_nf_types, "", OpenAPI_nf_type_ToString((OpenAPI_nf_type_e)allowed_nf_types_node->data)) == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nf_types]");
                goto end;
            }
        }
    }

    if (nf_service->allowed_nf_domains) {
        cJSON *allowed_nf_domains = cJSON_AddArrayToObject(item, "allowedNfDomains");
        if (allowed_nf_domains == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nf_domains]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_nf_domains_node;
        OpenAPI_list_for_each(nf_service->allowed_nf_domains, allowed_nf_domains_node)  {
            if (cJSON_AddStringToObject(allowed_nf_domains, "", (char*)allowed_nf_domains_node->data) == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nf_domains]");
                goto end;
            }
        }
    }

    if (nf_service->allowed_nssais) {
        cJSON *allowed_nssaisList = cJSON_AddArrayToObject(item, "allowedNssais");
        if (allowed_nssaisList == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nssais]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_nssais_node;
        if (nf_service->allowed_nssais) {
            OpenAPI_list_for_each(nf_service->allowed_nssais, allowed_nssais_node) {
                cJSON *itemLocal = OpenAPI_snssai_convertToJSON(allowed_nssais_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nssais]");
                    goto end;
                }
                cJSON_AddItemToArray(allowed_nssaisList, itemLocal);
            }
        }
    }

    if (nf_service->priority) {
        if (cJSON_AddNumberToObject(item, "priority", nf_service->priority) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [priority]");
            goto end;
        }
    }

    if (nf_service->capacity) {
        if (cJSON_AddNumberToObject(item, "capacity", nf_service->capacity) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [capacity]");
            goto end;
        }
    }

    if (nf_service->load) {
        if (cJSON_AddNumberToObject(item, "load", nf_service->load) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [load]");
            goto end;
        }
    }

    if (nf_service->load_time_stamp) {
        if (cJSON_AddStringToObject(item, "loadTimeStamp", nf_service->load_time_stamp) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [load_time_stamp]");
            goto end;
        }
    }

    if (nf_service->recovery_time) {
        if (cJSON_AddStringToObject(item, "recoveryTime", nf_service->recovery_time) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [recovery_time]");
            goto end;
        }
    }

    if (nf_service->chf_service_info) {
        cJSON *chf_service_info_local_JSON = OpenAPI_chf_service_info_convertToJSON(nf_service->chf_service_info);
        if (chf_service_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [chf_service_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "chfServiceInfo", chf_service_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [chf_service_info]");
            goto end;
        }
    }

    if (nf_service->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", nf_service->supported_features) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

    if (nf_service->nf_service_set_id_list) {
        cJSON *nf_service_set_id_list = cJSON_AddArrayToObject(item, "nfServiceSetIdList");
        if (nf_service_set_id_list == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [nf_service_set_id_list]");
            goto end;
        }

        OpenAPI_lnode_t *nf_service_set_id_list_node;
        OpenAPI_list_for_each(nf_service->nf_service_set_id_list, nf_service_set_id_list_node)  {
            if (cJSON_AddStringToObject(nf_service_set_id_list, "", (char*)nf_service_set_id_list_node->data) == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [nf_service_set_id_list]");
                goto end;
            }
        }
    }

    if (nf_service->s_nssais) {
        cJSON *s_nssaisList = cJSON_AddArrayToObject(item, "sNssais");
        if (s_nssaisList == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [s_nssais]");
            goto end;
        }

        OpenAPI_lnode_t *s_nssais_node;
        if (nf_service->s_nssais) {
            OpenAPI_list_for_each(nf_service->s_nssais, s_nssais_node) {
                cJSON *itemLocal = OpenAPI_snssai_convertToJSON(s_nssais_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_service_convertToJSON() failed [s_nssais]");
                    goto end;
                }
                cJSON_AddItemToArray(s_nssaisList, itemLocal);
            }
        }
    }

    if (nf_service->per_plmn_snssai_list) {
        cJSON *per_plmn_snssai_listList = cJSON_AddArrayToObject(item, "perPlmnSnssaiList");
        if (per_plmn_snssai_listList == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [per_plmn_snssai_list]");
            goto end;
        }

        OpenAPI_lnode_t *per_plmn_snssai_list_node;
        if (nf_service->per_plmn_snssai_list) {
            OpenAPI_list_for_each(nf_service->per_plmn_snssai_list, per_plmn_snssai_list_node) {
                cJSON *itemLocal = OpenAPI_plmn_snssai_convertToJSON(per_plmn_snssai_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_service_convertToJSON() failed [per_plmn_snssai_list]");
                    goto end;
                }
                cJSON_AddItemToArray(per_plmn_snssai_listList, itemLocal);
            }
        }
    }

    if (nf_service->vendor_id) {
        if (cJSON_AddStringToObject(item, "vendorId", nf_service->vendor_id) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [vendor_id]");
            goto end;
        }
    }

    if (nf_service->supported_vendor_specific_features) {
        cJSON *supported_vendor_specific_features = cJSON_AddObjectToObject(item, "supportedVendorSpecificFeatures");
        if (supported_vendor_specific_features == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [supported_vendor_specific_features]");
            goto end;
        }
        cJSON *localMapObject = supported_vendor_specific_features;
        OpenAPI_lnode_t *supported_vendor_specific_features_node;
        if (nf_service->supported_vendor_specific_features) {
            OpenAPI_list_for_each(nf_service->supported_vendor_specific_features, supported_vendor_specific_features_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)supported_vendor_specific_features_node->data;
            }
        }
    }

    if (nf_service->oauth2_required) {
        if (cJSON_AddBoolToObject(item, "oauth2Required", nf_service->oauth2_required) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [oauth2_required]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_nf_service_t *OpenAPI_nf_service_parseFromJSON(cJSON *nf_serviceJSON)
{
    OpenAPI_nf_service_t *nf_service_local_var = NULL;
    cJSON *service_instance_id = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "serviceInstanceId");
    if (!service_instance_id) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [service_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(service_instance_id)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [service_instance_id]");
        goto end;
    }

    cJSON *service_name = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "serviceName");
    if (!service_name) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [service_name]");
        goto end;
    }


    if (!cJSON_IsString(service_name)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [service_name]");
        goto end;
    }

    cJSON *versions = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "versions");
    if (!versions) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [versions]");
        goto end;
    }

    OpenAPI_list_t *versionsList;

    cJSON *versions_local_nonprimitive;
    if (!cJSON_IsArray(versions)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [versions]");
        goto end;
    }

    versionsList = OpenAPI_list_create();

    cJSON_ArrayForEach(versions_local_nonprimitive, versions ) {
        if (!cJSON_IsObject(versions_local_nonprimitive)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [versions]");
            goto end;
        }
        OpenAPI_nf_service_version_t *versionsItem = OpenAPI_nf_service_version_parseFromJSON(versions_local_nonprimitive);

        OpenAPI_list_add(versionsList, versionsItem);
    }

    cJSON *scheme = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "scheme");
    if (!scheme) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [scheme]");
        goto end;
    }

    OpenAPI_uri_scheme_e schemeVariable;

    if (!cJSON_IsString(scheme)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [scheme]");
        goto end;
    }
    schemeVariable = OpenAPI_uri_scheme_FromString(scheme->valuestring);

    cJSON *nf_service_status = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "nfServiceStatus");
    if (!nf_service_status) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [nf_service_status]");
        goto end;
    }

    OpenAPI_nf_service_status_e nf_service_statusVariable;

    if (!cJSON_IsString(nf_service_status)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [nf_service_status]");
        goto end;
    }
    nf_service_statusVariable = OpenAPI_nf_service_status_FromString(nf_service_status->valuestring);

    cJSON *fqdn = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "fqdn");

    if (fqdn) {
        if (!cJSON_IsString(fqdn)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [fqdn]");
            goto end;
        }
    }

    cJSON *inter_plmn_fqdn = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "interPlmnFqdn");

    if (inter_plmn_fqdn) {
        if (!cJSON_IsString(inter_plmn_fqdn)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [inter_plmn_fqdn]");
            goto end;
        }
    }

    cJSON *ip_end_points = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "ipEndPoints");

    OpenAPI_list_t *ip_end_pointsList;
    if (ip_end_points) {
        cJSON *ip_end_points_local_nonprimitive;
        if (!cJSON_IsArray(ip_end_points)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [ip_end_points]");
            goto end;
        }

        ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ip_end_points_local_nonprimitive, ip_end_points ) {
            if (!cJSON_IsObject(ip_end_points_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [ip_end_points]");
                goto end;
            }
            OpenAPI_ip_end_point_t *ip_end_pointsItem = OpenAPI_ip_end_point_parseFromJSON(ip_end_points_local_nonprimitive);

            OpenAPI_list_add(ip_end_pointsList, ip_end_pointsItem);
        }
    }

    cJSON *api_prefix = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "apiPrefix");

    if (api_prefix) {
        if (!cJSON_IsString(api_prefix)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [api_prefix]");
            goto end;
        }
    }

    cJSON *default_notification_subscriptions = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "defaultNotificationSubscriptions");

    OpenAPI_list_t *default_notification_subscriptionsList;
    if (default_notification_subscriptions) {
        cJSON *default_notification_subscriptions_local_nonprimitive;
        if (!cJSON_IsArray(default_notification_subscriptions)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [default_notification_subscriptions]");
            goto end;
        }

        default_notification_subscriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(default_notification_subscriptions_local_nonprimitive, default_notification_subscriptions ) {
            if (!cJSON_IsObject(default_notification_subscriptions_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [default_notification_subscriptions]");
                goto end;
            }
            OpenAPI_default_notification_subscription_t *default_notification_subscriptionsItem = OpenAPI_default_notification_subscription_parseFromJSON(default_notification_subscriptions_local_nonprimitive);

            OpenAPI_list_add(default_notification_subscriptionsList, default_notification_subscriptionsItem);
        }
    }

    cJSON *allowed_plmns = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedPlmns");

    OpenAPI_list_t *allowed_plmnsList;
    if (allowed_plmns) {
        cJSON *allowed_plmns_local_nonprimitive;
        if (!cJSON_IsArray(allowed_plmns)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_plmns]");
            goto end;
        }

        allowed_plmnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_plmns_local_nonprimitive, allowed_plmns ) {
            if (!cJSON_IsObject(allowed_plmns_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_plmns]");
                goto end;
            }
            OpenAPI_plmn_id_t *allowed_plmnsItem = OpenAPI_plmn_id_parseFromJSON(allowed_plmns_local_nonprimitive);

            OpenAPI_list_add(allowed_plmnsList, allowed_plmnsItem);
        }
    }

    cJSON *allowed_snpns = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedSnpns");

    OpenAPI_list_t *allowed_snpnsList;
    if (allowed_snpns) {
        cJSON *allowed_snpns_local_nonprimitive;
        if (!cJSON_IsArray(allowed_snpns)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_snpns]");
            goto end;
        }

        allowed_snpnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_snpns_local_nonprimitive, allowed_snpns ) {
            if (!cJSON_IsObject(allowed_snpns_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_snpns]");
                goto end;
            }
            OpenAPI_plmn_id_nid_t *allowed_snpnsItem = OpenAPI_plmn_id_nid_parseFromJSON(allowed_snpns_local_nonprimitive);

            OpenAPI_list_add(allowed_snpnsList, allowed_snpnsItem);
        }
    }

    cJSON *allowed_nf_types = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedNfTypes");

    OpenAPI_list_t *allowed_nf_typesList;
    if (allowed_nf_types) {
        cJSON *allowed_nf_types_local_nonprimitive;
        if (!cJSON_IsArray(allowed_nf_types)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nf_types]");
            goto end;
        }

        allowed_nf_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nf_types_local_nonprimitive, allowed_nf_types ) {
            if (!cJSON_IsString(allowed_nf_types_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nf_types]");
                goto end;
            }

            OpenAPI_list_add(allowed_nf_typesList, (void *)OpenAPI_nf_type_FromString(allowed_nf_types_local_nonprimitive->valuestring));
        }
    }

    cJSON *allowed_nf_domains = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedNfDomains");

    OpenAPI_list_t *allowed_nf_domainsList;
    if (allowed_nf_domains) {
        cJSON *allowed_nf_domains_local;
        if (!cJSON_IsArray(allowed_nf_domains)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nf_domains]");
            goto end;
        }
        allowed_nf_domainsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nf_domains_local, allowed_nf_domains) {
            if (!cJSON_IsString(allowed_nf_domains_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nf_domains]");
                goto end;
            }
            OpenAPI_list_add(allowed_nf_domainsList, ogs_strdup(allowed_nf_domains_local->valuestring));
        }
    }

    cJSON *allowed_nssais = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedNssais");

    OpenAPI_list_t *allowed_nssaisList;
    if (allowed_nssais) {
        cJSON *allowed_nssais_local_nonprimitive;
        if (!cJSON_IsArray(allowed_nssais)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nssais]");
            goto end;
        }

        allowed_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nssais_local_nonprimitive, allowed_nssais ) {
            if (!cJSON_IsObject(allowed_nssais_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nssais]");
                goto end;
            }
            OpenAPI_snssai_t *allowed_nssaisItem = OpenAPI_snssai_parseFromJSON(allowed_nssais_local_nonprimitive);

            OpenAPI_list_add(allowed_nssaisList, allowed_nssaisItem);
        }
    }

    cJSON *priority = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "priority");

    if (priority) {
        if (!cJSON_IsNumber(priority)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [priority]");
            goto end;
        }
    }

    cJSON *capacity = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "capacity");

    if (capacity) {
        if (!cJSON_IsNumber(capacity)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [capacity]");
            goto end;
        }
    }

    cJSON *load = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "load");

    if (load) {
        if (!cJSON_IsNumber(load)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [load]");
            goto end;
        }
    }

    cJSON *load_time_stamp = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "loadTimeStamp");

    if (load_time_stamp) {
        if (!cJSON_IsString(load_time_stamp)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [load_time_stamp]");
            goto end;
        }
    }

    cJSON *recovery_time = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "recoveryTime");

    if (recovery_time) {
        if (!cJSON_IsString(recovery_time)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [recovery_time]");
            goto end;
        }
    }

    cJSON *chf_service_info = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "chfServiceInfo");

    OpenAPI_chf_service_info_t *chf_service_info_local_nonprim = NULL;
    if (chf_service_info) {
        chf_service_info_local_nonprim = OpenAPI_chf_service_info_parseFromJSON(chf_service_info);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *nf_service_set_id_list = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "nfServiceSetIdList");

    OpenAPI_list_t *nf_service_set_id_listList;
    if (nf_service_set_id_list) {
        cJSON *nf_service_set_id_list_local;
        if (!cJSON_IsArray(nf_service_set_id_list)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [nf_service_set_id_list]");
            goto end;
        }
        nf_service_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_service_set_id_list_local, nf_service_set_id_list) {
            if (!cJSON_IsString(nf_service_set_id_list_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [nf_service_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(nf_service_set_id_listList, ogs_strdup(nf_service_set_id_list_local->valuestring));
        }
    }

    cJSON *s_nssais = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "sNssais");

    OpenAPI_list_t *s_nssaisList;
    if (s_nssais) {
        cJSON *s_nssais_local_nonprimitive;
        if (!cJSON_IsArray(s_nssais)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [s_nssais]");
            goto end;
        }

        s_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssais_local_nonprimitive, s_nssais ) {
            if (!cJSON_IsObject(s_nssais_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [s_nssais]");
                goto end;
            }
            OpenAPI_snssai_t *s_nssaisItem = OpenAPI_snssai_parseFromJSON(s_nssais_local_nonprimitive);

            OpenAPI_list_add(s_nssaisList, s_nssaisItem);
        }
    }

    cJSON *per_plmn_snssai_list = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "perPlmnSnssaiList");

    OpenAPI_list_t *per_plmn_snssai_listList;
    if (per_plmn_snssai_list) {
        cJSON *per_plmn_snssai_list_local_nonprimitive;
        if (!cJSON_IsArray(per_plmn_snssai_list)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [per_plmn_snssai_list]");
            goto end;
        }

        per_plmn_snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(per_plmn_snssai_list_local_nonprimitive, per_plmn_snssai_list ) {
            if (!cJSON_IsObject(per_plmn_snssai_list_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [per_plmn_snssai_list]");
                goto end;
            }
            OpenAPI_plmn_snssai_t *per_plmn_snssai_listItem = OpenAPI_plmn_snssai_parseFromJSON(per_plmn_snssai_list_local_nonprimitive);

            OpenAPI_list_add(per_plmn_snssai_listList, per_plmn_snssai_listItem);
        }
    }

    cJSON *vendor_id = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "vendorId");

    if (vendor_id) {
        if (!cJSON_IsString(vendor_id)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [vendor_id]");
            goto end;
        }
    }

    cJSON *supported_vendor_specific_features = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "supportedVendorSpecificFeatures");

    OpenAPI_list_t *supported_vendor_specific_featuresList;
    if (supported_vendor_specific_features) {
        cJSON *supported_vendor_specific_features_local_map;
        if (!cJSON_IsObject(supported_vendor_specific_features)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [supported_vendor_specific_features]");
            goto end;
        }
        supported_vendor_specific_featuresList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(supported_vendor_specific_features_local_map, supported_vendor_specific_features) {
            cJSON *localMapObject = supported_vendor_specific_features_local_map;
            OpenAPI_list_add(supported_vendor_specific_featuresList, localMapKeyPair);
        }
    }

    cJSON *oauth2_required = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "oauth2Required");

    if (oauth2_required) {
        if (!cJSON_IsBool(oauth2_required)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [oauth2_required]");
            goto end;
        }
    }

    nf_service_local_var = OpenAPI_nf_service_create (
        ogs_strdup(service_instance_id->valuestring),
        ogs_strdup(service_name->valuestring),
        versionsList,
        schemeVariable,
        nf_service_statusVariable,
        fqdn ? ogs_strdup(fqdn->valuestring) : NULL,
        inter_plmn_fqdn ? ogs_strdup(inter_plmn_fqdn->valuestring) : NULL,
        ip_end_points ? ip_end_pointsList : NULL,
        api_prefix ? ogs_strdup(api_prefix->valuestring) : NULL,
        default_notification_subscriptions ? default_notification_subscriptionsList : NULL,
        allowed_plmns ? allowed_plmnsList : NULL,
        allowed_snpns ? allowed_snpnsList : NULL,
        allowed_nf_types ? allowed_nf_typesList : NULL,
        allowed_nf_domains ? allowed_nf_domainsList : NULL,
        allowed_nssais ? allowed_nssaisList : NULL,
        priority ? priority->valuedouble : 0,
        capacity ? capacity->valuedouble : 0,
        load ? load->valuedouble : 0,
        load_time_stamp ? ogs_strdup(load_time_stamp->valuestring) : NULL,
        recovery_time ? ogs_strdup(recovery_time->valuestring) : NULL,
        chf_service_info ? chf_service_info_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        nf_service_set_id_list ? nf_service_set_id_listList : NULL,
        s_nssais ? s_nssaisList : NULL,
        per_plmn_snssai_list ? per_plmn_snssai_listList : NULL,
        vendor_id ? ogs_strdup(vendor_id->valuestring) : NULL,
        supported_vendor_specific_features ? supported_vendor_specific_featuresList : NULL,
        oauth2_required ? oauth2_required->valueint : 0
        );

    return nf_service_local_var;
end:
    return NULL;
}

OpenAPI_nf_service_t *OpenAPI_nf_service_copy(OpenAPI_nf_service_t *dst, OpenAPI_nf_service_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nf_service_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed");
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

    OpenAPI_nf_service_free(dst);
    dst = OpenAPI_nf_service_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


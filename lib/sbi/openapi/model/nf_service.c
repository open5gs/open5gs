
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
    OpenAPI_list_t* allowed_operations_per_nf_type,
    OpenAPI_list_t* allowed_operations_per_nf_instance,
    bool is_priority,
    int priority,
    bool is_capacity,
    int capacity,
    bool is_load,
    int load,
    char *load_time_stamp,
    char *recovery_time,
    char *supported_features,
    OpenAPI_list_t *nf_service_set_id_list,
    OpenAPI_list_t *s_nssais,
    OpenAPI_list_t *per_plmn_snssai_list,
    char *vendor_id,
    OpenAPI_list_t* supported_vendor_specific_features,
    bool is_oauth2_required,
    int oauth2_required,
    OpenAPI_plmn_oauth2_t *per_plmn_oauth2_req_list
)
{
    OpenAPI_nf_service_t *nf_service_local_var = ogs_malloc(sizeof(OpenAPI_nf_service_t));
    ogs_assert(nf_service_local_var);

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
    nf_service_local_var->allowed_operations_per_nf_type = allowed_operations_per_nf_type;
    nf_service_local_var->allowed_operations_per_nf_instance = allowed_operations_per_nf_instance;
    nf_service_local_var->is_priority = is_priority;
    nf_service_local_var->priority = priority;
    nf_service_local_var->is_capacity = is_capacity;
    nf_service_local_var->capacity = capacity;
    nf_service_local_var->is_load = is_load;
    nf_service_local_var->load = load;
    nf_service_local_var->load_time_stamp = load_time_stamp;
    nf_service_local_var->recovery_time = recovery_time;
    nf_service_local_var->supported_features = supported_features;
    nf_service_local_var->nf_service_set_id_list = nf_service_set_id_list;
    nf_service_local_var->s_nssais = s_nssais;
    nf_service_local_var->per_plmn_snssai_list = per_plmn_snssai_list;
    nf_service_local_var->vendor_id = vendor_id;
    nf_service_local_var->supported_vendor_specific_features = supported_vendor_specific_features;
    nf_service_local_var->is_oauth2_required = is_oauth2_required;
    nf_service_local_var->oauth2_required = oauth2_required;
    nf_service_local_var->per_plmn_oauth2_req_list = per_plmn_oauth2_req_list;

    return nf_service_local_var;
}

void OpenAPI_nf_service_free(OpenAPI_nf_service_t *nf_service)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nf_service) {
        return;
    }
    if (nf_service->service_instance_id) {
        ogs_free(nf_service->service_instance_id);
        nf_service->service_instance_id = NULL;
    }
    if (nf_service->service_name) {
        ogs_free(nf_service->service_name);
        nf_service->service_name = NULL;
    }
    if (nf_service->versions) {
        OpenAPI_list_for_each(nf_service->versions, node) {
            OpenAPI_nf_service_version_free(node->data);
        }
        OpenAPI_list_free(nf_service->versions);
        nf_service->versions = NULL;
    }
    if (nf_service->fqdn) {
        ogs_free(nf_service->fqdn);
        nf_service->fqdn = NULL;
    }
    if (nf_service->inter_plmn_fqdn) {
        ogs_free(nf_service->inter_plmn_fqdn);
        nf_service->inter_plmn_fqdn = NULL;
    }
    if (nf_service->ip_end_points) {
        OpenAPI_list_for_each(nf_service->ip_end_points, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(nf_service->ip_end_points);
        nf_service->ip_end_points = NULL;
    }
    if (nf_service->api_prefix) {
        ogs_free(nf_service->api_prefix);
        nf_service->api_prefix = NULL;
    }
    if (nf_service->default_notification_subscriptions) {
        OpenAPI_list_for_each(nf_service->default_notification_subscriptions, node) {
            OpenAPI_default_notification_subscription_free(node->data);
        }
        OpenAPI_list_free(nf_service->default_notification_subscriptions);
        nf_service->default_notification_subscriptions = NULL;
    }
    if (nf_service->allowed_plmns) {
        OpenAPI_list_for_each(nf_service->allowed_plmns, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(nf_service->allowed_plmns);
        nf_service->allowed_plmns = NULL;
    }
    if (nf_service->allowed_snpns) {
        OpenAPI_list_for_each(nf_service->allowed_snpns, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(nf_service->allowed_snpns);
        nf_service->allowed_snpns = NULL;
    }
    if (nf_service->allowed_nf_types) {
        OpenAPI_list_free(nf_service->allowed_nf_types);
        nf_service->allowed_nf_types = NULL;
    }
    if (nf_service->allowed_nf_domains) {
        OpenAPI_list_for_each(nf_service->allowed_nf_domains, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_service->allowed_nf_domains);
        nf_service->allowed_nf_domains = NULL;
    }
    if (nf_service->allowed_nssais) {
        OpenAPI_list_for_each(nf_service->allowed_nssais, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(nf_service->allowed_nssais);
        nf_service->allowed_nssais = NULL;
    }
    if (nf_service->allowed_operations_per_nf_type) {
        OpenAPI_list_for_each(nf_service->allowed_operations_per_nf_type, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nf_service->allowed_operations_per_nf_type);
        nf_service->allowed_operations_per_nf_type = NULL;
    }
    if (nf_service->allowed_operations_per_nf_instance) {
        OpenAPI_list_for_each(nf_service->allowed_operations_per_nf_instance, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nf_service->allowed_operations_per_nf_instance);
        nf_service->allowed_operations_per_nf_instance = NULL;
    }
    if (nf_service->load_time_stamp) {
        ogs_free(nf_service->load_time_stamp);
        nf_service->load_time_stamp = NULL;
    }
    if (nf_service->recovery_time) {
        ogs_free(nf_service->recovery_time);
        nf_service->recovery_time = NULL;
    }
    if (nf_service->supported_features) {
        ogs_free(nf_service->supported_features);
        nf_service->supported_features = NULL;
    }
    if (nf_service->nf_service_set_id_list) {
        OpenAPI_list_for_each(nf_service->nf_service_set_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_service->nf_service_set_id_list);
        nf_service->nf_service_set_id_list = NULL;
    }
    if (nf_service->s_nssais) {
        OpenAPI_list_for_each(nf_service->s_nssais, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(nf_service->s_nssais);
        nf_service->s_nssais = NULL;
    }
    if (nf_service->per_plmn_snssai_list) {
        OpenAPI_list_for_each(nf_service->per_plmn_snssai_list, node) {
            OpenAPI_plmn_snssai_free(node->data);
        }
        OpenAPI_list_free(nf_service->per_plmn_snssai_list);
        nf_service->per_plmn_snssai_list = NULL;
    }
    if (nf_service->vendor_id) {
        ogs_free(nf_service->vendor_id);
        nf_service->vendor_id = NULL;
    }
    if (nf_service->supported_vendor_specific_features) {
        OpenAPI_list_for_each(nf_service->supported_vendor_specific_features, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nf_service->supported_vendor_specific_features);
        nf_service->supported_vendor_specific_features = NULL;
    }
    if (nf_service->per_plmn_oauth2_req_list) {
        OpenAPI_plmn_oauth2_free(nf_service->per_plmn_oauth2_req_list);
        nf_service->per_plmn_oauth2_req_list = NULL;
    }
    ogs_free(nf_service);
}

cJSON *OpenAPI_nf_service_convertToJSON(OpenAPI_nf_service_t *nf_service)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nf_service == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [NFService]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nf_service->service_instance_id) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [service_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "serviceInstanceId", nf_service->service_instance_id) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [service_instance_id]");
        goto end;
    }

    if (!nf_service->service_name) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [service_name]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "serviceName", nf_service->service_name) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [service_name]");
        goto end;
    }

    if (!nf_service->versions) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [versions]");
        return NULL;
    }
    cJSON *versionsList = cJSON_AddArrayToObject(item, "versions");
    if (versionsList == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [versions]");
        goto end;
    }
    OpenAPI_list_for_each(nf_service->versions, node) {
        cJSON *itemLocal = OpenAPI_nf_service_version_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [versions]");
            goto end;
        }
        cJSON_AddItemToArray(versionsList, itemLocal);
    }

    if (nf_service->scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [scheme]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "scheme", OpenAPI_uri_scheme_ToString(nf_service->scheme)) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [scheme]");
        goto end;
    }

    if (nf_service->nf_service_status == OpenAPI_nf_service_status_NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [nf_service_status]");
        return NULL;
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
    OpenAPI_list_for_each(nf_service->ip_end_points, node) {
        cJSON *itemLocal = OpenAPI_ip_end_point_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [ip_end_points]");
            goto end;
        }
        cJSON_AddItemToArray(ip_end_pointsList, itemLocal);
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
    OpenAPI_list_for_each(nf_service->default_notification_subscriptions, node) {
        cJSON *itemLocal = OpenAPI_default_notification_subscription_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [default_notification_subscriptions]");
            goto end;
        }
        cJSON_AddItemToArray(default_notification_subscriptionsList, itemLocal);
    }
    }

    if (nf_service->allowed_plmns) {
    cJSON *allowed_plmnsList = cJSON_AddArrayToObject(item, "allowedPlmns");
    if (allowed_plmnsList == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_plmns]");
        goto end;
    }
    OpenAPI_list_for_each(nf_service->allowed_plmns, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_plmns]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_plmnsList, itemLocal);
    }
    }

    if (nf_service->allowed_snpns) {
    cJSON *allowed_snpnsList = cJSON_AddArrayToObject(item, "allowedSnpns");
    if (allowed_snpnsList == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_snpns]");
        goto end;
    }
    OpenAPI_list_for_each(nf_service->allowed_snpns, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_nid_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_snpns]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_snpnsList, itemLocal);
    }
    }

    if (nf_service->allowed_nf_types != OpenAPI_nf_type_NULL) {
    cJSON *allowed_nf_typesList = cJSON_AddArrayToObject(item, "allowedNfTypes");
    if (allowed_nf_typesList == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nf_types]");
        goto end;
    }
    OpenAPI_list_for_each(nf_service->allowed_nf_types, node) {
        if (cJSON_AddStringToObject(allowed_nf_typesList, "", OpenAPI_nf_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nf_types]");
            goto end;
        }
    }
    }

    if (nf_service->allowed_nf_domains) {
    cJSON *allowed_nf_domainsList = cJSON_AddArrayToObject(item, "allowedNfDomains");
    if (allowed_nf_domainsList == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nf_domains]");
        goto end;
    }
    OpenAPI_list_for_each(nf_service->allowed_nf_domains, node) {
        if (cJSON_AddStringToObject(allowed_nf_domainsList, "", (char*)node->data) == NULL) {
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
    OpenAPI_list_for_each(nf_service->allowed_nssais, node) {
        cJSON *itemLocal = OpenAPI_ext_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_nssais]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_nssaisList, itemLocal);
    }
    }

    if (nf_service->allowed_operations_per_nf_type) {
    cJSON *allowed_operations_per_nf_type = cJSON_AddObjectToObject(item, "allowedOperationsPerNfType");
    if (allowed_operations_per_nf_type == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_operations_per_nf_type]");
        goto end;
    }
    cJSON *localMapObject = allowed_operations_per_nf_type;
    if (nf_service->allowed_operations_per_nf_type) {
        OpenAPI_list_for_each(nf_service->allowed_operations_per_nf_type, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_operations_per_nf_type]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_operations_per_nf_type]");
                goto end;
            }
        }
    }
    }

    if (nf_service->allowed_operations_per_nf_instance) {
    cJSON *allowed_operations_per_nf_instance = cJSON_AddObjectToObject(item, "allowedOperationsPerNfInstance");
    if (allowed_operations_per_nf_instance == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_operations_per_nf_instance]");
        goto end;
    }
    cJSON *localMapObject = allowed_operations_per_nf_instance;
    if (nf_service->allowed_operations_per_nf_instance) {
        OpenAPI_list_for_each(nf_service->allowed_operations_per_nf_instance, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_operations_per_nf_instance]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [allowed_operations_per_nf_instance]");
                goto end;
            }
        }
    }
    }

    if (nf_service->is_priority) {
    if (cJSON_AddNumberToObject(item, "priority", nf_service->priority) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [priority]");
        goto end;
    }
    }

    if (nf_service->is_capacity) {
    if (cJSON_AddNumberToObject(item, "capacity", nf_service->capacity) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [capacity]");
        goto end;
    }
    }

    if (nf_service->is_load) {
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

    if (nf_service->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", nf_service->supported_features) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (nf_service->nf_service_set_id_list) {
    cJSON *nf_service_set_id_listList = cJSON_AddArrayToObject(item, "nfServiceSetIdList");
    if (nf_service_set_id_listList == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [nf_service_set_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(nf_service->nf_service_set_id_list, node) {
        if (cJSON_AddStringToObject(nf_service_set_id_listList, "", (char*)node->data) == NULL) {
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
    OpenAPI_list_for_each(nf_service->s_nssais, node) {
        cJSON *itemLocal = OpenAPI_ext_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [s_nssais]");
            goto end;
        }
        cJSON_AddItemToArray(s_nssaisList, itemLocal);
    }
    }

    if (nf_service->per_plmn_snssai_list) {
    cJSON *per_plmn_snssai_listList = cJSON_AddArrayToObject(item, "perPlmnSnssaiList");
    if (per_plmn_snssai_listList == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [per_plmn_snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(nf_service->per_plmn_snssai_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nf_service_convertToJSON() failed [per_plmn_snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(per_plmn_snssai_listList, itemLocal);
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
    if (nf_service->supported_vendor_specific_features) {
        OpenAPI_list_for_each(nf_service->supported_vendor_specific_features, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [supported_vendor_specific_features]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [supported_vendor_specific_features]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_vendor_specific_feature_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_nf_service_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (nf_service->is_oauth2_required) {
    if (cJSON_AddBoolToObject(item, "oauth2Required", nf_service->oauth2_required) == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [oauth2_required]");
        goto end;
    }
    }

    if (nf_service->per_plmn_oauth2_req_list) {
    cJSON *per_plmn_oauth2_req_list_local_JSON = OpenAPI_plmn_oauth2_convertToJSON(nf_service->per_plmn_oauth2_req_list);
    if (per_plmn_oauth2_req_list_local_JSON == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [per_plmn_oauth2_req_list]");
        goto end;
    }
    cJSON_AddItemToObject(item, "perPlmnOauth2ReqList", per_plmn_oauth2_req_list_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nf_service_convertToJSON() failed [per_plmn_oauth2_req_list]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nf_service_t *OpenAPI_nf_service_parseFromJSON(cJSON *nf_serviceJSON)
{
    OpenAPI_nf_service_t *nf_service_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *service_instance_id = NULL;
    cJSON *service_name = NULL;
    cJSON *versions = NULL;
    OpenAPI_list_t *versionsList = NULL;
    cJSON *scheme = NULL;
    OpenAPI_uri_scheme_e schemeVariable = 0;
    cJSON *nf_service_status = NULL;
    OpenAPI_nf_service_status_e nf_service_statusVariable = 0;
    cJSON *fqdn = NULL;
    cJSON *inter_plmn_fqdn = NULL;
    cJSON *ip_end_points = NULL;
    OpenAPI_list_t *ip_end_pointsList = NULL;
    cJSON *api_prefix = NULL;
    cJSON *default_notification_subscriptions = NULL;
    OpenAPI_list_t *default_notification_subscriptionsList = NULL;
    cJSON *allowed_plmns = NULL;
    OpenAPI_list_t *allowed_plmnsList = NULL;
    cJSON *allowed_snpns = NULL;
    OpenAPI_list_t *allowed_snpnsList = NULL;
    cJSON *allowed_nf_types = NULL;
    OpenAPI_list_t *allowed_nf_typesList = NULL;
    cJSON *allowed_nf_domains = NULL;
    OpenAPI_list_t *allowed_nf_domainsList = NULL;
    cJSON *allowed_nssais = NULL;
    OpenAPI_list_t *allowed_nssaisList = NULL;
    cJSON *allowed_operations_per_nf_type = NULL;
    OpenAPI_list_t *allowed_operations_per_nf_typeList = NULL;
    cJSON *allowed_operations_per_nf_instance = NULL;
    OpenAPI_list_t *allowed_operations_per_nf_instanceList = NULL;
    cJSON *priority = NULL;
    cJSON *capacity = NULL;
    cJSON *load = NULL;
    cJSON *load_time_stamp = NULL;
    cJSON *recovery_time = NULL;
    cJSON *supported_features = NULL;
    cJSON *nf_service_set_id_list = NULL;
    OpenAPI_list_t *nf_service_set_id_listList = NULL;
    cJSON *s_nssais = NULL;
    OpenAPI_list_t *s_nssaisList = NULL;
    cJSON *per_plmn_snssai_list = NULL;
    OpenAPI_list_t *per_plmn_snssai_listList = NULL;
    cJSON *vendor_id = NULL;
    cJSON *supported_vendor_specific_features = NULL;
    OpenAPI_list_t *supported_vendor_specific_featuresList = NULL;
    cJSON *oauth2_required = NULL;
    cJSON *per_plmn_oauth2_req_list = NULL;
    OpenAPI_plmn_oauth2_t *per_plmn_oauth2_req_list_local_nonprim = NULL;
    service_instance_id = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "serviceInstanceId");
    if (!service_instance_id) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [service_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(service_instance_id)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [service_instance_id]");
        goto end;
    }

    service_name = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "serviceName");
    if (!service_name) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [service_name]");
        goto end;
    }
    if (!cJSON_IsString(service_name)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [service_name]");
        goto end;
    }

    versions = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "versions");
    if (!versions) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [versions]");
        goto end;
    }
        cJSON *versions_local = NULL;
        if (!cJSON_IsArray(versions)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [versions]");
            goto end;
        }

        versionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(versions_local, versions) {
            if (!cJSON_IsObject(versions_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [versions]");
                goto end;
            }
            OpenAPI_nf_service_version_t *versionsItem = OpenAPI_nf_service_version_parseFromJSON(versions_local);
            if (!versionsItem) {
                ogs_error("No versionsItem");
                goto end;
            }
            OpenAPI_list_add(versionsList, versionsItem);
        }

    scheme = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "scheme");
    if (!scheme) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [scheme]");
        goto end;
    }
    if (!cJSON_IsString(scheme)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [scheme]");
        goto end;
    }
    schemeVariable = OpenAPI_uri_scheme_FromString(scheme->valuestring);

    nf_service_status = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "nfServiceStatus");
    if (!nf_service_status) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [nf_service_status]");
        goto end;
    }
    if (!cJSON_IsString(nf_service_status)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [nf_service_status]");
        goto end;
    }
    nf_service_statusVariable = OpenAPI_nf_service_status_FromString(nf_service_status->valuestring);

    fqdn = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "fqdn");
    if (fqdn) {
    if (!cJSON_IsString(fqdn) && !cJSON_IsNull(fqdn)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [fqdn]");
        goto end;
    }
    }

    inter_plmn_fqdn = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "interPlmnFqdn");
    if (inter_plmn_fqdn) {
    if (!cJSON_IsString(inter_plmn_fqdn) && !cJSON_IsNull(inter_plmn_fqdn)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [inter_plmn_fqdn]");
        goto end;
    }
    }

    ip_end_points = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "ipEndPoints");
    if (ip_end_points) {
        cJSON *ip_end_points_local = NULL;
        if (!cJSON_IsArray(ip_end_points)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [ip_end_points]");
            goto end;
        }

        ip_end_pointsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ip_end_points_local, ip_end_points) {
            if (!cJSON_IsObject(ip_end_points_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [ip_end_points]");
                goto end;
            }
            OpenAPI_ip_end_point_t *ip_end_pointsItem = OpenAPI_ip_end_point_parseFromJSON(ip_end_points_local);
            if (!ip_end_pointsItem) {
                ogs_error("No ip_end_pointsItem");
                goto end;
            }
            OpenAPI_list_add(ip_end_pointsList, ip_end_pointsItem);
        }
    }

    api_prefix = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "apiPrefix");
    if (api_prefix) {
    if (!cJSON_IsString(api_prefix) && !cJSON_IsNull(api_prefix)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [api_prefix]");
        goto end;
    }
    }

    default_notification_subscriptions = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "defaultNotificationSubscriptions");
    if (default_notification_subscriptions) {
        cJSON *default_notification_subscriptions_local = NULL;
        if (!cJSON_IsArray(default_notification_subscriptions)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [default_notification_subscriptions]");
            goto end;
        }

        default_notification_subscriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(default_notification_subscriptions_local, default_notification_subscriptions) {
            if (!cJSON_IsObject(default_notification_subscriptions_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [default_notification_subscriptions]");
                goto end;
            }
            OpenAPI_default_notification_subscription_t *default_notification_subscriptionsItem = OpenAPI_default_notification_subscription_parseFromJSON(default_notification_subscriptions_local);
            if (!default_notification_subscriptionsItem) {
                ogs_error("No default_notification_subscriptionsItem");
                goto end;
            }
            OpenAPI_list_add(default_notification_subscriptionsList, default_notification_subscriptionsItem);
        }
    }

    allowed_plmns = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedPlmns");
    if (allowed_plmns) {
        cJSON *allowed_plmns_local = NULL;
        if (!cJSON_IsArray(allowed_plmns)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_plmns]");
            goto end;
        }

        allowed_plmnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_plmns_local, allowed_plmns) {
            if (!cJSON_IsObject(allowed_plmns_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_plmns]");
                goto end;
            }
            OpenAPI_plmn_id_t *allowed_plmnsItem = OpenAPI_plmn_id_parseFromJSON(allowed_plmns_local);
            if (!allowed_plmnsItem) {
                ogs_error("No allowed_plmnsItem");
                goto end;
            }
            OpenAPI_list_add(allowed_plmnsList, allowed_plmnsItem);
        }
    }

    allowed_snpns = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedSnpns");
    if (allowed_snpns) {
        cJSON *allowed_snpns_local = NULL;
        if (!cJSON_IsArray(allowed_snpns)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_snpns]");
            goto end;
        }

        allowed_snpnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_snpns_local, allowed_snpns) {
            if (!cJSON_IsObject(allowed_snpns_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_snpns]");
                goto end;
            }
            OpenAPI_plmn_id_nid_t *allowed_snpnsItem = OpenAPI_plmn_id_nid_parseFromJSON(allowed_snpns_local);
            if (!allowed_snpnsItem) {
                ogs_error("No allowed_snpnsItem");
                goto end;
            }
            OpenAPI_list_add(allowed_snpnsList, allowed_snpnsItem);
        }
    }

    allowed_nf_types = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedNfTypes");
    if (allowed_nf_types) {
        cJSON *allowed_nf_types_local = NULL;
        if (!cJSON_IsArray(allowed_nf_types)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nf_types]");
            goto end;
        }

        allowed_nf_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nf_types_local, allowed_nf_types) {
            OpenAPI_nf_type_e localEnum = OpenAPI_nf_type_NULL;
            if (!cJSON_IsString(allowed_nf_types_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nf_types]");
                goto end;
            }
            localEnum = OpenAPI_nf_type_FromString(allowed_nf_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"allowed_nf_types\" is not supported. Ignoring it ...",
                         allowed_nf_types_local->valuestring);
            } else {
                OpenAPI_list_add(allowed_nf_typesList, (void *)localEnum);
            }
        }
        if (allowed_nf_typesList->count == 0) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed: Expected allowed_nf_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    allowed_nf_domains = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedNfDomains");
    if (allowed_nf_domains) {
        cJSON *allowed_nf_domains_local = NULL;
        if (!cJSON_IsArray(allowed_nf_domains)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nf_domains]");
            goto end;
        }

        allowed_nf_domainsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nf_domains_local, allowed_nf_domains) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(allowed_nf_domains_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nf_domains]");
                goto end;
            }
            OpenAPI_list_add(allowed_nf_domainsList, ogs_strdup(allowed_nf_domains_local->valuestring));
        }
    }

    allowed_nssais = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedNssais");
    if (allowed_nssais) {
        cJSON *allowed_nssais_local = NULL;
        if (!cJSON_IsArray(allowed_nssais)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nssais]");
            goto end;
        }

        allowed_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nssais_local, allowed_nssais) {
            if (!cJSON_IsObject(allowed_nssais_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_nssais]");
                goto end;
            }
            OpenAPI_ext_snssai_t *allowed_nssaisItem = OpenAPI_ext_snssai_parseFromJSON(allowed_nssais_local);
            if (!allowed_nssaisItem) {
                ogs_error("No allowed_nssaisItem");
                goto end;
            }
            OpenAPI_list_add(allowed_nssaisList, allowed_nssaisItem);
        }
    }

    allowed_operations_per_nf_type = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedOperationsPerNfType");
    if (allowed_operations_per_nf_type) {
        cJSON *allowed_operations_per_nf_type_local_map = NULL;
        if (!cJSON_IsObject(allowed_operations_per_nf_type) && !cJSON_IsNull(allowed_operations_per_nf_type)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_operations_per_nf_type]");
            goto end;
        }
        if (cJSON_IsObject(allowed_operations_per_nf_type)) {
            allowed_operations_per_nf_typeList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(allowed_operations_per_nf_type_local_map, allowed_operations_per_nf_type) {
                cJSON *localMapObject = allowed_operations_per_nf_type_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                OpenAPI_list_add(allowed_operations_per_nf_typeList, localMapKeyPair);
            }
        }
    }

    allowed_operations_per_nf_instance = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "allowedOperationsPerNfInstance");
    if (allowed_operations_per_nf_instance) {
        cJSON *allowed_operations_per_nf_instance_local_map = NULL;
        if (!cJSON_IsObject(allowed_operations_per_nf_instance) && !cJSON_IsNull(allowed_operations_per_nf_instance)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [allowed_operations_per_nf_instance]");
            goto end;
        }
        if (cJSON_IsObject(allowed_operations_per_nf_instance)) {
            allowed_operations_per_nf_instanceList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(allowed_operations_per_nf_instance_local_map, allowed_operations_per_nf_instance) {
                cJSON *localMapObject = allowed_operations_per_nf_instance_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                OpenAPI_list_add(allowed_operations_per_nf_instanceList, localMapKeyPair);
            }
        }
    }

    priority = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "priority");
    if (priority) {
    if (!cJSON_IsNumber(priority)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [priority]");
        goto end;
    }
    }

    capacity = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "capacity");
    if (capacity) {
    if (!cJSON_IsNumber(capacity)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [capacity]");
        goto end;
    }
    }

    load = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "load");
    if (load) {
    if (!cJSON_IsNumber(load)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [load]");
        goto end;
    }
    }

    load_time_stamp = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "loadTimeStamp");
    if (load_time_stamp) {
    if (!cJSON_IsString(load_time_stamp) && !cJSON_IsNull(load_time_stamp)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [load_time_stamp]");
        goto end;
    }
    }

    recovery_time = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "recoveryTime");
    if (recovery_time) {
    if (!cJSON_IsString(recovery_time) && !cJSON_IsNull(recovery_time)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    nf_service_set_id_list = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "nfServiceSetIdList");
    if (nf_service_set_id_list) {
        cJSON *nf_service_set_id_list_local = NULL;
        if (!cJSON_IsArray(nf_service_set_id_list)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [nf_service_set_id_list]");
            goto end;
        }

        nf_service_set_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_service_set_id_list_local, nf_service_set_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nf_service_set_id_list_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [nf_service_set_id_list]");
                goto end;
            }
            OpenAPI_list_add(nf_service_set_id_listList, ogs_strdup(nf_service_set_id_list_local->valuestring));
        }
    }

    s_nssais = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "sNssais");
    if (s_nssais) {
        cJSON *s_nssais_local = NULL;
        if (!cJSON_IsArray(s_nssais)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [s_nssais]");
            goto end;
        }

        s_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssais_local, s_nssais) {
            if (!cJSON_IsObject(s_nssais_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [s_nssais]");
                goto end;
            }
            OpenAPI_ext_snssai_t *s_nssaisItem = OpenAPI_ext_snssai_parseFromJSON(s_nssais_local);
            if (!s_nssaisItem) {
                ogs_error("No s_nssaisItem");
                goto end;
            }
            OpenAPI_list_add(s_nssaisList, s_nssaisItem);
        }
    }

    per_plmn_snssai_list = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "perPlmnSnssaiList");
    if (per_plmn_snssai_list) {
        cJSON *per_plmn_snssai_list_local = NULL;
        if (!cJSON_IsArray(per_plmn_snssai_list)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [per_plmn_snssai_list]");
            goto end;
        }

        per_plmn_snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(per_plmn_snssai_list_local, per_plmn_snssai_list) {
            if (!cJSON_IsObject(per_plmn_snssai_list_local)) {
                ogs_error("OpenAPI_nf_service_parseFromJSON() failed [per_plmn_snssai_list]");
                goto end;
            }
            OpenAPI_plmn_snssai_t *per_plmn_snssai_listItem = OpenAPI_plmn_snssai_parseFromJSON(per_plmn_snssai_list_local);
            if (!per_plmn_snssai_listItem) {
                ogs_error("No per_plmn_snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(per_plmn_snssai_listList, per_plmn_snssai_listItem);
        }
    }

    vendor_id = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "vendorId");
    if (vendor_id) {
    if (!cJSON_IsString(vendor_id) && !cJSON_IsNull(vendor_id)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [vendor_id]");
        goto end;
    }
    }

    supported_vendor_specific_features = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "supportedVendorSpecificFeatures");
    if (supported_vendor_specific_features) {
        cJSON *supported_vendor_specific_features_local_map = NULL;
        if (!cJSON_IsObject(supported_vendor_specific_features) && !cJSON_IsNull(supported_vendor_specific_features)) {
            ogs_error("OpenAPI_nf_service_parseFromJSON() failed [supported_vendor_specific_features]");
            goto end;
        }
        if (cJSON_IsObject(supported_vendor_specific_features)) {
            supported_vendor_specific_featuresList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(supported_vendor_specific_features_local_map, supported_vendor_specific_features) {
                cJSON *localMapObject = supported_vendor_specific_features_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_vendor_specific_feature_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_nf_service_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(supported_vendor_specific_featuresList, localMapKeyPair);
            }
        }
    }

    oauth2_required = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "oauth2Required");
    if (oauth2_required) {
    if (!cJSON_IsBool(oauth2_required)) {
        ogs_error("OpenAPI_nf_service_parseFromJSON() failed [oauth2_required]");
        goto end;
    }
    }

    per_plmn_oauth2_req_list = cJSON_GetObjectItemCaseSensitive(nf_serviceJSON, "perPlmnOauth2ReqList");
    if (per_plmn_oauth2_req_list) {
    per_plmn_oauth2_req_list_local_nonprim = OpenAPI_plmn_oauth2_parseFromJSON(per_plmn_oauth2_req_list);
    if (!per_plmn_oauth2_req_list_local_nonprim) {
        ogs_error("OpenAPI_plmn_oauth2_parseFromJSON failed [per_plmn_oauth2_req_list]");
        goto end;
    }
    }

    nf_service_local_var = OpenAPI_nf_service_create (
        ogs_strdup(service_instance_id->valuestring),
        ogs_strdup(service_name->valuestring),
        versionsList,
        schemeVariable,
        nf_service_statusVariable,
        fqdn && !cJSON_IsNull(fqdn) ? ogs_strdup(fqdn->valuestring) : NULL,
        inter_plmn_fqdn && !cJSON_IsNull(inter_plmn_fqdn) ? ogs_strdup(inter_plmn_fqdn->valuestring) : NULL,
        ip_end_points ? ip_end_pointsList : NULL,
        api_prefix && !cJSON_IsNull(api_prefix) ? ogs_strdup(api_prefix->valuestring) : NULL,
        default_notification_subscriptions ? default_notification_subscriptionsList : NULL,
        allowed_plmns ? allowed_plmnsList : NULL,
        allowed_snpns ? allowed_snpnsList : NULL,
        allowed_nf_types ? allowed_nf_typesList : NULL,
        allowed_nf_domains ? allowed_nf_domainsList : NULL,
        allowed_nssais ? allowed_nssaisList : NULL,
        allowed_operations_per_nf_type ? allowed_operations_per_nf_typeList : NULL,
        allowed_operations_per_nf_instance ? allowed_operations_per_nf_instanceList : NULL,
        priority ? true : false,
        priority ? priority->valuedouble : 0,
        capacity ? true : false,
        capacity ? capacity->valuedouble : 0,
        load ? true : false,
        load ? load->valuedouble : 0,
        load_time_stamp && !cJSON_IsNull(load_time_stamp) ? ogs_strdup(load_time_stamp->valuestring) : NULL,
        recovery_time && !cJSON_IsNull(recovery_time) ? ogs_strdup(recovery_time->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        nf_service_set_id_list ? nf_service_set_id_listList : NULL,
        s_nssais ? s_nssaisList : NULL,
        per_plmn_snssai_list ? per_plmn_snssai_listList : NULL,
        vendor_id && !cJSON_IsNull(vendor_id) ? ogs_strdup(vendor_id->valuestring) : NULL,
        supported_vendor_specific_features ? supported_vendor_specific_featuresList : NULL,
        oauth2_required ? true : false,
        oauth2_required ? oauth2_required->valueint : 0,
        per_plmn_oauth2_req_list ? per_plmn_oauth2_req_list_local_nonprim : NULL
    );

    return nf_service_local_var;
end:
    if (versionsList) {
        OpenAPI_list_for_each(versionsList, node) {
            OpenAPI_nf_service_version_free(node->data);
        }
        OpenAPI_list_free(versionsList);
        versionsList = NULL;
    }
    if (ip_end_pointsList) {
        OpenAPI_list_for_each(ip_end_pointsList, node) {
            OpenAPI_ip_end_point_free(node->data);
        }
        OpenAPI_list_free(ip_end_pointsList);
        ip_end_pointsList = NULL;
    }
    if (default_notification_subscriptionsList) {
        OpenAPI_list_for_each(default_notification_subscriptionsList, node) {
            OpenAPI_default_notification_subscription_free(node->data);
        }
        OpenAPI_list_free(default_notification_subscriptionsList);
        default_notification_subscriptionsList = NULL;
    }
    if (allowed_plmnsList) {
        OpenAPI_list_for_each(allowed_plmnsList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(allowed_plmnsList);
        allowed_plmnsList = NULL;
    }
    if (allowed_snpnsList) {
        OpenAPI_list_for_each(allowed_snpnsList, node) {
            OpenAPI_plmn_id_nid_free(node->data);
        }
        OpenAPI_list_free(allowed_snpnsList);
        allowed_snpnsList = NULL;
    }
    if (allowed_nf_typesList) {
        OpenAPI_list_free(allowed_nf_typesList);
        allowed_nf_typesList = NULL;
    }
    if (allowed_nf_domainsList) {
        OpenAPI_list_for_each(allowed_nf_domainsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(allowed_nf_domainsList);
        allowed_nf_domainsList = NULL;
    }
    if (allowed_nssaisList) {
        OpenAPI_list_for_each(allowed_nssaisList, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(allowed_nssaisList);
        allowed_nssaisList = NULL;
    }
    if (allowed_operations_per_nf_typeList) {
        OpenAPI_list_for_each(allowed_operations_per_nf_typeList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(allowed_operations_per_nf_typeList);
        allowed_operations_per_nf_typeList = NULL;
    }
    if (allowed_operations_per_nf_instanceList) {
        OpenAPI_list_for_each(allowed_operations_per_nf_instanceList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(allowed_operations_per_nf_instanceList);
        allowed_operations_per_nf_instanceList = NULL;
    }
    if (nf_service_set_id_listList) {
        OpenAPI_list_for_each(nf_service_set_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nf_service_set_id_listList);
        nf_service_set_id_listList = NULL;
    }
    if (s_nssaisList) {
        OpenAPI_list_for_each(s_nssaisList, node) {
            OpenAPI_ext_snssai_free(node->data);
        }
        OpenAPI_list_free(s_nssaisList);
        s_nssaisList = NULL;
    }
    if (per_plmn_snssai_listList) {
        OpenAPI_list_for_each(per_plmn_snssai_listList, node) {
            OpenAPI_plmn_snssai_free(node->data);
        }
        OpenAPI_list_free(per_plmn_snssai_listList);
        per_plmn_snssai_listList = NULL;
    }
    if (supported_vendor_specific_featuresList) {
        OpenAPI_list_for_each(supported_vendor_specific_featuresList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(supported_vendor_specific_featuresList);
        supported_vendor_specific_featuresList = NULL;
    }
    if (per_plmn_oauth2_req_list_local_nonprim) {
        OpenAPI_plmn_oauth2_free(per_plmn_oauth2_req_list_local_nonprim);
        per_plmn_oauth2_req_list_local_nonprim = NULL;
    }
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



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_profile.h"

OpenAPI_nf_profile_t *OpenAPI_nf_profile_create(
    char *nf_instance_id,
    char *nf_instance_name,
    OpenAPI_nf_type_e nf_type,
    OpenAPI_nf_status_e nf_status,
    int heart_beat_timer,
    OpenAPI_list_t *plmn_list,
    OpenAPI_list_t *s_nssais,
    OpenAPI_list_t *per_plmn_snssai_list,
    OpenAPI_list_t *nsi_list,
    char *fqdn,
    char *inter_plmn_fqdn,
    OpenAPI_list_t *ipv4_addresses,
    OpenAPI_list_t *ipv6_addresses,
    OpenAPI_list_t *allowed_plmns,
    OpenAPI_list_t *allowed_nf_types,
    OpenAPI_list_t *allowed_nf_domains,
    OpenAPI_list_t *allowed_nssais,
    int priority,
    int capacity,
    int load,
    char *locality,
    OpenAPI_udr_info_t *udr_info,
    OpenAPI_list_t *udr_info_ext,
    OpenAPI_udm_info_t *udm_info,
    OpenAPI_list_t *udm_info_ext,
    OpenAPI_ausf_info_t *ausf_info,
    OpenAPI_list_t *ausf_info_ext,
    OpenAPI_amf_info_t *amf_info,
    OpenAPI_list_t *amf_info_ext,
    OpenAPI_smf_info_t *smf_info,
    OpenAPI_list_t *smf_info_ext,
    OpenAPI_upf_info_t *upf_info,
    OpenAPI_list_t *upf_info_ext,
    OpenAPI_pcf_info_t *pcf_info,
    OpenAPI_list_t *pcf_info_ext,
    OpenAPI_bsf_info_t *bsf_info,
    OpenAPI_list_t *bsf_info_ext,
    OpenAPI_chf_info_t *chf_info,
    OpenAPI_list_t *chf_info_ext,
    OpenAPI_nrf_info_t *nrf_info,
    OpenAPI_nwdaf_info_t *nwdaf_info,
    OpenAPI_object_t *custom_info,
    char *recovery_time,
    int nf_service_persistence,
    OpenAPI_list_t *nf_services,
    int nf_profile_changes_support_ind,
    int nf_profile_changes_ind,
    OpenAPI_list_t *default_notification_subscriptions
    )
{
    OpenAPI_nf_profile_t *nf_profile_local_var = OpenAPI_malloc(sizeof(OpenAPI_nf_profile_t));
    if (!nf_profile_local_var) {
        return NULL;
    }
    nf_profile_local_var->nf_instance_id = nf_instance_id;
    nf_profile_local_var->nf_instance_name = nf_instance_name;
    nf_profile_local_var->nf_type = nf_type;
    nf_profile_local_var->nf_status = nf_status;
    nf_profile_local_var->heart_beat_timer = heart_beat_timer;
    nf_profile_local_var->plmn_list = plmn_list;
    nf_profile_local_var->s_nssais = s_nssais;
    nf_profile_local_var->per_plmn_snssai_list = per_plmn_snssai_list;
    nf_profile_local_var->nsi_list = nsi_list;
    nf_profile_local_var->fqdn = fqdn;
    nf_profile_local_var->inter_plmn_fqdn = inter_plmn_fqdn;
    nf_profile_local_var->ipv4_addresses = ipv4_addresses;
    nf_profile_local_var->ipv6_addresses = ipv6_addresses;
    nf_profile_local_var->allowed_plmns = allowed_plmns;
    nf_profile_local_var->allowed_nf_types = allowed_nf_types;
    nf_profile_local_var->allowed_nf_domains = allowed_nf_domains;
    nf_profile_local_var->allowed_nssais = allowed_nssais;
    nf_profile_local_var->priority = priority;
    nf_profile_local_var->capacity = capacity;
    nf_profile_local_var->load = load;
    nf_profile_local_var->locality = locality;
    nf_profile_local_var->udr_info = udr_info;
    nf_profile_local_var->udr_info_ext = udr_info_ext;
    nf_profile_local_var->udm_info = udm_info;
    nf_profile_local_var->udm_info_ext = udm_info_ext;
    nf_profile_local_var->ausf_info = ausf_info;
    nf_profile_local_var->ausf_info_ext = ausf_info_ext;
    nf_profile_local_var->amf_info = amf_info;
    nf_profile_local_var->amf_info_ext = amf_info_ext;
    nf_profile_local_var->smf_info = smf_info;
    nf_profile_local_var->smf_info_ext = smf_info_ext;
    nf_profile_local_var->upf_info = upf_info;
    nf_profile_local_var->upf_info_ext = upf_info_ext;
    nf_profile_local_var->pcf_info = pcf_info;
    nf_profile_local_var->pcf_info_ext = pcf_info_ext;
    nf_profile_local_var->bsf_info = bsf_info;
    nf_profile_local_var->bsf_info_ext = bsf_info_ext;
    nf_profile_local_var->chf_info = chf_info;
    nf_profile_local_var->chf_info_ext = chf_info_ext;
    nf_profile_local_var->nrf_info = nrf_info;
    nf_profile_local_var->nwdaf_info = nwdaf_info;
    nf_profile_local_var->custom_info = custom_info;
    nf_profile_local_var->recovery_time = recovery_time;
    nf_profile_local_var->nf_service_persistence = nf_service_persistence;
    nf_profile_local_var->nf_services = nf_services;
    nf_profile_local_var->nf_profile_changes_support_ind = nf_profile_changes_support_ind;
    nf_profile_local_var->nf_profile_changes_ind = nf_profile_changes_ind;
    nf_profile_local_var->default_notification_subscriptions = default_notification_subscriptions;

    return nf_profile_local_var;
}

void OpenAPI_nf_profile_free(OpenAPI_nf_profile_t *nf_profile)
{
    if (NULL == nf_profile) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(nf_profile->nf_instance_id);
    ogs_free(nf_profile->nf_instance_name);
    OpenAPI_list_for_each(nf_profile->plmn_list, node) {
        OpenAPI_plmn_id_free(node->data);
    }
    OpenAPI_list_free(nf_profile->plmn_list);
    OpenAPI_list_for_each(nf_profile->s_nssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(nf_profile->s_nssais);
    OpenAPI_list_for_each(nf_profile->per_plmn_snssai_list, node) {
        OpenAPI_plmn_snssai_free(node->data);
    }
    OpenAPI_list_free(nf_profile->per_plmn_snssai_list);
    OpenAPI_list_for_each(nf_profile->nsi_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(nf_profile->nsi_list);
    ogs_free(nf_profile->fqdn);
    ogs_free(nf_profile->inter_plmn_fqdn);
    OpenAPI_list_for_each(nf_profile->ipv4_addresses, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(nf_profile->ipv4_addresses);
    OpenAPI_list_for_each(nf_profile->ipv6_addresses, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(nf_profile->ipv6_addresses);
    OpenAPI_list_for_each(nf_profile->allowed_plmns, node) {
        OpenAPI_plmn_id_free(node->data);
    }
    OpenAPI_list_free(nf_profile->allowed_plmns);
    OpenAPI_list_free(nf_profile->allowed_nf_types);
    OpenAPI_list_for_each(nf_profile->allowed_nf_domains, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(nf_profile->allowed_nf_domains);
    OpenAPI_list_for_each(nf_profile->allowed_nssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(nf_profile->allowed_nssais);
    ogs_free(nf_profile->locality);
    OpenAPI_udr_info_free(nf_profile->udr_info);
    OpenAPI_list_for_each(nf_profile->udr_info_ext, node) {
        OpenAPI_udr_info_free(node->data);
    }
    OpenAPI_list_free(nf_profile->udr_info_ext);
    OpenAPI_udm_info_free(nf_profile->udm_info);
    OpenAPI_list_for_each(nf_profile->udm_info_ext, node) {
        OpenAPI_udm_info_free(node->data);
    }
    OpenAPI_list_free(nf_profile->udm_info_ext);
    OpenAPI_ausf_info_free(nf_profile->ausf_info);
    OpenAPI_list_for_each(nf_profile->ausf_info_ext, node) {
        OpenAPI_ausf_info_free(node->data);
    }
    OpenAPI_list_free(nf_profile->ausf_info_ext);
    OpenAPI_amf_info_free(nf_profile->amf_info);
    OpenAPI_list_for_each(nf_profile->amf_info_ext, node) {
        OpenAPI_amf_info_free(node->data);
    }
    OpenAPI_list_free(nf_profile->amf_info_ext);
    OpenAPI_smf_info_free(nf_profile->smf_info);
    OpenAPI_list_for_each(nf_profile->smf_info_ext, node) {
        OpenAPI_smf_info_free(node->data);
    }
    OpenAPI_list_free(nf_profile->smf_info_ext);
    OpenAPI_upf_info_free(nf_profile->upf_info);
    OpenAPI_list_for_each(nf_profile->upf_info_ext, node) {
        OpenAPI_upf_info_free(node->data);
    }
    OpenAPI_list_free(nf_profile->upf_info_ext);
    OpenAPI_pcf_info_free(nf_profile->pcf_info);
    OpenAPI_list_for_each(nf_profile->pcf_info_ext, node) {
        OpenAPI_pcf_info_free(node->data);
    }
    OpenAPI_list_free(nf_profile->pcf_info_ext);
    OpenAPI_bsf_info_free(nf_profile->bsf_info);
    OpenAPI_list_for_each(nf_profile->bsf_info_ext, node) {
        OpenAPI_bsf_info_free(node->data);
    }
    OpenAPI_list_free(nf_profile->bsf_info_ext);
    OpenAPI_chf_info_free(nf_profile->chf_info);
    OpenAPI_list_for_each(nf_profile->chf_info_ext, node) {
        OpenAPI_chf_info_free(node->data);
    }
    OpenAPI_list_free(nf_profile->chf_info_ext);
    OpenAPI_nrf_info_free(nf_profile->nrf_info);
    OpenAPI_nwdaf_info_free(nf_profile->nwdaf_info);
    OpenAPI_object_free(nf_profile->custom_info);
    ogs_free(nf_profile->recovery_time);
    OpenAPI_list_for_each(nf_profile->nf_services, node) {
        OpenAPI_nf_service_free(node->data);
    }
    OpenAPI_list_free(nf_profile->nf_services);
    OpenAPI_list_for_each(nf_profile->default_notification_subscriptions, node) {
        OpenAPI_default_notification_subscription_free(node->data);
    }
    OpenAPI_list_free(nf_profile->default_notification_subscriptions);
    ogs_free(nf_profile);
}

cJSON *OpenAPI_nf_profile_convertToJSON(OpenAPI_nf_profile_t *nf_profile)
{
    cJSON *item = NULL;

    if (nf_profile == NULL) {
        ogs_error("OpenAPI_nf_profile_convertToJSON() failed [NFProfile]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nf_profile->nf_instance_id) {
        ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfInstanceId", nf_profile->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_instance_id]");
        goto end;
    }

    if (nf_profile->nf_instance_name) {
        if (cJSON_AddStringToObject(item, "nfInstanceName", nf_profile->nf_instance_name) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_instance_name]");
            goto end;
        }
    }

    if (!nf_profile->nf_type) {
        ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfType", OpenAPI_nf_type_ToString(nf_profile->nf_type)) == NULL) {
        ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_type]");
        goto end;
    }

    if (!nf_profile->nf_status) {
        ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_status]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "nfStatus", OpenAPI_nf_status_ToString(nf_profile->nf_status)) == NULL) {
        ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_status]");
        goto end;
    }

    if (nf_profile->heart_beat_timer) {
        if (cJSON_AddNumberToObject(item, "heartBeatTimer", nf_profile->heart_beat_timer) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [heart_beat_timer]");
            goto end;
        }
    }

    if (nf_profile->plmn_list) {
        cJSON *plmn_listList = cJSON_AddArrayToObject(item, "plmnList");
        if (plmn_listList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [plmn_list]");
            goto end;
        }

        OpenAPI_lnode_t *plmn_list_node;
        if (nf_profile->plmn_list) {
            OpenAPI_list_for_each(nf_profile->plmn_list, plmn_list_node) {
                cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(plmn_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [plmn_list]");
                    goto end;
                }
                cJSON_AddItemToArray(plmn_listList, itemLocal);
            }
        }
    }

    if (nf_profile->s_nssais) {
        cJSON *s_nssaisList = cJSON_AddArrayToObject(item, "sNssais");
        if (s_nssaisList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [s_nssais]");
            goto end;
        }

        OpenAPI_lnode_t *s_nssais_node;
        if (nf_profile->s_nssais) {
            OpenAPI_list_for_each(nf_profile->s_nssais, s_nssais_node) {
                cJSON *itemLocal = OpenAPI_snssai_convertToJSON(s_nssais_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [s_nssais]");
                    goto end;
                }
                cJSON_AddItemToArray(s_nssaisList, itemLocal);
            }
        }
    }

    if (nf_profile->per_plmn_snssai_list) {
        cJSON *per_plmn_snssai_listList = cJSON_AddArrayToObject(item, "perPlmnSnssaiList");
        if (per_plmn_snssai_listList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [per_plmn_snssai_list]");
            goto end;
        }

        OpenAPI_lnode_t *per_plmn_snssai_list_node;
        if (nf_profile->per_plmn_snssai_list) {
            OpenAPI_list_for_each(nf_profile->per_plmn_snssai_list, per_plmn_snssai_list_node) {
                cJSON *itemLocal = OpenAPI_plmn_snssai_convertToJSON(per_plmn_snssai_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [per_plmn_snssai_list]");
                    goto end;
                }
                cJSON_AddItemToArray(per_plmn_snssai_listList, itemLocal);
            }
        }
    }

    if (nf_profile->nsi_list) {
        cJSON *nsi_list = cJSON_AddArrayToObject(item, "nsiList");
        if (nsi_list == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nsi_list]");
            goto end;
        }

        OpenAPI_lnode_t *nsi_list_node;
        OpenAPI_list_for_each(nf_profile->nsi_list, nsi_list_node)  {
            if (cJSON_AddStringToObject(nsi_list, "", (char*)nsi_list_node->data) == NULL) {
                ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nsi_list]");
                goto end;
            }
        }
    }

    if (nf_profile->fqdn) {
        if (cJSON_AddStringToObject(item, "fqdn", nf_profile->fqdn) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [fqdn]");
            goto end;
        }
    }

    if (nf_profile->inter_plmn_fqdn) {
        if (cJSON_AddStringToObject(item, "interPlmnFqdn", nf_profile->inter_plmn_fqdn) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [inter_plmn_fqdn]");
            goto end;
        }
    }

    if (nf_profile->ipv4_addresses) {
        cJSON *ipv4_addresses = cJSON_AddArrayToObject(item, "ipv4Addresses");
        if (ipv4_addresses == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [ipv4_addresses]");
            goto end;
        }

        OpenAPI_lnode_t *ipv4_addresses_node;
        OpenAPI_list_for_each(nf_profile->ipv4_addresses, ipv4_addresses_node)  {
            if (cJSON_AddStringToObject(ipv4_addresses, "", (char*)ipv4_addresses_node->data) == NULL) {
                ogs_error("OpenAPI_nf_profile_convertToJSON() failed [ipv4_addresses]");
                goto end;
            }
        }
    }

    if (nf_profile->ipv6_addresses) {
        cJSON *ipv6_addresses = cJSON_AddArrayToObject(item, "ipv6Addresses");
        if (ipv6_addresses == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [ipv6_addresses]");
            goto end;
        }

        OpenAPI_lnode_t *ipv6_addresses_node;
        OpenAPI_list_for_each(nf_profile->ipv6_addresses, ipv6_addresses_node)  {
            if (cJSON_AddStringToObject(ipv6_addresses, "", (char*)ipv6_addresses_node->data) == NULL) {
                ogs_error("OpenAPI_nf_profile_convertToJSON() failed [ipv6_addresses]");
                goto end;
            }
        }
    }

    if (nf_profile->allowed_plmns) {
        cJSON *allowed_plmnsList = cJSON_AddArrayToObject(item, "allowedPlmns");
        if (allowed_plmnsList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [allowed_plmns]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_plmns_node;
        if (nf_profile->allowed_plmns) {
            OpenAPI_list_for_each(nf_profile->allowed_plmns, allowed_plmns_node) {
                cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(allowed_plmns_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [allowed_plmns]");
                    goto end;
                }
                cJSON_AddItemToArray(allowed_plmnsList, itemLocal);
            }
        }
    }

    if (nf_profile->allowed_nf_types) {
        cJSON *allowed_nf_types = cJSON_AddArrayToObject(item, "allowedNfTypes");
        if (allowed_nf_types == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [allowed_nf_types]");
            goto end;
        }
        OpenAPI_lnode_t *allowed_nf_types_node;
        OpenAPI_list_for_each(nf_profile->allowed_nf_types, allowed_nf_types_node) {
            if (cJSON_AddStringToObject(allowed_nf_types, "", OpenAPI_nf_type_ToString((OpenAPI_nf_type_e)allowed_nf_types_node->data)) == NULL) {
                ogs_error("OpenAPI_nf_profile_convertToJSON() failed [allowed_nf_types]");
                goto end;
            }
        }
    }

    if (nf_profile->allowed_nf_domains) {
        cJSON *allowed_nf_domains = cJSON_AddArrayToObject(item, "allowedNfDomains");
        if (allowed_nf_domains == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [allowed_nf_domains]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_nf_domains_node;
        OpenAPI_list_for_each(nf_profile->allowed_nf_domains, allowed_nf_domains_node)  {
            if (cJSON_AddStringToObject(allowed_nf_domains, "", (char*)allowed_nf_domains_node->data) == NULL) {
                ogs_error("OpenAPI_nf_profile_convertToJSON() failed [allowed_nf_domains]");
                goto end;
            }
        }
    }

    if (nf_profile->allowed_nssais) {
        cJSON *allowed_nssaisList = cJSON_AddArrayToObject(item, "allowedNssais");
        if (allowed_nssaisList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [allowed_nssais]");
            goto end;
        }

        OpenAPI_lnode_t *allowed_nssais_node;
        if (nf_profile->allowed_nssais) {
            OpenAPI_list_for_each(nf_profile->allowed_nssais, allowed_nssais_node) {
                cJSON *itemLocal = OpenAPI_snssai_convertToJSON(allowed_nssais_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [allowed_nssais]");
                    goto end;
                }
                cJSON_AddItemToArray(allowed_nssaisList, itemLocal);
            }
        }
    }

    if (nf_profile->priority) {
        if (cJSON_AddNumberToObject(item, "priority", nf_profile->priority) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [priority]");
            goto end;
        }
    }

    if (nf_profile->capacity) {
        if (cJSON_AddNumberToObject(item, "capacity", nf_profile->capacity) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [capacity]");
            goto end;
        }
    }

    if (nf_profile->load) {
        if (cJSON_AddNumberToObject(item, "load", nf_profile->load) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [load]");
            goto end;
        }
    }

    if (nf_profile->locality) {
        if (cJSON_AddStringToObject(item, "locality", nf_profile->locality) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [locality]");
            goto end;
        }
    }

    if (nf_profile->udr_info) {
        cJSON *udr_info_local_JSON = OpenAPI_udr_info_convertToJSON(nf_profile->udr_info);
        if (udr_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [udr_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "udrInfo", udr_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [udr_info]");
            goto end;
        }
    }

    if (nf_profile->udr_info_ext) {
        cJSON *udr_info_extList = cJSON_AddArrayToObject(item, "udrInfoExt");
        if (udr_info_extList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [udr_info_ext]");
            goto end;
        }

        OpenAPI_lnode_t *udr_info_ext_node;
        if (nf_profile->udr_info_ext) {
            OpenAPI_list_for_each(nf_profile->udr_info_ext, udr_info_ext_node) {
                cJSON *itemLocal = OpenAPI_udr_info_convertToJSON(udr_info_ext_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [udr_info_ext]");
                    goto end;
                }
                cJSON_AddItemToArray(udr_info_extList, itemLocal);
            }
        }
    }

    if (nf_profile->udm_info) {
        cJSON *udm_info_local_JSON = OpenAPI_udm_info_convertToJSON(nf_profile->udm_info);
        if (udm_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [udm_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "udmInfo", udm_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [udm_info]");
            goto end;
        }
    }

    if (nf_profile->udm_info_ext) {
        cJSON *udm_info_extList = cJSON_AddArrayToObject(item, "udmInfoExt");
        if (udm_info_extList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [udm_info_ext]");
            goto end;
        }

        OpenAPI_lnode_t *udm_info_ext_node;
        if (nf_profile->udm_info_ext) {
            OpenAPI_list_for_each(nf_profile->udm_info_ext, udm_info_ext_node) {
                cJSON *itemLocal = OpenAPI_udm_info_convertToJSON(udm_info_ext_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [udm_info_ext]");
                    goto end;
                }
                cJSON_AddItemToArray(udm_info_extList, itemLocal);
            }
        }
    }

    if (nf_profile->ausf_info) {
        cJSON *ausf_info_local_JSON = OpenAPI_ausf_info_convertToJSON(nf_profile->ausf_info);
        if (ausf_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [ausf_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "ausfInfo", ausf_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [ausf_info]");
            goto end;
        }
    }

    if (nf_profile->ausf_info_ext) {
        cJSON *ausf_info_extList = cJSON_AddArrayToObject(item, "ausfInfoExt");
        if (ausf_info_extList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [ausf_info_ext]");
            goto end;
        }

        OpenAPI_lnode_t *ausf_info_ext_node;
        if (nf_profile->ausf_info_ext) {
            OpenAPI_list_for_each(nf_profile->ausf_info_ext, ausf_info_ext_node) {
                cJSON *itemLocal = OpenAPI_ausf_info_convertToJSON(ausf_info_ext_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [ausf_info_ext]");
                    goto end;
                }
                cJSON_AddItemToArray(ausf_info_extList, itemLocal);
            }
        }
    }

    if (nf_profile->amf_info) {
        cJSON *amf_info_local_JSON = OpenAPI_amf_info_convertToJSON(nf_profile->amf_info);
        if (amf_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [amf_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "amfInfo", amf_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [amf_info]");
            goto end;
        }
    }

    if (nf_profile->amf_info_ext) {
        cJSON *amf_info_extList = cJSON_AddArrayToObject(item, "amfInfoExt");
        if (amf_info_extList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [amf_info_ext]");
            goto end;
        }

        OpenAPI_lnode_t *amf_info_ext_node;
        if (nf_profile->amf_info_ext) {
            OpenAPI_list_for_each(nf_profile->amf_info_ext, amf_info_ext_node) {
                cJSON *itemLocal = OpenAPI_amf_info_convertToJSON(amf_info_ext_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [amf_info_ext]");
                    goto end;
                }
                cJSON_AddItemToArray(amf_info_extList, itemLocal);
            }
        }
    }

    if (nf_profile->smf_info) {
        cJSON *smf_info_local_JSON = OpenAPI_smf_info_convertToJSON(nf_profile->smf_info);
        if (smf_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [smf_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "smfInfo", smf_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [smf_info]");
            goto end;
        }
    }

    if (nf_profile->smf_info_ext) {
        cJSON *smf_info_extList = cJSON_AddArrayToObject(item, "smfInfoExt");
        if (smf_info_extList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [smf_info_ext]");
            goto end;
        }

        OpenAPI_lnode_t *smf_info_ext_node;
        if (nf_profile->smf_info_ext) {
            OpenAPI_list_for_each(nf_profile->smf_info_ext, smf_info_ext_node) {
                cJSON *itemLocal = OpenAPI_smf_info_convertToJSON(smf_info_ext_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [smf_info_ext]");
                    goto end;
                }
                cJSON_AddItemToArray(smf_info_extList, itemLocal);
            }
        }
    }

    if (nf_profile->upf_info) {
        cJSON *upf_info_local_JSON = OpenAPI_upf_info_convertToJSON(nf_profile->upf_info);
        if (upf_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [upf_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "upfInfo", upf_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [upf_info]");
            goto end;
        }
    }

    if (nf_profile->upf_info_ext) {
        cJSON *upf_info_extList = cJSON_AddArrayToObject(item, "upfInfoExt");
        if (upf_info_extList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [upf_info_ext]");
            goto end;
        }

        OpenAPI_lnode_t *upf_info_ext_node;
        if (nf_profile->upf_info_ext) {
            OpenAPI_list_for_each(nf_profile->upf_info_ext, upf_info_ext_node) {
                cJSON *itemLocal = OpenAPI_upf_info_convertToJSON(upf_info_ext_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [upf_info_ext]");
                    goto end;
                }
                cJSON_AddItemToArray(upf_info_extList, itemLocal);
            }
        }
    }

    if (nf_profile->pcf_info) {
        cJSON *pcf_info_local_JSON = OpenAPI_pcf_info_convertToJSON(nf_profile->pcf_info);
        if (pcf_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [pcf_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "pcfInfo", pcf_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [pcf_info]");
            goto end;
        }
    }

    if (nf_profile->pcf_info_ext) {
        cJSON *pcf_info_extList = cJSON_AddArrayToObject(item, "pcfInfoExt");
        if (pcf_info_extList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [pcf_info_ext]");
            goto end;
        }

        OpenAPI_lnode_t *pcf_info_ext_node;
        if (nf_profile->pcf_info_ext) {
            OpenAPI_list_for_each(nf_profile->pcf_info_ext, pcf_info_ext_node) {
                cJSON *itemLocal = OpenAPI_pcf_info_convertToJSON(pcf_info_ext_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [pcf_info_ext]");
                    goto end;
                }
                cJSON_AddItemToArray(pcf_info_extList, itemLocal);
            }
        }
    }

    if (nf_profile->bsf_info) {
        cJSON *bsf_info_local_JSON = OpenAPI_bsf_info_convertToJSON(nf_profile->bsf_info);
        if (bsf_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [bsf_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "bsfInfo", bsf_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [bsf_info]");
            goto end;
        }
    }

    if (nf_profile->bsf_info_ext) {
        cJSON *bsf_info_extList = cJSON_AddArrayToObject(item, "bsfInfoExt");
        if (bsf_info_extList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [bsf_info_ext]");
            goto end;
        }

        OpenAPI_lnode_t *bsf_info_ext_node;
        if (nf_profile->bsf_info_ext) {
            OpenAPI_list_for_each(nf_profile->bsf_info_ext, bsf_info_ext_node) {
                cJSON *itemLocal = OpenAPI_bsf_info_convertToJSON(bsf_info_ext_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [bsf_info_ext]");
                    goto end;
                }
                cJSON_AddItemToArray(bsf_info_extList, itemLocal);
            }
        }
    }

    if (nf_profile->chf_info) {
        cJSON *chf_info_local_JSON = OpenAPI_chf_info_convertToJSON(nf_profile->chf_info);
        if (chf_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [chf_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "chfInfo", chf_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [chf_info]");
            goto end;
        }
    }

    if (nf_profile->chf_info_ext) {
        cJSON *chf_info_extList = cJSON_AddArrayToObject(item, "chfInfoExt");
        if (chf_info_extList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [chf_info_ext]");
            goto end;
        }

        OpenAPI_lnode_t *chf_info_ext_node;
        if (nf_profile->chf_info_ext) {
            OpenAPI_list_for_each(nf_profile->chf_info_ext, chf_info_ext_node) {
                cJSON *itemLocal = OpenAPI_chf_info_convertToJSON(chf_info_ext_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [chf_info_ext]");
                    goto end;
                }
                cJSON_AddItemToArray(chf_info_extList, itemLocal);
            }
        }
    }

    if (nf_profile->nrf_info) {
        cJSON *nrf_info_local_JSON = OpenAPI_nrf_info_convertToJSON(nf_profile->nrf_info);
        if (nrf_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nrf_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "nrfInfo", nrf_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nrf_info]");
            goto end;
        }
    }

    if (nf_profile->nwdaf_info) {
        cJSON *nwdaf_info_local_JSON = OpenAPI_nwdaf_info_convertToJSON(nf_profile->nwdaf_info);
        if (nwdaf_info_local_JSON == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nwdaf_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "nwdafInfo", nwdaf_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nwdaf_info]");
            goto end;
        }
    }

    if (nf_profile->custom_info) {
        cJSON *custom_info_object = OpenAPI_object_convertToJSON(nf_profile->custom_info);
        if (custom_info_object == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [custom_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "customInfo", custom_info_object);
        if (item->child == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [custom_info]");
            goto end;
        }
    }

    if (nf_profile->recovery_time) {
        if (cJSON_AddStringToObject(item, "recoveryTime", nf_profile->recovery_time) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [recovery_time]");
            goto end;
        }
    }

    if (nf_profile->nf_service_persistence) {
        if (cJSON_AddBoolToObject(item, "nfServicePersistence", nf_profile->nf_service_persistence) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_service_persistence]");
            goto end;
        }
    }

    if (nf_profile->nf_services) {
        cJSON *nf_servicesList = cJSON_AddArrayToObject(item, "nfServices");
        if (nf_servicesList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_services]");
            goto end;
        }

        OpenAPI_lnode_t *nf_services_node;
        if (nf_profile->nf_services) {
            OpenAPI_list_for_each(nf_profile->nf_services, nf_services_node) {
                cJSON *itemLocal = OpenAPI_nf_service_convertToJSON(nf_services_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_services]");
                    goto end;
                }
                cJSON_AddItemToArray(nf_servicesList, itemLocal);
            }
        }
    }

    if (nf_profile->nf_profile_changes_support_ind) {
        if (cJSON_AddBoolToObject(item, "nfProfileChangesSupportInd", nf_profile->nf_profile_changes_support_ind) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_profile_changes_support_ind]");
            goto end;
        }
    }

    if (nf_profile->nf_profile_changes_ind) {
        if (cJSON_AddBoolToObject(item, "nfProfileChangesInd", nf_profile->nf_profile_changes_ind) == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [nf_profile_changes_ind]");
            goto end;
        }
    }

    if (nf_profile->default_notification_subscriptions) {
        cJSON *default_notification_subscriptionsList = cJSON_AddArrayToObject(item, "defaultNotificationSubscriptions");
        if (default_notification_subscriptionsList == NULL) {
            ogs_error("OpenAPI_nf_profile_convertToJSON() failed [default_notification_subscriptions]");
            goto end;
        }

        OpenAPI_lnode_t *default_notification_subscriptions_node;
        if (nf_profile->default_notification_subscriptions) {
            OpenAPI_list_for_each(nf_profile->default_notification_subscriptions, default_notification_subscriptions_node) {
                cJSON *itemLocal = OpenAPI_default_notification_subscription_convertToJSON(default_notification_subscriptions_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_nf_profile_convertToJSON() failed [default_notification_subscriptions]");
                    goto end;
                }
                cJSON_AddItemToArray(default_notification_subscriptionsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_nf_profile_t *OpenAPI_nf_profile_parseFromJSON(cJSON *nf_profileJSON)
{
    OpenAPI_nf_profile_t *nf_profile_local_var = NULL;
    cJSON *nf_instance_id = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    cJSON *nf_instance_name = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nfInstanceName");

    if (nf_instance_name) {
        if (!cJSON_IsString(nf_instance_name)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_instance_name]");
            goto end;
        }
    }

    cJSON *nf_type = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nfType");
    if (!nf_type) {
        ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_type]");
        goto end;
    }

    OpenAPI_nf_type_e nf_typeVariable;

    if (!cJSON_IsString(nf_type)) {
        ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_type]");
        goto end;
    }
    nf_typeVariable = OpenAPI_nf_type_FromString(nf_type->valuestring);

    cJSON *nf_status = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nfStatus");
    if (!nf_status) {
        ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_status]");
        goto end;
    }

    OpenAPI_nf_status_e nf_statusVariable;

    if (!cJSON_IsString(nf_status)) {
        ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_status]");
        goto end;
    }
    nf_statusVariable = OpenAPI_nf_status_FromString(nf_status->valuestring);

    cJSON *heart_beat_timer = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "heartBeatTimer");

    if (heart_beat_timer) {
        if (!cJSON_IsNumber(heart_beat_timer)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [heart_beat_timer]");
            goto end;
        }
    }

    cJSON *plmn_list = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "plmnList");

    OpenAPI_list_t *plmn_listList;
    if (plmn_list) {
        cJSON *plmn_list_local_nonprimitive;
        if (!cJSON_IsArray(plmn_list)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [plmn_list]");
            goto end;
        }

        plmn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmn_list_local_nonprimitive, plmn_list ) {
            if (!cJSON_IsObject(plmn_list_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [plmn_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *plmn_listItem = OpenAPI_plmn_id_parseFromJSON(plmn_list_local_nonprimitive);

            OpenAPI_list_add(plmn_listList, plmn_listItem);
        }
    }

    cJSON *s_nssais = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "sNssais");

    OpenAPI_list_t *s_nssaisList;
    if (s_nssais) {
        cJSON *s_nssais_local_nonprimitive;
        if (!cJSON_IsArray(s_nssais)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [s_nssais]");
            goto end;
        }

        s_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssais_local_nonprimitive, s_nssais ) {
            if (!cJSON_IsObject(s_nssais_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [s_nssais]");
                goto end;
            }
            OpenAPI_snssai_t *s_nssaisItem = OpenAPI_snssai_parseFromJSON(s_nssais_local_nonprimitive);

            OpenAPI_list_add(s_nssaisList, s_nssaisItem);
        }
    }

    cJSON *per_plmn_snssai_list = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "perPlmnSnssaiList");

    OpenAPI_list_t *per_plmn_snssai_listList;
    if (per_plmn_snssai_list) {
        cJSON *per_plmn_snssai_list_local_nonprimitive;
        if (!cJSON_IsArray(per_plmn_snssai_list)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [per_plmn_snssai_list]");
            goto end;
        }

        per_plmn_snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(per_plmn_snssai_list_local_nonprimitive, per_plmn_snssai_list ) {
            if (!cJSON_IsObject(per_plmn_snssai_list_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [per_plmn_snssai_list]");
                goto end;
            }
            OpenAPI_plmn_snssai_t *per_plmn_snssai_listItem = OpenAPI_plmn_snssai_parseFromJSON(per_plmn_snssai_list_local_nonprimitive);

            OpenAPI_list_add(per_plmn_snssai_listList, per_plmn_snssai_listItem);
        }
    }

    cJSON *nsi_list = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nsiList");

    OpenAPI_list_t *nsi_listList;
    if (nsi_list) {
        cJSON *nsi_list_local;
        if (!cJSON_IsArray(nsi_list)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nsi_list]");
            goto end;
        }
        nsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nsi_list_local, nsi_list) {
            if (!cJSON_IsString(nsi_list_local)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nsi_list]");
                goto end;
            }
            OpenAPI_list_add(nsi_listList, ogs_strdup(nsi_list_local->valuestring));
        }
    }

    cJSON *fqdn = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "fqdn");

    if (fqdn) {
        if (!cJSON_IsString(fqdn)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [fqdn]");
            goto end;
        }
    }

    cJSON *inter_plmn_fqdn = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "interPlmnFqdn");

    if (inter_plmn_fqdn) {
        if (!cJSON_IsString(inter_plmn_fqdn)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [inter_plmn_fqdn]");
            goto end;
        }
    }

    cJSON *ipv4_addresses = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "ipv4Addresses");

    OpenAPI_list_t *ipv4_addressesList;
    if (ipv4_addresses) {
        cJSON *ipv4_addresses_local;
        if (!cJSON_IsArray(ipv4_addresses)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [ipv4_addresses]");
            goto end;
        }
        ipv4_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv4_addresses_local, ipv4_addresses) {
            if (!cJSON_IsString(ipv4_addresses_local)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [ipv4_addresses]");
                goto end;
            }
            OpenAPI_list_add(ipv4_addressesList, ogs_strdup(ipv4_addresses_local->valuestring));
        }
    }

    cJSON *ipv6_addresses = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "ipv6Addresses");

    OpenAPI_list_t *ipv6_addressesList;
    if (ipv6_addresses) {
        cJSON *ipv6_addresses_local;
        if (!cJSON_IsArray(ipv6_addresses)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [ipv6_addresses]");
            goto end;
        }
        ipv6_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(ipv6_addresses_local, ipv6_addresses) {
            if (!cJSON_IsString(ipv6_addresses_local)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [ipv6_addresses]");
                goto end;
            }
            OpenAPI_list_add(ipv6_addressesList, ogs_strdup(ipv6_addresses_local->valuestring));
        }
    }

    cJSON *allowed_plmns = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "allowedPlmns");

    OpenAPI_list_t *allowed_plmnsList;
    if (allowed_plmns) {
        cJSON *allowed_plmns_local_nonprimitive;
        if (!cJSON_IsArray(allowed_plmns)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [allowed_plmns]");
            goto end;
        }

        allowed_plmnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_plmns_local_nonprimitive, allowed_plmns ) {
            if (!cJSON_IsObject(allowed_plmns_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [allowed_plmns]");
                goto end;
            }
            OpenAPI_plmn_id_t *allowed_plmnsItem = OpenAPI_plmn_id_parseFromJSON(allowed_plmns_local_nonprimitive);

            OpenAPI_list_add(allowed_plmnsList, allowed_plmnsItem);
        }
    }

    cJSON *allowed_nf_types = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "allowedNfTypes");

    OpenAPI_list_t *allowed_nf_typesList;
    if (allowed_nf_types) {
        cJSON *allowed_nf_types_local_nonprimitive;
        if (!cJSON_IsArray(allowed_nf_types)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [allowed_nf_types]");
            goto end;
        }

        allowed_nf_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nf_types_local_nonprimitive, allowed_nf_types ) {
            if (!cJSON_IsString(allowed_nf_types_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [allowed_nf_types]");
                goto end;
            }

            OpenAPI_list_add(allowed_nf_typesList, (void *)OpenAPI_nf_type_FromString(allowed_nf_types_local_nonprimitive->valuestring));
        }
    }

    cJSON *allowed_nf_domains = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "allowedNfDomains");

    OpenAPI_list_t *allowed_nf_domainsList;
    if (allowed_nf_domains) {
        cJSON *allowed_nf_domains_local;
        if (!cJSON_IsArray(allowed_nf_domains)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [allowed_nf_domains]");
            goto end;
        }
        allowed_nf_domainsList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nf_domains_local, allowed_nf_domains) {
            if (!cJSON_IsString(allowed_nf_domains_local)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [allowed_nf_domains]");
                goto end;
            }
            OpenAPI_list_add(allowed_nf_domainsList, ogs_strdup(allowed_nf_domains_local->valuestring));
        }
    }

    cJSON *allowed_nssais = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "allowedNssais");

    OpenAPI_list_t *allowed_nssaisList;
    if (allowed_nssais) {
        cJSON *allowed_nssais_local_nonprimitive;
        if (!cJSON_IsArray(allowed_nssais)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [allowed_nssais]");
            goto end;
        }

        allowed_nssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nssais_local_nonprimitive, allowed_nssais ) {
            if (!cJSON_IsObject(allowed_nssais_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [allowed_nssais]");
                goto end;
            }
            OpenAPI_snssai_t *allowed_nssaisItem = OpenAPI_snssai_parseFromJSON(allowed_nssais_local_nonprimitive);

            OpenAPI_list_add(allowed_nssaisList, allowed_nssaisItem);
        }
    }

    cJSON *priority = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "priority");

    if (priority) {
        if (!cJSON_IsNumber(priority)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [priority]");
            goto end;
        }
    }

    cJSON *capacity = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "capacity");

    if (capacity) {
        if (!cJSON_IsNumber(capacity)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [capacity]");
            goto end;
        }
    }

    cJSON *load = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "load");

    if (load) {
        if (!cJSON_IsNumber(load)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [load]");
            goto end;
        }
    }

    cJSON *locality = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "locality");

    if (locality) {
        if (!cJSON_IsString(locality)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [locality]");
            goto end;
        }
    }

    cJSON *udr_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "udrInfo");

    OpenAPI_udr_info_t *udr_info_local_nonprim = NULL;
    if (udr_info) {
        udr_info_local_nonprim = OpenAPI_udr_info_parseFromJSON(udr_info);
    }

    cJSON *udr_info_ext = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "udrInfoExt");

    OpenAPI_list_t *udr_info_extList;
    if (udr_info_ext) {
        cJSON *udr_info_ext_local_nonprimitive;
        if (!cJSON_IsArray(udr_info_ext)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [udr_info_ext]");
            goto end;
        }

        udr_info_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(udr_info_ext_local_nonprimitive, udr_info_ext ) {
            if (!cJSON_IsObject(udr_info_ext_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [udr_info_ext]");
                goto end;
            }
            OpenAPI_udr_info_t *udr_info_extItem = OpenAPI_udr_info_parseFromJSON(udr_info_ext_local_nonprimitive);

            OpenAPI_list_add(udr_info_extList, udr_info_extItem);
        }
    }

    cJSON *udm_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "udmInfo");

    OpenAPI_udm_info_t *udm_info_local_nonprim = NULL;
    if (udm_info) {
        udm_info_local_nonprim = OpenAPI_udm_info_parseFromJSON(udm_info);
    }

    cJSON *udm_info_ext = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "udmInfoExt");

    OpenAPI_list_t *udm_info_extList;
    if (udm_info_ext) {
        cJSON *udm_info_ext_local_nonprimitive;
        if (!cJSON_IsArray(udm_info_ext)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [udm_info_ext]");
            goto end;
        }

        udm_info_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(udm_info_ext_local_nonprimitive, udm_info_ext ) {
            if (!cJSON_IsObject(udm_info_ext_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [udm_info_ext]");
                goto end;
            }
            OpenAPI_udm_info_t *udm_info_extItem = OpenAPI_udm_info_parseFromJSON(udm_info_ext_local_nonprimitive);

            OpenAPI_list_add(udm_info_extList, udm_info_extItem);
        }
    }

    cJSON *ausf_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "ausfInfo");

    OpenAPI_ausf_info_t *ausf_info_local_nonprim = NULL;
    if (ausf_info) {
        ausf_info_local_nonprim = OpenAPI_ausf_info_parseFromJSON(ausf_info);
    }

    cJSON *ausf_info_ext = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "ausfInfoExt");

    OpenAPI_list_t *ausf_info_extList;
    if (ausf_info_ext) {
        cJSON *ausf_info_ext_local_nonprimitive;
        if (!cJSON_IsArray(ausf_info_ext)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [ausf_info_ext]");
            goto end;
        }

        ausf_info_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(ausf_info_ext_local_nonprimitive, ausf_info_ext ) {
            if (!cJSON_IsObject(ausf_info_ext_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [ausf_info_ext]");
                goto end;
            }
            OpenAPI_ausf_info_t *ausf_info_extItem = OpenAPI_ausf_info_parseFromJSON(ausf_info_ext_local_nonprimitive);

            OpenAPI_list_add(ausf_info_extList, ausf_info_extItem);
        }
    }

    cJSON *amf_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "amfInfo");

    OpenAPI_amf_info_t *amf_info_local_nonprim = NULL;
    if (amf_info) {
        amf_info_local_nonprim = OpenAPI_amf_info_parseFromJSON(amf_info);
    }

    cJSON *amf_info_ext = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "amfInfoExt");

    OpenAPI_list_t *amf_info_extList;
    if (amf_info_ext) {
        cJSON *amf_info_ext_local_nonprimitive;
        if (!cJSON_IsArray(amf_info_ext)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [amf_info_ext]");
            goto end;
        }

        amf_info_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(amf_info_ext_local_nonprimitive, amf_info_ext ) {
            if (!cJSON_IsObject(amf_info_ext_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [amf_info_ext]");
                goto end;
            }
            OpenAPI_amf_info_t *amf_info_extItem = OpenAPI_amf_info_parseFromJSON(amf_info_ext_local_nonprimitive);

            OpenAPI_list_add(amf_info_extList, amf_info_extItem);
        }
    }

    cJSON *smf_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "smfInfo");

    OpenAPI_smf_info_t *smf_info_local_nonprim = NULL;
    if (smf_info) {
        smf_info_local_nonprim = OpenAPI_smf_info_parseFromJSON(smf_info);
    }

    cJSON *smf_info_ext = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "smfInfoExt");

    OpenAPI_list_t *smf_info_extList;
    if (smf_info_ext) {
        cJSON *smf_info_ext_local_nonprimitive;
        if (!cJSON_IsArray(smf_info_ext)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [smf_info_ext]");
            goto end;
        }

        smf_info_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(smf_info_ext_local_nonprimitive, smf_info_ext ) {
            if (!cJSON_IsObject(smf_info_ext_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [smf_info_ext]");
                goto end;
            }
            OpenAPI_smf_info_t *smf_info_extItem = OpenAPI_smf_info_parseFromJSON(smf_info_ext_local_nonprimitive);

            OpenAPI_list_add(smf_info_extList, smf_info_extItem);
        }
    }

    cJSON *upf_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "upfInfo");

    OpenAPI_upf_info_t *upf_info_local_nonprim = NULL;
    if (upf_info) {
        upf_info_local_nonprim = OpenAPI_upf_info_parseFromJSON(upf_info);
    }

    cJSON *upf_info_ext = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "upfInfoExt");

    OpenAPI_list_t *upf_info_extList;
    if (upf_info_ext) {
        cJSON *upf_info_ext_local_nonprimitive;
        if (!cJSON_IsArray(upf_info_ext)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [upf_info_ext]");
            goto end;
        }

        upf_info_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(upf_info_ext_local_nonprimitive, upf_info_ext ) {
            if (!cJSON_IsObject(upf_info_ext_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [upf_info_ext]");
                goto end;
            }
            OpenAPI_upf_info_t *upf_info_extItem = OpenAPI_upf_info_parseFromJSON(upf_info_ext_local_nonprimitive);

            OpenAPI_list_add(upf_info_extList, upf_info_extItem);
        }
    }

    cJSON *pcf_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "pcfInfo");

    OpenAPI_pcf_info_t *pcf_info_local_nonprim = NULL;
    if (pcf_info) {
        pcf_info_local_nonprim = OpenAPI_pcf_info_parseFromJSON(pcf_info);
    }

    cJSON *pcf_info_ext = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "pcfInfoExt");

    OpenAPI_list_t *pcf_info_extList;
    if (pcf_info_ext) {
        cJSON *pcf_info_ext_local_nonprimitive;
        if (!cJSON_IsArray(pcf_info_ext)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [pcf_info_ext]");
            goto end;
        }

        pcf_info_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(pcf_info_ext_local_nonprimitive, pcf_info_ext ) {
            if (!cJSON_IsObject(pcf_info_ext_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [pcf_info_ext]");
                goto end;
            }
            OpenAPI_pcf_info_t *pcf_info_extItem = OpenAPI_pcf_info_parseFromJSON(pcf_info_ext_local_nonprimitive);

            OpenAPI_list_add(pcf_info_extList, pcf_info_extItem);
        }
    }

    cJSON *bsf_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "bsfInfo");

    OpenAPI_bsf_info_t *bsf_info_local_nonprim = NULL;
    if (bsf_info) {
        bsf_info_local_nonprim = OpenAPI_bsf_info_parseFromJSON(bsf_info);
    }

    cJSON *bsf_info_ext = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "bsfInfoExt");

    OpenAPI_list_t *bsf_info_extList;
    if (bsf_info_ext) {
        cJSON *bsf_info_ext_local_nonprimitive;
        if (!cJSON_IsArray(bsf_info_ext)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [bsf_info_ext]");
            goto end;
        }

        bsf_info_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(bsf_info_ext_local_nonprimitive, bsf_info_ext ) {
            if (!cJSON_IsObject(bsf_info_ext_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [bsf_info_ext]");
                goto end;
            }
            OpenAPI_bsf_info_t *bsf_info_extItem = OpenAPI_bsf_info_parseFromJSON(bsf_info_ext_local_nonprimitive);

            OpenAPI_list_add(bsf_info_extList, bsf_info_extItem);
        }
    }

    cJSON *chf_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "chfInfo");

    OpenAPI_chf_info_t *chf_info_local_nonprim = NULL;
    if (chf_info) {
        chf_info_local_nonprim = OpenAPI_chf_info_parseFromJSON(chf_info);
    }

    cJSON *chf_info_ext = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "chfInfoExt");

    OpenAPI_list_t *chf_info_extList;
    if (chf_info_ext) {
        cJSON *chf_info_ext_local_nonprimitive;
        if (!cJSON_IsArray(chf_info_ext)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [chf_info_ext]");
            goto end;
        }

        chf_info_extList = OpenAPI_list_create();

        cJSON_ArrayForEach(chf_info_ext_local_nonprimitive, chf_info_ext ) {
            if (!cJSON_IsObject(chf_info_ext_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [chf_info_ext]");
                goto end;
            }
            OpenAPI_chf_info_t *chf_info_extItem = OpenAPI_chf_info_parseFromJSON(chf_info_ext_local_nonprimitive);

            OpenAPI_list_add(chf_info_extList, chf_info_extItem);
        }
    }

    cJSON *nrf_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nrfInfo");

    OpenAPI_nrf_info_t *nrf_info_local_nonprim = NULL;
    if (nrf_info) {
        nrf_info_local_nonprim = OpenAPI_nrf_info_parseFromJSON(nrf_info);
    }

    cJSON *nwdaf_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nwdafInfo");

    OpenAPI_nwdaf_info_t *nwdaf_info_local_nonprim = NULL;
    if (nwdaf_info) {
        nwdaf_info_local_nonprim = OpenAPI_nwdaf_info_parseFromJSON(nwdaf_info);
    }

    cJSON *custom_info = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "customInfo");

    OpenAPI_object_t *custom_info_local_object = NULL;
    if (custom_info) {
        custom_info_local_object = OpenAPI_object_parseFromJSON(custom_info);
    }

    cJSON *recovery_time = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "recoveryTime");

    if (recovery_time) {
        if (!cJSON_IsString(recovery_time)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [recovery_time]");
            goto end;
        }
    }

    cJSON *nf_service_persistence = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nfServicePersistence");

    if (nf_service_persistence) {
        if (!cJSON_IsBool(nf_service_persistence)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_service_persistence]");
            goto end;
        }
    }

    cJSON *nf_services = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nfServices");

    OpenAPI_list_t *nf_servicesList;
    if (nf_services) {
        cJSON *nf_services_local_nonprimitive;
        if (!cJSON_IsArray(nf_services)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_services]");
            goto end;
        }

        nf_servicesList = OpenAPI_list_create();

        cJSON_ArrayForEach(nf_services_local_nonprimitive, nf_services ) {
            if (!cJSON_IsObject(nf_services_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_services]");
                goto end;
            }
            OpenAPI_nf_service_t *nf_servicesItem = OpenAPI_nf_service_parseFromJSON(nf_services_local_nonprimitive);

            OpenAPI_list_add(nf_servicesList, nf_servicesItem);
        }
    }

    cJSON *nf_profile_changes_support_ind = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nfProfileChangesSupportInd");

    if (nf_profile_changes_support_ind) {
        if (!cJSON_IsBool(nf_profile_changes_support_ind)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_profile_changes_support_ind]");
            goto end;
        }
    }

    cJSON *nf_profile_changes_ind = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "nfProfileChangesInd");

    if (nf_profile_changes_ind) {
        if (!cJSON_IsBool(nf_profile_changes_ind)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [nf_profile_changes_ind]");
            goto end;
        }
    }

    cJSON *default_notification_subscriptions = cJSON_GetObjectItemCaseSensitive(nf_profileJSON, "defaultNotificationSubscriptions");

    OpenAPI_list_t *default_notification_subscriptionsList;
    if (default_notification_subscriptions) {
        cJSON *default_notification_subscriptions_local_nonprimitive;
        if (!cJSON_IsArray(default_notification_subscriptions)) {
            ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [default_notification_subscriptions]");
            goto end;
        }

        default_notification_subscriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(default_notification_subscriptions_local_nonprimitive, default_notification_subscriptions ) {
            if (!cJSON_IsObject(default_notification_subscriptions_local_nonprimitive)) {
                ogs_error("OpenAPI_nf_profile_parseFromJSON() failed [default_notification_subscriptions]");
                goto end;
            }
            OpenAPI_default_notification_subscription_t *default_notification_subscriptionsItem = OpenAPI_default_notification_subscription_parseFromJSON(default_notification_subscriptions_local_nonprimitive);

            OpenAPI_list_add(default_notification_subscriptionsList, default_notification_subscriptionsItem);
        }
    }

    nf_profile_local_var = OpenAPI_nf_profile_create (
        ogs_strdup(nf_instance_id->valuestring),
        nf_instance_name ? ogs_strdup(nf_instance_name->valuestring) : NULL,
        nf_typeVariable,
        nf_statusVariable,
        heart_beat_timer ? heart_beat_timer->valuedouble : 0,
        plmn_list ? plmn_listList : NULL,
        s_nssais ? s_nssaisList : NULL,
        per_plmn_snssai_list ? per_plmn_snssai_listList : NULL,
        nsi_list ? nsi_listList : NULL,
        fqdn ? ogs_strdup(fqdn->valuestring) : NULL,
        inter_plmn_fqdn ? ogs_strdup(inter_plmn_fqdn->valuestring) : NULL,
        ipv4_addresses ? ipv4_addressesList : NULL,
        ipv6_addresses ? ipv6_addressesList : NULL,
        allowed_plmns ? allowed_plmnsList : NULL,
        allowed_nf_types ? allowed_nf_typesList : NULL,
        allowed_nf_domains ? allowed_nf_domainsList : NULL,
        allowed_nssais ? allowed_nssaisList : NULL,
        priority ? priority->valuedouble : 0,
        capacity ? capacity->valuedouble : 0,
        load ? load->valuedouble : 0,
        locality ? ogs_strdup(locality->valuestring) : NULL,
        udr_info ? udr_info_local_nonprim : NULL,
        udr_info_ext ? udr_info_extList : NULL,
        udm_info ? udm_info_local_nonprim : NULL,
        udm_info_ext ? udm_info_extList : NULL,
        ausf_info ? ausf_info_local_nonprim : NULL,
        ausf_info_ext ? ausf_info_extList : NULL,
        amf_info ? amf_info_local_nonprim : NULL,
        amf_info_ext ? amf_info_extList : NULL,
        smf_info ? smf_info_local_nonprim : NULL,
        smf_info_ext ? smf_info_extList : NULL,
        upf_info ? upf_info_local_nonprim : NULL,
        upf_info_ext ? upf_info_extList : NULL,
        pcf_info ? pcf_info_local_nonprim : NULL,
        pcf_info_ext ? pcf_info_extList : NULL,
        bsf_info ? bsf_info_local_nonprim : NULL,
        bsf_info_ext ? bsf_info_extList : NULL,
        chf_info ? chf_info_local_nonprim : NULL,
        chf_info_ext ? chf_info_extList : NULL,
        nrf_info ? nrf_info_local_nonprim : NULL,
        nwdaf_info ? nwdaf_info_local_nonprim : NULL,
        custom_info ? custom_info_local_object : NULL,
        recovery_time ? ogs_strdup(recovery_time->valuestring) : NULL,
        nf_service_persistence ? nf_service_persistence->valueint : 0,
        nf_services ? nf_servicesList : NULL,
        nf_profile_changes_support_ind ? nf_profile_changes_support_ind->valueint : 0,
        nf_profile_changes_ind ? nf_profile_changes_ind->valueint : 0,
        default_notification_subscriptions ? default_notification_subscriptionsList : NULL
        );

    return nf_profile_local_var;
end:
    return NULL;
}


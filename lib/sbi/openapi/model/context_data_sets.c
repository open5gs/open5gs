
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "context_data_sets.h"

OpenAPI_context_data_sets_t *OpenAPI_context_data_sets_create(
    OpenAPI_amf3_gpp_access_registration_t *amf3_gpp,
    OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp,
    OpenAPI_list_t *sdm_subscriptions,
    OpenAPI_list_t *ee_subscriptions,
    OpenAPI_smsf_registration_t *smsf3_gpp_access,
    OpenAPI_smsf_registration_t *smsf_non3_gpp_access,
    OpenAPI_list_t *subscription_data_subscriptions,
    OpenAPI_list_t *smf_registrations
    )
{
    OpenAPI_context_data_sets_t *context_data_sets_local_var = OpenAPI_malloc(sizeof(OpenAPI_context_data_sets_t));
    if (!context_data_sets_local_var) {
        return NULL;
    }
    context_data_sets_local_var->amf3_gpp = amf3_gpp;
    context_data_sets_local_var->amf_non3_gpp = amf_non3_gpp;
    context_data_sets_local_var->sdm_subscriptions = sdm_subscriptions;
    context_data_sets_local_var->ee_subscriptions = ee_subscriptions;
    context_data_sets_local_var->smsf3_gpp_access = smsf3_gpp_access;
    context_data_sets_local_var->smsf_non3_gpp_access = smsf_non3_gpp_access;
    context_data_sets_local_var->subscription_data_subscriptions = subscription_data_subscriptions;
    context_data_sets_local_var->smf_registrations = smf_registrations;

    return context_data_sets_local_var;
}

void OpenAPI_context_data_sets_free(OpenAPI_context_data_sets_t *context_data_sets)
{
    if (NULL == context_data_sets) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_amf3_gpp_access_registration_free(context_data_sets->amf3_gpp);
    OpenAPI_amf_non3_gpp_access_registration_free(context_data_sets->amf_non3_gpp);
    OpenAPI_list_for_each(context_data_sets->sdm_subscriptions, node) {
        OpenAPI_sdm_subscription_free(node->data);
    }
    OpenAPI_list_free(context_data_sets->sdm_subscriptions);
    OpenAPI_list_for_each(context_data_sets->ee_subscriptions, node) {
        OpenAPI_ee_subscription_free(node->data);
    }
    OpenAPI_list_free(context_data_sets->ee_subscriptions);
    OpenAPI_smsf_registration_free(context_data_sets->smsf3_gpp_access);
    OpenAPI_smsf_registration_free(context_data_sets->smsf_non3_gpp_access);
    OpenAPI_list_for_each(context_data_sets->subscription_data_subscriptions, node) {
        OpenAPI_subscription_data_subscriptions_free(node->data);
    }
    OpenAPI_list_free(context_data_sets->subscription_data_subscriptions);
    OpenAPI_list_for_each(context_data_sets->smf_registrations, node) {
        OpenAPI_smf_registration_free(node->data);
    }
    OpenAPI_list_free(context_data_sets->smf_registrations);
    ogs_free(context_data_sets);
}

cJSON *OpenAPI_context_data_sets_convertToJSON(OpenAPI_context_data_sets_t *context_data_sets)
{
    cJSON *item = NULL;

    if (context_data_sets == NULL) {
        ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [ContextDataSets]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (context_data_sets->amf3_gpp) {
        cJSON *amf3_gpp_local_JSON = OpenAPI_amf3_gpp_access_registration_convertToJSON(context_data_sets->amf3_gpp);
        if (amf3_gpp_local_JSON == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [amf3_gpp]");
            goto end;
        }
        cJSON_AddItemToObject(item, "amf3Gpp", amf3_gpp_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [amf3_gpp]");
            goto end;
        }
    }

    if (context_data_sets->amf_non3_gpp) {
        cJSON *amf_non3_gpp_local_JSON = OpenAPI_amf_non3_gpp_access_registration_convertToJSON(context_data_sets->amf_non3_gpp);
        if (amf_non3_gpp_local_JSON == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [amf_non3_gpp]");
            goto end;
        }
        cJSON_AddItemToObject(item, "amfNon3Gpp", amf_non3_gpp_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [amf_non3_gpp]");
            goto end;
        }
    }

    if (context_data_sets->sdm_subscriptions) {
        cJSON *sdm_subscriptionsList = cJSON_AddArrayToObject(item, "sdmSubscriptions");
        if (sdm_subscriptionsList == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [sdm_subscriptions]");
            goto end;
        }

        OpenAPI_lnode_t *sdm_subscriptions_node;
        if (context_data_sets->sdm_subscriptions) {
            OpenAPI_list_for_each(context_data_sets->sdm_subscriptions, sdm_subscriptions_node) {
                cJSON *itemLocal = OpenAPI_sdm_subscription_convertToJSON(sdm_subscriptions_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [sdm_subscriptions]");
                    goto end;
                }
                cJSON_AddItemToArray(sdm_subscriptionsList, itemLocal);
            }
        }
    }

    if (context_data_sets->ee_subscriptions) {
        cJSON *ee_subscriptionsList = cJSON_AddArrayToObject(item, "eeSubscriptions");
        if (ee_subscriptionsList == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [ee_subscriptions]");
            goto end;
        }

        OpenAPI_lnode_t *ee_subscriptions_node;
        if (context_data_sets->ee_subscriptions) {
            OpenAPI_list_for_each(context_data_sets->ee_subscriptions, ee_subscriptions_node) {
                cJSON *itemLocal = OpenAPI_ee_subscription_convertToJSON(ee_subscriptions_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [ee_subscriptions]");
                    goto end;
                }
                cJSON_AddItemToArray(ee_subscriptionsList, itemLocal);
            }
        }
    }

    if (context_data_sets->smsf3_gpp_access) {
        cJSON *smsf3_gpp_access_local_JSON = OpenAPI_smsf_registration_convertToJSON(context_data_sets->smsf3_gpp_access);
        if (smsf3_gpp_access_local_JSON == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [smsf3_gpp_access]");
            goto end;
        }
        cJSON_AddItemToObject(item, "smsf3GppAccess", smsf3_gpp_access_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [smsf3_gpp_access]");
            goto end;
        }
    }

    if (context_data_sets->smsf_non3_gpp_access) {
        cJSON *smsf_non3_gpp_access_local_JSON = OpenAPI_smsf_registration_convertToJSON(context_data_sets->smsf_non3_gpp_access);
        if (smsf_non3_gpp_access_local_JSON == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [smsf_non3_gpp_access]");
            goto end;
        }
        cJSON_AddItemToObject(item, "smsfNon3GppAccess", smsf_non3_gpp_access_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [smsf_non3_gpp_access]");
            goto end;
        }
    }

    if (context_data_sets->subscription_data_subscriptions) {
        cJSON *subscription_data_subscriptionsList = cJSON_AddArrayToObject(item, "subscriptionDataSubscriptions");
        if (subscription_data_subscriptionsList == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [subscription_data_subscriptions]");
            goto end;
        }

        OpenAPI_lnode_t *subscription_data_subscriptions_node;
        if (context_data_sets->subscription_data_subscriptions) {
            OpenAPI_list_for_each(context_data_sets->subscription_data_subscriptions, subscription_data_subscriptions_node) {
                cJSON *itemLocal = OpenAPI_subscription_data_subscriptions_convertToJSON(subscription_data_subscriptions_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [subscription_data_subscriptions]");
                    goto end;
                }
                cJSON_AddItemToArray(subscription_data_subscriptionsList, itemLocal);
            }
        }
    }

    if (context_data_sets->smf_registrations) {
        cJSON *smf_registrationsList = cJSON_AddArrayToObject(item, "smfRegistrations");
        if (smf_registrationsList == NULL) {
            ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [smf_registrations]");
            goto end;
        }

        OpenAPI_lnode_t *smf_registrations_node;
        if (context_data_sets->smf_registrations) {
            OpenAPI_list_for_each(context_data_sets->smf_registrations, smf_registrations_node) {
                cJSON *itemLocal = OpenAPI_smf_registration_convertToJSON(smf_registrations_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_context_data_sets_convertToJSON() failed [smf_registrations]");
                    goto end;
                }
                cJSON_AddItemToArray(smf_registrationsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_context_data_sets_t *OpenAPI_context_data_sets_parseFromJSON(cJSON *context_data_setsJSON)
{
    OpenAPI_context_data_sets_t *context_data_sets_local_var = NULL;
    cJSON *amf3_gpp = cJSON_GetObjectItemCaseSensitive(context_data_setsJSON, "amf3Gpp");

    OpenAPI_amf3_gpp_access_registration_t *amf3_gpp_local_nonprim = NULL;
    if (amf3_gpp) {
        amf3_gpp_local_nonprim = OpenAPI_amf3_gpp_access_registration_parseFromJSON(amf3_gpp);
    }

    cJSON *amf_non3_gpp = cJSON_GetObjectItemCaseSensitive(context_data_setsJSON, "amfNon3Gpp");

    OpenAPI_amf_non3_gpp_access_registration_t *amf_non3_gpp_local_nonprim = NULL;
    if (amf_non3_gpp) {
        amf_non3_gpp_local_nonprim = OpenAPI_amf_non3_gpp_access_registration_parseFromJSON(amf_non3_gpp);
    }

    cJSON *sdm_subscriptions = cJSON_GetObjectItemCaseSensitive(context_data_setsJSON, "sdmSubscriptions");

    OpenAPI_list_t *sdm_subscriptionsList;
    if (sdm_subscriptions) {
        cJSON *sdm_subscriptions_local_nonprimitive;
        if (!cJSON_IsArray(sdm_subscriptions)) {
            ogs_error("OpenAPI_context_data_sets_parseFromJSON() failed [sdm_subscriptions]");
            goto end;
        }

        sdm_subscriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(sdm_subscriptions_local_nonprimitive, sdm_subscriptions ) {
            if (!cJSON_IsObject(sdm_subscriptions_local_nonprimitive)) {
                ogs_error("OpenAPI_context_data_sets_parseFromJSON() failed [sdm_subscriptions]");
                goto end;
            }
            OpenAPI_sdm_subscription_t *sdm_subscriptionsItem = OpenAPI_sdm_subscription_parseFromJSON(sdm_subscriptions_local_nonprimitive);

            OpenAPI_list_add(sdm_subscriptionsList, sdm_subscriptionsItem);
        }
    }

    cJSON *ee_subscriptions = cJSON_GetObjectItemCaseSensitive(context_data_setsJSON, "eeSubscriptions");

    OpenAPI_list_t *ee_subscriptionsList;
    if (ee_subscriptions) {
        cJSON *ee_subscriptions_local_nonprimitive;
        if (!cJSON_IsArray(ee_subscriptions)) {
            ogs_error("OpenAPI_context_data_sets_parseFromJSON() failed [ee_subscriptions]");
            goto end;
        }

        ee_subscriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ee_subscriptions_local_nonprimitive, ee_subscriptions ) {
            if (!cJSON_IsObject(ee_subscriptions_local_nonprimitive)) {
                ogs_error("OpenAPI_context_data_sets_parseFromJSON() failed [ee_subscriptions]");
                goto end;
            }
            OpenAPI_ee_subscription_t *ee_subscriptionsItem = OpenAPI_ee_subscription_parseFromJSON(ee_subscriptions_local_nonprimitive);

            OpenAPI_list_add(ee_subscriptionsList, ee_subscriptionsItem);
        }
    }

    cJSON *smsf3_gpp_access = cJSON_GetObjectItemCaseSensitive(context_data_setsJSON, "smsf3GppAccess");

    OpenAPI_smsf_registration_t *smsf3_gpp_access_local_nonprim = NULL;
    if (smsf3_gpp_access) {
        smsf3_gpp_access_local_nonprim = OpenAPI_smsf_registration_parseFromJSON(smsf3_gpp_access);
    }

    cJSON *smsf_non3_gpp_access = cJSON_GetObjectItemCaseSensitive(context_data_setsJSON, "smsfNon3GppAccess");

    OpenAPI_smsf_registration_t *smsf_non3_gpp_access_local_nonprim = NULL;
    if (smsf_non3_gpp_access) {
        smsf_non3_gpp_access_local_nonprim = OpenAPI_smsf_registration_parseFromJSON(smsf_non3_gpp_access);
    }

    cJSON *subscription_data_subscriptions = cJSON_GetObjectItemCaseSensitive(context_data_setsJSON, "subscriptionDataSubscriptions");

    OpenAPI_list_t *subscription_data_subscriptionsList;
    if (subscription_data_subscriptions) {
        cJSON *subscription_data_subscriptions_local_nonprimitive;
        if (!cJSON_IsArray(subscription_data_subscriptions)) {
            ogs_error("OpenAPI_context_data_sets_parseFromJSON() failed [subscription_data_subscriptions]");
            goto end;
        }

        subscription_data_subscriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(subscription_data_subscriptions_local_nonprimitive, subscription_data_subscriptions ) {
            if (!cJSON_IsObject(subscription_data_subscriptions_local_nonprimitive)) {
                ogs_error("OpenAPI_context_data_sets_parseFromJSON() failed [subscription_data_subscriptions]");
                goto end;
            }
            OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptionsItem = OpenAPI_subscription_data_subscriptions_parseFromJSON(subscription_data_subscriptions_local_nonprimitive);

            OpenAPI_list_add(subscription_data_subscriptionsList, subscription_data_subscriptionsItem);
        }
    }

    cJSON *smf_registrations = cJSON_GetObjectItemCaseSensitive(context_data_setsJSON, "smfRegistrations");

    OpenAPI_list_t *smf_registrationsList;
    if (smf_registrations) {
        cJSON *smf_registrations_local_nonprimitive;
        if (!cJSON_IsArray(smf_registrations)) {
            ogs_error("OpenAPI_context_data_sets_parseFromJSON() failed [smf_registrations]");
            goto end;
        }

        smf_registrationsList = OpenAPI_list_create();

        cJSON_ArrayForEach(smf_registrations_local_nonprimitive, smf_registrations ) {
            if (!cJSON_IsObject(smf_registrations_local_nonprimitive)) {
                ogs_error("OpenAPI_context_data_sets_parseFromJSON() failed [smf_registrations]");
                goto end;
            }
            OpenAPI_smf_registration_t *smf_registrationsItem = OpenAPI_smf_registration_parseFromJSON(smf_registrations_local_nonprimitive);

            OpenAPI_list_add(smf_registrationsList, smf_registrationsItem);
        }
    }

    context_data_sets_local_var = OpenAPI_context_data_sets_create (
        amf3_gpp ? amf3_gpp_local_nonprim : NULL,
        amf_non3_gpp ? amf_non3_gpp_local_nonprim : NULL,
        sdm_subscriptions ? sdm_subscriptionsList : NULL,
        ee_subscriptions ? ee_subscriptionsList : NULL,
        smsf3_gpp_access ? smsf3_gpp_access_local_nonprim : NULL,
        smsf_non3_gpp_access ? smsf_non3_gpp_access_local_nonprim : NULL,
        subscription_data_subscriptions ? subscription_data_subscriptionsList : NULL,
        smf_registrations ? smf_registrationsList : NULL
        );

    return context_data_sets_local_var;
end:
    return NULL;
}

OpenAPI_context_data_sets_t *OpenAPI_context_data_sets_copy(OpenAPI_context_data_sets_t *dst, OpenAPI_context_data_sets_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_context_data_sets_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_context_data_sets_convertToJSON() failed");
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

    OpenAPI_context_data_sets_free(dst);
    dst = OpenAPI_context_data_sets_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


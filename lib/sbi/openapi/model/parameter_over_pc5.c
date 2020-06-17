
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parameter_over_pc5.h"

OpenAPI_parameter_over_pc5_t *OpenAPI_parameter_over_pc5_create(
    char *expiry,
    OpenAPI_list_t *plmm_rat_served,
    int auth_not_served,
    OpenAPI_list_t *radio_params_not_served,
    OpenAPI_list_t *ser_to_tx,
    OpenAPI_list_t *privacy_params,
    OpenAPI_configuration_parameters_eutra_t *config_para_eutra,
    OpenAPI_configuration_parameters_nr_t *config_para_nr
    )
{
    OpenAPI_parameter_over_pc5_t *parameter_over_pc5_local_var = OpenAPI_malloc(sizeof(OpenAPI_parameter_over_pc5_t));
    if (!parameter_over_pc5_local_var) {
        return NULL;
    }
    parameter_over_pc5_local_var->expiry = expiry;
    parameter_over_pc5_local_var->plmm_rat_served = plmm_rat_served;
    parameter_over_pc5_local_var->auth_not_served = auth_not_served;
    parameter_over_pc5_local_var->radio_params_not_served = radio_params_not_served;
    parameter_over_pc5_local_var->ser_to_tx = ser_to_tx;
    parameter_over_pc5_local_var->privacy_params = privacy_params;
    parameter_over_pc5_local_var->config_para_eutra = config_para_eutra;
    parameter_over_pc5_local_var->config_para_nr = config_para_nr;

    return parameter_over_pc5_local_var;
}

void OpenAPI_parameter_over_pc5_free(OpenAPI_parameter_over_pc5_t *parameter_over_pc5)
{
    if (NULL == parameter_over_pc5) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(parameter_over_pc5->expiry);
    OpenAPI_list_for_each(parameter_over_pc5->plmm_rat_served, node) {
        OpenAPI_plmn_rat_served_free(node->data);
    }
    OpenAPI_list_free(parameter_over_pc5->plmm_rat_served);
    OpenAPI_list_for_each(parameter_over_pc5->radio_params_not_served, node) {
        OpenAPI_radio_parameter_not_served_free(node->data);
    }
    OpenAPI_list_free(parameter_over_pc5->radio_params_not_served);
    OpenAPI_list_for_each(parameter_over_pc5->ser_to_tx, node) {
        OpenAPI_service_to_tx_free(node->data);
    }
    OpenAPI_list_free(parameter_over_pc5->ser_to_tx);
    OpenAPI_list_for_each(parameter_over_pc5->privacy_params, node) {
        OpenAPI_privacy_parameter_free(node->data);
    }
    OpenAPI_list_free(parameter_over_pc5->privacy_params);
    OpenAPI_configuration_parameters_eutra_free(parameter_over_pc5->config_para_eutra);
    OpenAPI_configuration_parameters_nr_free(parameter_over_pc5->config_para_nr);
    ogs_free(parameter_over_pc5);
}

cJSON *OpenAPI_parameter_over_pc5_convertToJSON(OpenAPI_parameter_over_pc5_t *parameter_over_pc5)
{
    cJSON *item = NULL;

    if (parameter_over_pc5 == NULL) {
        ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [ParameterOverPc5]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (parameter_over_pc5->expiry) {
        if (cJSON_AddStringToObject(item, "expiry", parameter_over_pc5->expiry) == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [expiry]");
            goto end;
        }
    }

    if (parameter_over_pc5->plmm_rat_served) {
        cJSON *plmm_rat_servedList = cJSON_AddArrayToObject(item, "plmmRatServed");
        if (plmm_rat_servedList == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [plmm_rat_served]");
            goto end;
        }

        OpenAPI_lnode_t *plmm_rat_served_node;
        if (parameter_over_pc5->plmm_rat_served) {
            OpenAPI_list_for_each(parameter_over_pc5->plmm_rat_served, plmm_rat_served_node) {
                cJSON *itemLocal = OpenAPI_plmn_rat_served_convertToJSON(plmm_rat_served_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [plmm_rat_served]");
                    goto end;
                }
                cJSON_AddItemToArray(plmm_rat_servedList, itemLocal);
            }
        }
    }

    if (parameter_over_pc5->auth_not_served) {
        if (cJSON_AddBoolToObject(item, "authNotServed", parameter_over_pc5->auth_not_served) == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [auth_not_served]");
            goto end;
        }
    }

    if (parameter_over_pc5->radio_params_not_served) {
        cJSON *radio_params_not_servedList = cJSON_AddArrayToObject(item, "radioParamsNotServed");
        if (radio_params_not_servedList == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [radio_params_not_served]");
            goto end;
        }

        OpenAPI_lnode_t *radio_params_not_served_node;
        if (parameter_over_pc5->radio_params_not_served) {
            OpenAPI_list_for_each(parameter_over_pc5->radio_params_not_served, radio_params_not_served_node) {
                cJSON *itemLocal = OpenAPI_radio_parameter_not_served_convertToJSON(radio_params_not_served_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [radio_params_not_served]");
                    goto end;
                }
                cJSON_AddItemToArray(radio_params_not_servedList, itemLocal);
            }
        }
    }

    if (parameter_over_pc5->ser_to_tx) {
        cJSON *ser_to_txList = cJSON_AddArrayToObject(item, "serToTx");
        if (ser_to_txList == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [ser_to_tx]");
            goto end;
        }

        OpenAPI_lnode_t *ser_to_tx_node;
        if (parameter_over_pc5->ser_to_tx) {
            OpenAPI_list_for_each(parameter_over_pc5->ser_to_tx, ser_to_tx_node) {
                cJSON *itemLocal = OpenAPI_service_to_tx_convertToJSON(ser_to_tx_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [ser_to_tx]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_to_txList, itemLocal);
            }
        }
    }

    if (parameter_over_pc5->privacy_params) {
        cJSON *privacy_paramsList = cJSON_AddArrayToObject(item, "privacyParams");
        if (privacy_paramsList == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [privacy_params]");
            goto end;
        }

        OpenAPI_lnode_t *privacy_params_node;
        if (parameter_over_pc5->privacy_params) {
            OpenAPI_list_for_each(parameter_over_pc5->privacy_params, privacy_params_node) {
                cJSON *itemLocal = OpenAPI_privacy_parameter_convertToJSON(privacy_params_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [privacy_params]");
                    goto end;
                }
                cJSON_AddItemToArray(privacy_paramsList, itemLocal);
            }
        }
    }

    if (parameter_over_pc5->config_para_eutra) {
        cJSON *config_para_eutra_local_JSON = OpenAPI_configuration_parameters_eutra_convertToJSON(parameter_over_pc5->config_para_eutra);
        if (config_para_eutra_local_JSON == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [config_para_eutra]");
            goto end;
        }
        cJSON_AddItemToObject(item, "configParaEutra", config_para_eutra_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [config_para_eutra]");
            goto end;
        }
    }

    if (parameter_over_pc5->config_para_nr) {
        cJSON *config_para_nr_local_JSON = OpenAPI_configuration_parameters_nr_convertToJSON(parameter_over_pc5->config_para_nr);
        if (config_para_nr_local_JSON == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [config_para_nr]");
            goto end;
        }
        cJSON_AddItemToObject(item, "configParaNr", config_para_nr_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed [config_para_nr]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_parameter_over_pc5_t *OpenAPI_parameter_over_pc5_parseFromJSON(cJSON *parameter_over_pc5JSON)
{
    OpenAPI_parameter_over_pc5_t *parameter_over_pc5_local_var = NULL;
    cJSON *expiry = cJSON_GetObjectItemCaseSensitive(parameter_over_pc5JSON, "expiry");

    if (expiry) {
        if (!cJSON_IsString(expiry)) {
            ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [expiry]");
            goto end;
        }
    }

    cJSON *plmm_rat_served = cJSON_GetObjectItemCaseSensitive(parameter_over_pc5JSON, "plmmRatServed");

    OpenAPI_list_t *plmm_rat_servedList;
    if (plmm_rat_served) {
        cJSON *plmm_rat_served_local_nonprimitive;
        if (!cJSON_IsArray(plmm_rat_served)) {
            ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [plmm_rat_served]");
            goto end;
        }

        plmm_rat_servedList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmm_rat_served_local_nonprimitive, plmm_rat_served ) {
            if (!cJSON_IsObject(plmm_rat_served_local_nonprimitive)) {
                ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [plmm_rat_served]");
                goto end;
            }
            OpenAPI_plmn_rat_served_t *plmm_rat_servedItem = OpenAPI_plmn_rat_served_parseFromJSON(plmm_rat_served_local_nonprimitive);

            OpenAPI_list_add(plmm_rat_servedList, plmm_rat_servedItem);
        }
    }

    cJSON *auth_not_served = cJSON_GetObjectItemCaseSensitive(parameter_over_pc5JSON, "authNotServed");

    if (auth_not_served) {
        if (!cJSON_IsBool(auth_not_served)) {
            ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [auth_not_served]");
            goto end;
        }
    }

    cJSON *radio_params_not_served = cJSON_GetObjectItemCaseSensitive(parameter_over_pc5JSON, "radioParamsNotServed");

    OpenAPI_list_t *radio_params_not_servedList;
    if (radio_params_not_served) {
        cJSON *radio_params_not_served_local_nonprimitive;
        if (!cJSON_IsArray(radio_params_not_served)) {
            ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [radio_params_not_served]");
            goto end;
        }

        radio_params_not_servedList = OpenAPI_list_create();

        cJSON_ArrayForEach(radio_params_not_served_local_nonprimitive, radio_params_not_served ) {
            if (!cJSON_IsObject(radio_params_not_served_local_nonprimitive)) {
                ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [radio_params_not_served]");
                goto end;
            }
            OpenAPI_radio_parameter_not_served_t *radio_params_not_servedItem = OpenAPI_radio_parameter_not_served_parseFromJSON(radio_params_not_served_local_nonprimitive);

            OpenAPI_list_add(radio_params_not_servedList, radio_params_not_servedItem);
        }
    }

    cJSON *ser_to_tx = cJSON_GetObjectItemCaseSensitive(parameter_over_pc5JSON, "serToTx");

    OpenAPI_list_t *ser_to_txList;
    if (ser_to_tx) {
        cJSON *ser_to_tx_local_nonprimitive;
        if (!cJSON_IsArray(ser_to_tx)) {
            ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [ser_to_tx]");
            goto end;
        }

        ser_to_txList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_to_tx_local_nonprimitive, ser_to_tx ) {
            if (!cJSON_IsObject(ser_to_tx_local_nonprimitive)) {
                ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [ser_to_tx]");
                goto end;
            }
            OpenAPI_service_to_tx_t *ser_to_txItem = OpenAPI_service_to_tx_parseFromJSON(ser_to_tx_local_nonprimitive);

            OpenAPI_list_add(ser_to_txList, ser_to_txItem);
        }
    }

    cJSON *privacy_params = cJSON_GetObjectItemCaseSensitive(parameter_over_pc5JSON, "privacyParams");

    OpenAPI_list_t *privacy_paramsList;
    if (privacy_params) {
        cJSON *privacy_params_local_nonprimitive;
        if (!cJSON_IsArray(privacy_params)) {
            ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [privacy_params]");
            goto end;
        }

        privacy_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(privacy_params_local_nonprimitive, privacy_params ) {
            if (!cJSON_IsObject(privacy_params_local_nonprimitive)) {
                ogs_error("OpenAPI_parameter_over_pc5_parseFromJSON() failed [privacy_params]");
                goto end;
            }
            OpenAPI_privacy_parameter_t *privacy_paramsItem = OpenAPI_privacy_parameter_parseFromJSON(privacy_params_local_nonprimitive);

            OpenAPI_list_add(privacy_paramsList, privacy_paramsItem);
        }
    }

    cJSON *config_para_eutra = cJSON_GetObjectItemCaseSensitive(parameter_over_pc5JSON, "configParaEutra");

    OpenAPI_configuration_parameters_eutra_t *config_para_eutra_local_nonprim = NULL;
    if (config_para_eutra) {
        config_para_eutra_local_nonprim = OpenAPI_configuration_parameters_eutra_parseFromJSON(config_para_eutra);
    }

    cJSON *config_para_nr = cJSON_GetObjectItemCaseSensitive(parameter_over_pc5JSON, "configParaNr");

    OpenAPI_configuration_parameters_nr_t *config_para_nr_local_nonprim = NULL;
    if (config_para_nr) {
        config_para_nr_local_nonprim = OpenAPI_configuration_parameters_nr_parseFromJSON(config_para_nr);
    }

    parameter_over_pc5_local_var = OpenAPI_parameter_over_pc5_create (
        expiry ? ogs_strdup(expiry->valuestring) : NULL,
        plmm_rat_served ? plmm_rat_servedList : NULL,
        auth_not_served ? auth_not_served->valueint : 0,
        radio_params_not_served ? radio_params_not_servedList : NULL,
        ser_to_tx ? ser_to_txList : NULL,
        privacy_params ? privacy_paramsList : NULL,
        config_para_eutra ? config_para_eutra_local_nonprim : NULL,
        config_para_nr ? config_para_nr_local_nonprim : NULL
        );

    return parameter_over_pc5_local_var;
end:
    return NULL;
}

OpenAPI_parameter_over_pc5_t *OpenAPI_parameter_over_pc5_copy(OpenAPI_parameter_over_pc5_t *dst, OpenAPI_parameter_over_pc5_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_parameter_over_pc5_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_parameter_over_pc5_convertToJSON() failed");
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

    OpenAPI_parameter_over_pc5_free(dst);
    dst = OpenAPI_parameter_over_pc5_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


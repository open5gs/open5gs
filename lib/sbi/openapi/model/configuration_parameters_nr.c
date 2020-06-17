
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "configuration_parameters_nr.h"

OpenAPI_configuration_parameters_nr_t *OpenAPI_configuration_parameters_nr_create(
    OpenAPI_list_t *ser_to_bro_layer2_ids,
    OpenAPI_list_t *ser_to_gro_layer2_ids,
    OpenAPI_list_t *ser_to_def_layer2_ids,
    OpenAPI_list_t *ser_id_to_frequs,
    OpenAPI_list_t *pc5_qos_mappings,
    OpenAPI_list_t *slrb_configs
    )
{
    OpenAPI_configuration_parameters_nr_t *configuration_parameters_nr_local_var = OpenAPI_malloc(sizeof(OpenAPI_configuration_parameters_nr_t));
    if (!configuration_parameters_nr_local_var) {
        return NULL;
    }
    configuration_parameters_nr_local_var->ser_to_bro_layer2_ids = ser_to_bro_layer2_ids;
    configuration_parameters_nr_local_var->ser_to_gro_layer2_ids = ser_to_gro_layer2_ids;
    configuration_parameters_nr_local_var->ser_to_def_layer2_ids = ser_to_def_layer2_ids;
    configuration_parameters_nr_local_var->ser_id_to_frequs = ser_id_to_frequs;
    configuration_parameters_nr_local_var->pc5_qos_mappings = pc5_qos_mappings;
    configuration_parameters_nr_local_var->slrb_configs = slrb_configs;

    return configuration_parameters_nr_local_var;
}

void OpenAPI_configuration_parameters_nr_free(OpenAPI_configuration_parameters_nr_t *configuration_parameters_nr)
{
    if (NULL == configuration_parameters_nr) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(configuration_parameters_nr->ser_to_bro_layer2_ids, node) {
        OpenAPI_service_id_to_layer2_id_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_nr->ser_to_bro_layer2_ids);
    OpenAPI_list_for_each(configuration_parameters_nr->ser_to_gro_layer2_ids, node) {
        OpenAPI_service_id_to_layer2_id_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_nr->ser_to_gro_layer2_ids);
    OpenAPI_list_for_each(configuration_parameters_nr->ser_to_def_layer2_ids, node) {
        OpenAPI_service_id_to_layer2_id_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_nr->ser_to_def_layer2_ids);
    OpenAPI_list_for_each(configuration_parameters_nr->ser_id_to_frequs, node) {
        OpenAPI_service_id_to_frequency_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_nr->ser_id_to_frequs);
    OpenAPI_list_for_each(configuration_parameters_nr->pc5_qos_mappings, node) {
        OpenAPI_pc5_qos_mapping_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_nr->pc5_qos_mappings);
    OpenAPI_list_for_each(configuration_parameters_nr->slrb_configs, node) {
        OpenAPI_slrb_configurations_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_nr->slrb_configs);
    ogs_free(configuration_parameters_nr);
}

cJSON *OpenAPI_configuration_parameters_nr_convertToJSON(OpenAPI_configuration_parameters_nr_t *configuration_parameters_nr)
{
    cJSON *item = NULL;

    if (configuration_parameters_nr == NULL) {
        ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [ConfigurationParametersNr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (configuration_parameters_nr->ser_to_bro_layer2_ids) {
        cJSON *ser_to_bro_layer2_idsList = cJSON_AddArrayToObject(item, "serToBroLayer2Ids");
        if (ser_to_bro_layer2_idsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [ser_to_bro_layer2_ids]");
            goto end;
        }

        OpenAPI_lnode_t *ser_to_bro_layer2_ids_node;
        if (configuration_parameters_nr->ser_to_bro_layer2_ids) {
            OpenAPI_list_for_each(configuration_parameters_nr->ser_to_bro_layer2_ids, ser_to_bro_layer2_ids_node) {
                cJSON *itemLocal = OpenAPI_service_id_to_layer2_id_convertToJSON(ser_to_bro_layer2_ids_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [ser_to_bro_layer2_ids]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_to_bro_layer2_idsList, itemLocal);
            }
        }
    }

    if (configuration_parameters_nr->ser_to_gro_layer2_ids) {
        cJSON *ser_to_gro_layer2_idsList = cJSON_AddArrayToObject(item, "serToGroLayer2Ids");
        if (ser_to_gro_layer2_idsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [ser_to_gro_layer2_ids]");
            goto end;
        }

        OpenAPI_lnode_t *ser_to_gro_layer2_ids_node;
        if (configuration_parameters_nr->ser_to_gro_layer2_ids) {
            OpenAPI_list_for_each(configuration_parameters_nr->ser_to_gro_layer2_ids, ser_to_gro_layer2_ids_node) {
                cJSON *itemLocal = OpenAPI_service_id_to_layer2_id_convertToJSON(ser_to_gro_layer2_ids_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [ser_to_gro_layer2_ids]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_to_gro_layer2_idsList, itemLocal);
            }
        }
    }

    if (configuration_parameters_nr->ser_to_def_layer2_ids) {
        cJSON *ser_to_def_layer2_idsList = cJSON_AddArrayToObject(item, "serToDefLayer2Ids");
        if (ser_to_def_layer2_idsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [ser_to_def_layer2_ids]");
            goto end;
        }

        OpenAPI_lnode_t *ser_to_def_layer2_ids_node;
        if (configuration_parameters_nr->ser_to_def_layer2_ids) {
            OpenAPI_list_for_each(configuration_parameters_nr->ser_to_def_layer2_ids, ser_to_def_layer2_ids_node) {
                cJSON *itemLocal = OpenAPI_service_id_to_layer2_id_convertToJSON(ser_to_def_layer2_ids_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [ser_to_def_layer2_ids]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_to_def_layer2_idsList, itemLocal);
            }
        }
    }

    if (configuration_parameters_nr->ser_id_to_frequs) {
        cJSON *ser_id_to_frequsList = cJSON_AddArrayToObject(item, "serIdToFrequs");
        if (ser_id_to_frequsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [ser_id_to_frequs]");
            goto end;
        }

        OpenAPI_lnode_t *ser_id_to_frequs_node;
        if (configuration_parameters_nr->ser_id_to_frequs) {
            OpenAPI_list_for_each(configuration_parameters_nr->ser_id_to_frequs, ser_id_to_frequs_node) {
                cJSON *itemLocal = OpenAPI_service_id_to_frequency_convertToJSON(ser_id_to_frequs_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [ser_id_to_frequs]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_id_to_frequsList, itemLocal);
            }
        }
    }

    if (configuration_parameters_nr->pc5_qos_mappings) {
        cJSON *pc5_qos_mappingsList = cJSON_AddArrayToObject(item, "pc5QosMappings");
        if (pc5_qos_mappingsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [pc5_qos_mappings]");
            goto end;
        }

        OpenAPI_lnode_t *pc5_qos_mappings_node;
        if (configuration_parameters_nr->pc5_qos_mappings) {
            OpenAPI_list_for_each(configuration_parameters_nr->pc5_qos_mappings, pc5_qos_mappings_node) {
                cJSON *itemLocal = OpenAPI_pc5_qos_mapping_convertToJSON(pc5_qos_mappings_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [pc5_qos_mappings]");
                    goto end;
                }
                cJSON_AddItemToArray(pc5_qos_mappingsList, itemLocal);
            }
        }
    }

    if (configuration_parameters_nr->slrb_configs) {
        cJSON *slrb_configsList = cJSON_AddArrayToObject(item, "slrbConfigs");
        if (slrb_configsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [slrb_configs]");
            goto end;
        }

        OpenAPI_lnode_t *slrb_configs_node;
        if (configuration_parameters_nr->slrb_configs) {
            OpenAPI_list_for_each(configuration_parameters_nr->slrb_configs, slrb_configs_node) {
                cJSON *itemLocal = OpenAPI_slrb_configurations_convertToJSON(slrb_configs_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed [slrb_configs]");
                    goto end;
                }
                cJSON_AddItemToArray(slrb_configsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_configuration_parameters_nr_t *OpenAPI_configuration_parameters_nr_parseFromJSON(cJSON *configuration_parameters_nrJSON)
{
    OpenAPI_configuration_parameters_nr_t *configuration_parameters_nr_local_var = NULL;
    cJSON *ser_to_bro_layer2_ids = cJSON_GetObjectItemCaseSensitive(configuration_parameters_nrJSON, "serToBroLayer2Ids");

    OpenAPI_list_t *ser_to_bro_layer2_idsList;
    if (ser_to_bro_layer2_ids) {
        cJSON *ser_to_bro_layer2_ids_local_nonprimitive;
        if (!cJSON_IsArray(ser_to_bro_layer2_ids)) {
            ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [ser_to_bro_layer2_ids]");
            goto end;
        }

        ser_to_bro_layer2_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_to_bro_layer2_ids_local_nonprimitive, ser_to_bro_layer2_ids ) {
            if (!cJSON_IsObject(ser_to_bro_layer2_ids_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [ser_to_bro_layer2_ids]");
                goto end;
            }
            OpenAPI_service_id_to_layer2_id_t *ser_to_bro_layer2_idsItem = OpenAPI_service_id_to_layer2_id_parseFromJSON(ser_to_bro_layer2_ids_local_nonprimitive);

            OpenAPI_list_add(ser_to_bro_layer2_idsList, ser_to_bro_layer2_idsItem);
        }
    }

    cJSON *ser_to_gro_layer2_ids = cJSON_GetObjectItemCaseSensitive(configuration_parameters_nrJSON, "serToGroLayer2Ids");

    OpenAPI_list_t *ser_to_gro_layer2_idsList;
    if (ser_to_gro_layer2_ids) {
        cJSON *ser_to_gro_layer2_ids_local_nonprimitive;
        if (!cJSON_IsArray(ser_to_gro_layer2_ids)) {
            ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [ser_to_gro_layer2_ids]");
            goto end;
        }

        ser_to_gro_layer2_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_to_gro_layer2_ids_local_nonprimitive, ser_to_gro_layer2_ids ) {
            if (!cJSON_IsObject(ser_to_gro_layer2_ids_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [ser_to_gro_layer2_ids]");
                goto end;
            }
            OpenAPI_service_id_to_layer2_id_t *ser_to_gro_layer2_idsItem = OpenAPI_service_id_to_layer2_id_parseFromJSON(ser_to_gro_layer2_ids_local_nonprimitive);

            OpenAPI_list_add(ser_to_gro_layer2_idsList, ser_to_gro_layer2_idsItem);
        }
    }

    cJSON *ser_to_def_layer2_ids = cJSON_GetObjectItemCaseSensitive(configuration_parameters_nrJSON, "serToDefLayer2Ids");

    OpenAPI_list_t *ser_to_def_layer2_idsList;
    if (ser_to_def_layer2_ids) {
        cJSON *ser_to_def_layer2_ids_local_nonprimitive;
        if (!cJSON_IsArray(ser_to_def_layer2_ids)) {
            ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [ser_to_def_layer2_ids]");
            goto end;
        }

        ser_to_def_layer2_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_to_def_layer2_ids_local_nonprimitive, ser_to_def_layer2_ids ) {
            if (!cJSON_IsObject(ser_to_def_layer2_ids_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [ser_to_def_layer2_ids]");
                goto end;
            }
            OpenAPI_service_id_to_layer2_id_t *ser_to_def_layer2_idsItem = OpenAPI_service_id_to_layer2_id_parseFromJSON(ser_to_def_layer2_ids_local_nonprimitive);

            OpenAPI_list_add(ser_to_def_layer2_idsList, ser_to_def_layer2_idsItem);
        }
    }

    cJSON *ser_id_to_frequs = cJSON_GetObjectItemCaseSensitive(configuration_parameters_nrJSON, "serIdToFrequs");

    OpenAPI_list_t *ser_id_to_frequsList;
    if (ser_id_to_frequs) {
        cJSON *ser_id_to_frequs_local_nonprimitive;
        if (!cJSON_IsArray(ser_id_to_frequs)) {
            ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [ser_id_to_frequs]");
            goto end;
        }

        ser_id_to_frequsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_id_to_frequs_local_nonprimitive, ser_id_to_frequs ) {
            if (!cJSON_IsObject(ser_id_to_frequs_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [ser_id_to_frequs]");
                goto end;
            }
            OpenAPI_service_id_to_frequency_t *ser_id_to_frequsItem = OpenAPI_service_id_to_frequency_parseFromJSON(ser_id_to_frequs_local_nonprimitive);

            OpenAPI_list_add(ser_id_to_frequsList, ser_id_to_frequsItem);
        }
    }

    cJSON *pc5_qos_mappings = cJSON_GetObjectItemCaseSensitive(configuration_parameters_nrJSON, "pc5QosMappings");

    OpenAPI_list_t *pc5_qos_mappingsList;
    if (pc5_qos_mappings) {
        cJSON *pc5_qos_mappings_local_nonprimitive;
        if (!cJSON_IsArray(pc5_qos_mappings)) {
            ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [pc5_qos_mappings]");
            goto end;
        }

        pc5_qos_mappingsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pc5_qos_mappings_local_nonprimitive, pc5_qos_mappings ) {
            if (!cJSON_IsObject(pc5_qos_mappings_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [pc5_qos_mappings]");
                goto end;
            }
            OpenAPI_pc5_qos_mapping_t *pc5_qos_mappingsItem = OpenAPI_pc5_qos_mapping_parseFromJSON(pc5_qos_mappings_local_nonprimitive);

            OpenAPI_list_add(pc5_qos_mappingsList, pc5_qos_mappingsItem);
        }
    }

    cJSON *slrb_configs = cJSON_GetObjectItemCaseSensitive(configuration_parameters_nrJSON, "slrbConfigs");

    OpenAPI_list_t *slrb_configsList;
    if (slrb_configs) {
        cJSON *slrb_configs_local_nonprimitive;
        if (!cJSON_IsArray(slrb_configs)) {
            ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [slrb_configs]");
            goto end;
        }

        slrb_configsList = OpenAPI_list_create();

        cJSON_ArrayForEach(slrb_configs_local_nonprimitive, slrb_configs ) {
            if (!cJSON_IsObject(slrb_configs_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_nr_parseFromJSON() failed [slrb_configs]");
                goto end;
            }
            OpenAPI_slrb_configurations_t *slrb_configsItem = OpenAPI_slrb_configurations_parseFromJSON(slrb_configs_local_nonprimitive);

            OpenAPI_list_add(slrb_configsList, slrb_configsItem);
        }
    }

    configuration_parameters_nr_local_var = OpenAPI_configuration_parameters_nr_create (
        ser_to_bro_layer2_ids ? ser_to_bro_layer2_idsList : NULL,
        ser_to_gro_layer2_ids ? ser_to_gro_layer2_idsList : NULL,
        ser_to_def_layer2_ids ? ser_to_def_layer2_idsList : NULL,
        ser_id_to_frequs ? ser_id_to_frequsList : NULL,
        pc5_qos_mappings ? pc5_qos_mappingsList : NULL,
        slrb_configs ? slrb_configsList : NULL
        );

    return configuration_parameters_nr_local_var;
end:
    return NULL;
}

OpenAPI_configuration_parameters_nr_t *OpenAPI_configuration_parameters_nr_copy(OpenAPI_configuration_parameters_nr_t *dst, OpenAPI_configuration_parameters_nr_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_configuration_parameters_nr_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_configuration_parameters_nr_convertToJSON() failed");
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

    OpenAPI_configuration_parameters_nr_free(dst);
    dst = OpenAPI_configuration_parameters_nr_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


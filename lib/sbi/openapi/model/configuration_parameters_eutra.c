
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "configuration_parameters_eutra.h"

OpenAPI_configuration_parameters_eutra_t *OpenAPI_configuration_parameters_eutra_create(
    OpenAPI_list_t *ser_to_layer2_ids,
    OpenAPI_list_t *pppp_to_pdbs,
    OpenAPI_list_t *ser_id_to_frequs,
    OpenAPI_list_t *ser_id_to_ppprs
    )
{
    OpenAPI_configuration_parameters_eutra_t *configuration_parameters_eutra_local_var = OpenAPI_malloc(sizeof(OpenAPI_configuration_parameters_eutra_t));
    if (!configuration_parameters_eutra_local_var) {
        return NULL;
    }
    configuration_parameters_eutra_local_var->ser_to_layer2_ids = ser_to_layer2_ids;
    configuration_parameters_eutra_local_var->pppp_to_pdbs = pppp_to_pdbs;
    configuration_parameters_eutra_local_var->ser_id_to_frequs = ser_id_to_frequs;
    configuration_parameters_eutra_local_var->ser_id_to_ppprs = ser_id_to_ppprs;

    return configuration_parameters_eutra_local_var;
}

void OpenAPI_configuration_parameters_eutra_free(OpenAPI_configuration_parameters_eutra_t *configuration_parameters_eutra)
{
    if (NULL == configuration_parameters_eutra) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(configuration_parameters_eutra->ser_to_layer2_ids, node) {
        OpenAPI_service_id_to_layer2_id_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_eutra->ser_to_layer2_ids);
    OpenAPI_list_for_each(configuration_parameters_eutra->pppp_to_pdbs, node) {
        OpenAPI_pppp_to_pdb_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_eutra->pppp_to_pdbs);
    OpenAPI_list_for_each(configuration_parameters_eutra->ser_id_to_frequs, node) {
        OpenAPI_service_id_to_frequency_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_eutra->ser_id_to_frequs);
    OpenAPI_list_for_each(configuration_parameters_eutra->ser_id_to_ppprs, node) {
        OpenAPI_service_id_to_pppr_free(node->data);
    }
    OpenAPI_list_free(configuration_parameters_eutra->ser_id_to_ppprs);
    ogs_free(configuration_parameters_eutra);
}

cJSON *OpenAPI_configuration_parameters_eutra_convertToJSON(OpenAPI_configuration_parameters_eutra_t *configuration_parameters_eutra)
{
    cJSON *item = NULL;

    if (configuration_parameters_eutra == NULL) {
        ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed [ConfigurationParametersEutra]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (configuration_parameters_eutra->ser_to_layer2_ids) {
        cJSON *ser_to_layer2_idsList = cJSON_AddArrayToObject(item, "serToLayer2Ids");
        if (ser_to_layer2_idsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed [ser_to_layer2_ids]");
            goto end;
        }

        OpenAPI_lnode_t *ser_to_layer2_ids_node;
        if (configuration_parameters_eutra->ser_to_layer2_ids) {
            OpenAPI_list_for_each(configuration_parameters_eutra->ser_to_layer2_ids, ser_to_layer2_ids_node) {
                cJSON *itemLocal = OpenAPI_service_id_to_layer2_id_convertToJSON(ser_to_layer2_ids_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed [ser_to_layer2_ids]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_to_layer2_idsList, itemLocal);
            }
        }
    }

    if (configuration_parameters_eutra->pppp_to_pdbs) {
        cJSON *pppp_to_pdbsList = cJSON_AddArrayToObject(item, "ppppToPdbs");
        if (pppp_to_pdbsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed [pppp_to_pdbs]");
            goto end;
        }

        OpenAPI_lnode_t *pppp_to_pdbs_node;
        if (configuration_parameters_eutra->pppp_to_pdbs) {
            OpenAPI_list_for_each(configuration_parameters_eutra->pppp_to_pdbs, pppp_to_pdbs_node) {
                cJSON *itemLocal = OpenAPI_pppp_to_pdb_convertToJSON(pppp_to_pdbs_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed [pppp_to_pdbs]");
                    goto end;
                }
                cJSON_AddItemToArray(pppp_to_pdbsList, itemLocal);
            }
        }
    }

    if (configuration_parameters_eutra->ser_id_to_frequs) {
        cJSON *ser_id_to_frequsList = cJSON_AddArrayToObject(item, "serIdToFrequs");
        if (ser_id_to_frequsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed [ser_id_to_frequs]");
            goto end;
        }

        OpenAPI_lnode_t *ser_id_to_frequs_node;
        if (configuration_parameters_eutra->ser_id_to_frequs) {
            OpenAPI_list_for_each(configuration_parameters_eutra->ser_id_to_frequs, ser_id_to_frequs_node) {
                cJSON *itemLocal = OpenAPI_service_id_to_frequency_convertToJSON(ser_id_to_frequs_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed [ser_id_to_frequs]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_id_to_frequsList, itemLocal);
            }
        }
    }

    if (configuration_parameters_eutra->ser_id_to_ppprs) {
        cJSON *ser_id_to_ppprsList = cJSON_AddArrayToObject(item, "serIdToPpprs");
        if (ser_id_to_ppprsList == NULL) {
            ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed [ser_id_to_ppprs]");
            goto end;
        }

        OpenAPI_lnode_t *ser_id_to_ppprs_node;
        if (configuration_parameters_eutra->ser_id_to_ppprs) {
            OpenAPI_list_for_each(configuration_parameters_eutra->ser_id_to_ppprs, ser_id_to_ppprs_node) {
                cJSON *itemLocal = OpenAPI_service_id_to_pppr_convertToJSON(ser_id_to_ppprs_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed [ser_id_to_ppprs]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_id_to_ppprsList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_configuration_parameters_eutra_t *OpenAPI_configuration_parameters_eutra_parseFromJSON(cJSON *configuration_parameters_eutraJSON)
{
    OpenAPI_configuration_parameters_eutra_t *configuration_parameters_eutra_local_var = NULL;
    cJSON *ser_to_layer2_ids = cJSON_GetObjectItemCaseSensitive(configuration_parameters_eutraJSON, "serToLayer2Ids");

    OpenAPI_list_t *ser_to_layer2_idsList;
    if (ser_to_layer2_ids) {
        cJSON *ser_to_layer2_ids_local_nonprimitive;
        if (!cJSON_IsArray(ser_to_layer2_ids)) {
            ogs_error("OpenAPI_configuration_parameters_eutra_parseFromJSON() failed [ser_to_layer2_ids]");
            goto end;
        }

        ser_to_layer2_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_to_layer2_ids_local_nonprimitive, ser_to_layer2_ids ) {
            if (!cJSON_IsObject(ser_to_layer2_ids_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_eutra_parseFromJSON() failed [ser_to_layer2_ids]");
                goto end;
            }
            OpenAPI_service_id_to_layer2_id_t *ser_to_layer2_idsItem = OpenAPI_service_id_to_layer2_id_parseFromJSON(ser_to_layer2_ids_local_nonprimitive);

            OpenAPI_list_add(ser_to_layer2_idsList, ser_to_layer2_idsItem);
        }
    }

    cJSON *pppp_to_pdbs = cJSON_GetObjectItemCaseSensitive(configuration_parameters_eutraJSON, "ppppToPdbs");

    OpenAPI_list_t *pppp_to_pdbsList;
    if (pppp_to_pdbs) {
        cJSON *pppp_to_pdbs_local_nonprimitive;
        if (!cJSON_IsArray(pppp_to_pdbs)) {
            ogs_error("OpenAPI_configuration_parameters_eutra_parseFromJSON() failed [pppp_to_pdbs]");
            goto end;
        }

        pppp_to_pdbsList = OpenAPI_list_create();

        cJSON_ArrayForEach(pppp_to_pdbs_local_nonprimitive, pppp_to_pdbs ) {
            if (!cJSON_IsObject(pppp_to_pdbs_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_eutra_parseFromJSON() failed [pppp_to_pdbs]");
                goto end;
            }
            OpenAPI_pppp_to_pdb_t *pppp_to_pdbsItem = OpenAPI_pppp_to_pdb_parseFromJSON(pppp_to_pdbs_local_nonprimitive);

            OpenAPI_list_add(pppp_to_pdbsList, pppp_to_pdbsItem);
        }
    }

    cJSON *ser_id_to_frequs = cJSON_GetObjectItemCaseSensitive(configuration_parameters_eutraJSON, "serIdToFrequs");

    OpenAPI_list_t *ser_id_to_frequsList;
    if (ser_id_to_frequs) {
        cJSON *ser_id_to_frequs_local_nonprimitive;
        if (!cJSON_IsArray(ser_id_to_frequs)) {
            ogs_error("OpenAPI_configuration_parameters_eutra_parseFromJSON() failed [ser_id_to_frequs]");
            goto end;
        }

        ser_id_to_frequsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_id_to_frequs_local_nonprimitive, ser_id_to_frequs ) {
            if (!cJSON_IsObject(ser_id_to_frequs_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_eutra_parseFromJSON() failed [ser_id_to_frequs]");
                goto end;
            }
            OpenAPI_service_id_to_frequency_t *ser_id_to_frequsItem = OpenAPI_service_id_to_frequency_parseFromJSON(ser_id_to_frequs_local_nonprimitive);

            OpenAPI_list_add(ser_id_to_frequsList, ser_id_to_frequsItem);
        }
    }

    cJSON *ser_id_to_ppprs = cJSON_GetObjectItemCaseSensitive(configuration_parameters_eutraJSON, "serIdToPpprs");

    OpenAPI_list_t *ser_id_to_ppprsList;
    if (ser_id_to_ppprs) {
        cJSON *ser_id_to_ppprs_local_nonprimitive;
        if (!cJSON_IsArray(ser_id_to_ppprs)) {
            ogs_error("OpenAPI_configuration_parameters_eutra_parseFromJSON() failed [ser_id_to_ppprs]");
            goto end;
        }

        ser_id_to_ppprsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_id_to_ppprs_local_nonprimitive, ser_id_to_ppprs ) {
            if (!cJSON_IsObject(ser_id_to_ppprs_local_nonprimitive)) {
                ogs_error("OpenAPI_configuration_parameters_eutra_parseFromJSON() failed [ser_id_to_ppprs]");
                goto end;
            }
            OpenAPI_service_id_to_pppr_t *ser_id_to_ppprsItem = OpenAPI_service_id_to_pppr_parseFromJSON(ser_id_to_ppprs_local_nonprimitive);

            OpenAPI_list_add(ser_id_to_ppprsList, ser_id_to_ppprsItem);
        }
    }

    configuration_parameters_eutra_local_var = OpenAPI_configuration_parameters_eutra_create (
        ser_to_layer2_ids ? ser_to_layer2_idsList : NULL,
        pppp_to_pdbs ? pppp_to_pdbsList : NULL,
        ser_id_to_frequs ? ser_id_to_frequsList : NULL,
        ser_id_to_ppprs ? ser_id_to_ppprsList : NULL
        );

    return configuration_parameters_eutra_local_var;
end:
    return NULL;
}

OpenAPI_configuration_parameters_eutra_t *OpenAPI_configuration_parameters_eutra_copy(OpenAPI_configuration_parameters_eutra_t *dst, OpenAPI_configuration_parameters_eutra_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_configuration_parameters_eutra_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_configuration_parameters_eutra_convertToJSON() failed");
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

    OpenAPI_configuration_parameters_eutra_free(dst);
    dst = OpenAPI_configuration_parameters_eutra_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}



#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_measurement.h"

OpenAPI_additional_measurement_t *OpenAPI_additional_measurement_create(
    OpenAPI_network_area_info_t *unexp_loc,
    OpenAPI_list_t *unexp_flow_teps,
    OpenAPI_list_t *unexp_wakes,
    OpenAPI_address_list_t *ddos_attack,
    OpenAPI_address_list_t *wrg_dest,
    OpenAPI_list_t *circums
)
{
    OpenAPI_additional_measurement_t *additional_measurement_local_var = ogs_malloc(sizeof(OpenAPI_additional_measurement_t));
    ogs_assert(additional_measurement_local_var);

    additional_measurement_local_var->unexp_loc = unexp_loc;
    additional_measurement_local_var->unexp_flow_teps = unexp_flow_teps;
    additional_measurement_local_var->unexp_wakes = unexp_wakes;
    additional_measurement_local_var->ddos_attack = ddos_attack;
    additional_measurement_local_var->wrg_dest = wrg_dest;
    additional_measurement_local_var->circums = circums;

    return additional_measurement_local_var;
}

void OpenAPI_additional_measurement_free(OpenAPI_additional_measurement_t *additional_measurement)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == additional_measurement) {
        return;
    }
    if (additional_measurement->unexp_loc) {
        OpenAPI_network_area_info_free(additional_measurement->unexp_loc);
        additional_measurement->unexp_loc = NULL;
    }
    if (additional_measurement->unexp_flow_teps) {
        OpenAPI_list_for_each(additional_measurement->unexp_flow_teps, node) {
            OpenAPI_ip_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(additional_measurement->unexp_flow_teps);
        additional_measurement->unexp_flow_teps = NULL;
    }
    if (additional_measurement->unexp_wakes) {
        OpenAPI_list_for_each(additional_measurement->unexp_wakes, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_measurement->unexp_wakes);
        additional_measurement->unexp_wakes = NULL;
    }
    if (additional_measurement->ddos_attack) {
        OpenAPI_address_list_free(additional_measurement->ddos_attack);
        additional_measurement->ddos_attack = NULL;
    }
    if (additional_measurement->wrg_dest) {
        OpenAPI_address_list_free(additional_measurement->wrg_dest);
        additional_measurement->wrg_dest = NULL;
    }
    if (additional_measurement->circums) {
        OpenAPI_list_for_each(additional_measurement->circums, node) {
            OpenAPI_circumstance_description_free(node->data);
        }
        OpenAPI_list_free(additional_measurement->circums);
        additional_measurement->circums = NULL;
    }
    ogs_free(additional_measurement);
}

cJSON *OpenAPI_additional_measurement_convertToJSON(OpenAPI_additional_measurement_t *additional_measurement)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (additional_measurement == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [AdditionalMeasurement]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (additional_measurement->unexp_loc) {
    cJSON *unexp_loc_local_JSON = OpenAPI_network_area_info_convertToJSON(additional_measurement->unexp_loc);
    if (unexp_loc_local_JSON == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [unexp_loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "unexpLoc", unexp_loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [unexp_loc]");
        goto end;
    }
    }

    if (additional_measurement->unexp_flow_teps) {
    cJSON *unexp_flow_tepsList = cJSON_AddArrayToObject(item, "unexpFlowTeps");
    if (unexp_flow_tepsList == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [unexp_flow_teps]");
        goto end;
    }
    OpenAPI_list_for_each(additional_measurement->unexp_flow_teps, node) {
        cJSON *itemLocal = OpenAPI_ip_eth_flow_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [unexp_flow_teps]");
            goto end;
        }
        cJSON_AddItemToArray(unexp_flow_tepsList, itemLocal);
    }
    }

    if (additional_measurement->unexp_wakes) {
    cJSON *unexp_wakesList = cJSON_AddArrayToObject(item, "unexpWakes");
    if (unexp_wakesList == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [unexp_wakes]");
        goto end;
    }
    OpenAPI_list_for_each(additional_measurement->unexp_wakes, node) {
    }
    }

    if (additional_measurement->ddos_attack) {
    cJSON *ddos_attack_local_JSON = OpenAPI_address_list_convertToJSON(additional_measurement->ddos_attack);
    if (ddos_attack_local_JSON == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [ddos_attack]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ddosAttack", ddos_attack_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [ddos_attack]");
        goto end;
    }
    }

    if (additional_measurement->wrg_dest) {
    cJSON *wrg_dest_local_JSON = OpenAPI_address_list_convertToJSON(additional_measurement->wrg_dest);
    if (wrg_dest_local_JSON == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [wrg_dest]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wrgDest", wrg_dest_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [wrg_dest]");
        goto end;
    }
    }

    if (additional_measurement->circums) {
    cJSON *circumsList = cJSON_AddArrayToObject(item, "circums");
    if (circumsList == NULL) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [circums]");
        goto end;
    }
    OpenAPI_list_for_each(additional_measurement->circums, node) {
        cJSON *itemLocal = OpenAPI_circumstance_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_additional_measurement_convertToJSON() failed [circums]");
            goto end;
        }
        cJSON_AddItemToArray(circumsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_additional_measurement_t *OpenAPI_additional_measurement_parseFromJSON(cJSON *additional_measurementJSON)
{
    OpenAPI_additional_measurement_t *additional_measurement_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *unexp_loc = NULL;
    OpenAPI_network_area_info_t *unexp_loc_local_nonprim = NULL;
    cJSON *unexp_flow_teps = NULL;
    OpenAPI_list_t *unexp_flow_tepsList = NULL;
    cJSON *unexp_wakes = NULL;
    OpenAPI_list_t *unexp_wakesList = NULL;
    cJSON *ddos_attack = NULL;
    OpenAPI_address_list_t *ddos_attack_local_nonprim = NULL;
    cJSON *wrg_dest = NULL;
    OpenAPI_address_list_t *wrg_dest_local_nonprim = NULL;
    cJSON *circums = NULL;
    OpenAPI_list_t *circumsList = NULL;
    unexp_loc = cJSON_GetObjectItemCaseSensitive(additional_measurementJSON, "unexpLoc");
    if (unexp_loc) {
    unexp_loc_local_nonprim = OpenAPI_network_area_info_parseFromJSON(unexp_loc);
    if (!unexp_loc_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [unexp_loc]");
        goto end;
    }
    }

    unexp_flow_teps = cJSON_GetObjectItemCaseSensitive(additional_measurementJSON, "unexpFlowTeps");
    if (unexp_flow_teps) {
        cJSON *unexp_flow_teps_local = NULL;
        if (!cJSON_IsArray(unexp_flow_teps)) {
            ogs_error("OpenAPI_additional_measurement_parseFromJSON() failed [unexp_flow_teps]");
            goto end;
        }

        unexp_flow_tepsList = OpenAPI_list_create();

        cJSON_ArrayForEach(unexp_flow_teps_local, unexp_flow_teps) {
            if (!cJSON_IsObject(unexp_flow_teps_local)) {
                ogs_error("OpenAPI_additional_measurement_parseFromJSON() failed [unexp_flow_teps]");
                goto end;
            }
            OpenAPI_ip_eth_flow_description_t *unexp_flow_tepsItem = OpenAPI_ip_eth_flow_description_parseFromJSON(unexp_flow_teps_local);
            if (!unexp_flow_tepsItem) {
                ogs_error("No unexp_flow_tepsItem");
                goto end;
            }
            OpenAPI_list_add(unexp_flow_tepsList, unexp_flow_tepsItem);
        }
    }

    unexp_wakes = cJSON_GetObjectItemCaseSensitive(additional_measurementJSON, "unexpWakes");
    if (unexp_wakes) {
        cJSON *unexp_wakes_local = NULL;
        if (!cJSON_IsArray(unexp_wakes)) {
            ogs_error("OpenAPI_additional_measurement_parseFromJSON() failed [unexp_wakes]");
            goto end;
        }

        unexp_wakesList = OpenAPI_list_create();

        cJSON_ArrayForEach(unexp_wakes_local, unexp_wakes) {
            double *localDouble = NULL;
            int *localInt = NULL;
        }
    }

    ddos_attack = cJSON_GetObjectItemCaseSensitive(additional_measurementJSON, "ddosAttack");
    if (ddos_attack) {
    ddos_attack_local_nonprim = OpenAPI_address_list_parseFromJSON(ddos_attack);
    if (!ddos_attack_local_nonprim) {
        ogs_error("OpenAPI_address_list_parseFromJSON failed [ddos_attack]");
        goto end;
    }
    }

    wrg_dest = cJSON_GetObjectItemCaseSensitive(additional_measurementJSON, "wrgDest");
    if (wrg_dest) {
    wrg_dest_local_nonprim = OpenAPI_address_list_parseFromJSON(wrg_dest);
    if (!wrg_dest_local_nonprim) {
        ogs_error("OpenAPI_address_list_parseFromJSON failed [wrg_dest]");
        goto end;
    }
    }

    circums = cJSON_GetObjectItemCaseSensitive(additional_measurementJSON, "circums");
    if (circums) {
        cJSON *circums_local = NULL;
        if (!cJSON_IsArray(circums)) {
            ogs_error("OpenAPI_additional_measurement_parseFromJSON() failed [circums]");
            goto end;
        }

        circumsList = OpenAPI_list_create();

        cJSON_ArrayForEach(circums_local, circums) {
            if (!cJSON_IsObject(circums_local)) {
                ogs_error("OpenAPI_additional_measurement_parseFromJSON() failed [circums]");
                goto end;
            }
            OpenAPI_circumstance_description_t *circumsItem = OpenAPI_circumstance_description_parseFromJSON(circums_local);
            if (!circumsItem) {
                ogs_error("No circumsItem");
                goto end;
            }
            OpenAPI_list_add(circumsList, circumsItem);
        }
    }

    additional_measurement_local_var = OpenAPI_additional_measurement_create (
        unexp_loc ? unexp_loc_local_nonprim : NULL,
        unexp_flow_teps ? unexp_flow_tepsList : NULL,
        unexp_wakes ? unexp_wakesList : NULL,
        ddos_attack ? ddos_attack_local_nonprim : NULL,
        wrg_dest ? wrg_dest_local_nonprim : NULL,
        circums ? circumsList : NULL
    );

    return additional_measurement_local_var;
end:
    if (unexp_loc_local_nonprim) {
        OpenAPI_network_area_info_free(unexp_loc_local_nonprim);
        unexp_loc_local_nonprim = NULL;
    }
    if (unexp_flow_tepsList) {
        OpenAPI_list_for_each(unexp_flow_tepsList, node) {
            OpenAPI_ip_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(unexp_flow_tepsList);
        unexp_flow_tepsList = NULL;
    }
    if (unexp_wakesList) {
        OpenAPI_list_for_each(unexp_wakesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(unexp_wakesList);
        unexp_wakesList = NULL;
    }
    if (ddos_attack_local_nonprim) {
        OpenAPI_address_list_free(ddos_attack_local_nonprim);
        ddos_attack_local_nonprim = NULL;
    }
    if (wrg_dest_local_nonprim) {
        OpenAPI_address_list_free(wrg_dest_local_nonprim);
        wrg_dest_local_nonprim = NULL;
    }
    if (circumsList) {
        OpenAPI_list_for_each(circumsList, node) {
            OpenAPI_circumstance_description_free(node->data);
        }
        OpenAPI_list_free(circumsList);
        circumsList = NULL;
    }
    return NULL;
}

OpenAPI_additional_measurement_t *OpenAPI_additional_measurement_copy(OpenAPI_additional_measurement_t *dst, OpenAPI_additional_measurement_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_additional_measurement_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_additional_measurement_convertToJSON() failed");
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

    OpenAPI_additional_measurement_free(dst);
    dst = OpenAPI_additional_measurement_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


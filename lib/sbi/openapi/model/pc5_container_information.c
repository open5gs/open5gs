
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pc5_container_information.h"

OpenAPI_pc5_container_information_t *OpenAPI_pc5_container_information_create(
    OpenAPI_list_t *coverage_info_list,
    OpenAPI_list_t *radio_parameter_set_info_list,
    OpenAPI_list_t *transmitter_info_list,
    char *time_of_first_transmission,
    char *time_of_first_reception
)
{
    OpenAPI_pc5_container_information_t *pc5_container_information_local_var = ogs_malloc(sizeof(OpenAPI_pc5_container_information_t));
    ogs_assert(pc5_container_information_local_var);

    pc5_container_information_local_var->coverage_info_list = coverage_info_list;
    pc5_container_information_local_var->radio_parameter_set_info_list = radio_parameter_set_info_list;
    pc5_container_information_local_var->transmitter_info_list = transmitter_info_list;
    pc5_container_information_local_var->time_of_first_transmission = time_of_first_transmission;
    pc5_container_information_local_var->time_of_first_reception = time_of_first_reception;

    return pc5_container_information_local_var;
}

void OpenAPI_pc5_container_information_free(OpenAPI_pc5_container_information_t *pc5_container_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pc5_container_information) {
        return;
    }
    if (pc5_container_information->coverage_info_list) {
        OpenAPI_list_for_each(pc5_container_information->coverage_info_list, node) {
            OpenAPI_coverage_info_free(node->data);
        }
        OpenAPI_list_free(pc5_container_information->coverage_info_list);
        pc5_container_information->coverage_info_list = NULL;
    }
    if (pc5_container_information->radio_parameter_set_info_list) {
        OpenAPI_list_for_each(pc5_container_information->radio_parameter_set_info_list, node) {
            OpenAPI_radio_parameter_set_info_free(node->data);
        }
        OpenAPI_list_free(pc5_container_information->radio_parameter_set_info_list);
        pc5_container_information->radio_parameter_set_info_list = NULL;
    }
    if (pc5_container_information->transmitter_info_list) {
        OpenAPI_list_for_each(pc5_container_information->transmitter_info_list, node) {
            OpenAPI_transmitter_info_free(node->data);
        }
        OpenAPI_list_free(pc5_container_information->transmitter_info_list);
        pc5_container_information->transmitter_info_list = NULL;
    }
    if (pc5_container_information->time_of_first_transmission) {
        ogs_free(pc5_container_information->time_of_first_transmission);
        pc5_container_information->time_of_first_transmission = NULL;
    }
    if (pc5_container_information->time_of_first_reception) {
        ogs_free(pc5_container_information->time_of_first_reception);
        pc5_container_information->time_of_first_reception = NULL;
    }
    ogs_free(pc5_container_information);
}

cJSON *OpenAPI_pc5_container_information_convertToJSON(OpenAPI_pc5_container_information_t *pc5_container_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pc5_container_information == NULL) {
        ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed [PC5ContainerInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pc5_container_information->coverage_info_list) {
    cJSON *coverage_info_listList = cJSON_AddArrayToObject(item, "coverageInfoList");
    if (coverage_info_listList == NULL) {
        ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed [coverage_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(pc5_container_information->coverage_info_list, node) {
        cJSON *itemLocal = OpenAPI_coverage_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed [coverage_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(coverage_info_listList, itemLocal);
    }
    }

    if (pc5_container_information->radio_parameter_set_info_list) {
    cJSON *radio_parameter_set_info_listList = cJSON_AddArrayToObject(item, "radioParameterSetInfoList");
    if (radio_parameter_set_info_listList == NULL) {
        ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed [radio_parameter_set_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(pc5_container_information->radio_parameter_set_info_list, node) {
        cJSON *itemLocal = OpenAPI_radio_parameter_set_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed [radio_parameter_set_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(radio_parameter_set_info_listList, itemLocal);
    }
    }

    if (pc5_container_information->transmitter_info_list) {
    cJSON *transmitter_info_listList = cJSON_AddArrayToObject(item, "transmitterInfoList");
    if (transmitter_info_listList == NULL) {
        ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed [transmitter_info_list]");
        goto end;
    }
    OpenAPI_list_for_each(pc5_container_information->transmitter_info_list, node) {
        cJSON *itemLocal = OpenAPI_transmitter_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed [transmitter_info_list]");
            goto end;
        }
        cJSON_AddItemToArray(transmitter_info_listList, itemLocal);
    }
    }

    if (pc5_container_information->time_of_first_transmission) {
    if (cJSON_AddStringToObject(item, "timeOfFirstTransmission", pc5_container_information->time_of_first_transmission) == NULL) {
        ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed [time_of_first_transmission]");
        goto end;
    }
    }

    if (pc5_container_information->time_of_first_reception) {
    if (cJSON_AddStringToObject(item, "timeOfFirstReception", pc5_container_information->time_of_first_reception) == NULL) {
        ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed [time_of_first_reception]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pc5_container_information_t *OpenAPI_pc5_container_information_parseFromJSON(cJSON *pc5_container_informationJSON)
{
    OpenAPI_pc5_container_information_t *pc5_container_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *coverage_info_list = NULL;
    OpenAPI_list_t *coverage_info_listList = NULL;
    cJSON *radio_parameter_set_info_list = NULL;
    OpenAPI_list_t *radio_parameter_set_info_listList = NULL;
    cJSON *transmitter_info_list = NULL;
    OpenAPI_list_t *transmitter_info_listList = NULL;
    cJSON *time_of_first_transmission = NULL;
    cJSON *time_of_first_reception = NULL;
    coverage_info_list = cJSON_GetObjectItemCaseSensitive(pc5_container_informationJSON, "coverageInfoList");
    if (coverage_info_list) {
        cJSON *coverage_info_list_local = NULL;
        if (!cJSON_IsArray(coverage_info_list)) {
            ogs_error("OpenAPI_pc5_container_information_parseFromJSON() failed [coverage_info_list]");
            goto end;
        }

        coverage_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(coverage_info_list_local, coverage_info_list) {
            if (!cJSON_IsObject(coverage_info_list_local)) {
                ogs_error("OpenAPI_pc5_container_information_parseFromJSON() failed [coverage_info_list]");
                goto end;
            }
            OpenAPI_coverage_info_t *coverage_info_listItem = OpenAPI_coverage_info_parseFromJSON(coverage_info_list_local);
            if (!coverage_info_listItem) {
                ogs_error("No coverage_info_listItem");
                goto end;
            }
            OpenAPI_list_add(coverage_info_listList, coverage_info_listItem);
        }
    }

    radio_parameter_set_info_list = cJSON_GetObjectItemCaseSensitive(pc5_container_informationJSON, "radioParameterSetInfoList");
    if (radio_parameter_set_info_list) {
        cJSON *radio_parameter_set_info_list_local = NULL;
        if (!cJSON_IsArray(radio_parameter_set_info_list)) {
            ogs_error("OpenAPI_pc5_container_information_parseFromJSON() failed [radio_parameter_set_info_list]");
            goto end;
        }

        radio_parameter_set_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(radio_parameter_set_info_list_local, radio_parameter_set_info_list) {
            if (!cJSON_IsObject(radio_parameter_set_info_list_local)) {
                ogs_error("OpenAPI_pc5_container_information_parseFromJSON() failed [radio_parameter_set_info_list]");
                goto end;
            }
            OpenAPI_radio_parameter_set_info_t *radio_parameter_set_info_listItem = OpenAPI_radio_parameter_set_info_parseFromJSON(radio_parameter_set_info_list_local);
            if (!radio_parameter_set_info_listItem) {
                ogs_error("No radio_parameter_set_info_listItem");
                goto end;
            }
            OpenAPI_list_add(radio_parameter_set_info_listList, radio_parameter_set_info_listItem);
        }
    }

    transmitter_info_list = cJSON_GetObjectItemCaseSensitive(pc5_container_informationJSON, "transmitterInfoList");
    if (transmitter_info_list) {
        cJSON *transmitter_info_list_local = NULL;
        if (!cJSON_IsArray(transmitter_info_list)) {
            ogs_error("OpenAPI_pc5_container_information_parseFromJSON() failed [transmitter_info_list]");
            goto end;
        }

        transmitter_info_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(transmitter_info_list_local, transmitter_info_list) {
            if (!cJSON_IsObject(transmitter_info_list_local)) {
                ogs_error("OpenAPI_pc5_container_information_parseFromJSON() failed [transmitter_info_list]");
                goto end;
            }
            OpenAPI_transmitter_info_t *transmitter_info_listItem = OpenAPI_transmitter_info_parseFromJSON(transmitter_info_list_local);
            if (!transmitter_info_listItem) {
                ogs_error("No transmitter_info_listItem");
                goto end;
            }
            OpenAPI_list_add(transmitter_info_listList, transmitter_info_listItem);
        }
    }

    time_of_first_transmission = cJSON_GetObjectItemCaseSensitive(pc5_container_informationJSON, "timeOfFirstTransmission");
    if (time_of_first_transmission) {
    if (!cJSON_IsString(time_of_first_transmission) && !cJSON_IsNull(time_of_first_transmission)) {
        ogs_error("OpenAPI_pc5_container_information_parseFromJSON() failed [time_of_first_transmission]");
        goto end;
    }
    }

    time_of_first_reception = cJSON_GetObjectItemCaseSensitive(pc5_container_informationJSON, "timeOfFirstReception");
    if (time_of_first_reception) {
    if (!cJSON_IsString(time_of_first_reception) && !cJSON_IsNull(time_of_first_reception)) {
        ogs_error("OpenAPI_pc5_container_information_parseFromJSON() failed [time_of_first_reception]");
        goto end;
    }
    }

    pc5_container_information_local_var = OpenAPI_pc5_container_information_create (
        coverage_info_list ? coverage_info_listList : NULL,
        radio_parameter_set_info_list ? radio_parameter_set_info_listList : NULL,
        transmitter_info_list ? transmitter_info_listList : NULL,
        time_of_first_transmission && !cJSON_IsNull(time_of_first_transmission) ? ogs_strdup(time_of_first_transmission->valuestring) : NULL,
        time_of_first_reception && !cJSON_IsNull(time_of_first_reception) ? ogs_strdup(time_of_first_reception->valuestring) : NULL
    );

    return pc5_container_information_local_var;
end:
    if (coverage_info_listList) {
        OpenAPI_list_for_each(coverage_info_listList, node) {
            OpenAPI_coverage_info_free(node->data);
        }
        OpenAPI_list_free(coverage_info_listList);
        coverage_info_listList = NULL;
    }
    if (radio_parameter_set_info_listList) {
        OpenAPI_list_for_each(radio_parameter_set_info_listList, node) {
            OpenAPI_radio_parameter_set_info_free(node->data);
        }
        OpenAPI_list_free(radio_parameter_set_info_listList);
        radio_parameter_set_info_listList = NULL;
    }
    if (transmitter_info_listList) {
        OpenAPI_list_for_each(transmitter_info_listList, node) {
            OpenAPI_transmitter_info_free(node->data);
        }
        OpenAPI_list_free(transmitter_info_listList);
        transmitter_info_listList = NULL;
    }
    return NULL;
}

OpenAPI_pc5_container_information_t *OpenAPI_pc5_container_information_copy(OpenAPI_pc5_container_information_t *dst, OpenAPI_pc5_container_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pc5_container_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pc5_container_information_convertToJSON() failed");
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

    OpenAPI_pc5_container_information_free(dst);
    dst = OpenAPI_pc5_container_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


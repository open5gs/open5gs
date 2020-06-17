
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parameter_over_uu.h"

OpenAPI_parameter_over_uu_t *OpenAPI_parameter_over_uu_create(
    char *expiry,
    OpenAPI_list_t *ser_to_pdu_sess,
    OpenAPI_list_t *ser_to_app_addr
    )
{
    OpenAPI_parameter_over_uu_t *parameter_over_uu_local_var = OpenAPI_malloc(sizeof(OpenAPI_parameter_over_uu_t));
    if (!parameter_over_uu_local_var) {
        return NULL;
    }
    parameter_over_uu_local_var->expiry = expiry;
    parameter_over_uu_local_var->ser_to_pdu_sess = ser_to_pdu_sess;
    parameter_over_uu_local_var->ser_to_app_addr = ser_to_app_addr;

    return parameter_over_uu_local_var;
}

void OpenAPI_parameter_over_uu_free(OpenAPI_parameter_over_uu_t *parameter_over_uu)
{
    if (NULL == parameter_over_uu) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(parameter_over_uu->expiry);
    OpenAPI_list_for_each(parameter_over_uu->ser_to_pdu_sess, node) {
        OpenAPI_service_to_pdu_session_free(node->data);
    }
    OpenAPI_list_free(parameter_over_uu->ser_to_pdu_sess);
    OpenAPI_list_for_each(parameter_over_uu->ser_to_app_addr, node) {
        OpenAPI_service_application_server_address_free(node->data);
    }
    OpenAPI_list_free(parameter_over_uu->ser_to_app_addr);
    ogs_free(parameter_over_uu);
}

cJSON *OpenAPI_parameter_over_uu_convertToJSON(OpenAPI_parameter_over_uu_t *parameter_over_uu)
{
    cJSON *item = NULL;

    if (parameter_over_uu == NULL) {
        ogs_error("OpenAPI_parameter_over_uu_convertToJSON() failed [ParameterOverUu]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (parameter_over_uu->expiry) {
        if (cJSON_AddStringToObject(item, "expiry", parameter_over_uu->expiry) == NULL) {
            ogs_error("OpenAPI_parameter_over_uu_convertToJSON() failed [expiry]");
            goto end;
        }
    }

    if (parameter_over_uu->ser_to_pdu_sess) {
        cJSON *ser_to_pdu_sessList = cJSON_AddArrayToObject(item, "serToPduSess");
        if (ser_to_pdu_sessList == NULL) {
            ogs_error("OpenAPI_parameter_over_uu_convertToJSON() failed [ser_to_pdu_sess]");
            goto end;
        }

        OpenAPI_lnode_t *ser_to_pdu_sess_node;
        if (parameter_over_uu->ser_to_pdu_sess) {
            OpenAPI_list_for_each(parameter_over_uu->ser_to_pdu_sess, ser_to_pdu_sess_node) {
                cJSON *itemLocal = OpenAPI_service_to_pdu_session_convertToJSON(ser_to_pdu_sess_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_parameter_over_uu_convertToJSON() failed [ser_to_pdu_sess]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_to_pdu_sessList, itemLocal);
            }
        }
    }

    if (parameter_over_uu->ser_to_app_addr) {
        cJSON *ser_to_app_addrList = cJSON_AddArrayToObject(item, "serToAppAddr");
        if (ser_to_app_addrList == NULL) {
            ogs_error("OpenAPI_parameter_over_uu_convertToJSON() failed [ser_to_app_addr]");
            goto end;
        }

        OpenAPI_lnode_t *ser_to_app_addr_node;
        if (parameter_over_uu->ser_to_app_addr) {
            OpenAPI_list_for_each(parameter_over_uu->ser_to_app_addr, ser_to_app_addr_node) {
                cJSON *itemLocal = OpenAPI_service_application_server_address_convertToJSON(ser_to_app_addr_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_parameter_over_uu_convertToJSON() failed [ser_to_app_addr]");
                    goto end;
                }
                cJSON_AddItemToArray(ser_to_app_addrList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_parameter_over_uu_t *OpenAPI_parameter_over_uu_parseFromJSON(cJSON *parameter_over_uuJSON)
{
    OpenAPI_parameter_over_uu_t *parameter_over_uu_local_var = NULL;
    cJSON *expiry = cJSON_GetObjectItemCaseSensitive(parameter_over_uuJSON, "expiry");

    if (expiry) {
        if (!cJSON_IsString(expiry)) {
            ogs_error("OpenAPI_parameter_over_uu_parseFromJSON() failed [expiry]");
            goto end;
        }
    }

    cJSON *ser_to_pdu_sess = cJSON_GetObjectItemCaseSensitive(parameter_over_uuJSON, "serToPduSess");

    OpenAPI_list_t *ser_to_pdu_sessList;
    if (ser_to_pdu_sess) {
        cJSON *ser_to_pdu_sess_local_nonprimitive;
        if (!cJSON_IsArray(ser_to_pdu_sess)) {
            ogs_error("OpenAPI_parameter_over_uu_parseFromJSON() failed [ser_to_pdu_sess]");
            goto end;
        }

        ser_to_pdu_sessList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_to_pdu_sess_local_nonprimitive, ser_to_pdu_sess ) {
            if (!cJSON_IsObject(ser_to_pdu_sess_local_nonprimitive)) {
                ogs_error("OpenAPI_parameter_over_uu_parseFromJSON() failed [ser_to_pdu_sess]");
                goto end;
            }
            OpenAPI_service_to_pdu_session_t *ser_to_pdu_sessItem = OpenAPI_service_to_pdu_session_parseFromJSON(ser_to_pdu_sess_local_nonprimitive);

            OpenAPI_list_add(ser_to_pdu_sessList, ser_to_pdu_sessItem);
        }
    }

    cJSON *ser_to_app_addr = cJSON_GetObjectItemCaseSensitive(parameter_over_uuJSON, "serToAppAddr");

    OpenAPI_list_t *ser_to_app_addrList;
    if (ser_to_app_addr) {
        cJSON *ser_to_app_addr_local_nonprimitive;
        if (!cJSON_IsArray(ser_to_app_addr)) {
            ogs_error("OpenAPI_parameter_over_uu_parseFromJSON() failed [ser_to_app_addr]");
            goto end;
        }

        ser_to_app_addrList = OpenAPI_list_create();

        cJSON_ArrayForEach(ser_to_app_addr_local_nonprimitive, ser_to_app_addr ) {
            if (!cJSON_IsObject(ser_to_app_addr_local_nonprimitive)) {
                ogs_error("OpenAPI_parameter_over_uu_parseFromJSON() failed [ser_to_app_addr]");
                goto end;
            }
            OpenAPI_service_application_server_address_t *ser_to_app_addrItem = OpenAPI_service_application_server_address_parseFromJSON(ser_to_app_addr_local_nonprimitive);

            OpenAPI_list_add(ser_to_app_addrList, ser_to_app_addrItem);
        }
    }

    parameter_over_uu_local_var = OpenAPI_parameter_over_uu_create (
        expiry ? ogs_strdup(expiry->valuestring) : NULL,
        ser_to_pdu_sess ? ser_to_pdu_sessList : NULL,
        ser_to_app_addr ? ser_to_app_addrList : NULL
        );

    return parameter_over_uu_local_var;
end:
    return NULL;
}

OpenAPI_parameter_over_uu_t *OpenAPI_parameter_over_uu_copy(OpenAPI_parameter_over_uu_t *dst, OpenAPI_parameter_over_uu_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_parameter_over_uu_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_parameter_over_uu_convertToJSON() failed");
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

    OpenAPI_parameter_over_uu_free(dst);
    dst = OpenAPI_parameter_over_uu_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


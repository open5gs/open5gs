
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "am_requested_value_rep.h"

OpenAPI_am_requested_value_rep_t *OpenAPI_am_requested_value_rep_create(
    OpenAPI_user_location_t *user_loc,
    OpenAPI_list_t* pra_statuses,
    OpenAPI_list_t *access_types,
    OpenAPI_list_t *rat_types,
    OpenAPI_list_t *allowed_snssais,
    OpenAPI_list_t *n3g_allowed_snssais
)
{
    OpenAPI_am_requested_value_rep_t *am_requested_value_rep_local_var = ogs_malloc(sizeof(OpenAPI_am_requested_value_rep_t));
    ogs_assert(am_requested_value_rep_local_var);

    am_requested_value_rep_local_var->user_loc = user_loc;
    am_requested_value_rep_local_var->pra_statuses = pra_statuses;
    am_requested_value_rep_local_var->access_types = access_types;
    am_requested_value_rep_local_var->rat_types = rat_types;
    am_requested_value_rep_local_var->allowed_snssais = allowed_snssais;
    am_requested_value_rep_local_var->n3g_allowed_snssais = n3g_allowed_snssais;

    return am_requested_value_rep_local_var;
}

void OpenAPI_am_requested_value_rep_free(OpenAPI_am_requested_value_rep_t *am_requested_value_rep)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == am_requested_value_rep) {
        return;
    }
    if (am_requested_value_rep->user_loc) {
        OpenAPI_user_location_free(am_requested_value_rep->user_loc);
        am_requested_value_rep->user_loc = NULL;
    }
    if (am_requested_value_rep->pra_statuses) {
        OpenAPI_list_for_each(am_requested_value_rep->pra_statuses, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(am_requested_value_rep->pra_statuses);
        am_requested_value_rep->pra_statuses = NULL;
    }
    if (am_requested_value_rep->access_types) {
        OpenAPI_list_free(am_requested_value_rep->access_types);
        am_requested_value_rep->access_types = NULL;
    }
    if (am_requested_value_rep->rat_types) {
        OpenAPI_list_free(am_requested_value_rep->rat_types);
        am_requested_value_rep->rat_types = NULL;
    }
    if (am_requested_value_rep->allowed_snssais) {
        OpenAPI_list_for_each(am_requested_value_rep->allowed_snssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(am_requested_value_rep->allowed_snssais);
        am_requested_value_rep->allowed_snssais = NULL;
    }
    if (am_requested_value_rep->n3g_allowed_snssais) {
        OpenAPI_list_for_each(am_requested_value_rep->n3g_allowed_snssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(am_requested_value_rep->n3g_allowed_snssais);
        am_requested_value_rep->n3g_allowed_snssais = NULL;
    }
    ogs_free(am_requested_value_rep);
}

cJSON *OpenAPI_am_requested_value_rep_convertToJSON(OpenAPI_am_requested_value_rep_t *am_requested_value_rep)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (am_requested_value_rep == NULL) {
        ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [AmRequestedValueRep]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (am_requested_value_rep->user_loc) {
    cJSON *user_loc_local_JSON = OpenAPI_user_location_convertToJSON(am_requested_value_rep->user_loc);
    if (user_loc_local_JSON == NULL) {
        ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [user_loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLoc", user_loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [user_loc]");
        goto end;
    }
    }

    if (am_requested_value_rep->pra_statuses) {
    cJSON *pra_statuses = cJSON_AddObjectToObject(item, "praStatuses");
    if (pra_statuses == NULL) {
        ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [pra_statuses]");
        goto end;
    }
    cJSON *localMapObject = pra_statuses;
    if (am_requested_value_rep->pra_statuses) {
        OpenAPI_list_for_each(am_requested_value_rep->pra_statuses, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [pra_statuses]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [pra_statuses]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (am_requested_value_rep->access_types != OpenAPI_access_type_NULL) {
    cJSON *access_typesList = cJSON_AddArrayToObject(item, "accessTypes");
    if (access_typesList == NULL) {
        ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [access_types]");
        goto end;
    }
    OpenAPI_list_for_each(am_requested_value_rep->access_types, node) {
        if (cJSON_AddStringToObject(access_typesList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [access_types]");
            goto end;
        }
    }
    }

    if (am_requested_value_rep->rat_types != OpenAPI_rat_type_NULL) {
    cJSON *rat_typesList = cJSON_AddArrayToObject(item, "ratTypes");
    if (rat_typesList == NULL) {
        ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [rat_types]");
        goto end;
    }
    OpenAPI_list_for_each(am_requested_value_rep->rat_types, node) {
        if (cJSON_AddStringToObject(rat_typesList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [rat_types]");
            goto end;
        }
    }
    }

    if (am_requested_value_rep->allowed_snssais) {
    cJSON *allowed_snssaisList = cJSON_AddArrayToObject(item, "allowedSnssais");
    if (allowed_snssaisList == NULL) {
        ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [allowed_snssais]");
        goto end;
    }
    OpenAPI_list_for_each(am_requested_value_rep->allowed_snssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [allowed_snssais]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_snssaisList, itemLocal);
    }
    }

    if (am_requested_value_rep->n3g_allowed_snssais) {
    cJSON *n3g_allowed_snssaisList = cJSON_AddArrayToObject(item, "n3gAllowedSnssais");
    if (n3g_allowed_snssaisList == NULL) {
        ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [n3g_allowed_snssais]");
        goto end;
    }
    OpenAPI_list_for_each(am_requested_value_rep->n3g_allowed_snssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed [n3g_allowed_snssais]");
            goto end;
        }
        cJSON_AddItemToArray(n3g_allowed_snssaisList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_am_requested_value_rep_t *OpenAPI_am_requested_value_rep_parseFromJSON(cJSON *am_requested_value_repJSON)
{
    OpenAPI_am_requested_value_rep_t *am_requested_value_rep_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *user_loc = NULL;
    OpenAPI_user_location_t *user_loc_local_nonprim = NULL;
    cJSON *pra_statuses = NULL;
    OpenAPI_list_t *pra_statusesList = NULL;
    cJSON *access_types = NULL;
    OpenAPI_list_t *access_typesList = NULL;
    cJSON *rat_types = NULL;
    OpenAPI_list_t *rat_typesList = NULL;
    cJSON *allowed_snssais = NULL;
    OpenAPI_list_t *allowed_snssaisList = NULL;
    cJSON *n3g_allowed_snssais = NULL;
    OpenAPI_list_t *n3g_allowed_snssaisList = NULL;
    user_loc = cJSON_GetObjectItemCaseSensitive(am_requested_value_repJSON, "userLoc");
    if (user_loc) {
    user_loc_local_nonprim = OpenAPI_user_location_parseFromJSON(user_loc);
    if (!user_loc_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [user_loc]");
        goto end;
    }
    }

    pra_statuses = cJSON_GetObjectItemCaseSensitive(am_requested_value_repJSON, "praStatuses");
    if (pra_statuses) {
        cJSON *pra_statuses_local_map = NULL;
        if (!cJSON_IsObject(pra_statuses) && !cJSON_IsNull(pra_statuses)) {
            ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [pra_statuses]");
            goto end;
        }
        if (cJSON_IsObject(pra_statuses)) {
            pra_statusesList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pra_statuses_local_map, pra_statuses) {
                cJSON *localMapObject = pra_statuses_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pra_statusesList, localMapKeyPair);
            }
        }
    }

    access_types = cJSON_GetObjectItemCaseSensitive(am_requested_value_repJSON, "accessTypes");
    if (access_types) {
        cJSON *access_types_local = NULL;
        if (!cJSON_IsArray(access_types)) {
            ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [access_types]");
            goto end;
        }

        access_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_types_local, access_types) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(access_types_local)) {
                ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [access_types]");
                goto end;
            }
            localEnum = OpenAPI_access_type_FromString(access_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"access_types\" is not supported. Ignoring it ...",
                         access_types_local->valuestring);
            } else {
                OpenAPI_list_add(access_typesList, (void *)localEnum);
            }
        }
        if (access_typesList->count == 0) {
            ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed: Expected access_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    rat_types = cJSON_GetObjectItemCaseSensitive(am_requested_value_repJSON, "ratTypes");
    if (rat_types) {
        cJSON *rat_types_local = NULL;
        if (!cJSON_IsArray(rat_types)) {
            ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [rat_types]");
            goto end;
        }

        rat_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_types_local, rat_types) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(rat_types_local)) {
                ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [rat_types]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(rat_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rat_types\" is not supported. Ignoring it ...",
                         rat_types_local->valuestring);
            } else {
                OpenAPI_list_add(rat_typesList, (void *)localEnum);
            }
        }
        if (rat_typesList->count == 0) {
            ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed: Expected rat_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    allowed_snssais = cJSON_GetObjectItemCaseSensitive(am_requested_value_repJSON, "allowedSnssais");
    if (allowed_snssais) {
        cJSON *allowed_snssais_local = NULL;
        if (!cJSON_IsArray(allowed_snssais)) {
            ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [allowed_snssais]");
            goto end;
        }

        allowed_snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_snssais_local, allowed_snssais) {
            if (!cJSON_IsObject(allowed_snssais_local)) {
                ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [allowed_snssais]");
                goto end;
            }
            OpenAPI_snssai_t *allowed_snssaisItem = OpenAPI_snssai_parseFromJSON(allowed_snssais_local);
            if (!allowed_snssaisItem) {
                ogs_error("No allowed_snssaisItem");
                goto end;
            }
            OpenAPI_list_add(allowed_snssaisList, allowed_snssaisItem);
        }
    }

    n3g_allowed_snssais = cJSON_GetObjectItemCaseSensitive(am_requested_value_repJSON, "n3gAllowedSnssais");
    if (n3g_allowed_snssais) {
        cJSON *n3g_allowed_snssais_local = NULL;
        if (!cJSON_IsArray(n3g_allowed_snssais)) {
            ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [n3g_allowed_snssais]");
            goto end;
        }

        n3g_allowed_snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(n3g_allowed_snssais_local, n3g_allowed_snssais) {
            if (!cJSON_IsObject(n3g_allowed_snssais_local)) {
                ogs_error("OpenAPI_am_requested_value_rep_parseFromJSON() failed [n3g_allowed_snssais]");
                goto end;
            }
            OpenAPI_snssai_t *n3g_allowed_snssaisItem = OpenAPI_snssai_parseFromJSON(n3g_allowed_snssais_local);
            if (!n3g_allowed_snssaisItem) {
                ogs_error("No n3g_allowed_snssaisItem");
                goto end;
            }
            OpenAPI_list_add(n3g_allowed_snssaisList, n3g_allowed_snssaisItem);
        }
    }

    am_requested_value_rep_local_var = OpenAPI_am_requested_value_rep_create (
        user_loc ? user_loc_local_nonprim : NULL,
        pra_statuses ? pra_statusesList : NULL,
        access_types ? access_typesList : NULL,
        rat_types ? rat_typesList : NULL,
        allowed_snssais ? allowed_snssaisList : NULL,
        n3g_allowed_snssais ? n3g_allowed_snssaisList : NULL
    );

    return am_requested_value_rep_local_var;
end:
    if (user_loc_local_nonprim) {
        OpenAPI_user_location_free(user_loc_local_nonprim);
        user_loc_local_nonprim = NULL;
    }
    if (pra_statusesList) {
        OpenAPI_list_for_each(pra_statusesList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pra_statusesList);
        pra_statusesList = NULL;
    }
    if (access_typesList) {
        OpenAPI_list_free(access_typesList);
        access_typesList = NULL;
    }
    if (rat_typesList) {
        OpenAPI_list_free(rat_typesList);
        rat_typesList = NULL;
    }
    if (allowed_snssaisList) {
        OpenAPI_list_for_each(allowed_snssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(allowed_snssaisList);
        allowed_snssaisList = NULL;
    }
    if (n3g_allowed_snssaisList) {
        OpenAPI_list_for_each(n3g_allowed_snssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(n3g_allowed_snssaisList);
        n3g_allowed_snssaisList = NULL;
    }
    return NULL;
}

OpenAPI_am_requested_value_rep_t *OpenAPI_am_requested_value_rep_copy(OpenAPI_am_requested_value_rep_t *dst, OpenAPI_am_requested_value_rep_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_am_requested_value_rep_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_am_requested_value_rep_convertToJSON() failed");
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

    OpenAPI_am_requested_value_rep_free(dst);
    dst = OpenAPI_am_requested_value_rep_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


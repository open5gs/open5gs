
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traffic_descriptor_components.h"

OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_create(
    OpenAPI_list_t* app_descs,
    OpenAPI_list_t *flow_descs,
    OpenAPI_list_t *domain_descs,
    OpenAPI_list_t *eth_flow_descs,
    OpenAPI_list_t *dnns,
    OpenAPI_list_t *conn_caps
)
{
    OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components_local_var = ogs_malloc(sizeof(OpenAPI_traffic_descriptor_components_t));
    ogs_assert(traffic_descriptor_components_local_var);

    traffic_descriptor_components_local_var->app_descs = app_descs;
    traffic_descriptor_components_local_var->flow_descs = flow_descs;
    traffic_descriptor_components_local_var->domain_descs = domain_descs;
    traffic_descriptor_components_local_var->eth_flow_descs = eth_flow_descs;
    traffic_descriptor_components_local_var->dnns = dnns;
    traffic_descriptor_components_local_var->conn_caps = conn_caps;

    return traffic_descriptor_components_local_var;
}

void OpenAPI_traffic_descriptor_components_free(OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traffic_descriptor_components) {
        return;
    }
    if (traffic_descriptor_components->app_descs) {
        OpenAPI_list_for_each(traffic_descriptor_components->app_descs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_app_descriptor_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(traffic_descriptor_components->app_descs);
        traffic_descriptor_components->app_descs = NULL;
    }
    if (traffic_descriptor_components->flow_descs) {
        OpenAPI_list_for_each(traffic_descriptor_components->flow_descs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_descriptor_components->flow_descs);
        traffic_descriptor_components->flow_descs = NULL;
    }
    if (traffic_descriptor_components->domain_descs) {
        OpenAPI_list_for_each(traffic_descriptor_components->domain_descs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_descriptor_components->domain_descs);
        traffic_descriptor_components->domain_descs = NULL;
    }
    if (traffic_descriptor_components->eth_flow_descs) {
        OpenAPI_list_for_each(traffic_descriptor_components->eth_flow_descs, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(traffic_descriptor_components->eth_flow_descs);
        traffic_descriptor_components->eth_flow_descs = NULL;
    }
    if (traffic_descriptor_components->dnns) {
        OpenAPI_list_for_each(traffic_descriptor_components->dnns, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(traffic_descriptor_components->dnns);
        traffic_descriptor_components->dnns = NULL;
    }
    if (traffic_descriptor_components->conn_caps) {
        OpenAPI_list_for_each(traffic_descriptor_components->conn_caps, node) {
            OpenAPI_connection_capabilities_free(node->data);
        }
        OpenAPI_list_free(traffic_descriptor_components->conn_caps);
        traffic_descriptor_components->conn_caps = NULL;
    }
    ogs_free(traffic_descriptor_components);
}

cJSON *OpenAPI_traffic_descriptor_components_convertToJSON(OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traffic_descriptor_components == NULL) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [TrafficDescriptorComponents]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (traffic_descriptor_components->app_descs) {
    cJSON *app_descs = cJSON_AddObjectToObject(item, "appDescs");
    if (app_descs == NULL) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [app_descs]");
        goto end;
    }
    cJSON *localMapObject = app_descs;
    if (traffic_descriptor_components->app_descs) {
        OpenAPI_list_for_each(traffic_descriptor_components->app_descs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [app_descs]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [app_descs]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_app_descriptor_1_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (traffic_descriptor_components->flow_descs) {
    cJSON *flow_descsList = cJSON_AddArrayToObject(item, "flowDescs");
    if (flow_descsList == NULL) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [flow_descs]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_descriptor_components->flow_descs, node) {
        if (cJSON_AddStringToObject(flow_descsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [flow_descs]");
            goto end;
        }
    }
    }

    if (traffic_descriptor_components->domain_descs) {
    cJSON *domain_descsList = cJSON_AddArrayToObject(item, "domainDescs");
    if (domain_descsList == NULL) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [domain_descs]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_descriptor_components->domain_descs, node) {
        if (cJSON_AddStringToObject(domain_descsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [domain_descs]");
            goto end;
        }
    }
    }

    if (traffic_descriptor_components->eth_flow_descs) {
    cJSON *eth_flow_descsList = cJSON_AddArrayToObject(item, "ethFlowDescs");
    if (eth_flow_descsList == NULL) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [eth_flow_descs]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_descriptor_components->eth_flow_descs, node) {
        cJSON *itemLocal = OpenAPI_eth_flow_description_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [eth_flow_descs]");
            goto end;
        }
        cJSON_AddItemToArray(eth_flow_descsList, itemLocal);
    }
    }

    if (traffic_descriptor_components->dnns) {
    cJSON *dnnsList = cJSON_AddArrayToObject(item, "dnns");
    if (dnnsList == NULL) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [dnns]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_descriptor_components->dnns, node) {
        if (cJSON_AddStringToObject(dnnsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [dnns]");
            goto end;
        }
    }
    }

    if (traffic_descriptor_components->conn_caps) {
    cJSON *conn_capsList = cJSON_AddArrayToObject(item, "connCaps");
    if (conn_capsList == NULL) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [conn_caps]");
        goto end;
    }
    OpenAPI_list_for_each(traffic_descriptor_components->conn_caps, node) {
        cJSON *itemLocal = OpenAPI_connection_capabilities_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed [conn_caps]");
            goto end;
        }
        cJSON_AddItemToArray(conn_capsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_parseFromJSON(cJSON *traffic_descriptor_componentsJSON)
{
    OpenAPI_traffic_descriptor_components_t *traffic_descriptor_components_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_descs = NULL;
    OpenAPI_list_t *app_descsList = NULL;
    cJSON *flow_descs = NULL;
    OpenAPI_list_t *flow_descsList = NULL;
    cJSON *domain_descs = NULL;
    OpenAPI_list_t *domain_descsList = NULL;
    cJSON *eth_flow_descs = NULL;
    OpenAPI_list_t *eth_flow_descsList = NULL;
    cJSON *dnns = NULL;
    OpenAPI_list_t *dnnsList = NULL;
    cJSON *conn_caps = NULL;
    OpenAPI_list_t *conn_capsList = NULL;
    app_descs = cJSON_GetObjectItemCaseSensitive(traffic_descriptor_componentsJSON, "appDescs");
    if (app_descs) {
        cJSON *app_descs_local_map = NULL;
        if (!cJSON_IsObject(app_descs) && !cJSON_IsNull(app_descs)) {
            ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [app_descs]");
            goto end;
        }
        if (cJSON_IsObject(app_descs)) {
            app_descsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(app_descs_local_map, app_descs) {
                cJSON *localMapObject = app_descs_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_app_descriptor_1_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(app_descsList, localMapKeyPair);
            }
        }
    }

    flow_descs = cJSON_GetObjectItemCaseSensitive(traffic_descriptor_componentsJSON, "flowDescs");
    if (flow_descs) {
        cJSON *flow_descs_local = NULL;
        if (!cJSON_IsArray(flow_descs)) {
            ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [flow_descs]");
            goto end;
        }

        flow_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flow_descs_local, flow_descs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(flow_descs_local)) {
                ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [flow_descs]");
                goto end;
            }
            OpenAPI_list_add(flow_descsList, ogs_strdup(flow_descs_local->valuestring));
        }
    }

    domain_descs = cJSON_GetObjectItemCaseSensitive(traffic_descriptor_componentsJSON, "domainDescs");
    if (domain_descs) {
        cJSON *domain_descs_local = NULL;
        if (!cJSON_IsArray(domain_descs)) {
            ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [domain_descs]");
            goto end;
        }

        domain_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(domain_descs_local, domain_descs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(domain_descs_local)) {
                ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [domain_descs]");
                goto end;
            }
            OpenAPI_list_add(domain_descsList, ogs_strdup(domain_descs_local->valuestring));
        }
    }

    eth_flow_descs = cJSON_GetObjectItemCaseSensitive(traffic_descriptor_componentsJSON, "ethFlowDescs");
    if (eth_flow_descs) {
        cJSON *eth_flow_descs_local = NULL;
        if (!cJSON_IsArray(eth_flow_descs)) {
            ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [eth_flow_descs]");
            goto end;
        }

        eth_flow_descsList = OpenAPI_list_create();

        cJSON_ArrayForEach(eth_flow_descs_local, eth_flow_descs) {
            if (!cJSON_IsObject(eth_flow_descs_local)) {
                ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [eth_flow_descs]");
                goto end;
            }
            OpenAPI_eth_flow_description_t *eth_flow_descsItem = OpenAPI_eth_flow_description_parseFromJSON(eth_flow_descs_local);
            if (!eth_flow_descsItem) {
                ogs_error("No eth_flow_descsItem");
                goto end;
            }
            OpenAPI_list_add(eth_flow_descsList, eth_flow_descsItem);
        }
    }

    dnns = cJSON_GetObjectItemCaseSensitive(traffic_descriptor_componentsJSON, "dnns");
    if (dnns) {
        cJSON *dnns_local = NULL;
        if (!cJSON_IsArray(dnns)) {
            ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [dnns]");
            goto end;
        }

        dnnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnns_local, dnns) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnns_local)) {
                ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [dnns]");
                goto end;
            }
            OpenAPI_list_add(dnnsList, ogs_strdup(dnns_local->valuestring));
        }
    }

    conn_caps = cJSON_GetObjectItemCaseSensitive(traffic_descriptor_componentsJSON, "connCaps");
    if (conn_caps) {
        cJSON *conn_caps_local = NULL;
        if (!cJSON_IsArray(conn_caps)) {
            ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [conn_caps]");
            goto end;
        }

        conn_capsList = OpenAPI_list_create();

        cJSON_ArrayForEach(conn_caps_local, conn_caps) {
            if (!cJSON_IsObject(conn_caps_local)) {
                ogs_error("OpenAPI_traffic_descriptor_components_parseFromJSON() failed [conn_caps]");
                goto end;
            }
            OpenAPI_connection_capabilities_t *conn_capsItem = OpenAPI_connection_capabilities_parseFromJSON(conn_caps_local);
            if (!conn_capsItem) {
                ogs_error("No conn_capsItem");
                goto end;
            }
            OpenAPI_list_add(conn_capsList, conn_capsItem);
        }
    }

    traffic_descriptor_components_local_var = OpenAPI_traffic_descriptor_components_create (
        app_descs ? app_descsList : NULL,
        flow_descs ? flow_descsList : NULL,
        domain_descs ? domain_descsList : NULL,
        eth_flow_descs ? eth_flow_descsList : NULL,
        dnns ? dnnsList : NULL,
        conn_caps ? conn_capsList : NULL
    );

    return traffic_descriptor_components_local_var;
end:
    if (app_descsList) {
        OpenAPI_list_for_each(app_descsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_app_descriptor_1_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(app_descsList);
        app_descsList = NULL;
    }
    if (flow_descsList) {
        OpenAPI_list_for_each(flow_descsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(flow_descsList);
        flow_descsList = NULL;
    }
    if (domain_descsList) {
        OpenAPI_list_for_each(domain_descsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(domain_descsList);
        domain_descsList = NULL;
    }
    if (eth_flow_descsList) {
        OpenAPI_list_for_each(eth_flow_descsList, node) {
            OpenAPI_eth_flow_description_free(node->data);
        }
        OpenAPI_list_free(eth_flow_descsList);
        eth_flow_descsList = NULL;
    }
    if (dnnsList) {
        OpenAPI_list_for_each(dnnsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnnsList);
        dnnsList = NULL;
    }
    if (conn_capsList) {
        OpenAPI_list_for_each(conn_capsList, node) {
            OpenAPI_connection_capabilities_free(node->data);
        }
        OpenAPI_list_free(conn_capsList);
        conn_capsList = NULL;
    }
    return NULL;
}

OpenAPI_traffic_descriptor_components_t *OpenAPI_traffic_descriptor_components_copy(OpenAPI_traffic_descriptor_components_t *dst, OpenAPI_traffic_descriptor_components_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traffic_descriptor_components_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traffic_descriptor_components_convertToJSON() failed");
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

    OpenAPI_traffic_descriptor_components_free(dst);
    dst = OpenAPI_traffic_descriptor_components_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}


#define TRACE_MODULE _yaml_helper

#include "yaml_helper.h"

const char *YAML_MAPPING_KEY(
        yaml_document_t *document, yaml_node_pair_t *node_pair)
{
    yaml_node_t *node = NULL;

    node = yaml_document_get_node(document, node_pair->key);
    d_assert(node, return NULL,);
    d_assert(node->type == YAML_SCALAR_NODE, return NULL,);

    return (const char *)node->data.scalar.value;
}

const char *YAML_MAPPING_VALUE(
        yaml_document_t *document, yaml_node_pair_t *node_pair)
{
    yaml_node_t *node = NULL;

    node = yaml_document_get_node(document, node_pair->value);
    d_assert(node, return NULL,);
    d_assert(node->type == YAML_SCALAR_NODE, return NULL,);

    return (const char *)node->data.scalar.value;
}

int YAML_MAPPING_BOOL(
        yaml_document_t *document, yaml_node_pair_t *node_pair)
{
    yaml_node_t *node = NULL;
    const char *v = NULL;

    node = yaml_document_get_node(document, node_pair->value);
    d_assert(node, return 0,);
    d_assert(node->type == YAML_SCALAR_NODE, return 0,);

    v = (const char *)node->data.scalar.value;
    d_assert(v, return 0,);
    if (!strcasecmp(v, "true") || !strcasecmp(v, "yes")) return 1;
    if (atoi(v)) return 1;

    return 0;
}

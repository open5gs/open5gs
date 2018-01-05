#define TRACE_MODULE _yaml_helper

#include "yaml_helper.h"

void yaml_iter_init(yaml_iter_t *iter, yaml_document_t *document)
{
    d_assert(iter, return,);
    d_assert(document, return,);

    memset(iter, 0, sizeof(yaml_iter_t));

    iter->document = document;
    iter->node = yaml_document_get_root_node(document);

    d_assert(iter->node, return,);
    if (iter->node->type == YAML_MAPPING_NODE)
        iter->pair = iter->node->data.mapping.pairs.start - 1;
    else if (iter->node->type == YAML_SEQUENCE_NODE)
        iter->item = iter->node->data.sequence.items.start - 1;
}

int yaml_iter_next(yaml_iter_t *iter)
{
    d_assert(iter, return 0,);
    d_assert(iter->document, return 0,);
    d_assert(iter->node, return 0,);

    if (iter->node->type == YAML_MAPPING_NODE)
    {
        if (iter->pair)
        {
            iter->pair++;
            if (iter->pair < iter->node->data.mapping.pairs.top)
                return 1;
        }

    }
    else if (iter->node->type == YAML_SEQUENCE_NODE)
    {
        if (iter->item)
        {
            iter->item++;
            if (iter->item < iter->node->data.sequence.items.top)
                return 1;
        }
    }

    return 0;
}

void yaml_iter_recurse(yaml_iter_t *parent, yaml_iter_t *iter)
{
    d_assert(parent, return,);
    d_assert(parent->document, return,);
    d_assert(parent->node, return,);
    d_assert(iter, return,);

    memset(iter, 0, sizeof(yaml_iter_t));

    iter->document = parent->document;

    if (parent->node->type == YAML_MAPPING_NODE)
    {
        d_assert(parent->pair, return,);
        iter->node = yaml_document_get_node(
                parent->document, parent->pair->value);
        d_assert(iter->node, return,);
        if (iter->node->type == YAML_MAPPING_NODE)
            iter->pair = iter->node->data.mapping.pairs.start - 1;
        else if (iter->node->type == YAML_SEQUENCE_NODE)
            iter->item = iter->node->data.sequence.items.start - 1;
    }
    else if (parent->node->type == YAML_SEQUENCE_NODE)
    {
        d_assert(parent->item, return,);
        iter->node = yaml_document_get_node(parent->document, *parent->item);
        d_assert(iter->node, return,);
        if (iter->node->type == YAML_MAPPING_NODE)
            iter->pair = iter->node->data.mapping.pairs.start - 1;
        else if (iter->node->type == YAML_SEQUENCE_NODE)
            iter->item = iter->node->data.sequence.items.start - 1;
    }
    else
        d_assert(0, return,);
}

int yaml_iter_type(yaml_iter_t *iter)
{
    d_assert(iter, return 0,);
    d_assert(iter->node, return 0,);

    return iter->node->type;
}


const char *yaml_iter_key(yaml_iter_t *iter)
{
    yaml_node_t *node = NULL;

    d_assert(iter, return NULL,);
    d_assert(iter->document, return NULL,);
    d_assert(iter->node, return NULL,);

    if (iter->node->type == YAML_MAPPING_NODE)
    {
        d_assert(iter->pair, return NULL,);
        node = yaml_document_get_node(iter->document, iter->pair->key);
        d_assert(node, return NULL,);
        d_assert(node->type == YAML_SCALAR_NODE, return NULL,);

        return (const char *)node->data.scalar.value;
    }
    else if (iter->node->type == YAML_SEQUENCE_NODE)
    {
        d_assert(iter->item, return NULL,);
        node = yaml_document_get_node(iter->document, *iter->item);
        d_assert(node, return NULL,);
        d_assert(node->type == YAML_SCALAR_NODE, return NULL,);

        return (const char *)node->data.scalar.value;
    }
    else
        d_assert(0, return 0,);
}

const char *yaml_iter_value(yaml_iter_t *iter)
{
    d_assert(iter, return NULL,);
    d_assert(iter->document, return NULL,);
    d_assert(iter->node, return NULL,);

    if (iter->node->type == YAML_SCALAR_NODE)
    {
        return (const char *)iter->node->data.scalar.value;
    }
    else if (iter->node->type == YAML_MAPPING_NODE)
    {
        yaml_node_t *node = NULL;

        d_assert(iter->pair, return NULL,);
        node = yaml_document_get_node(iter->document, iter->pair->value);
        d_assert(node, return NULL,);
        d_assert(node->type == YAML_SCALAR_NODE, return NULL,);

        return (const char *)node->data.scalar.value;
    }
    else if (iter->node->type == YAML_SEQUENCE_NODE)
    {
        yaml_node_t *node = NULL;

        d_assert(iter->item, return NULL,);
        node = yaml_document_get_node(iter->document, *iter->item);
        d_assert(node, return NULL,);
        d_assert(node->type == YAML_SCALAR_NODE, return NULL,);

        return (const char *)node->data.scalar.value;
    }
    else
        d_assert(0, return NULL,);
}

int yaml_iter_bool(yaml_iter_t *iter)
{
    const char *v = yaml_iter_value(iter);
    if (v)
    {
        if (!strcasecmp(v, "true") || !strcasecmp(v, "yes")) return 1;
        if (atoi(v)) return 1;
    }

    return 0;
}

#ifndef __YAML_HELPER_H__
#define __YAML_HELPER_H__

#include "core_debug.h"

#include <yaml.h>

typedef struct {
    yaml_document_t *document;
    yaml_node_t *node;
    yaml_node_pair_t *pair;
    yaml_node_item_t *item;
} yaml_iter_t;

CORE_DECLARE(void) yaml_iter_init(yaml_iter_t *iter, yaml_document_t *document);
CORE_DECLARE(int) yaml_iter_next(yaml_iter_t *iter);
CORE_DECLARE(void) yaml_iter_recurse(yaml_iter_t *parent, yaml_iter_t *iter);

CORE_DECLARE(int) yaml_iter_type(yaml_iter_t *iter);
CORE_DECLARE(const char *) yaml_iter_key(yaml_iter_t *iter);
CORE_DECLARE(const char *) yaml_iter_value(yaml_iter_t *iter);
CORE_DECLARE(int) yaml_iter_bool(yaml_iter_t *iter);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __YAML_HELPER_H__ */

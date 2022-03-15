/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined(OGS_APP_INSIDE) && !defined(OGS_APP_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_YAML_H
#define OGS_YAML_H

#include <yaml.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    yaml_document_t *document;
    yaml_node_t *node;
    yaml_node_pair_t *pair;
    yaml_node_item_t *item;
} ogs_yaml_iter_t;

void ogs_yaml_iter_init(ogs_yaml_iter_t *iter, yaml_document_t *document);
int ogs_yaml_iter_next(ogs_yaml_iter_t *iter);
void ogs_yaml_iter_recurse(ogs_yaml_iter_t *parent, ogs_yaml_iter_t *iter);

int ogs_yaml_iter_type(ogs_yaml_iter_t *iter);
const char *ogs_yaml_iter_key(ogs_yaml_iter_t *iter);
const char *ogs_yaml_iter_value(ogs_yaml_iter_t *iter);
int ogs_yaml_iter_bool(ogs_yaml_iter_t *iter);

#ifdef __cplusplus
}
#endif

#endif /* OGS_YAML_H */

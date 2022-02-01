/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#include "ogs-core.h"

ogs_tlv_desc_t ogs_tlv_desc_more1 = {
    OGS_TLV_MORE, "More", 0, 1, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more2 = {
    OGS_TLV_MORE, "More", 0, 2, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more3 = {
    OGS_TLV_MORE, "More", 0, 3, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more4 = {
    OGS_TLV_MORE, "More", 0, 4, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more5 = {
    OGS_TLV_MORE, "More", 0, 5, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more6 = {
    OGS_TLV_MORE, "More", 0, 6, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more7 = {
    OGS_TLV_MORE, "More", 0, 7, 0, 0, { NULL } };
ogs_tlv_desc_t ogs_tlv_desc_more8 = {
    OGS_TLV_MORE, "More", 0, 8, 0, 0, { NULL } };

/* Return specific TLV mode based on its TLV description type and the msg
 * provided mode (used to know the type length) */
static uint8_t tlv_ctype2mode(ogs_tlv_type_e ctype, uint8_t msg_mode)
{
    switch(ctype) {
    case OGS_TLV_UINT8:
    case OGS_TLV_UINT16:
    case OGS_TLV_UINT24:
    case OGS_TLV_UINT32:
    case OGS_TLV_INT8:
    case OGS_TLV_INT16:
    case OGS_TLV_INT24:
    case OGS_TLV_INT32:
    case OGS_TLV_FIXED_STR:
    case OGS_TLV_VAR_STR:
    case OGS_TLV_NULL:
    case OGS_TLV_MORE:
    case OGS_TLV_COMPOUND:
    case OGS_TLV_MESSAGE:
        return msg_mode;
    case OGS_TV_UINT8:
    case OGS_TV_UINT16:
    case OGS_TV_UINT24:
    case OGS_TV_UINT32:
    case OGS_TV_INT8:
    case OGS_TV_INT16:
    case OGS_TV_INT24:
    case OGS_TV_INT32:
    case OGS_TV_FIXED_STR:
    case OGS_TV_VAR_STR:
    case OGS_TV_NULL:
    case OGS_TV_MORE:
        if (msg_mode == OGS_TLV_MODE_T2_L2)
            return OGS_TLV_MODE_T1; /* Here return OGS_TLV_MODE_T2 if ever existing */
        else
            return OGS_TLV_MODE_T1;
    default:
        ogs_assert_if_reached();
        break;
    }
}

static ogs_tlv_t *tlv_add_leaf(
        ogs_tlv_t *parent_tlv, ogs_tlv_t *tlv, ogs_tlv_desc_t *desc, void *msg, uint8_t msg_mode)
{
    uint8_t tlv_mode = tlv_ctype2mode(desc->ctype, msg_mode);

    switch (desc->ctype) {
    case OGS_TLV_UINT8:
    case OGS_TLV_INT8:
    case OGS_TV_UINT8:
    case OGS_TV_INT8:
    {
        ogs_tlv_uint8_t *v = (ogs_tlv_uint8_t *)msg;
        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, tlv_mode,
                    desc->type, 1, desc->instance, &v->u8);
        else
            tlv = ogs_tlv_add(tlv, tlv_mode, desc->type, 1, desc->instance, &v->u8);
        ogs_expect_or_return_val(tlv, NULL);
        break;
    }
    case OGS_TLV_UINT16:
    case OGS_TLV_INT16:
    case OGS_TV_UINT16:
    case OGS_TV_INT16:
    {
        ogs_tlv_uint16_t *v = (ogs_tlv_uint16_t *)msg;

        v->u16 = htobe16(v->u16);

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, tlv_mode,
                    desc->type, 2, desc->instance, &v->u16);
        else
            tlv = ogs_tlv_add(tlv, tlv_mode, desc->type, 2, desc->instance, &v->u16);
        ogs_expect_or_return_val(tlv, NULL);
        break;
    }
    case OGS_TLV_UINT24:
    case OGS_TLV_INT24:
    case OGS_TV_UINT24:
    case OGS_TV_INT24:
    {
        ogs_tlv_uint24_t *v = (ogs_tlv_uint24_t *)msg;

        v->u24 = v->u24 << 8;
        v->u24 = htobe32(v->u24);

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, tlv_mode,
                    desc->type, 3, desc->instance, &v->u24);
        else
            tlv = ogs_tlv_add(tlv, tlv_mode, desc->type, 3, desc->instance, &v->u24);
        ogs_expect_or_return_val(tlv, NULL);
        break;
    }
    case OGS_TLV_UINT32:
    case OGS_TLV_INT32:
    case OGS_TV_UINT32:
    case OGS_TV_INT32:
    {
        ogs_tlv_uint32_t *v = (ogs_tlv_uint32_t *)msg;

        v->u32 = htobe32(v->u32);

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, tlv_mode,
                    desc->type, 4, desc->instance, &v->u32);
        else
            tlv = ogs_tlv_add(tlv, tlv_mode,
                    desc->type, 4, desc->instance, &v->u32);
        ogs_expect_or_return_val(tlv, NULL);
        break;
    }
    case OGS_TLV_FIXED_STR:
    case OGS_TV_FIXED_STR:
    {
        ogs_tlv_octet_t *v = (ogs_tlv_octet_t *)msg;

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, tlv_mode,
                    desc->type, desc->length, desc->instance, v->data);
        else
            tlv = ogs_tlv_add(tlv, tlv_mode,
                    desc->type, desc->length, desc->instance, v->data);
        ogs_expect_or_return_val(tlv, NULL);
        break;
    }
    case OGS_TLV_VAR_STR:
    {
        ogs_tlv_octet_t *v = (ogs_tlv_octet_t *)msg;

        if (v->len == 0) {
            ogs_error("No TLV length - [%s] T:%d I:%d (vsz=%d)",
                    desc->name, desc->type, desc->instance, desc->vsize);
            ogs_expect_or_return_val(0, NULL);
        }

        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, tlv_mode,
                    desc->type, v->len, desc->instance, v->data);
        else
            tlv = ogs_tlv_add(tlv, tlv_mode,
                    desc->type, v->len, desc->instance, v->data);
        ogs_expect_or_return_val(tlv, NULL);
        break;
    }
    case OGS_TLV_NULL:
    case OGS_TV_NULL:
    {
        if (parent_tlv)
            tlv = ogs_tlv_embed(parent_tlv, tlv_mode,
                    desc->type, 0, desc->instance, NULL);
        else
            tlv = ogs_tlv_add(tlv, tlv_mode,
                    desc->type, 0, desc->instance, NULL);
        ogs_expect_or_return_val(tlv, NULL);
        break;
    }
    default:
        ogs_expect_or_return_val(0, NULL);
        break;
    }

    return tlv;
}

static uint32_t tlv_add_compound(ogs_tlv_t **root, ogs_tlv_t *parent_tlv,
        ogs_tlv_desc_t *parent_desc, void *msg, int depth, uint8_t mode)
{
    ogs_tlv_presence_t *presence_p;
    ogs_tlv_desc_t *desc = NULL, *next_desc = NULL;
    ogs_tlv_t *tlv = NULL, *emb_tlv = NULL;
    uint8_t *p = msg;
    uint32_t offset = 0, count = 0;
    int i, j, r;
    char indent[17] = "                "; /* 16 spaces */

    ogs_assert(root);
    ogs_assert(parent_desc);
    ogs_assert(msg);

    ogs_assert(depth <= 8);
    indent[depth*2] = 0;

    *root = NULL;

    for (i = 0, desc = parent_desc->child_descs[i]; desc != NULL;
            i++, desc = parent_desc->child_descs[i]) {
        next_desc = parent_desc->child_descs[i+1];
        if (next_desc != NULL && next_desc->ctype == OGS_TLV_MORE) {
            int offset2 = offset;
            for (j = 0; j < next_desc->length; j++) {
                presence_p = (ogs_tlv_presence_t *)(p + offset2);

                if (*presence_p == 0)
                    break;

                if (desc->ctype == OGS_TLV_COMPOUND) {
                    ogs_trace("BUILD %sC#%d [%s] T:%d I:%d (vsz=%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->instance,
                            desc->vsize, p + offset2);

                    if (parent_tlv)
                        tlv = ogs_tlv_embed(parent_tlv, tlv_ctype2mode(desc->ctype, mode),
                                desc->type, 0, desc->instance, NULL);
                    else
                        tlv = ogs_tlv_add(tlv, tlv_ctype2mode(desc->ctype, mode),
                                desc->type, 0, desc->instance, NULL);

                    r = tlv_add_compound(&emb_tlv, tlv, desc,
                            p + offset2 + sizeof(ogs_tlv_presence_t),
                            depth + 1, mode);
                    ogs_expect_or_return_val(r > 0 && emb_tlv, 0);
                    count += 1 + r;
                } else {
                    ogs_trace("BUILD %sL#%d [%s] T:%d L:%d I:%d "
                            "(cls:%d vsz:%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->length,
                            desc->instance, desc->ctype, desc->vsize,
                            p + offset2);

                    tlv = tlv_add_leaf(parent_tlv, tlv, desc, p + offset2, mode);
                    ogs_expect_or_return_val(tlv, 0);
                    count++;
                }

                if (*root == NULL)
                    *root = tlv;

                offset2 += desc->vsize;
            }
            offset += desc->vsize * next_desc->length;
            i++;
        } else {
            presence_p = (ogs_tlv_presence_t *)(p + offset);

            if (*presence_p) {
                if (desc->ctype == OGS_TLV_COMPOUND) {
                    ogs_trace("BUILD %sC#%d [%s] T:%d I:%d (vsz=%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->instance,
                            desc->vsize, p + offset);

                    if (parent_tlv)
                        tlv = ogs_tlv_embed(parent_tlv, tlv_ctype2mode(desc->ctype, mode),
                                desc->type, 0, desc->instance, NULL);
                    else
                        tlv = ogs_tlv_add(tlv, tlv_ctype2mode(desc->ctype, mode),
                                desc->type, 0, desc->instance, NULL);

                    r = tlv_add_compound(&emb_tlv, tlv, desc,
                            p + offset + sizeof(ogs_tlv_presence_t),
                            depth + 1, mode);
                    ogs_expect_or_return_val(r > 0 && emb_tlv, 0);
                    count += 1 + r;
                } else {
                    ogs_trace("BUILD %sL#%d [%s] T:%d L:%d I:%d "
                            "(cls:%d vsz:%d) off:%p ",
                            indent, i, desc->name, desc->type, desc->length,
                            desc->instance, desc->ctype, desc->vsize,
                            p + offset);

                    tlv = tlv_add_leaf(parent_tlv, tlv, desc, p + offset, mode);
                    ogs_expect_or_return_val(tlv, 0);
                    count++;
                }

                if (*root == NULL)
                    *root = tlv;
            }
            offset += desc->vsize;
        }
    }

    return count;
}

ogs_pkbuf_t *ogs_tlv_build_msg(ogs_tlv_desc_t *desc, void *msg, int mode)
{
    ogs_tlv_t *root = NULL;
    uint32_t r, length, rendlen;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(desc);
    ogs_assert(msg);

    ogs_assert(desc->ctype == OGS_TLV_MESSAGE);

    if (desc->child_descs[0]) {
        r = tlv_add_compound(&root, NULL, desc, msg, 0, mode);
        ogs_expect_or_return_val(r > 0 && root, NULL);

        length = ogs_tlv_calc_length(root);
    } else {
        length = 0;
    }
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_TLV_MAX_HEADROOM+length);
    ogs_expect_or_return_val(pkbuf, NULL);
    ogs_pkbuf_reserve(pkbuf, OGS_TLV_MAX_HEADROOM);
    ogs_pkbuf_put(pkbuf, length);

    if (desc->child_descs[0]) {
        rendlen = ogs_tlv_render(root, pkbuf->data, length);
        ogs_expect_or_return_val(rendlen == length, NULL);

        ogs_tlv_free_all(root);
    }

    return pkbuf;
}

static ogs_tlv_desc_t* tlv_find_desc_by_type_inst(uint8_t *desc_index,
        uint32_t *tlv_offset, ogs_tlv_desc_t *parent_desc, uint16_t match_type, uint8_t match_instance, uint8_t match_type_pos)
{
    ogs_tlv_desc_t *prev_desc = NULL, *desc = NULL;
    int i, offset = 0;
    unsigned match_i = 0;

    ogs_assert(parent_desc);

    for (i = 0, desc = parent_desc->child_descs[i]; desc != NULL;
            i++, desc = parent_desc->child_descs[i]) {
        if (desc->type == match_type && desc->instance == match_instance) {
            if (match_i == match_type_pos) {
                *desc_index = i;
                *tlv_offset = offset;
                break;
            }
            match_i++;
        }

        if (desc->ctype == OGS_TLV_MORE) {
            ogs_assert(prev_desc && prev_desc->ctype != OGS_TLV_MORE);
            offset += prev_desc->vsize * (desc->length - 1);
        } else {
            offset += desc->vsize;
        }

        prev_desc = desc;
    }

    return desc;
}

static int tlv_parse_leaf(void *msg, ogs_tlv_desc_t *desc, ogs_tlv_t *tlv)
{
    ogs_assert(msg);
    ogs_assert(desc);
    ogs_assert(tlv);

    switch (desc->ctype) {
    case OGS_TV_UINT8:
    case OGS_TV_INT8:
    case OGS_TLV_UINT8:
    case OGS_TLV_INT8:
    {
        ogs_tlv_uint8_t *v = (ogs_tlv_uint8_t *)msg;

        if (tlv->length != 1)
        {
            ogs_error("Invalid TLV length %d. It should be 1", tlv->length);
            return OGS_ERROR;
        }
        v->u8 = *(uint8_t*)(tlv->value);
        break;
    }
    case OGS_TV_UINT16:
    case OGS_TV_INT16:
    case OGS_TLV_UINT16:
    case OGS_TLV_INT16:
    {
        ogs_tlv_uint16_t *v = (ogs_tlv_uint16_t *)msg;

        if (tlv->length != 2)
        {
            ogs_error("Invalid TLV length %d. It should be 2", tlv->length);
            return OGS_ERROR;
        }
        v->u16 = ((((uint8_t*)tlv->value)[0]<< 8)&0xff00) |
               ((((uint8_t*)tlv->value)[1]    )&0x00ff);
        break;
    }
    case OGS_TV_UINT24:
    case OGS_TV_INT24:
    case OGS_TLV_UINT24:
    case OGS_TLV_INT24:
    {
        ogs_tlv_uint24_t *v = (ogs_tlv_uint24_t *)msg;

        if (tlv->length != 3)
        {
            ogs_error("Invalid TLV length %d. It should be 3", tlv->length);
            return OGS_ERROR;
        }
        v->u24 = ((((uint8_t*)tlv->value)[0]<<16)&0x00ff0000) |
               ((((uint8_t*)tlv->value)[1]<< 8)&0x0000ff00) |
               ((((uint8_t*)tlv->value)[2]    )&0x000000ff);
        break;
    }
    case OGS_TV_UINT32:
    case OGS_TV_INT32:
    case OGS_TLV_UINT32:
    case OGS_TLV_INT32:
    {
        ogs_tlv_uint32_t *v = (ogs_tlv_uint32_t *)msg;

        if (tlv->length != 4)
        {
            ogs_error("Invalid TLV length %d. It should be 4", tlv->length);
            return OGS_ERROR;
        }
        v->u32 = ((((uint8_t*)tlv->value)[0]<<24)&0xff000000) |
               ((((uint8_t*)tlv->value)[1]<<16)&0x00ff0000) |
               ((((uint8_t*)tlv->value)[2]<< 8)&0x0000ff00) |
               ((((uint8_t*)tlv->value)[3]    )&0x000000ff);
        break;
    }
    case OGS_TV_FIXED_STR:
    case OGS_TLV_FIXED_STR:
    {
        ogs_tlv_octet_t *v = (ogs_tlv_octet_t *)msg;

        if (tlv->length != desc->length)
        {
            ogs_error("Invalid TLV length %d. It should be %d",
                    tlv->length, desc->length);
            return OGS_ERROR;
        }

        v->data = tlv->value;
        v->len = tlv->length;
        break;
    }
    case OGS_TLV_VAR_STR:
    {
        ogs_tlv_octet_t *v = (ogs_tlv_octet_t *)msg;

        v->data = tlv->value;
        v->len = tlv->length;
        break;
    }
    case OGS_TV_NULL:
    case OGS_TLV_NULL:
    {
        if (tlv->length != 0) {
            ogs_error("Invalid TLV length %d. It should be 0", tlv->length);
            return OGS_ERROR;
        }
        break;
    }
    default:
        ogs_assert_if_reached();
        break;
    }

    return OGS_OK;
}

/* rbtree structure used to keep track count of TLVs with given <type,instance>
 * while parsing. This is used to link it to the matching nth field in a
 * ogs_tlv_desc_t struct */
typedef struct tlv_count_node_s {
    ogs_rbnode_t node;
    uint32_t key; /* type<<8 + instance */
    unsigned count;
} tlv_count_node_t;

static tlv_count_node_t *tlv_count_node_find(ogs_rbtree_t *tree, tlv_count_node_t *count_node_arr, unsigned count_node_arr_len,
        unsigned *count_alloc_next, uint16_t type, uint8_t instance)
{
    ogs_rbnode_t **new = NULL;
    ogs_rbnode_t *parent = NULL;
    tlv_count_node_t *this;

    uint64_t key = (((uint32_t)type)<<8) | instance;

    new = &tree->root;
    while (*new) {
        this = ogs_rb_entry(*new, tlv_count_node_t, node);
        parent = *new;
        if (key < this->key) {
            new = &(*new)->left;
        } else if (key > this->key) {
            new = &(*new)->right;
        } else { /* found entry: */
            return this;
        }
    }

    /* No entry, need to add one: */
    if (*count_alloc_next == count_node_arr_len) {
        ogs_error("This TLV has to many entries, can't parse");
        return NULL;
    }
    this = &count_node_arr[(*count_alloc_next)++];
    this->key = key;
    this->count = 0;
    ogs_rbtree_link_node(&this->node, parent, new);
    ogs_rbtree_insert_color(tree, &this->node);
    return this;
}

static int tlv_parse_compound(void *msg, ogs_tlv_desc_t *parent_desc,
        ogs_tlv_t *parent_tlv, int depth, int mode)
{
    int rv;
    ogs_tlv_presence_t *presence_p = (ogs_tlv_presence_t *)msg;
    ogs_tlv_desc_t *desc = NULL, *next_desc = NULL;
    ogs_tlv_t *tlv = NULL, *emb_tlv = NULL;
    uint8_t *p = msg;
    uint32_t offset = 0;
    uint8_t index = 0;
    int i = 0, j;
    OGS_RBTREE(tree);
    tlv_count_node_t count_node[OGS_TLV_MAX_CHILD_DESC];
    unsigned count_node_alloc_next = 0;
    char indent[17] = "                "; /* 16 spaces */

    ogs_assert(msg);
    ogs_assert(parent_desc);
    ogs_assert(parent_tlv);

    ogs_assert(depth <= 8);
    indent[depth*2] = 0;

    tlv = parent_tlv;
    while (tlv) {
        tlv_count_node_t *curr_count = tlv_count_node_find(&tree, &count_node[0],
                OGS_ARRAY_SIZE(count_node), &count_node_alloc_next, tlv->type, tlv->instance);
        if (!curr_count)
            return OGS_ERROR;
        desc = tlv_find_desc_by_type_inst(&index, &offset, parent_desc, tlv->type, tlv->instance, curr_count->count);
        if (desc == NULL) {
            ogs_warn("Unknown TLV type [%d]", tlv->type);
            tlv = tlv->next;
            continue;
        }

        presence_p = (ogs_tlv_presence_t *)(p + offset);

        /* Multiple of the same type TLV may be included */
        next_desc = parent_desc->child_descs[index+1];
        if (next_desc != NULL && next_desc->ctype == OGS_TLV_MORE) {
            for (j = 0; j < next_desc->length; j++) {
                presence_p =
                    (ogs_tlv_presence_t *)(p + offset + desc->vsize * j);
                if (*presence_p == 0) {
                    offset += desc->vsize * j;
                    break;
                }
            }
            if (j == next_desc->length) {
                ogs_fatal("Multiple of the same type TLV need more room");
                ogs_assert_if_reached();
                tlv = tlv->next;
                continue;
            }
        } else {
            curr_count->count++;
        }

        if (desc->ctype == OGS_TLV_COMPOUND) {
            emb_tlv = ogs_tlv_parse_embedded_block(tlv, mode);
            if (emb_tlv == NULL) {
                ogs_error("Error while parse TLV");
                return OGS_ERROR;
            }

            ogs_trace("PARSE %sC#%d [%s] T:%d I:%d (vsz=%d) off:%p ",
                    indent, i++, desc->name, desc->type, desc->instance,
                    desc->vsize, p + offset);

            offset += sizeof(ogs_tlv_presence_t);

            rv = tlv_parse_compound(
                    p + offset, desc, emb_tlv, depth + 1, mode);
            if (rv != OGS_OK) {
                ogs_error("Can't parse compound TLV");
                return OGS_ERROR;
            }

            *presence_p = 1;
        } else {
            ogs_trace("PARSE %sL#%d [%s] T:%d L:%d I:%d "
                    "(cls:%d vsz:%d) off:%p ",
                    indent, i++, desc->name, desc->type, desc->length,
                    desc->instance, desc->ctype, desc->vsize, p + offset);

            rv = tlv_parse_leaf(p + offset, desc, tlv);
            if (rv != OGS_OK) {
                ogs_error("Can't parse leaf TLV");
                return OGS_ERROR;
            }

            *presence_p = 1;
        }

        tlv = tlv->next;
    }

    return OGS_OK;
}

int ogs_tlv_parse_msg(void *msg, ogs_tlv_desc_t *desc, ogs_pkbuf_t *pkbuf,
        int mode)
{
    int rv;
    ogs_tlv_t *root;

    ogs_assert(msg);
    ogs_assert(desc);
    ogs_assert(pkbuf);

    ogs_assert(desc->ctype == OGS_TLV_MESSAGE);
    ogs_assert(desc->child_descs[0]);

    root = ogs_tlv_parse_block(pkbuf->len, pkbuf->data, mode);
    if (root == NULL) {
        ogs_error("Can't parse TLV message");
        return OGS_ERROR;
    }

    rv = tlv_parse_compound(msg, desc, root, 0, mode);

    ogs_tlv_free_all(root);

    return rv;
}

static uint16_t parse_get_element_type(uint8_t *pos, uint8_t mode)
{
    uint16_t type;

    switch(mode) {
    case OGS_TLV_MODE_T1_L1:
    case OGS_TLV_MODE_T1_L2:
    case OGS_TLV_MODE_T1_L2_I1:
    case OGS_TLV_MODE_T1:
        type = *pos;
        break;
    case OGS_TLV_MODE_T2_L2:
        type = *(pos++) << 8;
        type += *(pos++);
        break;
    default:
        ogs_assert_if_reached();
        break;
    }

    return type;
}

/* Get TLV element taking into account msg_mode (to know TLV tag length) +
 * specific TLV information from "desc" (to know whether the specific IE is TLV
 * or TV, and its expected length in the later case). */
static uint8_t *tlv_get_element_desc(ogs_tlv_t *tlv, uint8_t *blk, uint8_t msg_mode, ogs_tlv_desc_t *desc)
{
    uint8_t instance;
    unsigned tlv_tag_pos;
    uint8_t desc_index = 0;
    uint32_t tlv_offset = 0;
    uint16_t tlv_tag;
    uint8_t tlv_mode;
    static ogs_tlv_desc_t* tlv_desc;

    tlv_tag = parse_get_element_type(blk, msg_mode);
    instance = 0;  /* TODO: support instance != 0 if ever really needed by looking it up in pos */
    tlv_tag_pos = 0; /* All tags with same instance should use the same tlv_desc, so take the first one */
    tlv_desc = tlv_find_desc_by_type_inst(&desc_index, &tlv_offset, desc, tlv_tag, instance, tlv_tag_pos);
    if (!tlv_desc) {
        ogs_error("Can't parse find TLV description for type %u", tlv_tag);
        return NULL;
    }
    tlv_mode = tlv_ctype2mode(tlv_desc->ctype, msg_mode);

    if (tlv_mode == OGS_TLV_MODE_T1)
        return tlv_get_element_fixed(tlv, blk, tlv_mode, tlv_desc->length);
    return tlv_get_element(tlv, blk, tlv_mode);
}

/* Similar to ogs_tlv_parse_block(), but taking into account each TLV format from "desc". */
static ogs_tlv_t *ogs_tlv_parse_block_desc(uint32_t length, void *data, uint8_t msg_mode, ogs_tlv_desc_t *desc)
{
    uint8_t *pos = data;
    uint8_t *blk = data;

    ogs_tlv_t *root = NULL;
    ogs_tlv_t *prev = NULL;
    ogs_tlv_t *curr = NULL;

    root = curr = ogs_tlv_get();

    ogs_assert(curr);

    pos = tlv_get_element_desc(curr, pos, msg_mode, desc);

    ogs_assert(pos);

    while(pos - blk < length) {
        prev = curr;

        curr = ogs_tlv_get();
        ogs_assert(curr);
        prev->next = curr;

        pos = tlv_get_element_desc(curr, pos, msg_mode, desc);
        ogs_assert(pos);
    }

    ogs_assert(length == (pos - blk));

    return root;
}

/* Similar to ogs_tlv_parse_msg(), but takes each TLV type from the desc
 * defintion. This allows parsing messages which have different types of TLVs in
 * it (for instance GTPv1-C). */
int ogs_tlv_parse_msg_desc(
        void *msg, ogs_tlv_desc_t *desc, ogs_pkbuf_t *pkbuf, int msg_mode)
{
    int rv;
    ogs_tlv_t *root;

    ogs_assert(msg);
    ogs_assert(desc);
    ogs_assert(pkbuf);

    ogs_assert(desc->ctype == OGS_TLV_MESSAGE);
    ogs_assert(desc->child_descs[0]);

    root = ogs_tlv_parse_block_desc(pkbuf->len, pkbuf->data, msg_mode, desc);
    if (root == NULL) {
        ogs_error("Can't parse TLV message");
        return OGS_ERROR;
    }

    rv = tlv_parse_compound(msg, desc, root, 0, msg_mode);

    ogs_tlv_free_all(root);

    return rv;
}

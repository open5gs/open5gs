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

#include "ogs-core.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_tlv_domain

static OGS_POOL(pool, ogs_tlv_t);

/* ogs_tlv_t common functions */
ogs_tlv_t *ogs_tlv_get(void)
{
    ogs_tlv_t *tlv = NULL;

    /* get tlv node from node pool */
    ogs_pool_alloc(&pool, &tlv);

    /* check for error */
    ogs_assert(tlv);

    /* intialize tlv node */
    memset(tlv, 0, sizeof(ogs_tlv_t));
    return tlv;
}

void ogs_tlv_free(ogs_tlv_t *tlv)
{
    /* free tlv node to the node pool */
    ogs_pool_free(&pool, tlv);
}

void ogs_tlv_init(void)
{
    ogs_pool_init(&pool, ogs_core()->tlv.pool);
}

void ogs_tlv_final(void)
{
    ogs_pool_final(&pool);
}

uint32_t ogs_tlv_pool_avail(void)
{
    return ogs_pool_avail(&pool);
}

void ogs_tlv_free_all(ogs_tlv_t *root)
{
    /* free all tlv node to the node pool */
    ogs_tlv_t *iter = root;
    ogs_tlv_t *next = NULL;
    while (iter) {
        if(iter->embedded != NULL) {
            ogs_tlv_free_all(iter->embedded);
        }
        next = iter->next;
        ogs_tlv_free(iter);
        iter = next;
    }
}

uint8_t ogs_tlv_value_8(ogs_tlv_t *tlv)
{
    return (*((uint8_t*)(tlv->value)));
}

uint16_t ogs_tlv_value_16(ogs_tlv_t *tlv)
{
    uint16_t u_16;
    uint8_t *v = tlv->value;

    u_16 = ((v[0] <<  8) & 0xff00) |
           ((v[1]      ) & 0x00ff);

    return u_16;
}

uint32_t ogs_tlv_value_32(ogs_tlv_t *tlv)
{
    uint32_t u_32;
    uint8_t *v = tlv->value;

    u_32 = ((v[0] << 24) & 0xff000000) |
           ((v[1] << 16) & 0x00ff0000) |
           ((v[2] <<  8) & 0x0000ff00) |
           ((v[3]      ) & 0x000000ff);

    return u_32;
}

uint32_t ogs_tlv_calc_length(ogs_tlv_t *tlv, uint8_t mode)
{
    ogs_tlv_t *iter = tlv;
    uint32_t length = 0;

    while(iter) {
        /* this is length for type field */
        switch(mode) {
        case OGS_TLV_MODE_T1_L1:
            length += 2;
            break;
        case OGS_TLV_MODE_T1_L2:
            length += 3;
            break;
        case OGS_TLV_MODE_T1_L2_I1:
        case OGS_TLV_MODE_T2_L2:
            length += 4;
            break;
        default:
            ogs_assert_if_reached();
            break;
        }

        /* this is length for type field */
        if(iter->embedded != NULL) {
            iter->length = ogs_tlv_calc_length(iter->embedded, mode);
        }

        /* this is length for value field */
        length += iter->length;

        iter = iter->next;
    }
    return length;
}

uint32_t ogs_tlv_calc_count(ogs_tlv_t *tlv)
{
    ogs_tlv_t *iter = tlv;
    uint32_t count = 0;

    while(iter) {
        if(iter->embedded != NULL) {
            count += ogs_tlv_calc_count(iter->embedded);
        } else {
            count++;
        }
        iter = iter->next;
    }
    return count;
}

static uint8_t *tlv_put_type(uint32_t type, uint8_t *pos, uint8_t mode)
{    
    switch(mode) {
    case OGS_TLV_MODE_T1_L1:
    case OGS_TLV_MODE_T1_L2:
    case OGS_TLV_MODE_T1_L2_I1:
        *(pos++) = type & 0xFF;
        break;
    case OGS_TLV_MODE_T2_L2:
        *(pos++) = (type >> 8) & 0xFF;
        *(pos++) = type & 0xFF;
        break;
    default:
        ogs_assert_if_reached();
        break;
    }
    return pos;
}

static uint8_t *tlv_put_length(uint32_t length, uint8_t *pos, uint8_t mode)
{
    switch(mode) {
    case OGS_TLV_MODE_T1_L1:
        *(pos++) = length & 0xFF;
        break;
    case OGS_TLV_MODE_T1_L2:
    case OGS_TLV_MODE_T1_L2_I1:
    case OGS_TLV_MODE_T2_L2:
        *(pos++) = (length >> 8) & 0xFF;
        *(pos++) = length & 0xFF;
        break;
    default:
        ogs_assert_if_reached();
        break;
    }

    return pos;
}

static uint8_t *tlv_put_instance(uint8_t instance, uint8_t *pos, uint8_t mode)
{
    switch(mode) {
        case OGS_TLV_MODE_T1_L2_I1:
            *(pos++) = instance & 0xFF;
            break;
        default:
            break;
    }

    return pos;
}

static uint8_t *tlv_get_element(ogs_tlv_t *tlv, uint8_t *blk, uint8_t mode)
{
    uint8_t *pos = blk;

    switch(mode) {
    case OGS_TLV_MODE_T1_L1:
        tlv->type = *(pos++);
        tlv->length = *(pos++);
        break;
    case OGS_TLV_MODE_T1_L2:
        tlv->type = *(pos++);
        tlv->length = *(pos++) << 8;
        tlv->length += *(pos++);
        break;
    case OGS_TLV_MODE_T1_L2_I1:
        tlv->type = *(pos++);
        tlv->length = *(pos++) << 8;
        tlv->length += *(pos++);
        tlv->instance = *(pos++);
        break;
    case OGS_TLV_MODE_T2_L2:
        tlv->type = *(pos++) << 8;
        tlv->type += *(pos++);
        tlv->length = *(pos++) << 8;
        tlv->length += *(pos++);
        break;
    default:
        ogs_assert_if_reached();
        break;
    }

    tlv->value = pos;

    return (pos + ogs_tlv_length(tlv));
}

static void tlv_alloc_buff_to_tlv(
        ogs_tlv_t *head, uint8_t *buff, uint32_t buff_len)
{
    head->buff_allocated = true;
    head->buff_len = buff_len;
    head->buff_ptr = buff;
    head->buff = buff;
}

ogs_tlv_t *ogs_tlv_find_root(ogs_tlv_t *tlv)
{
    ogs_tlv_t *head = tlv->head;
    ogs_tlv_t *parent;

    parent = head->parent;
    while(parent) {
        head = parent->head;
        parent = head->parent;
    }

    return head;
}

ogs_tlv_t *ogs_tlv_add(ogs_tlv_t *head, 
    uint32_t type, uint32_t length, uint8_t instance, void *value)
{
    ogs_tlv_t *curr = head;
    ogs_tlv_t *new = NULL;

    new = ogs_tlv_get();
    ogs_assert(new);
    if(length != 0)
        ogs_assert(value);

    new->type = type;
    new->length = length;
    new->instance = instance;
    new->value = value;

    if (head != NULL && head->buff_allocated == true) {
        ogs_assert((head->buff_ptr - head->buff + length) < head->buff_len);

        memcpy(head->buff_ptr, value, length);
        new->value = head->buff_ptr;
        head->buff_ptr += length;
    }

    if(curr == NULL) {
        new->head = new;
        new->tail = new;
    } else {
        head = head->head; /* in case head is not head */
        new->head = head;
        head->tail->next = new;
        head->tail = new;
    }
    return new;
}

ogs_tlv_t *ogs_tlv_copy(void *buff, uint32_t buff_len,
    uint32_t type, uint32_t length, uint8_t instance, void *value)
{
    ogs_tlv_t *new = NULL;

    new = ogs_tlv_get();
    ogs_assert(new);

    new->type = type;
    new->length = length;
    new->instance = instance;
    new->value = value;
    new->head = new->tail = new;

    tlv_alloc_buff_to_tlv(new, buff, buff_len);

    memcpy(new->buff_ptr, value, length);
    new->value = new->buff_ptr;
    new->buff_ptr += length;

    return new;
}

ogs_tlv_t *ogs_tlv_embed(ogs_tlv_t *parent, 
    uint32_t type, uint32_t length, uint8_t instance, void *value)
{
    ogs_tlv_t *new = NULL, *root = NULL;

    ogs_assert(parent);

    new = ogs_tlv_get();
    ogs_assert(new);

    new->type = type;
    new->length = length;
    new->instance = instance;
    new->value = value;

    root = ogs_tlv_find_root(parent);

    if(root->buff_allocated == true) {
        ogs_assert((root->buff_ptr - root->buff + length) < root->buff_len);

        memcpy(root->buff_ptr, value, length);
        new->value = root->buff_ptr;
        root->buff_ptr += length;
    }

    if(parent->embedded == NULL) {
        parent->embedded = new->head = new->tail = new;
        new->parent = parent;
    } else {
        new->head = parent->embedded;
        parent->embedded->tail->next = new;
        parent->embedded->tail = new;
    }

    return new;
}

uint32_t ogs_tlv_render(ogs_tlv_t *root, 
    void *data, uint32_t length, uint8_t mode)
{
    ogs_tlv_t *curr = root;
    uint8_t *pos = data;
    uint8_t *blk = data;
    uint32_t embedded_len = 0;

    while(curr) {
        pos = tlv_put_type(curr->type, pos, mode);

        if(curr->embedded == NULL) {
            pos = tlv_put_length(curr->length, pos, mode);
            pos = tlv_put_instance(curr->instance, pos, mode);

            if ((pos - blk) + ogs_tlv_length(curr) > length)
                ogs_assert_if_reached();

            memcpy((char*)pos, (char*)curr->value, curr->length);
            pos += curr->length;
        } else {
            embedded_len = ogs_tlv_calc_length(curr->embedded, mode);
            pos = tlv_put_length(embedded_len, pos, mode);
            pos = tlv_put_instance(curr->instance, pos, mode);
            ogs_tlv_render(curr->embedded,
                pos, length - (uint32_t)(pos-blk), mode);
            pos += embedded_len;
        }
        curr = curr->next;
    }

    return (pos - blk);
}

/* ogs_tlv_t parsing functions */
ogs_tlv_t *ogs_tlv_parse_block(uint32_t length, void *data, uint8_t mode)
{
    uint8_t *pos = data;
    uint8_t *blk = data;

    ogs_tlv_t *root = NULL;
    ogs_tlv_t *prev = NULL;
    ogs_tlv_t *curr = NULL;

    root = curr = ogs_tlv_get();

    ogs_assert(curr);

    pos = tlv_get_element(curr, pos, mode);

    ogs_assert(pos);

    while(pos - blk < length) {
        prev = curr;

        curr = ogs_tlv_get();
        ogs_assert(curr);
        prev->next = curr;

        pos = tlv_get_element(curr, pos, mode);
        ogs_assert(pos);
    }

    ogs_assert(length == (pos - blk));

    return root;
}

ogs_tlv_t *ogs_tlv_parse_embedded_block(ogs_tlv_t *tlv, uint8_t mode)
{
    tlv->embedded = ogs_tlv_parse_block(tlv->length, tlv->value, mode);

    return tlv->embedded;
}

/* tlv operation-related function */
ogs_tlv_t *ogs_tlv_find(ogs_tlv_t *root, uint32_t type)
{
    ogs_tlv_t *iter = root, *embed = NULL;
    while(iter) {
        if(iter->type == type) {
            return iter;
        }

        if(iter->embedded != NULL) {
            embed = ogs_tlv_find(iter->embedded, type);
            if(embed != NULL) {
                return embed;
            }
        }
        iter = iter->next;
    }

    /* tlv for the designated type doesn't exist */
    return NULL;
}

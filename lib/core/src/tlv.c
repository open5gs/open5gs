#define TRACE_MODULE _tlv
#include "core_debug.h"
#include "core_tlv.h"
#include "core_lib.h"

pool_declare(tlv_pool, tlv_t, NUM_OF_TLV_NODE);

/* tlv_t common functions */
tlv_t* tlv_get(void)
{
    tlv_t *tlv = NULL;

    /* get tlv node from node pool */
    pool_alloc_node(&tlv_pool, &tlv);

    /* check for error */
    d_assert(tlv != NULL, return NULL, "fail to get tlv pool\n");

    /* intialize tlv node */
    memset((char*)tlv, 0x00, sizeof(tlv_t));
    return tlv;
}

void tlv_free(tlv_t *p_tlv)
{
    /* free tlv node to the node pool */
    pool_free_node(&tlv_pool, p_tlv);
    return;
}

status_t tlv_init(void)
{
    pool_init(&tlv_pool, NUM_OF_TLV_NODE);
    return CORE_OK;
}

status_t tlv_final(void)
{
    pool_final(&tlv_pool);
    return CORE_OK;
}

c_uint32_t tlv_pool_avail(void)
{
    return pool_avail(&tlv_pool);
}

void tlv_free_all(tlv_t *root_tlv)
{
    /* free all tlv node to the node pool */
    tlv_t *p_tlv = root_tlv;
    tlv_t *next = NULL;
    while(p_tlv)
    {
        if(p_tlv->embedded != NULL)
        {
            tlv_free_all(p_tlv->embedded);
        }
        next = p_tlv->next;
        tlv_free(p_tlv);
        p_tlv = next;
    }
    return;
}

c_uint8_t tlv_value_8(tlv_t *tlv)
{
    return (*((c_uint8_t*)(tlv->value)));
}

c_uint16_t tlv_value_16(tlv_t *tlv)
{
    c_uint16_t c_16;
    c_uint8_t *v = tlv->value;

    c_16 = ((v[0] <<  8) & 0xff00) |
           ((v[1]      ) & 0x00ff);

    return c_16;
}

c_uint32_t tlv_value_32(tlv_t *tlv)
{
    c_uint32_t c_32;
    c_uint8_t *v = tlv->value;

    c_32 = ((v[0] << 24) & 0xff000000) |
           ((v[1] << 16) & 0x00ff0000) |
           ((v[2] <<  8) & 0x0000ff00) |
           ((v[3]      ) & 0x000000ff);

    return c_32;
}

c_uint32_t tlv_calc_length(tlv_t *p_tlv, c_uint8_t mode)
{
    tlv_t *tmp_tlv = p_tlv;
    c_uint32_t length = 0;

    while(tmp_tlv)
    {
        /* this is length for type field */
        switch(mode)
        {
            case TLV_MODE_T1_L1:
                length += 2;
                break;
            case TLV_MODE_T1_L2:
                length += 3;
                break;
            case TLV_MODE_T1_L2_I1:
            case TLV_MODE_T2_L2:
                length += 4;
                break;
            default:
                d_assert(0, return 0, "Invalid mode(%d)", mode);
                break;
        }

        /* this is length for type field */
        if(tmp_tlv->embedded != NULL)
        {
            tmp_tlv->length = tlv_calc_length(tmp_tlv->embedded, mode);
        }

        /* this is length for value field */
        length += tmp_tlv->length;

        tmp_tlv = tmp_tlv->next;
    }
    return length;
}

c_uint32_t tlv_calc_count(tlv_t *p_tlv)
{
    tlv_t *tmp_tlv = p_tlv;
    c_uint32_t count = 0;

    while(tmp_tlv)
    {
        if(tmp_tlv->embedded != NULL)
        {
            count += tlv_calc_count(tmp_tlv->embedded);
        }
        else
        {
            count++;
        }
        tmp_tlv = tmp_tlv->next;
    }
    return count;
}

c_uint8_t *_tlv_put_type(c_uint32_t type, c_uint8_t *pos, c_uint8_t mode)
{    
    switch(mode)
    {
        case TLV_MODE_T1_L1:
        case TLV_MODE_T1_L2:
        case TLV_MODE_T1_L2_I1:
            *(pos++) = type & 0xFF;
            break;
        case TLV_MODE_T2_L2:
            *(pos++) = (type >> 8) & 0xFF;
            *(pos++) = type & 0xFF;
            break;
        default:
            d_assert(0, return 0, "Invalid mode(%d)", mode);
            break;
    }
    return pos;
}

c_uint8_t *_tlv_put_length(c_uint32_t length, c_uint8_t *pos, c_uint8_t mode)
{
    switch(mode)
    {
        case TLV_MODE_T1_L1:
            *(pos++) = length & 0xFF;
            break;
        case TLV_MODE_T1_L2:
        case TLV_MODE_T1_L2_I1:
        case TLV_MODE_T2_L2:
            *(pos++) = (length >> 8) & 0xFF;
            *(pos++) = length & 0xFF;
            break;
        default:
            d_assert(0, return 0, "Invalid mode(%d)", mode);
            break;
    }

    return pos;
}

c_uint8_t *_tlv_put_instance(c_uint8_t instance, c_uint8_t *pos, c_uint8_t mode)
{
    switch(mode)
    {
        case TLV_MODE_T1_L2_I1:
            *(pos++) = instance & 0xFF;
            break;
        default:
            break;
    }

    return pos;
}

c_uint8_t *_tlv_get_element(tlv_t *p_tlv, c_uint8_t *tlvBlock, c_uint8_t mode)
{
    c_uint8_t *pos = tlvBlock;

    switch(mode)
    {
        case TLV_MODE_T1_L1:
            p_tlv->type = *(pos++);
            p_tlv->length = *(pos++);
            break;
        case TLV_MODE_T1_L2:
            p_tlv->type = *(pos++);
            p_tlv->length = *(pos++) << 8;
            p_tlv->length += *(pos++);
            break;
        case TLV_MODE_T1_L2_I1:
            p_tlv->type = *(pos++);
            p_tlv->length = *(pos++) << 8;
            p_tlv->length += *(pos++);
            p_tlv->instance = *(pos++);
            break;
        case TLV_MODE_T2_L2:
            p_tlv->type = *(pos++) << 8;
            p_tlv->type += *(pos++);
            p_tlv->length = *(pos++) << 8;
            p_tlv->length += *(pos++);
            break;
        default:
            d_assert(0, return 0, "Invalid mode(%d)", mode);
            break;
    }

    p_tlv->value = pos;

    return (pos + tlv_length(p_tlv));
}

void _tlv_alloc_buff_to_tlv(
        tlv_t* head_tlv, c_uint8_t* buff, c_uint32_t buff_len)
{
    head_tlv->buff_allocated = TRUE;
    head_tlv->buff_len = buff_len;
    head_tlv->buff_ptr = buff;
    head_tlv->buff = buff;
}

tlv_t *tlv_find_root(tlv_t* p_tlv)
{
    tlv_t *head_tlv = p_tlv->head;
    tlv_t *parentTlv;

    parentTlv = head_tlv->parent;
    while(parentTlv)
    {
        head_tlv = parentTlv->head;
        parentTlv = head_tlv->parent;
    }

    return head_tlv;
}

tlv_t *tlv_add(tlv_t *head_tlv, 
    c_uint32_t type, c_uint32_t length, c_uint8_t instance, c_uint8_t *value)
{
    tlv_t* curr_tlv = head_tlv;
    tlv_t* new_tlv = NULL;

    new_tlv = tlv_get();
    d_assert(new_tlv, return NULL, "can't get tlv node\n");
    if(length != 0)
    {
        d_assert(value, return NULL, "tlv value is NULL\n");
    }

    new_tlv->type = type;
    new_tlv->length = length;
    new_tlv->instance = instance;
    new_tlv->value = value;

    if(head_tlv != NULL && head_tlv->buff_allocated == TRUE)
    {
        d_assert((head_tlv->buff_ptr - head_tlv->buff + length)<
            head_tlv->buff_len, tlv_free(new_tlv); return NULL,
            "overflow in tlv buffer\n");

        memcpy(head_tlv->buff_ptr, value, length);
        new_tlv->value = head_tlv->buff_ptr;
        head_tlv->buff_ptr += length;
    }

    if(curr_tlv == NULL)
    {
        new_tlv->head = new_tlv;
        new_tlv->tail = new_tlv;
    }
    else
    {
        head_tlv = head_tlv->head; /* in case head_tlv is not head */
        new_tlv->head = head_tlv;
        head_tlv->tail->next = new_tlv;
        head_tlv->tail = new_tlv;
    }
    return new_tlv;
}

tlv_t *tlv_copy(c_uint8_t *buff, c_uint32_t buff_len,
    c_uint32_t type, c_uint32_t length, c_uint8_t instance, c_uint8_t *value)
{
    tlv_t* new_tlv = NULL;

    new_tlv = tlv_get();
    d_assert(new_tlv, return NULL, "can't get tlv node\n");

    new_tlv->type = type;
    new_tlv->length = length;
    new_tlv->instance = instance;
    new_tlv->value = value;
    new_tlv->head = new_tlv->tail = new_tlv;

    _tlv_alloc_buff_to_tlv(new_tlv, buff, buff_len);

    memcpy(new_tlv->buff_ptr, value, length);
    new_tlv->value = new_tlv->buff_ptr;
    new_tlv->buff_ptr += length;

    return new_tlv;
}

tlv_t *tlv_embed(tlv_t *parent_tlv, 
    c_uint32_t type, c_uint32_t length, c_uint8_t instance, c_uint8_t *value)
{
    tlv_t* new_tlv = NULL, *root_tlv = NULL;

    d_assert(parent_tlv, return NULL, "parent tlv is NULL\n");

    new_tlv = tlv_get();
    d_assert(new_tlv, return NULL, "can't get tlv node\n");

    new_tlv->type = type;
    new_tlv->length = length;
    new_tlv->instance = instance;
    new_tlv->value = value;

    root_tlv = tlv_find_root(parent_tlv);

    if(root_tlv->buff_allocated == TRUE)
    {
        d_assert((root_tlv->buff_ptr - root_tlv->buff + length)<root_tlv->buff_len,
            tlv_free(new_tlv); return NULL, "overflow in tlv buffer\n");

        memcpy(root_tlv->buff_ptr, value, length);
        new_tlv->value = root_tlv->buff_ptr;
        root_tlv->buff_ptr += length;
    }

    if(parent_tlv->embedded == NULL)
    {
        parent_tlv->embedded = new_tlv->head = new_tlv->tail = new_tlv;
        new_tlv->parent = parent_tlv;
    }
    else
    {
        new_tlv->head = parent_tlv->embedded;
        parent_tlv->embedded->tail->next = new_tlv;
        parent_tlv->embedded->tail = new_tlv;
    }

    return new_tlv;
}

c_uint32_t tlv_render(tlv_t *root_tlv, 
    c_uint8_t *blk, c_uint32_t length, c_uint8_t mode)
{
    tlv_t* curr_tlv = root_tlv;
    c_uint8_t* pos = blk;
    c_uint32_t embedded_len = 0;

    while(curr_tlv)
    {
        pos = _tlv_put_type(curr_tlv->type, pos, mode);

        if(curr_tlv->embedded == NULL)
        {
            pos = _tlv_put_length(curr_tlv->length, pos, mode);
            pos = _tlv_put_instance(curr_tlv->instance, pos, mode);

            if((pos - blk) + tlv_length(curr_tlv) > length)
            {
                d_assert(FALSE, ;,
                    "tlv_t encoding error:overflow for given buff length\n");
                memcpy((char*)pos, (char*)curr_tlv->value, length - (pos-blk));
                pos += length - (pos-blk);

                return (pos - blk);
            }

            memcpy((char*)pos, (char*)curr_tlv->value, curr_tlv->length);
            pos += curr_tlv->length;
        }
        else
        {
            embedded_len = tlv_calc_length(curr_tlv->embedded, mode);
            pos = _tlv_put_length(embedded_len, pos, mode);
            pos = _tlv_put_instance(curr_tlv->instance, pos, mode);
            tlv_render(curr_tlv->embedded,
                pos, length - (c_uint32_t)(pos-blk), mode);
            pos += embedded_len;
        }
        curr_tlv = curr_tlv->next;
    }

    return (pos - blk);
}

/* tlv_t parsing functions */
tlv_t *tlv_parse_block(c_uint32_t length, c_uint8_t *blk, c_uint8_t mode)
{
    c_uint8_t* pos = blk;

    tlv_t* root_tlv = NULL;
    tlv_t* prev_tlv = NULL;
    tlv_t* curr_tlv = NULL;

    root_tlv = curr_tlv = tlv_get();

    d_assert(curr_tlv, return NULL, "can't get tlv node\n");

    pos = _tlv_get_element(curr_tlv, pos, mode);

    d_assert(pos != NULL, tlv_free_all(root_tlv); return NULL,
        "tlvGetElement Failed\n");

    while(pos - blk < length)
    {
        prev_tlv = curr_tlv;

        curr_tlv = tlv_get();
        d_assert(curr_tlv, tlv_free_all(root_tlv); return NULL,
            "can't get tlv node\n");
        prev_tlv->next = curr_tlv;

        pos = _tlv_get_element(curr_tlv, pos, mode);
        d_assert(pos != NULL, tlv_free_all(root_tlv); return NULL,
            "tlvGetElement Failed\n");
    }

    d_assert(length == (pos - blk),
            tlv_free_all(root_tlv); return NULL,
            "total size is not equal to sum of each tlv. %d %d",
            length, pos - blk);

    return root_tlv;
}

tlv_t *tlv_parse_embedded_block(tlv_t* p_tlv, c_uint8_t mode)
{
    p_tlv->embedded = tlv_parse_block(p_tlv->length, p_tlv->value, mode);

    return p_tlv->embedded;
}

/* tlv operation-related function */
tlv_t *tlv_find(tlv_t* p_tlv, c_uint32_t type)
{
    tlv_t *tmp_tlv = p_tlv, *embed_tlv = NULL;
    while(tmp_tlv)
    {
        if(tmp_tlv->type == type)
        {
            return tmp_tlv;
        }

        if(tmp_tlv->embedded != NULL)
        {
            embed_tlv = tlv_find(tmp_tlv->embedded, type);
            if(embed_tlv != NULL)
            {
                return embed_tlv;
            }
        }
        tmp_tlv = tmp_tlv->next;
    }

    /* tlv for the designated type doesn't exist */
    return NULL;
}

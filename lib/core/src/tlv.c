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
        if(mode == TLV_MODE_WMX_R4_R6)
        {
            length += 2;
        }
        else
        {
            length += 1;
        }


        /* this is length for type field */
        if(tmp_tlv->embedded != NULL)
        {
            tmp_tlv->length = tlv_calc_length(tmp_tlv->embedded, mode);
        }

        if(mode == TLV_MODE_WMX_R4_R6)
        {
            length += 2;
        }
        else if(mode == TLV_MODE_UTIS)
        {
            if(tmp_tlv->length < UPDU_IE_TYPE_1_MAX_SIZE)
                length += 1;
            else if(tmp_tlv->length < UPDU_IE_TYPE_2_MAX_SIZE)
                length += 2;
            else if(tmp_tlv->length < UPDU_IE_TYPE_3_MAX_SIZE)
                length += 4;
            else
            {
                /* Error : Invalid  IE length field */
            }
        }
        else
        {
            length += 1;
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


c_uint32_t _tlv_get_length_utis(c_uint8_t **pos)
{
    c_uint8_t *blk = *pos;
    c_uint32_t length = 0;

    switch(*blk >> 6)
    {
        case UPDU_IE_TYPE_1_CODE:
            length = *blk & 0x3F;
            *pos += 1;
            break;

        case UPDU_IE_TYPE_2_CODE:
            length = ((*blk & 0x3F) << 8) + *(blk + 1);
            *pos += 2;
            break;

        case UPDU_IE_TYPE_3_CODE:
            length = ((*blk & 0x3F) << 24) +
                (*(blk + 1) << 16) + (*(blk + 2) << 8) + *(blk + 3);
            *pos += 4;
            break;
        default:
            /* Error : Invalid  IE length field */
            return CORE_ERROR;
            break;
    }

    return length;
}

c_uint8_t* _tlv_put_length_utis(c_uint32_t length, c_uint8_t *pos)
{
    if(length < UPDU_IE_TYPE_1_MAX_SIZE)
    {
        *(pos++) = (UPDU_IE_TYPE_1_CODE << 6) | length;
    }
    else if(length < UPDU_IE_TYPE_2_MAX_SIZE)
    {
        *(pos++) = (UPDU_IE_TYPE_2_CODE << 6) | (length >> 8);
        *(pos++) = length & 0xFF;
    }
    else if(length < UPDU_IE_TYPE_3_MAX_SIZE)
    {
        *(pos++) = (length >> 24) & 0xFF;
        *(pos++) = (length >> 16) & 0xFF;
        *(pos++) = (length >> 8) & 0xFF;
        *(pos++) = length & 0xFF;
    }
    else
    {
        d_assert(FALSE, return NULL, "invalid tlv length\n");
    }

    return pos;
}


c_uint32_t _tlv_get_length_wifi(c_uint8_t **pos)
{
    c_uint8_t *blk = *pos;
    c_uint32_t length = 0;

    if(*blk & 0x80)
    {
        d_error("TLV for wifi is not implemented in case of (length > 127)");
    }
    else
    {
        length = *blk;
        *pos += 1;
    }

    return length;
}

c_uint8_t* _tlv_put_length_wifi(c_uint32_t length, c_uint8_t *pos)
{
    if(length < 128)
    {
        *(pos++) = length;
    }
    else
    {
        d_error("TLV for wifi is not implemented in case of (length > 127)");
    }
    return pos;
}


c_uint32_t _tlv_get_length_wmx_r4_r6(c_uint8_t **pos)
{
    c_uint8_t *blk = *pos;
    c_uint32_t length = 0;

    length = ((*blk) << 8) + *(blk+1);
    *pos += 2;

    return length;
}

c_uint8_t* _tlv_put_length_wmx_r4_r6(c_uint32_t length, c_uint8_t *pos)
{
    *(pos++) = (length >> 8) & 0xFF;
    *(pos++) = length & 0xFF;

    return pos;
}

/* Not Implemented for WMX_R1*/
c_uint32_t _tlv_get_length_wmx_r1(c_uint8_t **pos)
{
    c_uint8_t *blk = *pos;
    c_uint32_t length = 0;

    if(*blk & 0x80)
    {
        d_error("TLV is not implemented in case of (length > 127)");
    }
    else
    {
        length = *blk;
        *pos += 1;
    }

    return length;
}

c_uint8_t* _tlv_put_length_wmx_r1(c_uint32_t length, c_uint8_t *pos)
{
    if(length < 128)
    {
        *(pos++) = length;
    }
    else
    {
        d_error("TLV is not implemented in case of (length > 127)");
    }
    return pos;
}

c_uint32_t _tlv_get_length_common(c_uint8_t **pos)
{
    c_uint8_t *blk = *pos;
    c_uint32_t length = 0;

    if(*blk & 0x80)
    {
        d_error("TLV is not implemented in case of (length > 127)");
    }
    else
    {
        length = *blk;
        *pos += 1;
    }

    return length;
}

c_uint8_t* _tlv_put_length_common(c_uint32_t length, c_uint8_t *pos)
{
    if(length < 128)
    {
        *(pos++) = length;
    }
    else
    {
        d_error("TLV is not implemented in case of (length > 127)");
    }
    return pos;
}


c_uint8_t* _tlv_put_length(c_uint32_t length, c_uint8_t *pos, c_uint8_t mode)
{
    if(mode == TLV_MODE_UTIS)
    {
        pos = _tlv_put_length_utis(length, pos);

    }
    else if(mode == TLV_MODE_WIFI)
    {
        pos = _tlv_put_length_wifi(length, pos);
    }
    else if(mode == TLV_MODE_WMX_R4_R6)
    {
        pos = _tlv_put_length_wmx_r4_r6(length, pos);
    }
    else if(mode == TLV_MODE_WMX_R1)
    {
        pos = _tlv_put_length_wmx_r1(length, pos);
    }    
    else
    {
        pos = _tlv_put_length_common(length, pos);
    }

    return pos;
}


c_uint8_t* _tlv_get_element(tlv_t *p_tlv, c_uint8_t *tlvBlock, c_uint8_t mode)
{
    c_uint8_t *pos = tlvBlock;

    if(mode == TLV_MODE_WMX_R4_R6)
    {
        p_tlv->type = *(pos++) << 8;
        p_tlv->type += *(pos++);
    }
    else
    {
        p_tlv->type = *(pos++);
    }
    
    
    if(mode == TLV_MODE_UTIS)
    {
        p_tlv->length = _tlv_get_length_utis(&pos);
    }
    else if(mode == TLV_MODE_WIFI)
    {
        p_tlv->length = _tlv_get_length_wifi(&pos);
    }
    else if(mode == TLV_MODE_WMX_R4_R6)
    {
        p_tlv->length = _tlv_get_length_wmx_r4_r6(&pos);
    }
    else if(mode == TLV_MODE_WMX_R1)
    {
        p_tlv->length = _tlv_get_length_wmx_r1(&pos);
    }    
    else
    {
        p_tlv->length = _tlv_get_length_common(&pos);
    }
    p_tlv->value = pos;

    return (pos + tlv_length(p_tlv));
}


c_uint8_t* _tlv_put_type(c_uint32_t type, c_uint8_t *pos, c_uint8_t mode)
{    
    if(mode == TLV_MODE_WMX_R4_R6)
    {
        *(pos++) = (type >> 8) & 0xFF;
        *(pos++) = type & 0xFF;
    }    
    else
    {
        *(pos++) = type & 0xFF;
    }
    return pos;
}


c_uint8_t* _tlv_get_type(tlv_t *p_tlv, c_uint8_t *tlvBlock, c_uint8_t mode)
{
    c_uint8_t *pos = tlvBlock;

    p_tlv->type = *(pos++);
    if(mode == TLV_MODE_UTIS)
    {
        p_tlv->length = _tlv_get_length_utis(&pos);
    }
    else if(mode == TLV_MODE_WIFI)
    {
        p_tlv->length = _tlv_get_length_wifi(&pos);
    }
    else if(mode == TLV_MODE_WMX_R4_R6)
    {
        p_tlv->length = _tlv_get_length_wmx_r4_r6(&pos);
    }
    else if(mode == TLV_MODE_WMX_R1)
    {
        p_tlv->length = _tlv_get_length_wmx_r1(&pos);
    }    
    else
    {
        p_tlv->length = _tlv_get_length_common(&pos);
    }
    p_tlv->value = pos;

    return (pos + tlv_length(p_tlv));
}



/* tlv_t encoding functions */

c_uint8_t *tlv_write_to_buff(
        c_uint8_t *blk, c_uint32_t type, c_uint32_t length,
        c_uint8_t *value, c_uint8_t mode)
{
    c_uint8_t *pos = blk;
    *(pos++) = type;
    pos = _tlv_put_length(length, pos, mode);

    memcpy((c_uint8_t*)pos, (c_uint8_t*)value, length);

    pos += length;
    return pos;
}

void _tlv_alloc_buff_to_tlv(
        tlv_t* head_tlv, c_uint8_t* buff, c_uint32_t buff_len)
{
    head_tlv->buff_allocated = TRUE;
    head_tlv->buff_len = buff_len;
    head_tlv->buff_ptr = buff;
    head_tlv->buff = buff;
}

tlv_t * tlv_find_root(tlv_t* p_tlv)
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


tlv_t * tlv_add(
        tlv_t *head_tlv, c_uint32_t type, c_uint32_t length, c_uint8_t *value)
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

tlv_t * tlv_create_buff_enabled_tlv(
        c_uint8_t *buff, c_uint32_t buff_len,
        c_uint32_t type, c_uint32_t length, c_uint8_t *value)
{
    tlv_t* new_tlv = NULL;

    new_tlv = tlv_get();
    d_assert(new_tlv, return NULL, "can't get tlv node\n");

    new_tlv->type = type;
    new_tlv->length = length;
    new_tlv->value = value;
    new_tlv->head = new_tlv->tail = new_tlv;

    _tlv_alloc_buff_to_tlv(new_tlv,buff, buff_len);

    memcpy(new_tlv->buff_ptr, value, length);
    new_tlv->value = new_tlv->buff_ptr;
    new_tlv->buff_ptr += length;

    return new_tlv;
}



tlv_t * tlv_embed(
        tlv_t *parent_tlv, c_uint32_t type, c_uint32_t length, c_uint8_t *value)
{
    tlv_t* new_tlv = NULL, *root_tlv = NULL;

    d_assert(parent_tlv, return NULL, "parent tlv is NULL\n");

    new_tlv = tlv_get();
    d_assert(new_tlv, return NULL, "can't get tlv node\n");

    new_tlv->type = type;
    new_tlv->length = length;
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


c_uint32_t tlv_render(
        tlv_t *root_tlv, c_uint8_t *blk, c_uint32_t length, c_uint8_t mode)
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
            tlv_render(curr_tlv->embedded,
                pos, length - (c_uint32_t)(pos-blk), mode);
            pos += embedded_len;
        }
        curr_tlv = curr_tlv->next;
    }

    return (pos - blk);
}


/* tlv_t parsing functions */

tlv_t * tlv_parse_tlv_block(c_uint32_t length, c_uint8_t *blk, c_uint8_t mode)
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

tlv_t * tlv_parse_embedded_tlv_block(tlv_t* p_tlv, c_uint8_t mode)
{
    p_tlv->embedded = tlv_parse_tlv_block(p_tlv->length, p_tlv->value, mode);

    return p_tlv->embedded;
}



/* tlv operation-related function */

tlv_t * tlv_find(tlv_t* p_tlv, c_uint32_t type)
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

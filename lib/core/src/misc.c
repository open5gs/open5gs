#include "core_errno.h"
#include "core_lib.h"

void *core_ascii_to_hex(char *in, int in_len, void *out, int out_len)
{
    int i = 0, j = 0, k = 0, hex;
    c_uint8_t *out_p = out;

    while(i < in_len && j < out_len)
    {
        if (!c_isspace(in[i]))
        {
            hex = c_isdigit(in[i]) ? in[i] - '0' : 
                c_islower(in[i]) ? in[i] - 'a' + 10 : in[i] - 'A' + 10;
            if ((k & 0x1) == 0)
            {
                out_p[j] = (hex << 4);
            }
            else
            {
                out_p[j] |= hex;
                j++;
            }
            k++;
        }
        i++;
    }

    return out;
}

void *core_hex_to_ascii(void *in, int in_len, void *out, int out_len)
{
    char *p;
    int i = 0, l, off = 0;

    p = out;
    p[0] = 0;

    l = (in_len - off) > out_len ? out_len : in_len - off;
    for (i = 0; i < l; i++)
    {
        p += sprintf(p, "%02X", ((char*)in)[off+i] & 0xff);
        if ((i & 0x3) == 3 && (i != (l-1))) p += sprintf(p, " ");
    }

    return out;
}

void *core_uint64_to_buffer(c_uint64_t num, int size, void *buffer)
{
    int i;
    c_uint8_t *buffer_p = buffer;
    for (i = 0; i < size; i++) 
        buffer_p[i] = (num >> ((size-1-i) * 8)) & 0xff; 

    return buffer;
}

c_uint64_t core_buffer_to_uint64(void *buffer, int size)
{
    c_uint64_t num = 0;
    c_uint8_t *buffer_p = buffer;
    int i;

    for (i = 0; i < size; i++)
    {
        num |= (((c_uint64_t)buffer_p[i]) << ((size-1-i) * 8));
    }

    return num;
}

void *core_bcd_to_buffer(c_int8_t *in, void *out, int *out_len)
{
    int i = 0;
    c_uint8_t *out_p = out;
    int in_len = strlen(in);

    for (i = 0; i < in_len; i++) 
    {
        if (i & 0x01)
            out_p[i>>1] = out_p[i>>1] | (((in[i] - 0x30) << 4) & 0xF0);
        else
            out_p[i>>1] = (in[i] - 0x30) & 0x0F;
    }

    *out_len = (in_len + 1) / 2;
    if (in_len & 0x01)
    {
        out_p[(*out_len)-1] |= 0xF0;
    }

    return out;
}

void *core_buffer_to_bcd(c_uint8_t *in, int in_len, void *out)
{
    int i = 0;
    c_uint8_t *out_p = out;

    for (i = 0; i < in_len; i++) 
    {
        out_p[i*2] = 0x30 + (in[i] & 0x0F);
        out_p[i*2+1] = 0x30 + ((in[i] & 0xF0) >> 4);
    }

    out_p[in_len*2] = 0;
    if ((out_p[in_len*2-1] & 0x0F) == 0x0F)
    {
        out_p[in_len*2-1] = 0;
    }

    return out;
}

char *core_cpystrn(char *dst, const char *src, size_t dst_size)
{

    char *d = dst, *end;

    if (dst_size == 0) {
        return (dst);
    }

    if (src) {
        end = dst + dst_size - 1;

        for (; d < end; ++d, ++src) {
            if (!(*d = *src)) {
                return (d);
            }
        }
    }

    *d = '\0';	/* always null terminate */

    return (d);
}

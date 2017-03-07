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

void *core_uint64_to_buffer(c_uint64_t num, int size, c_uint8_t *buffer)
{
    int i;
    for (i = 0; i < size; i++) 
        buffer[i] = (num >> ((size-1-i) * 8)) & 0xff; 

    return buffer;
}

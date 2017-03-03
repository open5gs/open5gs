#include "core_errno.h"
#include "core_lib.h"

void *core_ascii_to_hex(char *in, int len, void *out)
{
    int i = 0, j = 0, k = 0, high, low;
    c_uint8_t *out_p = out;
    while(i < len)
    {
        if (!c_isspace(in[i]))
        {
            low = c_isdigit(in[i]) ? in[i] - '0' : 
                c_islower(in[i]) ? in[i] - 'a' + 10 : in[i] - 'A' + 10;
            if ((k & 0x1) == 0)
            {
                high = low;
            }
            else
            {
                out_p[j] = (high << 4) | low;
                j++;
            }
            k++;
        }
        i++;
    }

    return out;
}

void *core_int_to_buffer(c_uint64_t num, c_uint8_t *buffer, int size)
{
    int i;
    for (i = 0; i < size; i++) 
        buffer[i] = (num >> ((size-1-i) * 8)) & 0xff; 

    return buffer;
}

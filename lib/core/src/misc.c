#include "core_errno.h"
#include "core_lib.h"

void *core_ascii_to_hex(char *in, int len, char *out)
{
    int i, j, high, low;
    for (i = 0, j = 0; i < len; i+=2, j+=1)
    {
        high = c_isdigit(in[i]) ? in[i] - '0' : 
            c_islower(in[i]) ? in[i] - 'a' + 10 : in[i] - 'A' + 10;
        low = c_isdigit(in[i+1]) ? in[i+1] - '0' : 
            c_islower(in[i+1]) ? in[i+1] - 'a' + 10 : in[i+1] - 'A' + 10;

#if 0
        high = in[i]  > '9' ? in[i] - 'A' + 10 : in[i] - '0';
        low = in[i+1] > '9' ? in[i+1] - 'A' + 10 : in[i+1] - '0';
#endif

        out[j] = (high << 4) | low;
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

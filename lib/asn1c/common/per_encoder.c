#include <asn_internal.h>
#include <per_encoder.h>

int
ignore_output(const void *data, size_t size, void *app_key) {
    (void)data;
    (void)size;
    (void)app_key;
    return 0;
}

int
encode_dyn_cb(const void *buffer, size_t size, void *key) {
    enc_dyn_arg *arg = key;
    if(arg->length + size >= arg->allocated) {
        size_t new_size = arg->allocated ? arg->allocated : 8;
        void *p;

        do {
            new_size <<= 2;
        } while(arg->length + size >= new_size);

        p = REALLOC(arg->buffer, new_size);
        if(!p) {
            FREEMEM(arg->buffer);
            memset(arg, 0, sizeof(*arg));
            return -1;
        }
        arg->buffer = p;
        arg->allocated = new_size;
    }
    memcpy(((char *)arg->buffer) + arg->length, buffer, size);
    arg->length += size;
    return 0;
}

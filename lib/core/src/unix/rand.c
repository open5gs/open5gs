#include "core_errno.h"

#define DEV_RANDOM "/dev/urandom"

status_t core_generate_random_bytes(c_uint8_t *buf, int length)
{
    int fd = -1;

    /* On BSD/OS 4.1, /dev/random gives out 8 bytes at a time, then
     * gives EOF, so reading 'length' bytes may require opening the
     * device several times. */
    do {
        int rc;

        if (fd == -1)
            if ((fd = open(DEV_RANDOM, O_RDONLY)) == -1)
                return errno;
        
        do {
            rc = read(fd, buf, length);
        } while (rc == -1 && errno == EINTR);

        if (rc < 0) {
            int errnum = errno;
            close(fd);
            return errnum;
        }
        else if (rc == 0) {
            close(fd);
            fd = -1; /* force open() again */
        }
        else {
            buf += rc;
            length -= rc;
        }
    } while (length > 0);
    
    close(fd);

    return CORE_OK;
}

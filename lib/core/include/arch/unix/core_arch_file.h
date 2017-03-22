#ifndef __CORE_ARCH_FILE_H__
#define __CORE_ARCH_FILE_H__

#include "core_portable.h"
#include "core_time.h"
#include "core_file.h"

#define FILE_DEFAULT_BUFSIZE 4096
/* For backwards-compat */
#define FILE_BUFSIZE  FILE_DEFAULT_BUFSIZE

struct file_t {
    os_file_t filedes;
    char fname[MAX_FILENAME_SIZE];
    c_int32_t flags;
    int eof_hit;
    c_time_t timeout;
    off_t filePtr;              /* position in file of handle */
};

typedef struct stat struct_stat;

mode_t unix_perms2mode(file_perms_t perms);
file_perms_t unix_mode2perms(mode_t mode);

#endif  /* ! __CORE_ARCH_FILE_H__ */


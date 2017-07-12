#define TRACE_MODULE _config

#include "core_file.h"
#include "core_debug.h"

#include "app.h"

#define MAX_CONFIG_FILE_SIZE 1024
#define MAX_JSMN_TOKEN_SIZE 128
#define DEFAULT_CONFIG_PATH SYSCONFDIR PACKAGE ".conf"

status_t config_initialize(char *config_path)
{
    status_t rv;
    size_t nbytes = MAX_CONFIG_FILE_SIZE;
    char str[MAX_CONFIG_FILE_SIZE+1];
    file_t *file;

    char *path = config_path;
    if (path == NULL) path = DEFAULT_CONFIG_PATH;

    rv = file_open(&file, path, FILE_READ, FILE_OS_DEFAULT);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't open configuration file '%s' (errno = %d, %s)", 
                path, rv, core_strerror(rv, str, MAX_CONFIG_FILE_SIZE));
        return rv;
    }

    rv = file_read(file, str, &nbytes);
    if (rv != CORE_OK) 
    {
        d_fatal("Can't read configuration file '%s' (errno = %d, %s)", 
                path, rv, core_strerror(rv, str, MAX_CONFIG_FILE_SIZE));
        return rv;
    }
    str[nbytes-1] = 0;

    file_close(file);

    return CORE_OK;
}

void config_terminate(void)
{
}

#ifndef __CORE_FILE_H__
#define __CORE_FILE_H__

/**
 * @file core_file.h
 * @brief CORE File I/O Handling
 */

#include "core.h"
#include "core_time.h"
#include "core_errno.h"
#include "core_param.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup file_io File I/O Handling Functions
 * @ingroup CORE
 * @{
 */

/** filetype_e values for the filetype member of the
 * file_info_t structure
 * @warning: Not all of the filetypes below can be determined.
 * For example, a given platform might not correctly report
 * a socket descriptor as CORE_SOCK if that type isn't
 * well-identified on that platform.  In such cases where
 * a filetype exists but cannot be described by the recognized
 * flags below, the filetype will be CORE_UNKFILE.  If the
 * filetype member is not determined, the type will be CORE_NOFILE.
 */

typedef enum {
    FILE_NOFILE = 0,     /**< no file type determined */
    FILE_REG,            /**< a regular file */
    FILE_DIR,            /**< a directory */
    FILE_CHR,            /**< a character device */
    FILE_BLK,            /**< a block device */
    FILE_PIPE,           /**< a FIFO / pipe */
    FILE_LNK,            /**< a symbolic link */
    FILE_SOCK,           /**< a [unix domain] socket */
    FILE_UNKFILE = 127   /**< a file of some other unknown type */
} filetype_e;

/**
 * @defgroup file_permissions File Permissions flags
 * @{
 */

#define FILE_USETID      0x8000 /**< Set user id */
#define FILE_UREAD       0x0400 /**< Read by user */
#define FILE_UWRITE      0x0200 /**< Write by user */
#define FILE_UEXECUTE    0x0100 /**< Execute by user */

#define FILE_GSETID      0x4000 /**< Set group id */
#define FILE_GREAD       0x0040 /**< Read by group */
#define FILE_GWRITE      0x0020 /**< Write by group */
#define FILE_GEXECUTE    0x0010 /**< Execute by group */

#define FILE_WSTICKY     0x2000 /**< Sticky bit */
#define FILE_WREAD       0x0004 /**< Read by others */
#define FILE_WWRITE      0x0002 /**< Write by others */
#define FILE_WEXECUTE    0x0001 /**< Execute by others */

#define FILE_OS_DEFAULT  0x0FFF /**< use OS's default permissions */

/* additional permission flags for file_copy  and file_append */
#define FILE_SOURCE_PERMS 0x1000 /**< Copy source file's permissions */

/** @} */

/**
 * @defgroup file_open_flags File Open Flags/Routines
 * @{
 */

/* Note to implementors: Values in the range 0x00100000--0x80000000
   are reserved for platform-specific values. */

#define FILE_READ       0x00001  /**< Open the file for reading */
#define FILE_WRITE      0x00002  /**< Open the file for writing */
#define FILE_CREATE     0x00004  /**< Create the file if not there */
#define FILE_APPEND     0x00008  /**< Append to the end of the file */
#define FILE_TRUNCATE   0x00010  /**< Open the file and truncate
                                         to 0 length */
#define FILE_BINARY     0x00020  /**< Open the file in binary mode */
#define FILE_EXCL       0x00040  /**< Open should fail if FILE_CREATE
                                         and file exists. */
#define FILE_DELONCLOSE 0x00100  /**< Delete the file after close */

/** @} */

/**
 * @defgroup file_seek_flags File Seek Flags
 * @{
 */

/* flags for file_seek */
/** Set the file position */
#define FILE_SET SEEK_SET
/** Current */
#define FILE_CUR SEEK_CUR
/** Go to end of file */
#define FILE_END SEEK_END
/** @} */

/**
 * @defgroup file_attrs_set_flags File Attribute Flags
 * @{
 */

/* flags for file_attrs_set */
#define ATTR_READONLY   0x01          /**< File is read-only */
#define ATTR_EXECUTABLE 0x02          /**< File is executable */
#define ATTR_HIDDEN     0x04          /**< File is hidden */
/** @} */

/**
 * @defgroup file_writev{_full} max iovec size
 * @{
 */
#if defined(DOXYGEN)
#define MAX_IOVEC_SIZE 1024                /**< System dependent maximum
                                                    size of an iovec array */
#elif defined(IOV_MAX)
#define MAX_IOVEC_SIZE IOV_MAX
#elif defined(MAX_IOVEC)
#define MAX_IOVEC_SIZE MAX_IOVEC
#else
#define MAX_IOVEC_SIZE 1024
#endif
/** @} */

/** File attributes */
typedef c_uint32_t          file_attrs_t;

/** Type to pass as whence argument to file_seek. */
typedef int                 seek_where_t;

/**
 * Structure for referencing files.
 */
typedef struct file_t       file_t;

/**
 * Structure for referencing directories.
 */
typedef struct dir_t        dir_t;
/**
 * Structure for determining file permissions.
 */
typedef c_int32_t           file_perms_t;
#if (defined WIN32) || (defined NETWARE)
/**
 * Structure for determining the device the file is on.
 */
typedef c_uint32_t          dev_t;
#endif

/** @} */

/**
 * @defgroup file_stat Stat Functions
 * @{
 */
/** file info structure */
typedef struct file_info_t        file_info_t;

#define FILE_INFO_LINK   0x00000001 /**< Stat the link not the file itself
                                      if it is a link */
#define FILE_INFO_MTIME  0x00000010 /**< Modification Time */
#define FILE_INFO_CTIME  0x00000020 /**< Creation or inode-changed time */
#define FILE_INFO_ATIME  0x00000040 /**< Access Time */
#define FILE_INFO_SIZE   0x00000100 /**< Size of the file */
#define FILE_INFO_CSIZE  0x00000200 /**< Storage size consumed by the file */
#define FILE_INFO_DEV    0x00001000 /**< Device */
#define FILE_INFO_INODE  0x00002000 /**< Inode */
#define FILE_INFO_NLINK  0x00004000 /**< Number of links */
#define FILE_INFO_TYPE   0x00008000 /**< Type */
#define FILE_INFO_USER   0x00010000 /**< User */
#define FILE_INFO_GROUP  0x00020000 /**< Group */
#define FILE_INFO_UPROT  0x00100000 /**< User protection bits */
#define FILE_INFO_GPROT  0x00200000 /**< Group protection bits */
#define FILE_INFO_WPROT  0x00400000 /**< World protection bits */
#define FILE_INFO_ICASE  0x01000000 /**< if dev is case insensitive */
#define FILE_INFO_NAME   0x02000000 /**< ->name in proper case */

#define FILE_INFO_MIN    0x00008170 /**< type, mtime, ctime, atime, size */
#define FILE_INFO_IDENT  0x00003000 /**< dev and inode */
#define FILE_INFO_OWNER  0x00030000 /**< user and group */
#define FILE_INFO_PROT   0x00700000 /**<  all protections */
#define FILE_INFO_NORM   0x0073b170 /**<  an atomic unix file_stat() */
#define FILE_INFO_DIRENT 0x02000000 /**<  an atomic unix dir_read() */

/**
 * The file information structure.  This is analogous to the POSIX
 * stat structure.
 */
struct file_info_t {
    /** The bitmask describing valid fields of this c_file_info_t structure
     *  including all available 'wanted' fields and potentially more */
    c_int32_t valid;
    /** The access permissions of the file.  Mimics Unix access rights. */
    file_perms_t protection;
    /** The type of file.  One of CORE_REG, CORE_DIR, CORE_CHR, CORE_BLK,
      CORE_PIPE, CORE_LNK or CORE_SOCK.  If the type is undetermined,
      the value is CORE_NOFILE.
     * If the type cannot be determined, the value is CORE_UNKFILE.
     */
    filetype_e filetype;
    /** The user id that owns the file */
    uid_t user;
    /** The group id that owns the file */
    gid_t group;
    /** The inode of the file. */
    ino_t inode;
    /** The id of the device the file is on. */
    dev_t device;
    /** The number of hard links to the file. */
    c_int32_t nlink;
    /** The size of the file */
    off_t size;
    /** The storage size consumed by the file */
    off_t csize;
    /** The time the file was last accessed */
    c_time_t atime;
    /** The time the file was last modified */
    c_time_t mtime;
    /** The time the file was created, or the inode was last changed */
    c_time_t ctime;
    /** The pathname of the file (possibly unrooted) */
    char fname[MAX_FILENAME_SIZE];
    /** The file's name (no path) in filesystem case */
    char name[MAX_FILENAME_SIZE];
    /** The file's handle, if accessed (can be submitted to c_duphandle) */
    struct c_file_t *filehand;
};

/** @} */

/**
 * Initialize the file utility.
 */
CORE_DECLARE(status_t) file_init(void);

/**
 * Finalize the file utility.
 */
CORE_DECLARE(status_t) file_final(void);

/**
 * Open the specified file.
 * @param newf The opened file descriptor.
 * @param fname The full path to the file (using / on all systems)
 * @param flag Or'ed value of:
 * <PRE>
 *         FILE_READ              open for reading
 *         FILE_WRITE             open for writing
 *         FILE_CREATE            create the file if not there
 *         FILE_APPEND            file ptr is set to end prior to all writes
 *         FILE_TRUNCATE          set length to zero if file exists
 *         FILE_BINARY            not a text file (This flag is ignored on
 *                               UNIX because it has no meaning)
 *         FILE_EXCL              return error if FILE_CREATE and file exists
 *         FILE_DELONCLOSE        delete the file after closing.
 * </PRE>
 * @param perm Access permissions for file.
 * @remark If perm is FILE_OS_DEFAULT and the file is being created,
 * appropriate default permissions will be used.
 */
CORE_DECLARE(status_t) file_open(file_t **newf,
        const char *fname, c_int32_t flag, file_perms_t perm);

/**
 * Close the specified file.
 * @param file The file descriptor to close.
 */
CORE_DECLARE(status_t) file_close(file_t *file);

/**
 * Delete the specified file.
 * @param path The full path to the file (using / on all systems)
 * @remark If the file is open, it won't be removed until all
 * instances are closed.
 */
CORE_DECLARE(status_t) file_remove(const char *path);

/**
 * Rename the specified file.
 * @param from_path The full path to the original file (using / on all systems)
 * @param to_path The full path to the new file (using / on all systems)
 * @warning If a file exists at the new location, then it will be
 * overwritten.  Moving files or directories across devices may not be
 * possible.
 */
CORE_DECLARE(status_t) file_rename(const char *from_path, const char *to_path);

/**
 * Create a hard link to the specified file.
 * @param from_path The full path to the original file (using / on all systems)
 * @param to_path The full path to the new file (using / on all systems)
 * @remark Both files must reside on the same device.
 */
CORE_DECLARE(status_t) file_link(const char *from_path, const char *to_path);

/**
 * Copy the specified file to another file.
 * @param from_path The full path to the original file (using / on all systems)
 * @param to_path The full path to the new file (using / on all systems)
 * @param perms Access permissions for the new file if it is created.
 *     In place of the usual or'd combination of file permissions, the
 *     value CORE_SOURCE_PERMS may be given, in which case the source
 *     file's permissions are copied.
 * @remark The new file does not need to exist, it will be created if required.
 * @warning If the new file already exists, its contents will be overwritten.
 */
CORE_DECLARE(status_t) file_copy(
    const char *from_path, const char *to_path, file_perms_t perms);

/**
 * Append the specified file to another file.
 * @param from_path The full path to the source file (use / on all systems)
 * @param to_path The full path to the destination file (use / on all systems)
 * @param perms Access permissions for the destination file if it is created.
 *     In place of the usual or'd combination of file permissions, the
 *     value CORE_SOURCE_PERMS may be given, in which case the source
 *     file's permissions are copied.
 * @remark The new file does not need to exist, it will be created if required.
 */
CORE_DECLARE(status_t) file_append(
    const char *from_path, const char *to_path, file_perms_t perms);

/**
 * Are we at the end of the file
 * @param fptr The core file we are testing.
 * @remark Returns CORE_EOF if we are at the end of file,
 *         CORE_OK otherwise.
 */
CORE_DECLARE(status_t) file_eof(file_t *fptr);

/**
 * Read data from the specified file.
 * @param thefile The file descriptor to read from.
 * @param buf The buffer to store the data to.
 * @param nbytes On entry, the number of bytes to read; on exit, the number
 * of bytes read.
 *
 * @remark file_read will read up to the specified number of
 * bytes, but never more.  If there isn't enough data to fill that
 * number of bytes, all of the available data is read.  The third
 * argument is modified to reflect the number of bytes read.  If a
 * char was put back into the stream via ungetc, it will be the first
 * character returned.
 *
 * @remark It is not possible for both bytes to be read and an CORE_EOF
 * or other error to be returned.  CORE_EINTR is never returned.
 */
CORE_DECLARE(status_t) file_read(file_t *thefile, void *buf, size_t *nbytes);

/**
 * Write data to the specified file.
 * @param thefile The file descriptor to write to.
 * @param buf The buffer which contains the data.
 * @param nbytes On entry, the number of bytes to write; on exit, the number
 *               of bytes written.
 *
 * @remark file_write will write up to the specified number of
 * bytes, but never more.  If the OS cannot write that many bytes, it
 * will write as many as it can.  The third argument is modified to
 * reflect the * number of bytes written.
 *
 * @remark It is possible for both bytes to be written and an error to
 * be returned.  CORE_EINTR is never returned.
 */
CORE_DECLARE(status_t) file_write(
        file_t *thefile, const void *buf, size_t *nbytes);

/**
 * Write data from iovec array to the specified file.
 * @param thefile The file descriptor to write to.
 * @param vec The array from which to get the data to write to the file.
 * @param nvec The number of elements in the struct iovec array. This must
 *             be smaller than MAX_IOVEC_SIZE.  If it isn't, the function
 *             will fail with CORE_EINVAL.
 * @param nbytes The number of bytes written.
 *
 * @remark It is possible for both bytes to be written and an error to
 * be returned.  CORE_EINTR is never returned.
 *
 * @remark file_writev is available even if the underlying
 * operating system doesn't provide writev().
 */
CORE_DECLARE(status_t) file_writev(file_t *thefile,
        const struct iovec *vec, size_t nvec, size_t *nbytes);

/**
 * Read data from the specified file, ensuring that the buffer is filled
 * before returning.
 * @param thefile The file descriptor to read from.
 * @param buf The buffer to store the data to.
 * @param nbytes The number of bytes to read.
 * @param bytes_read If non-NULL, this will contain the number of bytes read.
 *
 * @remark file_read will read up to the specified number of
 * bytes, but never more.  If there isn't enough data to fill that
 * number of bytes, then the process/thread will block until it is
 * available or EOF is reached.  If a char was put back into the
 * stream via ungetc, it will be the first character returned.
 *
 * @remark It is possible for both bytes to be read and an error to be
 * returned.  And if *bytes_read is less than nbytes, an accompanying
 * error is _always_ returned.
 *
 * @remark CORE_EINTR is never returned.
 */
CORE_DECLARE(status_t) file_read_full(file_t *thefile, void *buf,
        size_t nbytes, size_t *bytes_read);

/**
 * Write data to the specified file, ensuring that all of the data is
 * written before returning.
 * @param thefile The file descriptor to write to.
 * @param buf The buffer which contains the data.
 * @param nbytes The number of bytes to write.
 * @param bytes_written If non-NULL, set to the number of bytes written.
 *
 * @remark file_write will write up to the specified number of
 * bytes, but never more.  If the OS cannot write that many bytes, the
 * process/thread will block until they can be written. Exceptional
 * error such as "out of space" or "pipe closed" will terminate with
 * an error.
 *
 * @remark It is possible for both bytes to be written and an error to
 * be returned.  And if *bytes_written is less than nbytes, an
 * accompanying error is _always_ returned.
 *
 * @remark CORE_EINTR is never returned.
 */
CORE_DECLARE(status_t) file_write_full(file_t *thefile,
        const void *buf, size_t nbytes, size_t *bytes_written);


/**
 * Write data from iovec array to the specified file, ensuring that all of the
 * data is written before returning.
 * @param thefile The file descriptor to write to.
 * @param vec The array from which to get the data to write to the file.
 * @param nvec The number of elements in the struct iovec array. This must
 *             be smaller than MAX_IOVEC_SIZE.  If it isn't, the function
 *             will fail with CORE_EINVAL.
 * @param nbytes The number of bytes written.
 *
 * @remark file_writev_full is available even if the underlying
 * operating system doesn't provide writev().
 */
CORE_DECLARE(status_t) file_writev_full(file_t *thefile,
        const struct iovec *vec, size_t nvec, size_t *nbytes);
/**
 * Write a character into the specified file.
 * @param ch The character to write.
 * @param thefile The file descriptor to write to
 */
CORE_DECLARE(status_t) file_putc(char ch, file_t *thefile);

/**
 * Read a character from the specified file.
 * @param ch The character to read into
 * @param thefile The file descriptor to read from
 */
CORE_DECLARE(status_t) file_getc(char *ch, file_t *thefile);

/**
 * Put a character back onto a specified stream.
 * @param ch The character to write.
 * @param thefile The file descriptor to write to
 */
CORE_DECLARE(status_t) file_ungetc(char ch, file_t *thefile);

/**
 * Read a string from the specified file.
 * @param str The buffer to store the string in.
 * @param len The length of the string
 * @param thefile The file descriptor to read from
 * @remark The buffer will be NUL-terminated if any characters are stored.
 */
CORE_DECLARE(status_t) file_gets(char *str, int len, file_t *thefile);

/**
 * Write the string into the specified file.
 * @param str The string to write.
 * @param thefile The file descriptor to write to
 */
CORE_DECLARE(status_t) file_puts(const char *str, file_t *thefile);

/**
 * Transfer all file modified data and metadata to disk.
 * @param thefile The file descriptor to sync
 */
CORE_DECLARE(status_t) file_sync(file_t *thefile);

/**
 * Move the read/write file offset to a specified byte within a file.
 * @param thefile The file descriptor
 * @param where How to move the pointer, one of:
 * <PRE>
 *            CORE_SET  --  set the offset to offset
 *            CORE_CUR  --  add the offset to the current position
 *            CORE_END  --  add the offset to the current file size
 * </PRE>
 * @param offset The offset to move the pointer to.
 * @remark The third argument is modified to be the offset the pointer
          was actually moved to.
 */
CORE_DECLARE(status_t) file_seek(file_t *thefile,
        seek_where_t where, off_t *offset);

/**accessor and general file_io functions. */

/**
 * return the file name of the current file.
 * @param new_path The path of the file.
 * @param thefile The currently open file.
 */
CORE_DECLARE(status_t) file_name_get(const char **fname, file_t *thefile);

/**
 * set the specified file's permission bits.
 * @param fname The file (name) to apply the permissions to.
 * @param perms The permission bits to apply to the file.
 *
 * @warning Some platforms may not be able to apply all of the
 * available permission bits; CORE_INCOMPLETE will be returned if some
 * permissions are specified which could not be set.
 *
 * @warning Platforms which do not implement this feature will return
 * CORE_ENOTIMPL.
 */
CORE_DECLARE(status_t) file_perms_set(const char *fname, file_perms_t perms);

/**
 * Set attributes of the specified file.
 * @param fname The full path to the file (using / on all systems)
 * @param attributes Or'd combination of
 * <PRE>
 *            ATTR_READONLY   - make the file readonly
 *            ATTR_EXECUTABLE - make the file executable
 *            ATTR_HIDDEN     - make the file hidden
 * </PRE>
 * @param attr_mask Mask of valid bits in attributes.
 * @remark This function should be used in preference to explict manipulation
 *      of the file permissions, because the operations to provide these
 *      attributes are platform specific and may involve more than simply
 *      setting permission bits.
 * @warning Platforms which do not implement this feature will return
 *      CORE_ENOTIMPL.
 */
CORE_DECLARE(status_t) file_attrs_set(const char *fname,
        file_attrs_t attributes, file_attrs_t attr_mask);

/**
 * Set the mtime of the specified file.
 * @param fname The full path to the file (using / on all systems)
 * @param mtime The mtime to apply to the file.
 * @warning Platforms which do not implement this feature will return
 *      CORE_ENOTIMPL.
 */
CORE_DECLARE(status_t) file_mtime_set(const char *fname, c_time_t mtime);

/**
 * Create a new directory on the file system.
 * @param path the path for the directory to be created. (use / on all systems)
 * @param perm Permissions for the new direcoty.
 */
CORE_DECLARE(status_t) dir_make(const char *path, file_perms_t perm);

/** Creates a new directory on the file system, but behaves like
 * 'mkdir -p'. Creates intermediate directories as required. No error
 * will be reported if PATH already exists.
 * @param path the path for the directory to be created. (use / on all systems)
 * @param perm Permissions for the new direcoty.
 */
CORE_DECLARE(void) path_remove_last_component(char *dir, const char *path);
CORE_DECLARE(status_t) dir_make_recursive(const char *path, file_perms_t perm);

/**
 * Remove directory from the file system.
 * @param path the path for the directory to be removed. (use / on all systems)
 * @remark Removing a directory which is in-use (e.g., the current working
 * directory, or during core_dir_read, or with an open file) is not portable.
 */
CORE_DECLARE(status_t) dir_remove(const char *path);

/**
 * get the specified file's stats.
 * @param finfo Where to store the information about the file.
 * @param wanted The desired file_info_t fields, as a bit flag of
 *        CORE_FINFO_ values
 * @param thefile The file to get information about.
 */
CORE_DECLARE(status_t) file_info_get(file_info_t *finfo,
        c_int32_t wanted, file_t *thefile);


/**
 * Truncate the file's length to the specified offset
 * @param fp The file to truncate
 * @param offset The offset to truncate to.
 * @remark The read/write file offset is repositioned to offset.
 */
CORE_DECLARE(status_t) file_trunc(file_t *fp, off_t offset);

/**
 * Retrieve the flags that were passed into file_open()
 * when the file was opened.
 * @return c_int32_t the flags
 */
CORE_DECLARE(c_int32_t) file_flags_get(file_t *f);

/**
 * get the specified file's stats.  The file is specified by filename,
 * instead of using a pre-opened file.
 * @param finfo Where to store the information about the file, which is
 * never touched if the call fails.
 * @param fname The name of the file to stat.
 * @param wanted The desired file_info_t fields, as a bit flag of FILE_INFO_
                 values
 *
 * @note If @c CORE_INCOMPLETE is returned all the fields in @a finfo may
 *       not be filled in, and you need to check the @c finfo->valid bitmask
 *       to verify that what you're looking for is there.
 */
CORE_DECLARE(status_t) file_stat(file_info_t *finfo,
        const char *fname, c_int32_t wanted);

/**
 * Find an existing directory suitable as a temporary storage location.
 * @param temp_dir The temp directory.
 * @remark
 * This function uses an algorithm to search for a directory that an
 * an application can use for temporary storage.
 *
 */
CORE_DECLARE(status_t) temp_dir_get(char *temp_dir);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* ! __CORE_FILE_H__ */

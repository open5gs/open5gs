#include "core_arch_file.h"
#include "core.h"
#include "core_errno.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pool.h"

pool_declare(file_pool, file_t, MAX_NUM_OF_FILE);

status_t file_init(void)
{
    pool_init(&file_pool, MAX_NUM_OF_FILE);
    return CORE_OK;
}

status_t file_final(void)
{
    pool_final(&file_pool);
    return CORE_OK;
}

#if !defined(OS2) && !defined(WIN32)
mode_t unix_perms2mode(file_perms_t perms)
{
    mode_t mode = 0;

    if (perms & FILE_USETID)
        mode |= S_ISUID;
    if (perms & FILE_UREAD)
        mode |= S_IRUSR;
    if (perms & FILE_UWRITE)
        mode |= S_IWUSR;
    if (perms & FILE_UEXECUTE)
        mode |= S_IXUSR;

    if (perms & FILE_GSETID)
        mode |= S_ISGID;
    if (perms & FILE_GREAD)
        mode |= S_IRGRP;
    if (perms & FILE_GWRITE)
        mode |= S_IWGRP;
    if (perms & FILE_GEXECUTE)
        mode |= S_IXGRP;

#ifdef S_ISVTX
    if (perms & FILE_WSTICKY)
        mode |= S_ISVTX;
#endif
    if (perms & FILE_WREAD)
        mode |= S_IROTH;
    if (perms & FILE_WWRITE)
        mode |= S_IWOTH;
    if (perms & FILE_WEXECUTE)
        mode |= S_IXOTH;

    return mode;
}

file_perms_t unix_mode2perms(mode_t mode)
{
    file_perms_t perms = 0;

    if (mode & S_ISUID)
        perms |= FILE_USETID;
    if (mode & S_IRUSR)
        perms |= FILE_UREAD;
    if (mode & S_IWUSR)
        perms |= FILE_UWRITE;
    if (mode & S_IXUSR)
        perms |= FILE_UEXECUTE;

    if (mode & S_ISGID)
        perms |= FILE_GSETID;
    if (mode & S_IRGRP)
        perms |= FILE_GREAD;
    if (mode & S_IWGRP)
        perms |= FILE_GWRITE;
    if (mode & S_IXGRP)
        perms |= FILE_GEXECUTE;

#ifdef S_ISVTX
    if (mode & S_ISVTX)
        perms |= FILE_WSTICKY;
#endif
    if (mode & S_IROTH)
        perms |= FILE_WREAD;
    if (mode & S_IWOTH)
        perms |= FILE_WWRITE;
    if (mode & S_IXOTH)
        perms |= FILE_WEXECUTE;

    return perms;
}
#endif

status_t file_open(file_t **new,
        const char *fname, c_int32_t flag, file_perms_t perm)
{
    os_file_t fd;
    int oflags = 0;

    d_assert(fname, return CORE_ERROR,);

    if ((flag & FILE_READ) && (flag & FILE_WRITE))
    {
        oflags = O_RDWR;
    }
    else if (flag & FILE_READ)
    {
        oflags = O_RDONLY;
    }
    else if (flag & FILE_WRITE)
    {
        oflags = O_WRONLY;
    }
    else
    {
        return CORE_EACCES;
    }

    if (flag & FILE_CREATE)
    {
        oflags |= O_CREAT;
        if (flag & FILE_EXCL)
        {
            oflags |= O_EXCL;
        }
    }
    if ((flag & FILE_EXCL) && !(flag & FILE_CREATE))
    {
        return CORE_EACCES;
    }

    if (flag & FILE_APPEND)
    {
        oflags |= O_APPEND;
    }
    if (flag & FILE_TRUNCATE)
    {
        oflags |= O_TRUNC;
    }
#ifdef O_BINARY
    if (flag & FILE_BINARY)
    {
        oflags |= O_BINARY;
    }
#endif

    if (perm == FILE_OS_DEFAULT)
    {
        fd = open(fname, oflags, 0666);
    }
    else
    {
        fd = open(fname, oflags, unix_perms2mode(perm));
    }
    if (fd < 0)
    {
       return errno;
    }

    pool_alloc_node(&file_pool, &(*new));
    d_assert((*new), return CORE_ENOMEM, "file_pool(%d) is not enough\n",
            MAX_NUM_OF_FILE);

    (*new)->flags = flag;
    (*new)->filedes = fd;

    strcpy((*new)->fname, fname);

    (*new)->timeout = -1;
    (*new)->eof_hit = 0;
    (*new)->filePtr = 0;

    return CORE_OK;
}

status_t file_close(file_t *file)
{
    status_t rv = CORE_OK;

    d_assert(file, return CORE_ERROR,);

    if (close(file->filedes) == 0)
    {
        file->filedes = -1;

        /* Only the parent process should delete the file! */
        if (file->flags & FILE_DELONCLOSE)
        {
            unlink(file->fname);
        }
    }
    else
    {
        /* Are there any error conditions other than EINTR or EBADF? */
        rv = errno;
    }

    pool_free_node(&file_pool, file);

    return rv;
}

status_t file_remove(const char *path)
{
    d_assert(path, return CORE_ERROR,);

    if (unlink(path) == 0)
    {
        return CORE_OK;
    }
    else
    {
        return errno;
    }
}

static status_t file_transfer_contents(
        const char *from_path, const char *to_path,
        c_int32_t flags, file_perms_t to_perms)
{
    file_t *s, *d;
    status_t status;
    file_info_t finfo;
    file_perms_t perms;

    d_assert(from_path, return CORE_ERROR,);
    d_assert(to_path, return CORE_ERROR,);

    /* Open source file. */
    status = file_open(&s, from_path, FILE_READ, FILE_OS_DEFAULT);
    if (status)
        return status;

    /* Maybe get its permissions. */
    if (to_perms == FILE_SOURCE_PERMS)
    {
        status = file_info_get(&finfo, FILE_INFO_PROT, s);
        if (status != CORE_OK && status != CORE_INCOMPLETE)
        {
            file_close(s);  /* toss any error */
            return status;
        }
        perms = finfo.protection;
    }
    else
        perms = to_perms;

    /* Open dest file. */
    status = file_open(&d, to_path, flags, perms);
    if (status)
    {
        file_close(s);  /* toss any error */
        return status;
    }

#if BUFSIZ > FILE_DEFAULT_BUFSIZE
#define COPY_BUFSIZ BUFSIZ
#else
#define COPY_BUFSIZ FILE_DEFAULT_BUFSIZE
#endif

    /* Copy bytes till the cows come home. */
    while (1)
    {
        char buf[COPY_BUFSIZ];
        size_t bytes_this_time = sizeof(buf);
        status_t read_err;
        status_t write_err;

        /* Read 'em. */
        read_err = file_read(s, buf, &bytes_this_time);
        if (read_err && !STATUS_IS_EOF(read_err))
        {
            file_close(s);  /* toss any error */
            file_close(d);  /* toss any error */
            return read_err;
        }

        /* Write 'em. */
        write_err = file_write_full(d, buf, bytes_this_time, NULL);
        if (write_err)
        {
            file_close(s);  /* toss any error */
            file_close(d);  /* toss any error */
            return write_err;
        }

        if (read_err && STATUS_IS_EOF(read_err))
        {
            status = file_close(s);
            if (status)
            {
                file_close(d);  /* toss any error */
                return status;
            }

            /* return the results of this close: an error, or success */
            return file_close(d);
        }
    }
    /* NOTREACHED */
}

status_t file_rename(const char *from_path, const char *to_path)
{
    d_assert(from_path, return CORE_ERROR,);
    d_assert(to_path, return CORE_ERROR,);

    if (rename(from_path, to_path) != 0)
    {
        return errno;
    }
    return CORE_OK;
}

status_t file_link(const char *from_path, const char *to_path)
{
    d_assert(from_path, return CORE_ERROR,);
    d_assert(to_path, return CORE_ERROR,);

    if (link(from_path, to_path) == -1)
    {
        return errno;
    }

    return CORE_OK;
}

status_t file_copy(
    const char *from_path, const char *to_path, file_perms_t perms)
{
    return file_transfer_contents(from_path, to_path,
              (FILE_WRITE | FILE_CREATE | FILE_TRUNCATE), perms);
}

status_t file_append(
    const char *from_path, const char *to_path, file_perms_t perms)
{
    return file_transfer_contents(from_path, to_path,
              (FILE_WRITE | FILE_CREATE | FILE_APPEND), perms);
}

status_t file_eof(file_t *fptr)
{
    d_assert(fptr, return CORE_ERROR,);

    if (fptr->eof_hit == 1)
    {
        return CORE_EOF;
    }
    return CORE_OK;
}

status_t file_read(file_t *thefile, void *buf, size_t *nbytes)
{
    ssize_t rv;
    size_t bytes_read;

    d_assert(thefile, return CORE_ERROR,);
    d_assert(nbytes, return CORE_ERROR,);

    if (*nbytes <= 0)
    {
        *nbytes = 0;
        return CORE_OK;
    }

    bytes_read = 0;

    do
    {
        rv = read(thefile->filedes, buf, *nbytes);
    } while (rv == -1 && errno == EINTR);
    *nbytes = bytes_read;
    if (rv == 0)
    {
        thefile->eof_hit = TRUE;
        return CORE_EOF;
    }
    if (rv > 0)
    {
        *nbytes += rv;
        return CORE_OK;
    }
    return errno;
}

status_t file_write(
        file_t *thefile, const void *buf, size_t *nbytes)
{
    size_t rv;

    d_assert(thefile, return CORE_ERROR,);
    d_assert(buf, return CORE_ERROR,);
    d_assert(nbytes, return CORE_ERROR,);

    do
    {
        rv = write(thefile->filedes, buf, *nbytes);
    } while (rv == (size_t)-1 && errno == EINTR);

    if (rv == (size_t)-1)
    {
        (*nbytes) = 0;
        return errno;
    }
    *nbytes = rv;
    return CORE_OK;
}
status_t file_writev(file_t *thefile,
        const struct iovec *vec, size_t nvec, size_t *nbytes)
{
#ifdef HAVE_WRITEV
    status_t rv;
    ssize_t bytes;

    d_assert(thefile, return CORE_ERROR,);
    d_assert(vec, return CORE_ERROR,);
    d_assert(nvec, return CORE_ERROR,);
    d_assert(nbytes, return CORE_ERROR,);

    if ((bytes = writev(thefile->filedes, vec, nvec)) < 0)
    {
        *nbytes = 0;
        rv = errno;
    }
    else
    {
        *nbytes = bytes;
        rv = CORE_OK;
    }
    return rv;
#else
    /**
     * The problem with trying to output the entire iovec is that we cannot
     * maintain the behaviour that a real writev would have.  If we iterate
     * over the iovec one at a time, we lose the atomic properties of
     * writev().  The other option is to combine the entire iovec into one
     * buffer that we could then send in one call to write().  This is not
     * reasonable since we do not know how much data an iovec could contain.
     *
     * The only reasonable option, that maintains the semantics of a real
     * writev(), is to only write the first iovec.  Callers of file_writev()
     * must deal with partial writes as they normally would. If you want to
     * ensure an entire iovec is written, use file_writev_full().
     */

    *nbytes = vec[0].iov_len;
    return file_write(thefile, vec[0].iov_base, nbytes);
#endif
}

status_t file_read_full(file_t *thefile, void *buf,
        size_t nbytes, size_t *bytes_read)
{
    status_t status;
    size_t total_read = 0;

    d_assert(thefile, return CORE_ERROR,);
    d_assert(buf, return CORE_ERROR,);

    do {
        size_t amt = nbytes;

        status = file_read(thefile, buf, &amt);
        buf = (char *)buf + amt;
        nbytes -= amt;
        total_read += amt;
    } while (status == CORE_OK && nbytes > 0);

    if (bytes_read != NULL)
        *bytes_read = total_read;

    return status;
}
status_t file_write_full(file_t *thefile,
        const void *buf, size_t nbytes, size_t *bytes_written)
{
    status_t status;
    size_t total_written = 0;

    d_assert(thefile, return CORE_ERROR,);
    d_assert(buf, return CORE_ERROR,);

    do {
        size_t amt = nbytes;

        status = file_write(thefile, buf, &amt);
        buf = (char *)buf + amt;
        nbytes -= amt;
        total_written += amt;
    } while (status == CORE_OK && nbytes > 0);

    if (bytes_written != NULL)
        *bytes_written = total_written;

    return status;
}

status_t file_writev_full(file_t *thefile,
        const struct iovec *vec, size_t nvec, size_t *bytes_written)
{
    status_t rv = CORE_OK;
    size_t i;
    size_t amt = 0;
    size_t total = 0;

    d_assert(thefile, return CORE_ERROR,);
    d_assert(vec, return CORE_ERROR,);
    d_assert(nvec, return CORE_ERROR,);
    d_assert(bytes_written, return CORE_ERROR,);

    for (i = 0; i < nvec; i++)
    {
        total += vec[i].iov_len;
    }

    rv = file_writev(thefile, vec, nvec, &amt);

    if (bytes_written != NULL)
        *bytes_written = amt;

    if (rv != CORE_OK || (amt == total))
    {
        return rv;
    }

    for (i = 0; i < nvec && amt; i++)
    {
        if (amt >= vec[i].iov_len)
        {
            amt -= vec[i].iov_len;
        }
        else
        {
            break;
        }
    }

    if (amt)
    {
        rv = file_write_full(thefile, (const char *)vec[i].iov_base + amt,
                                 vec[i].iov_len - amt, NULL);
    }

    for (; i < nvec && rv == CORE_OK; i++)
    {
        rv = file_write_full(thefile, vec[i].iov_base,
                                 vec[i].iov_len, &amt);
    }

    if (bytes_written != NULL)
        *bytes_written = total;

    return rv;
}

status_t file_putc(char ch, file_t *thefile)
{
    size_t nbytes = 1;

    d_assert(thefile, return CORE_ERROR,);

    return file_write(thefile, &ch, &nbytes);
}
status_t file_getc(char *ch, file_t *thefile)
{
    size_t nbytes = 1;

    d_assert(thefile, return CORE_ERROR,);

    return file_read(thefile, ch, &nbytes);
}

status_t file_gets(char *str, int len, file_t *thefile)
{
    status_t rv = CORE_OK; /* get rid of gcc warning */
    size_t nbytes;
    const char *str_start = str;
    char *final = str + len - 1;

    d_assert(str, return CORE_ERROR,);
    d_assert(thefile, return CORE_ERROR,);

    if (len <= 1)
    {
        /* sort of like fgets(), which returns NULL and stores no bytes
         */
        return CORE_OK;
    }

    while (str < final)
    { /* leave room for trailing '\0' */
        nbytes = 1;
        rv = file_read(thefile, str, &nbytes);
        if (rv != CORE_OK)
        {
            break;
        }
        if (*str == '\n')
        {
            ++str;
            break;
        }
        ++str;
    }

    /* We must store a terminating '\0' if we've stored any chars. We can
     * get away with storing it if we hit an error first.
     */
    *str = '\0';
    if (str > str_start)
    {
        /* we stored chars; don't report EOF or any other errors;
         * the app will find out about that on the next call
         */
        return CORE_OK;
    }
    return rv;
}

status_t file_puts(const char *str, file_t *thefile)
{
    d_assert(str, return CORE_ERROR,);
    d_assert(thefile, return CORE_ERROR,);

    return file_write_full(thefile, str, strlen(str), NULL);
}

status_t file_sync(file_t *thefile)
{
    status_t rv = CORE_OK;

    d_assert(thefile, return CORE_ERROR,);

    if (fsync(thefile->filedes))
    {
        rv = get_os_error();
    }

    return rv;
}

status_t file_seek(file_t *thefile,
        seek_where_t where, off_t *offset)
{
    off_t rv;

    d_assert(thefile, return CORE_ERROR,);
    d_assert(offset, return CORE_ERROR,);

    thefile->eof_hit = 0;

    rv = lseek(thefile->filedes, *offset, where);
    if (rv == -1)
    {
        *offset = -1;
        return errno;
    }
    else
    {
        *offset = rv;
        return CORE_OK;
    }
}

status_t file_name_get(const char **fname, file_t *thefile)
{
    d_assert(fname, return CORE_ERROR,);
    d_assert(thefile, return CORE_ERROR,);

    *fname = thefile->fname;
    return CORE_OK;
}

status_t file_perms_set(const char *fname, file_perms_t perms)
{
    mode_t mode = unix_perms2mode(perms);

    d_assert(fname, return CORE_ERROR,);

    if (chmod(fname, mode) == -1)
        return errno;
    return CORE_OK;
}

status_t file_attrs_set(const char *fname,
        file_attrs_t attributes, file_attrs_t attr_mask)
{
    status_t status;
    file_info_t finfo;

    d_assert(fname, return CORE_ERROR,);

    /* Don't do anything if we can't handle the requested attributes */
    if (!(attr_mask & (ATTR_READONLY | ATTR_EXECUTABLE)))
        return CORE_OK;

    status = file_stat(&finfo, fname, FILE_INFO_PROT);
    if (status)
        return status;

    /* ### TODO: should added bits be umask'd? */
    if (attr_mask & ATTR_READONLY)
    {
        if (attributes & ATTR_READONLY)
        {
            finfo.protection &= ~FILE_UWRITE;
            finfo.protection &= ~FILE_GWRITE;
            finfo.protection &= ~FILE_WWRITE;
        }
        else
        {
            /* ### umask this! */
            finfo.protection |= FILE_UWRITE;
            finfo.protection |= FILE_GWRITE;
            finfo.protection |= FILE_WWRITE;
        }
    }

    if (attr_mask & ATTR_EXECUTABLE)
    {
        if (attributes & ATTR_EXECUTABLE)
        {
            /* ### umask this! */
            finfo.protection |= FILE_UEXECUTE;
            finfo.protection |= FILE_GEXECUTE;
            finfo.protection |= FILE_WEXECUTE;
        }
        else
        {
            finfo.protection &= ~FILE_UEXECUTE;
            finfo.protection &= ~FILE_GEXECUTE;
            finfo.protection &= ~FILE_WEXECUTE;
        }
    }

    return file_perms_set(fname, finfo.protection);
}

status_t file_mtime_set(const char *fname, c_time_t mtime)
{
    status_t status;
    file_info_t finfo;

    d_assert(fname, return CORE_ERROR,);

    status = file_stat(&finfo, fname, FILE_INFO_ATIME);
    if (status)
    {
        return status;
    }

#ifdef HAVE_UTIMES
    {
      struct timeval tvp[2];

      tvp[0].tv_sec = time_sec(finfo.atime);
      tvp[0].tv_usec = time_usec(finfo.atime);
      tvp[1].tv_sec = time_sec(mtime);
      tvp[1].tv_usec = time_usec(mtime);

      if (utimes(fname, tvp) == -1)
      {
        return errno;
      }
    }
#elif defined(HAVE_UTIME)
    {
      struct utimbuf buf;

      buf.actime = (time_t) (finfo.atime / USEC_PER_SEC);
      buf.modtime = (time_t) (mtime / USEC_PER_SEC);

      if (utime(fname, &buf) == -1)
      {
        return errno;
      }
    }
#else
    return CORE_ENOTIMPL;
#endif

    return CORE_OK;
}

status_t dir_make(const char *path, file_perms_t perm)
{
    mode_t mode = unix_perms2mode(perm);

    d_assert(path, return CORE_ERROR,);

    if (mkdir(path, mode) == 0)
    {
        return CORE_OK;
    }
    else
    {
        return errno;
    }
}

#define PATH_SEPARATOR '/'

/* Remove trailing separators that don't affect the meaning of PATH. */
static void path_canonicalize (char *dir)
{
    d_assert(dir, return,);

    /* At some point this could eliminate redundant components.  For
     * now, it just makes sure there is no trailing slash. */
    size_t len = strlen (dir);
    size_t orig_len = len;

    while ((len > 0) && (dir[len - 1] == PATH_SEPARATOR))
        len--;

    if (len != orig_len)
        dir[len] = 0;
}

/* Remove one component off the end of PATH. */
void path_remove_last_component (char *dir, const char *path)
{
    int i;
    int len = 0;

    d_assert(dir, return,);
    d_assert(path, return,);

    strcpy(dir, path);
    path_canonicalize (dir);
    for (i = (strlen(dir) - 1); i >= 0; i--) {
        if (path[i] == PATH_SEPARATOR)
            break;
    }

    len = (i < 0) ? 0 : i;
    dir[len] = 0;
}

status_t dir_make_recursive(const char *path, file_perms_t perm)
{
    status_t err = 0;

    d_assert(path, return CORE_ERROR,);

    err = dir_make(path, perm); /* Try to make PATH right out */

    if (err == EEXIST) /* It's OK if PATH exists */
        return CORE_OK;

    if (err == ENOENT) /* Missing an intermediate dir */
    {
        char dir[MAX_DIRNAME_SIZE];

        path_remove_last_component(dir, path);
        /* If there is no path left, give up. */
        if (dir[0] == '\0')
        {
            return err;
        }

        err = dir_make_recursive(dir, perm);

        if (!err)
            err = dir_make (path, perm);
    }

    return err;
}
status_t dir_remove(const char *path)
{
    d_assert(path, return CORE_ERROR,);

    if (rmdir(path) == 0)
    {
        return CORE_OK;
    }
    else {
        return errno;
    }
}

static filetype_e filetype_from_mode(mode_t mode)
{
    filetype_e type;

    switch (mode & S_IFMT)
    {
        case S_IFREG:
            type = FILE_REG;  break;
        case S_IFDIR:
            type = FILE_DIR;  break;
        case S_IFLNK:
            type = FILE_LNK;  break;
        case S_IFCHR:
            type = FILE_CHR;  break;
        case S_IFBLK:
            type = FILE_BLK;  break;
#if defined(S_IFFIFO)
        case S_IFFIFO:
            type = FILE_PIPE; break;
#endif
#if !defined(BEOS) && defined(S_IFSOCK)
        case S_IFSOCK:
            type = FILE_SOCK; break;
#endif

        default:
        /* Work around missing S_IFxxx values above
             * for Linux et al.
             */
#if !defined(S_IFFIFO) && defined(S_ISFIFO)
            if (S_ISFIFO(mode))
            {
                type = FILE_PIPE;
            }
            else
#endif
#if !defined(BEOS) && !defined(S_IFSOCK) && defined(S_ISSOCK)
            if (S_ISSOCK(mode))
            {
                type = FILE_SOCK;
            } else
#endif
                type = FILE_UNKFILE;
    }
    return type;
}

static void fill_out_finfo(file_info_t *finfo, struct_stat *info,
                           c_int32_t wanted)
{
    d_assert(finfo, return,);
    d_assert(info, return,);

    finfo->valid = FILE_INFO_MIN | FILE_INFO_IDENT | FILE_INFO_NLINK
                 | FILE_INFO_OWNER | FILE_INFO_PROT;
    finfo->protection = unix_mode2perms(info->st_mode);
    finfo->filetype = filetype_from_mode(info->st_mode);
    finfo->user = info->st_uid;
    finfo->group = info->st_gid;
    finfo->size = info->st_size;
    finfo->device = info->st_dev;
    finfo->nlink = info->st_nlink;

    /* Check for overflow if storing a 64-bit st_ino in a 32-bit
     * ino_t for LFS builds: */
    if (sizeof(ino_t) >= sizeof(info->st_ino)
        || (ino_t)info->st_ino == info->st_ino)
    {
        finfo->inode = info->st_ino;
    } else
    {
        finfo->valid &= ~FILE_INFO_INODE;
    }

    time_ansi_put(&finfo->atime, info->st_atime);
#ifdef HAVE_STRUCT_STAT_ST_ATIM_TV_NSEC
    finfo->atime += info->st_atim.tv_nsec / TIME_C(1000);
#elif defined(HAVE_STRUCT_STAT_ST_ATIMENSEC)
    finfo->atime += info->st_atimensec / TIME_C(1000);
#elif defined(HAVE_STRUCT_STAT_ST_ATIME_N)
    finfo->ctime += info->st_atime_n / TIME_C(1000);
#endif

    time_ansi_put(&finfo->mtime, info->st_mtime);
#ifdef HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC
    finfo->mtime += info->st_mtim.tv_nsec / TIME_C(1000);
#elif defined(HAVE_STRUCT_STAT_ST_MTIMENSEC)
    finfo->mtime += info->st_mtimensec / TIME_C(1000);
#elif defined(HAVE_STRUCT_STAT_ST_MTIME_N)
    finfo->ctime += info->st_mtime_n / TIME_C(1000);
#endif

    time_ansi_put(&finfo->ctime, info->st_ctime);
#ifdef HAVE_STRUCT_STAT_ST_CTIM_TV_NSEC
    finfo->ctime += info->st_ctim.tv_nsec / TIME_C(1000);
#elif defined(HAVE_STRUCT_STAT_ST_CTIMENSEC)
    finfo->ctime += info->st_ctimensec / TIME_C(1000);
#elif defined(HAVE_STRUCT_STAT_ST_CTIME_N)
    finfo->ctime += info->st_ctime_n / TIME_C(1000);
#endif

#ifdef HAVE_STRUCT_STAT_ST_BLOCKS
#ifdef DEV_BSIZE
    finfo->csize = (off_t)info->st_blocks * (off_t)DEV_BSIZE;
#else
    finfo->csize = (off_t)info->st_blocks * (off_t)512;
#endif
    finfo->valid |= FILE_INFO_CSIZE;
#endif
}


status_t file_info_get(file_info_t *finfo,
        c_int32_t wanted, file_t *thefile)
{
    struct_stat info;

    d_assert(finfo, return CORE_ERROR,);
    d_assert(thefile, return CORE_ERROR,);

    if (fstat(thefile->filedes, &info) == 0)
    {
        strcpy(finfo->fname, thefile->fname);
        fill_out_finfo(finfo, &info, wanted);
        return (wanted & ~finfo->valid) ? CORE_INCOMPLETE : CORE_OK;
    }
    else {
        return errno;
    }
}

status_t file_trunc(file_t *fp, off_t offset)
{
    d_assert(fp, return CORE_ERROR,);

    if (ftruncate(fp->filedes, offset) == -1)
    {
        return errno;
    }
    return file_seek(fp, FILE_SET, &offset);
}

c_int32_t file_flags_get(file_t *f)
{
    d_assert(f, return CORE_ERROR,);

    return f->flags;
}

status_t file_stat(file_info_t *finfo,
        const char *fname, c_int32_t wanted)
{
    struct_stat info;
    int srv;

    d_assert(finfo, return CORE_ERROR,);
    d_assert(fname, return CORE_ERROR,);

    if (wanted & FILE_INFO_LINK)
        srv = lstat(fname, &info);
    else
        srv = stat(fname, &info);

    if (srv == 0)
    {
        strcpy(finfo->fname, fname);
        fill_out_finfo(finfo, &info, wanted);
        if (wanted & FILE_INFO_LINK)
            wanted &= ~FILE_INFO_LINK;
        return (wanted & ~finfo->valid) ? CORE_INCOMPLETE : CORE_OK;
    }
    else
    {
#if !defined(ENOENT) || !defined(ENOTDIR)
#error ENOENT || ENOTDIR not defined; please see the
#error comments at this line in the source for a workaround.
        /*
         * If ENOENT || ENOTDIR is not defined in one of the your OS's
         * include files, CORE cannot report a good reason why the stat()
         * of the file failed; there are cases where it can fail even though
         * the file exists.  This opens holes in Apache, for example, because
         * it becomes possible for someone to get a directory listing of a
         * directory even though there is an index (eg. index.html) file in
         * it.  If you do not have a problem with this, delete the above
         * #error lines and start the compile again.  If you need to do this,
         * please submit a bug report to http://www.apache.org/bug_report.html
         * letting us know that you needed to do this.  Please be sure to
         * include the operating system you are using.
         */
        /* WARNING: All errors will be handled as not found
         */
#if !defined(ENOENT)
        return CORE_ENOENT;
#else
        /* WARNING: All errors but not found will be handled as not directory
         */
        if (errno != ENOENT)
            return CORE_ENOENT;
        else
            return errno;
#endif
#else /* All was defined well, report the usual: */
        return errno;
#endif
    }
}

status_t temp_dir_get(char *temp_dir)
{
    d_assert(temp_dir, return CORE_ERROR,);

    strcpy(temp_dir, "/tmp");
    return CORE_OK;
}


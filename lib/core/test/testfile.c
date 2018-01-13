#include "core_file.h"
#include "core_pkbuf.h"
#include "testutil.h"

#define DIRNAME "data"
#define FILENAME DIRNAME "/file_datafile.txt"
#define TESTSTR  "This is the file data file."

#define TESTREAD_BLKSIZE 1024
#define FILE_BUFFERSIZE   4096 /* This should match FILE's buffer size. */


static void test_file_init(abts_case *tc, void *data)
{
    status_t rv;
    file_info_t finfo;
    size_t bytes;
    file_t *filetest = NULL;

    rv = dir_make("data", FILE_UREAD | FILE_UWRITE | FILE_UEXECUTE);

    rv = file_stat(&finfo, "data", FILE_INFO_TYPE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, FILE_DIR, finfo.filetype);

    rv = file_open(&filetest, FILENAME,
                       FILE_WRITE | FILE_CREATE,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    bytes = strlen(TESTSTR);
    rv = file_write(filetest, TESTSTR, &bytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    file_close(filetest);

}

static void test_open_noreadwrite(abts_case *tc, void *data)
{
    status_t rv;
    file_t *thefile = NULL;

    rv = file_open(&thefile, FILENAME,
                       FILE_CREATE | FILE_EXCL,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_TRUE(tc, rv != CORE_OK);
    ABTS_INT_EQUAL(tc, 1, STATUS_IS_EACCES(rv));
    ABTS_PTR_EQUAL(tc, NULL, thefile);
}

static void test_open_excl(abts_case *tc, void *data)
{
    status_t rv;
    file_t *thefile = NULL;

    rv = file_open(&thefile, FILENAME,
                       FILE_CREATE | FILE_EXCL | FILE_WRITE,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_TRUE(tc, rv != CORE_OK);
    ABTS_INT_EQUAL(tc, 1, STATUS_IS_EEXIST(rv));
    ABTS_PTR_EQUAL(tc, NULL, thefile);
}

static void test_open_read(abts_case *tc, void *data)
{
    status_t rv;
    file_t *filetest = NULL;

    rv = file_open(&filetest, FILENAME,
                       FILE_READ,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_PTR_NOTNULL(tc, filetest);
    file_close(filetest);
}

static void link_existing(abts_case *tc, void *data)
{
    status_t rv;

    rv = file_link("data/file_datafile.txt", "data/file_datafile2.txt");
    file_remove("data/file_datafile2.txt");
    ABTS_ASSERT(tc, "Couldn't create hardlink to file", rv == CORE_OK);
}

static void link_nonexisting(abts_case *tc, void *data)
{
    status_t rv;

    rv = file_link("data/does_not_exist.txt", "data/fake.txt");
    ABTS_ASSERT(tc, "", rv != CORE_OK);
}

static void test_read(abts_case *tc, void *data)
{
    status_t rv;
    size_t nbytes = 256;
    char *str = core_malloc(nbytes + 1);
    file_t *filetest = NULL;

    rv = file_open(&filetest, FILENAME,
                       FILE_READ,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);

    CORE_ASSERT_OK(tc, "Opening test file " FILENAME, rv);
    rv = file_read(filetest, str, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, strlen(TESTSTR), nbytes);
    ABTS_STR_EQUAL(tc, TESTSTR, str);

    CORE_FREE(str);

    file_close(filetest);
}

static void test_readzero(abts_case *tc, void *data)
{
    status_t rv;
    size_t nbytes = 0;
    char *str = NULL;
    file_t *filetest;

    rv = file_open(&filetest, FILENAME, FILE_READ, FILE_OS_DEFAULT);
    CORE_ASSERT_OK(tc, "Opening test file " FILENAME, rv);

    rv = file_read(filetest, str, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, 0, nbytes);

    file_close(filetest);
}

static void test_filename(abts_case *tc, void *data)
{
    const char *str;
    status_t rv;
    file_t *filetest = NULL;

    rv = file_open(&filetest, FILENAME,
                       FILE_READ,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    CORE_ASSERT_OK(tc, "Opening test file " FILENAME, rv);

    rv = file_name_get(&str, filetest);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, FILENAME, str);

    file_close(filetest);
}

static void test_fileclose(abts_case *tc, void *data)
{
    char str;
    status_t rv;
    size_t one = 1;
    file_t *filetest = NULL;

    rv = file_open(&filetest, FILENAME,
                       FILE_READ,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    CORE_ASSERT_OK(tc, "Opening test file " FILENAME, rv);

    rv = file_close(filetest);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    /* We just closed the file, so this should fail */
    rv = file_read(filetest, &str, &one);
    ABTS_INT_EQUAL(tc, 1, STATUS_IS_EBADF(rv));
}

static void test_file_remove(abts_case *tc, void *data)
{
    status_t rv;
    file_t *filetest = NULL;

    rv = file_remove(FILENAME);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_open(&filetest, FILENAME, FILE_READ,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_INT_EQUAL(tc, 1, STATUS_IS_ENOENT(rv));
}

static void test_open_write(abts_case *tc, void *data)
{
    status_t rv;
    file_t *filetest = NULL;

    filetest = NULL;
    rv = file_open(&filetest, FILENAME,
                       FILE_WRITE,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_INT_EQUAL(tc, 1, STATUS_IS_ENOENT(rv));
    ABTS_PTR_EQUAL(tc, NULL, filetest);
}

static void test_open_writecreate(abts_case *tc, void *data)
{
    status_t rv;
    file_t *filetest = NULL;

    filetest = NULL;
    rv = file_open(&filetest, FILENAME,
                       FILE_WRITE | FILE_CREATE,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    file_close(filetest);
}

static void test_write(abts_case *tc, void *data)
{
    status_t rv;
    size_t bytes = strlen(TESTSTR);
    file_t *filetest = NULL;

    rv = file_open(&filetest, FILENAME,
                       FILE_WRITE | FILE_CREATE,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_write(filetest, TESTSTR, &bytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    file_close(filetest);
}

static void test_open_readwrite(abts_case *tc, void *data)
{
    status_t rv;
    file_t *filetest = NULL;

    filetest = NULL;
    rv = file_open(&filetest, FILENAME,
                       FILE_READ | FILE_WRITE,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_PTR_NOTNULL(tc, filetest);

    file_close(filetest);
}

static void test_seek(abts_case *tc, void *data)
{
    status_t rv;
    off_t offset = 5;
    size_t nbytes = 256;
    char *str = core_malloc(nbytes + 1);
    file_t *filetest = NULL;

    rv = file_open(&filetest, FILENAME,
                       FILE_READ,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    CORE_ASSERT_OK(tc, "Open test file " FILENAME, rv);

    rv = file_read(filetest, str, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, strlen(TESTSTR), nbytes);
    ABTS_STR_EQUAL(tc, TESTSTR, str);

    memset(str, 0, nbytes + 1);

    rv = file_seek(filetest, SEEK_SET, &offset);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_read(filetest, str, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, strlen(TESTSTR) - 5, nbytes);
    ABTS_STR_EQUAL(tc, TESTSTR + 5, str);

    file_close(filetest);

    /* Test for regression of sign error bug with SEEK_END and
       buffered files. */
    rv = file_open(&filetest, FILENAME,
                       FILE_READ,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    CORE_ASSERT_OK(tc, "Open test file " FILENAME, rv);

    offset = -5;
    rv = file_seek(filetest, SEEK_END, &offset);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, strlen(TESTSTR) - 5, nbytes);

    memset(str, 0, nbytes + 1);
    nbytes = 256;
    rv = file_read(filetest, str, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, 5, nbytes);
    ABTS_STR_EQUAL(tc, TESTSTR + strlen(TESTSTR) - 5, str);

    CORE_FREE(str);

    file_close(filetest);
}

static void test_getc(abts_case *tc, void *data)
{
    file_t *f = NULL;
    status_t rv;
    char ch;

    rv = file_open(&f, FILENAME, FILE_READ, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    file_getc(&ch, f);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, (int)TESTSTR[0], (int)ch);
    file_close(f);
}

static void test_gets(abts_case *tc, void *data)
{
    file_t *f = NULL;
    status_t rv;
    char *str = core_malloc(256);

    rv = file_open(&f, FILENAME, FILE_READ, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_gets(str, 256, f);
    /* Only one line in the test file, so FILE will encounter EOF on the first
     * call to gets, but we should get CORE_OK on this call and
     * CORE_EOF on the next.
     */
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, TESTSTR, str);
    rv = file_gets(str, 256, f);
    ABTS_INT_EQUAL(tc, CORE_EOF, rv);
    ABTS_STR_EQUAL(tc, "", str);

    CORE_FREE(str);
    file_close(f);
}

static void test_bigread(abts_case *tc, void *data)
{
    file_t *f = NULL;
    status_t rv;
    char buf[FILE_BUFFERSIZE * 2];
    size_t nbytes;

    /* Create a test file with known content.
     */
    rv = file_open(&f, "data/created_file",
                       FILE_CREATE | FILE_WRITE | FILE_TRUNCATE,
                       FILE_UREAD | FILE_UWRITE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    nbytes = FILE_BUFFERSIZE;
    memset(buf, 0xFE, nbytes);

    rv = file_write(f, buf, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, FILE_BUFFERSIZE, nbytes);

    rv = file_close(f);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    f = NULL;
    rv = file_open(&f, "data/created_file", FILE_READ, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    nbytes = sizeof buf;
    rv = file_read(f, buf, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, FILE_BUFFERSIZE, nbytes);

    rv = file_close(f);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_remove("data/created_file");
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

/* This is a horrible name for this function.  We are testing FILE, not how
 * Apache uses FILE.  And, this function tests _way_ too much stuff.
 */
static void test_mod_neg(abts_case *tc, void *data)
{
    status_t rv;
    file_t *f;
    const char *s;
    int i;
    size_t nbytes;
    char buf[8192];
    off_t cur;
    const char *fname = "data/modneg.dat";

    rv = file_open(&f, fname,
                       FILE_CREATE | FILE_WRITE, FILE_UREAD | FILE_UWRITE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    s = "body56789\n";
    nbytes = strlen(s);
    rv = file_write(f, s, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, strlen(s), nbytes);

    for (i = 0; i < 7980; i++) {
        s = "0";
        nbytes = strlen(s);
        rv = file_write(f, s, &nbytes);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);
        ABTS_SIZE_EQUAL(tc, strlen(s), nbytes);
    }

    s = "end456789\n";
    nbytes = strlen(s);
    rv = file_write(f, s, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, strlen(s), nbytes);

    for (i = 0; i < 10000; i++) {
        s = "1";
        nbytes = strlen(s);
        rv = file_write(f, s, &nbytes);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);
        ABTS_SIZE_EQUAL(tc, strlen(s), nbytes);
    }

    rv = file_close(f);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_open(&f, fname, FILE_READ, 0);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_gets(buf, 11, f);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "body56789\n", buf);

    cur = 0;
    rv = file_seek(f, FILE_CUR, &cur);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_ASSERT(tc, "File Pointer Mismatch, expected 10", cur == 10);

    nbytes = sizeof(buf);
    rv = file_read(f, buf, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, nbytes, sizeof(buf));

    cur = -((off_t)nbytes - 7980);
    rv = file_seek(f, FILE_CUR, &cur);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_ASSERT(tc, "File Pointer Mismatch, expected 7990", cur == 7990);

    rv = file_gets(buf, 11, f);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_STR_EQUAL(tc, "end456789\n", buf);

    rv = file_close(f);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_remove(fname);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

/* Test that the contents of file FNAME are equal to data EXPECT of
 * length EXPECTLEN. */
static void file_contents_equal(abts_case *tc,
                                const char *fname,
                                const void *expect,
                                size_t expectlen)
{
    void *actual = core_malloc(expectlen);
    file_t *f;

    CORE_ASSERT_OK(tc, "open file",
                       file_open(&f, fname, FILE_READ,
                                     0));
    CORE_ASSERT_OK(tc, "read from file",
                       file_read_full(f, actual, expectlen, NULL));

    ABTS_ASSERT(tc, "matched expected file contents",
                memcmp(expect, actual, expectlen) == 0);

    CORE_ASSERT_OK(tc, "close file", file_close(f));

    CORE_FREE(actual);
}

#define LINE1 "this is a line of text\n"
#define LINE2 "this is a second line of text\n"

static void test_puts(abts_case *tc, void *data)
{
    file_t *f;
    const char *fname = "data/testputs.txt";

    CORE_ASSERT_OK(tc, "open file for writing",
                       file_open(&f, fname,
                                     FILE_WRITE|FILE_CREATE|FILE_TRUNCATE,
                                     FILE_OS_DEFAULT));
    CORE_ASSERT_OK(tc, "write line to file",
                       file_puts(LINE1, f));
    CORE_ASSERT_OK(tc, "write second line to file",
                       file_puts(LINE2, f));

    CORE_ASSERT_OK(tc, "close for writing",
                       file_close(f));

    file_contents_equal(tc, fname, LINE1 LINE2, strlen(LINE1 LINE2));
}

static void test_writev(abts_case *tc, void *data)
{
    file_t *f;
    size_t nbytes;
    struct iovec vec[5];
    const char *fname = "data/testwritev.txt";

    CORE_ASSERT_OK(tc, "open file for writing",
                       file_open(&f, fname,
                                     FILE_WRITE|FILE_CREATE|FILE_TRUNCATE,
                                     FILE_OS_DEFAULT));

    vec[0].iov_base = LINE1;
    vec[0].iov_len = strlen(LINE1);

    CORE_ASSERT_OK(tc, "writev of size 1 to file",
                       file_writev(f, vec, 1, &nbytes));

    file_contents_equal(tc, fname, LINE1, strlen(LINE1));

    vec[0].iov_base = LINE1;
    vec[0].iov_len = strlen(LINE1);
    vec[1].iov_base = LINE2;
    vec[1].iov_len = strlen(LINE2);
    vec[2].iov_base = LINE1;
    vec[2].iov_len = strlen(LINE1);
    vec[3].iov_base = LINE1;
    vec[3].iov_len = strlen(LINE1);
    vec[4].iov_base = LINE2;
    vec[4].iov_len = strlen(LINE2);

    CORE_ASSERT_OK(tc, "writev of size 5 to file",
                       file_writev(f, vec, 5, &nbytes));

    CORE_ASSERT_OK(tc, "close for writing",
                       file_close(f));

    file_contents_equal(tc, fname, LINE1 LINE1 LINE2 LINE1 LINE1 LINE2,
                        strlen(LINE1)*4 + strlen(LINE2)*2);

}

static void test_writev_full(abts_case *tc, void *data)
{
    file_t *f;
    size_t nbytes;
    struct iovec vec[5];
    const char *fname = "data/testwritev_full.txt";

    CORE_ASSERT_OK(tc, "open file for writing",
                       file_open(&f, fname,
                                     FILE_WRITE|FILE_CREATE|FILE_TRUNCATE,
                                     FILE_OS_DEFAULT));

    vec[0].iov_base = LINE1;
    vec[0].iov_len = strlen(LINE1);
    vec[1].iov_base = LINE2;
    vec[1].iov_len = strlen(LINE2);
    vec[2].iov_base = LINE1;
    vec[2].iov_len = strlen(LINE1);
    vec[3].iov_base = LINE1;
    vec[3].iov_len = strlen(LINE1);
    vec[4].iov_base = LINE2;
    vec[4].iov_len = strlen(LINE2);

    CORE_ASSERT_OK(tc, "writev_full of size 5 to file",
                       file_writev_full(f, vec, 5, &nbytes));

    ABTS_SIZE_EQUAL(tc, strlen(LINE1)*3 + strlen(LINE2)*2, nbytes);

    CORE_ASSERT_OK(tc, "close for writing",
                       file_close(f));

    file_contents_equal(tc, fname, LINE1 LINE2 LINE1 LINE1 LINE2,
                        strlen(LINE1)*3 + strlen(LINE2)*2);

}

static void test_truncate(abts_case *tc, void *data)
{
    status_t rv;
    file_t *f;
    const char *fname = "data/testtruncate.dat";
    const char *s;
    size_t nbytes;
    file_info_t finfo;

    file_remove(fname);

    rv = file_open(&f, fname,
                       FILE_CREATE | FILE_WRITE, FILE_UREAD | FILE_UWRITE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    s = "some data";
    nbytes = strlen(s);
    rv = file_write(f, s, &nbytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_SIZE_EQUAL(tc, strlen(s), nbytes);

    rv = file_close(f);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_open(&f, fname,
                       FILE_TRUNCATE | FILE_WRITE, FILE_UREAD | FILE_UWRITE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_close(f);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    rv = file_stat(&finfo, fname, FILE_INFO_SIZE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_ASSERT(tc, "File size mismatch, expected 0 (empty)", finfo.size == 0);

    rv = file_remove(fname);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
}

static void test_fail_write_flush(abts_case *tc, void *data)
{
    file_t *f;
    const char *fname = "data/testflush.dat";
    status_t rv;
    char buf[FILE_BUFFERSIZE];
    int n;

    file_remove(fname);

    CORE_ASSERT_OK(tc, "open test file",
                       file_open(&f, fname,
                                     FILE_CREATE|FILE_READ,
                                     FILE_UREAD|FILE_UWRITE));

    memset(buf, 'A', sizeof buf);

    /* Try three writes.  One of these should fail when it exceeds the
     * internal buffer and actually tries to write to the file, which
     * was opened read-only and hence should be unwritable. */
    for (n = 0, rv = CORE_OK; n < 4 && rv == CORE_OK; n++) {
        size_t bytes = sizeof buf;
        rv = file_write(f, buf, &bytes);
    }

    ABTS_ASSERT(tc, "failed to write to read-only buffered fd",
                rv != CORE_OK);

    file_close(f);
}

abts_suite *testfile(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_file_init, NULL);
    abts_run_test(suite, test_open_noreadwrite, NULL);
    abts_run_test(suite, test_open_excl, NULL);
    abts_run_test(suite, test_open_read, NULL);
    abts_run_test(suite, test_open_readwrite, NULL);
    abts_run_test(suite, link_existing, NULL);
    abts_run_test(suite, link_nonexisting, NULL);
    abts_run_test(suite, test_read, NULL);
    abts_run_test(suite, test_readzero, NULL);
    abts_run_test(suite, test_seek, NULL);
    abts_run_test(suite, test_filename, NULL);
    abts_run_test(suite, test_fileclose, NULL);
    abts_run_test(suite, test_file_remove, NULL);
    abts_run_test(suite, test_open_write, NULL);
    abts_run_test(suite, test_open_writecreate, NULL);
    abts_run_test(suite, test_write, NULL);
    abts_run_test(suite, test_getc, NULL);
    abts_run_test(suite, test_gets, NULL);
    abts_run_test(suite, test_puts, NULL);
    abts_run_test(suite, test_writev, NULL);
    abts_run_test(suite, test_writev_full, NULL);
    abts_run_test(suite, test_bigread, NULL);
    abts_run_test(suite, test_mod_neg, NULL);
    abts_run_test(suite, test_truncate, NULL);
    abts_run_test(suite, test_fail_write_flush, NULL);

    return suite;
}


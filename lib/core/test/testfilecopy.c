#include "core_file.h"
#include "testutil.h"

#define DIRNAME "data"
#define FILENAME1 DIRNAME "/file_datafile.txt"
#define TESTSTR1  "This is the file data file."
#define FILENAME2 DIRNAME "/mmap_datafile.txt"
#define TESTSTR2  "This is the MMAP data file."

static void test_filecopy_init(abts_case *tc, void *data)
{
    status_t rv;
    file_info_t finfo;
    size_t bytes;
    file_t *filetest = NULL;

    rv = dir_make("data", FILE_UREAD | FILE_UWRITE | FILE_UEXECUTE);

    rv = file_stat(&finfo, "data", FILE_INFO_TYPE);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_INT_EQUAL(tc, FILE_DIR, finfo.filetype);

    rv = file_open(&filetest, FILENAME1,
                       FILE_WRITE | FILE_CREATE,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    bytes = strlen(TESTSTR1);
    rv = file_write(filetest, TESTSTR1, &bytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    file_close(filetest);

    rv = file_open(&filetest, FILENAME2,
                       FILE_WRITE | FILE_CREATE,
                       FILE_UREAD | FILE_UWRITE | FILE_GREAD);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    bytes = strlen(TESTSTR2);
    rv = file_write(filetest, TESTSTR2, &bytes);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    file_close(filetest);
}

static void copy_helper(abts_case *tc, const char *from, const char * to,
                        file_perms_t perms, int append)
{
    status_t rv;
    status_t dest_rv;
    file_info_t copy;
    file_info_t orig;
    file_info_t dest;

    dest_rv = file_stat(&dest, to, FILE_INFO_SIZE);

    if (!append) {
        rv = file_copy(from, to, perms);
    }
    else {
        rv = file_append(from, to, perms);
    }
    CORE_ASSERT_OK(tc, "Error copying file", rv);

    rv = file_stat(&orig, from, FILE_INFO_SIZE);
    CORE_ASSERT_OK(tc, "Couldn't stat original file", rv);

    rv = file_stat(&copy, to, FILE_INFO_SIZE);
    CORE_ASSERT_OK(tc, "Couldn't stat copy file", rv);

    if (!append) {
        ABTS_ASSERT(tc, "File size differs", orig.size == copy.size);
    }
    else {
        ABTS_ASSERT(tc, "File size differs",
                        ((dest_rv == CORE_OK)
                          ? dest.size : 0) + orig.size == copy.size);
    }
}

static void copy_short_file(abts_case *tc, void *data)
{
    status_t rv;

    /* make absolutely sure that the dest file doesn't exist. */
    file_remove("data/file_copy.txt");

    copy_helper(tc, "data/file_datafile.txt", "data/file_copy.txt",
                FILE_SOURCE_PERMS, 0);
    rv = file_remove("data/file_copy.txt");
    CORE_ASSERT_OK(tc, "Couldn't remove copy file", rv);
}

static void copy_over_existing(abts_case *tc, void *data)
{
    status_t rv;

    /* make absolutely sure that the dest file doesn't exist. */
    file_remove("data/file_copy.txt");

    /* This is a cheat.  I don't want to create a new file, so I just copy
     * one file, then I copy another.  If the second copy succeeds, then
     * this works.
     */
    copy_helper(tc, "data/file_datafile.txt", "data/file_copy.txt",
                FILE_SOURCE_PERMS, 0);

    copy_helper(tc, "data/mmap_datafile.txt", "data/file_copy.txt",
                FILE_SOURCE_PERMS, 0);

    rv = file_remove("data/file_copy.txt");
    CORE_ASSERT_OK(tc, "Couldn't remove copy file", rv);
}

static void append_nonexist(abts_case *tc, void *data)
{
    status_t rv;

    /* make absolutely sure that the dest file doesn't exist. */
    file_remove("data/file_copy.txt");

    copy_helper(tc, "data/file_datafile.txt", "data/file_copy.txt",
                FILE_SOURCE_PERMS, 0);
    rv = file_remove("data/file_copy.txt");
    CORE_ASSERT_OK(tc, "Couldn't remove copy file", rv);
}

static void append_exist(abts_case *tc, void *data)
{
    status_t rv;

    /* make absolutely sure that the dest file doesn't exist. */
    file_remove("data/file_copy.txt");

    /* This is a cheat.  I don't want to create a new file, so I just copy
     * one file, then I copy another.  If the second copy succeeds, then
     * this works.
     */
    copy_helper(tc, "data/file_datafile.txt", "data/file_copy.txt",
                FILE_SOURCE_PERMS, 0);

    copy_helper(tc, "data/mmap_datafile.txt", "data/file_copy.txt",
                FILE_SOURCE_PERMS, 1);

    rv = file_remove("data/file_copy.txt");
    CORE_ASSERT_OK(tc, "Couldn't remove copy file", rv);
}

abts_suite *testfilecopy(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_filecopy_init, NULL);

    abts_run_test(suite, copy_short_file, NULL);
    abts_run_test(suite, copy_over_existing, NULL);

    abts_run_test(suite, append_nonexist, NULL);
    abts_run_test(suite, append_exist, NULL);

    return suite;
}


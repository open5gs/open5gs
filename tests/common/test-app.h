#ifndef TEST_APP_H
#define TEST_APP_H

#ifdef __cplusplus
extern "C" {
#endif

void test_main(int argc, char **argv,
        const char *name, void (*init)(char **argv));

#ifdef __cplusplus
}
#endif

#endif /* TEST_APP_H */

#include "core.h"

#define ST_BSS    1
#define ST_COMMON 2
#define ST_DATA   3
#define ST_TEXT   4

typedef struct {
    int   sym_type;
    void *sym_addr;
    char *sym_name;
} symbol_tbl_t;

extern symbol_tbl_t st[];

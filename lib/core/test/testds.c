#include "core_pool.h"
#include "core_list.h"
#include "core_queue.h"
#include "core_ringbuf.h"
#include "testutil.h"


/*****************************************************************************
 * test for core_pool.h
 */
typedef struct {
    char m1;
    int m2;
} pt_type1;

#define SIZE_OF_TPOOL1  5
#define SIZE_OF_TPOOL2  5

typedef char type_of_tpool1;
typedef pt_type1 type_of_tpool2;

pool_declare(tpool1, type_of_tpool1, SIZE_OF_TPOOL1);

static void pool_test_core1(abts_case *tc, void *data, int start)
{
    int i, j, n;

    type_of_tpool1 *org[SIZE_OF_TPOOL1+1];
    type_of_tpool1 *node[SIZE_OF_TPOOL1+1];

    pool_init(&tpool1, SIZE_OF_TPOOL1);

    /* Check basic members */
    n = pool_size(&tpool1);
    ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1, n);
    n = pool_avail(&tpool1);
    ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1, n);
    n = pool_used(&tpool1);
    ABTS_INT_EQUAL(tc, 0, n);

    /* Allocation up to start index */
    for (i = 0; i < start; i++)
    {
        pool_alloc_node(&tpool1, &node[i]);
        /* Check not null */
        ABTS_PTR_NOTNULL(tc, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1 - (i + 1), n);
    }

    /* Free all allocated nodes */
    for (i = 0; i < start; i++)
    {
        pool_free_node(&tpool1, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1 - start + (i + 1), n);
    }

    /* Allocation up to maximum pool size */
    for (i = 0; i < SIZE_OF_TPOOL1; i++)
    {
        pool_alloc_node(&tpool1, &node[i]);
        org[i] = node[i];
        /* Check not null */
        ABTS_PTR_NOTNULL(tc, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1 - (i + 1), n);
    }

    /* Free all allocated */
    for (i = 0; i < SIZE_OF_TPOOL1; i++)
    {
        pool_free_node(&tpool1, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, i + 1, n);
    }

    /* Check that addresses of nodes are same with original ones */
    n = 0;
    for (i = 0; i < SIZE_OF_TPOOL1; i++)
    {
        pool_alloc_node(&tpool1, &node[i]);
        for (j = 0; j < SIZE_OF_TPOOL1; j++)
        {
            if (node[i] == org[j])
                n++;
        }
    }

    /* Free all allocated */
    for (i = 0; i < SIZE_OF_TPOOL1; i++)
    {
        pool_free_node(&tpool1, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, i + 1, n);
    }

    pool_final(&tpool1);
}

static void pool_test_core2(abts_case *tc, void *data, int start)
{
    int i, j, n;

    type_of_tpool2 *org[SIZE_OF_TPOOL2+1];
    type_of_tpool2 *node[SIZE_OF_TPOOL2+1];

    pool_init(&tpool1, SIZE_OF_TPOOL2);

    /* Check basic members */
    n = pool_size(&tpool1);
    ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2, n);
    n = pool_avail(&tpool1);
    ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2, n);
    n = pool_used(&tpool1);
    ABTS_INT_EQUAL(tc, 0, n);

    /* Allocation up to start index */
    for (i = 0; i < start; i++)
    {
        pool_alloc_node(&tpool1, &node[i]);
        /* Check not null */
        ABTS_PTR_NOTNULL(tc, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2 - (i + 1), n);
    }

    /* Free all allocated nodes */
    for (i = 0; i < start; i++)
    {
        pool_free_node(&tpool1, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2 - start + (i + 1), n);
    }

    /* Allocation up to maximum pool size */
    for (i = 0; i < SIZE_OF_TPOOL2; i++)
    {
        pool_alloc_node(&tpool1, &node[i]);
        org[i] = node[i];
        /* Check not null */
        ABTS_PTR_NOTNULL(tc, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2 - (i + 1), n);
    }

    /* Free all allocated */
    for (i = 0; i < SIZE_OF_TPOOL2; i++)
    {
        pool_free_node(&tpool1, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, i + 1, n);
    }

    /* Check that addresses of nodes are same with original ones */
    n = 0;
    for (i = 0; i < SIZE_OF_TPOOL2; i++)
    {
        pool_alloc_node(&tpool1, &node[i]);
        for (j = 0; j < SIZE_OF_TPOOL2; j++)
        {
            if (node[i] == org[j])
                n++;
        }
    }

    /* Free all allocated */
    for (i = 0; i < SIZE_OF_TPOOL2; i++)
    {
        pool_free_node(&tpool1, node[i]);
        /* Check the number of available nodes */
        n = pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, i + 1, n);
    }

    pool_final(&tpool1);
}

static void pool_test1(abts_case *tc, void *data)
{
    int i;

    for (i = 0; i < SIZE_OF_TPOOL1; i++)
        pool_test_core1(tc, data, i);
}

static void pool_test2(abts_case *tc, void *data)
{
    int i;

    for (i = 0; i < SIZE_OF_TPOOL2; i++)
        pool_test_core2(tc, data, i);
}



/*****************************************************************************
 * test for core_list.h
 */
typedef struct {
    lnode_t node;
    int m1;
} lt_type1;

list_t tlist1;

int lttype1_compare(lt_type1 *pnode1, lt_type1 *pnode2)
{
    if (pnode1->m1 == pnode2->m1)
        return 0;
    else if (pnode1->m1 < pnode2->m1)
        return -1;
    else
        return 1;
}

#define SIZE_OF_lt_type1 16

static void list_test1(abts_case *tc, void *data)
{
    int i;

    lt_type1 *iter, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    /* Initialize head of list */
    list_init(&tlist1);

    /* Check list is empty */
    ABTS_TRUE(tc, list_is_empty(&tlist1));

    /* Confirm that any node can't be get */
    iter = list_first(&tlist1);
    ABTS_PTR_NULL(tc, iter);

    /* Add a node */
    list_append(&tlist1, &node[0]);

    /* Iterate from the first. And check the pointers */
    iter = list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_next(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Iterate from the last. And check the pointers */
    iter = list_last(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_prev(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Add two nodes */
    list_append(&tlist1, &node[1]);
    list_append(&tlist1, &node[2]);

    /* Iterate from the first. And check the pointers */
    iter = list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = list_next(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Iterate from the last. And check the pointers */
    iter = list_last(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = list_prev(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = list_prev(iter);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_prev(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Remove all nodes */
    list_remove(&tlist1, &node[0]);
    list_remove(&tlist1, &node[1]);
    list_remove(&tlist1, &node[2]);

    /* Check list is empty */
    ABTS_TRUE(tc, list_is_empty(&tlist1));
}

static void list_test2(abts_case *tc, void *data)
{
    int i;

    lt_type1 *iter, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    /* Initialize head of list */
    list_init(&tlist1);

    /* Check list is empty */
    ABTS_TRUE(tc, list_is_empty(&tlist1));

    /* Confirm that any node can't be get */
    iter = list_first(&tlist1);
    ABTS_PTR_NULL(tc, iter);

    /* Add a node */
    list_prepend(&tlist1, &node[0]);

    /* Iterate from the first. And check the pointers */
    iter = list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_next(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Iterate from the last. And check the pointers */
    iter = list_last(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_prev(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Add two nodes */
    list_prepend(&tlist1, &node[1]);
    list_prepend(&tlist1, &node[2]);

    /* Iterate from the first. And check the pointers */
    iter = list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_next(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Iterate from the last. And check the pointers */
    iter = list_last(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_prev(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = list_prev(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = list_prev(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Remove all nodes */
    list_remove(&tlist1, &node[2]);
    list_remove(&tlist1, &node[1]);
    list_remove(&tlist1, &node[0]);

    /* Check list is empty */
    ABTS_TRUE(tc, list_is_empty(&tlist1));
}

static void list_test3(abts_case *tc, void *data)
{
    int i;

    lt_type1 *iter, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    /* Initialize head of list */
    list_init(&tlist1);

    /* Add three nodes */
    for (i = 0; i < 3; i++)
        list_append(&tlist1, &node[i]);

    /* Iterate from the first. And check the pointers */
    i = 0; iter = list_first(&tlist1);
    while (iter)
    {
        ABTS_PTR_EQUAL(tc, &node[i++], iter);
        iter = list_next(iter);
    }
    ABTS_INT_EQUAL(tc, 3, i);

    /* Iterate from the last. And check the pointers */
    i = 0; iter = list_last(&tlist1);
    while (iter)
    {
        ABTS_PTR_EQUAL(tc, &node[3 - (++i)], iter);
        iter = list_prev(iter);
    }
    ABTS_INT_EQUAL(tc, 3, i);

    /* Insert three nodes by list_insert_prev() */
    list_insert_prev(&tlist1, &node[0], &node[3]);
    list_insert_prev(&tlist1, &node[1], &node[4]);
    list_insert_prev(&tlist1, &node[2], &node[5]);

    /* Iterate from the first. And check the pointers */
    iter = list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[3], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[4], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[5], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = list_next(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Remove three nodes inserted by list_insert_prev() */
    list_remove(&tlist1, &node[3]);
    list_remove(&tlist1, &node[4]);
    list_remove(&tlist1, &node[5]);

    /* Iterate from the first. And check the pointers */
    iter = list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = list_next(iter);
    ABTS_PTR_NULL(tc, iter);
}

static void list_test4(abts_case *tc, void *data)
{
    int i;

    lt_type1 *iter, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    /* Initialize head of list */
    list_init(&tlist1);

    /* Add three nodes */
    for (i = 0; i < 3; i++)
        list_append(&tlist1, &node[i]);

    /* Iterate from the first. And check the pointers */
    i = 0; iter = list_first(&tlist1);
    while (iter)
    {
        ABTS_PTR_EQUAL(tc, &node[i++], iter);
        iter = list_next(iter);
    }
    ABTS_INT_EQUAL(tc, 3, i);

    /* Iterate from the last. And check the pointers */
    i = 0; iter = list_last(&tlist1);
    while (iter)
    {
        ABTS_PTR_EQUAL(tc, &node[3 - (++i)], iter);
        iter = list_prev(iter);
    }
    ABTS_INT_EQUAL(tc, 3, i);

    /* Insert three nodes by list_insert_next() */
    list_insert_next(&tlist1, &node[0], &node[3]);
    list_insert_next(&tlist1, &node[1], &node[4]);
    list_insert_next(&tlist1, &node[2], &node[5]);

    /* Iterate from the first. And check the pointers */
    iter = list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[3], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[4], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[5], iter);
    iter = list_next(iter);
    ABTS_PTR_NULL(tc, iter);

    /* Remove three nodes inserted by list_insert_next() */
    list_remove(&tlist1, &node[3]);
    list_remove(&tlist1, &node[4]);
    list_remove(&tlist1, &node[5]);

    /* Iterate from the first. And check the pointers */
    iter = list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = list_next(iter);
    ABTS_PTR_NULL(tc, iter);
}

static void list_test5(abts_case *tc, void *data)
{
    int i, j;

    /* List up posssible oders with four nodes */
    int od[24][4] = {
        {0,1,2,3}, {0,1,3,2}, {0,2,1,3}, {0,2,3,1}, {0,3,1,2}, {0,3,2,1},
        {1,0,2,3}, {1,0,3,2}, {1,2,0,3}, {1,2,3,0}, {1,3,0,2}, {1,3,2,0},
        {2,1,0,3}, {2,1,3,0}, {2,0,1,3}, {2,0,3,1}, {2,3,1,0}, {2,3,0,1},
        {3,1,2,0}, {3,1,0,2}, {3,2,1,0}, {3,2,0,1}, {3,0,1,2}, {3,0,2,1}
    };

    lt_type1 *iter, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    for (i = 0; i < 24; i++)
    {
        /* Initialize head of list */
        list_init(&tlist1);

        /* Add four nodes using predefined orders */
        for (j = 0; j < 4; j++)
            list_insert_sorted(&tlist1, &node[od[i][j]], &lttype1_compare);

        /* Iterate from the first. And check the SORTED */
        j = 0; iter = list_first(&tlist1);
        while (iter)
        {
            ABTS_INT_EQUAL(tc, iter->m1, j++);
            iter = list_next(iter);
        }
    }
}



/*****************************************************************************
 * test for core_queue.h
 */
typedef struct {
    int m1;
} qt_type1;

#define SIZE_OF_TQUE1 4
#define SIZE_OF_TQUE2 3

typedef int type_of_tque1;
typedef qt_type1 type_of_tque2;

que_declare(tque1, type_of_tque1, SIZE_OF_TQUE1);
que_declare(tque2, type_of_tque2, SIZE_OF_TQUE2);
que_declare(tque3, type_of_tque2 *, SIZE_OF_TQUE2);

static void que_test1(abts_case *tc, void *data)
{
    int i, r;

    type_of_tque1 n, node[SIZE_OF_TQUE1+1];

    n = 0;
    for (i = 0; i < SIZE_OF_TQUE1; i++)
        node[i] = i;

    /* Initialize queue */
    que_init(&tque1, SIZE_OF_TQUE1);

    /* Check basic members */
    ABTS_TRUE(tc, que_is_empty(&tque1));
    ABTS_FALSE(tc, que_is_full(&tque1));
    ABTS_INT_EQUAL(tc, SIZE_OF_TQUE1, que_avail(&tque1));
    ABTS_INT_EQUAL(tc, 0, que_used(&tque1));

    /* Push a node */
    r = que_push(&tque1, &node[0]);
    ABTS_INT_EQUAL(tc, 1, r);

    /* Check queue status */
    ABTS_FALSE(tc, que_is_empty(&tque1));
    ABTS_FALSE(tc, que_is_full(&tque1));
    ABTS_INT_EQUAL(tc, SIZE_OF_TQUE1 - 1, que_avail(&tque1));
    ABTS_INT_EQUAL(tc, 1, que_used(&tque1));

    /* Pop a node */
    r = que_pop(&tque1, &n);
    ABTS_INT_EQUAL(tc, 0, r);

    /* Check queue status */
    ABTS_TRUE(tc, que_is_empty(&tque1));
    ABTS_FALSE(tc, que_is_full(&tque1));
    ABTS_INT_EQUAL(tc, SIZE_OF_TQUE1, que_avail(&tque1));
    ABTS_INT_EQUAL(tc, 0, que_used(&tque1));

    /* Push nodes up to queue size */
    for (i = 0; i < SIZE_OF_TQUE1; i++)
    {
        r = que_push(&tque1, &node[i]);
        ABTS_INT_EQUAL(tc, i + 1, r);
    }

    /* Check queue status */
    ABTS_FALSE(tc, que_is_empty(&tque1));
    ABTS_TRUE(tc, que_is_full(&tque1));
    ABTS_INT_EQUAL(tc, 0, que_avail(&tque1));
    ABTS_INT_EQUAL(tc, SIZE_OF_TQUE1, que_used(&tque1));

    /* Push a node to full queue */
    r = que_push(&tque1, &node[0]);
    ABTS_INT_EQUAL(tc, -1, r);

    /* Pop all pushed nodes and check pop order */
    for (i = 0; i < SIZE_OF_TQUE1; i++)
    {
        r = que_pop(&tque1, &n);
        ABTS_INT_EQUAL(tc, node[i], n);
        ABTS_INT_EQUAL(tc, SIZE_OF_TQUE1 - (i + 1), r);
    }

    /* Check queue status */
    ABTS_TRUE(tc, que_is_empty(&tque1));
    ABTS_FALSE(tc, que_is_full(&tque1));
    ABTS_INT_EQUAL(tc, SIZE_OF_TQUE1, que_avail(&tque1));
    ABTS_INT_EQUAL(tc, 0, que_used(&tque1));

    /* Pop a node from empty queue */
    r = que_pop(&tque1, &n);
    ABTS_INT_EQUAL(tc, -1, r);
}



/*****************************************************************************
 * test for core_ringbuf.h
 */

#define SIZE_OF_TRBUF1 7
rbuf_declare(trbuf1, SIZE_OF_TRBUF1);

#define SIZE_OF_TRBUF2 7
rbuf_declare_ext(trbuf2);
char trbuf2_ext_buf[SIZE_OF_TRBUF2];

static void rbuf_test1(abts_case *tc, void *data)
{
    int i, n;
    char wbuf[32];
    char rbuf[32];

    for (i = 0; i < 26; i++)
        wbuf[i] = 'a' + i;

    rbuf_init(&trbuf1, SIZE_OF_TRBUF1);

    /* Check basic members */
    ABTS_TRUE(tc, rbuf_is_empty(&trbuf1));
    ABTS_FALSE(tc, rbuf_is_full(&trbuf1));
    ABTS_INT_EQUAL(tc, SIZE_OF_TRBUF1, rbuf_free_bytes(&trbuf1));
    ABTS_INT_EQUAL(tc, 0, rbuf_bytes(&trbuf1));

    /* Write 3 bytes */
    n = rbuf_write(&trbuf1, wbuf, 3);
    ABTS_INT_EQUAL(tc, 3, n);

    /* Check basic members */
    ABTS_FALSE(tc, rbuf_is_empty(&trbuf1));
    ABTS_FALSE(tc, rbuf_is_full(&trbuf1));
    ABTS_INT_EQUAL(tc, SIZE_OF_TRBUF1 - 3, rbuf_free_bytes(&trbuf1));
    ABTS_INT_EQUAL(tc, 3, rbuf_bytes(&trbuf1));

    /* Read 3 bytes */
    n = rbuf_read(&trbuf1, rbuf, 3); rbuf[3] = 0;
    ABTS_INT_EQUAL(tc, 3, n);
    ABTS_STR_EQUAL(tc, "abc", rbuf);

    /* Check basic members */
    ABTS_TRUE(tc, rbuf_is_empty(&trbuf1));
    ABTS_FALSE(tc, rbuf_is_full(&trbuf1));
    ABTS_INT_EQUAL(tc, SIZE_OF_TRBUF1, rbuf_free_bytes(&trbuf1));
    ABTS_INT_EQUAL(tc, 0, rbuf_bytes(&trbuf1));

    /* Write 3 bytes */
    n = rbuf_write(&trbuf1, wbuf, 3);
    ABTS_INT_EQUAL(tc, 3, n);

    /* Write 5 bytes, but only 4 bytes shall be written */
    n = rbuf_write(&trbuf1, wbuf + 3, 5);
    ABTS_INT_EQUAL(tc, 4, n);

    /* Write 1 bytes to full buffer */
    n = rbuf_write(&trbuf1, wbuf, 1);
    ABTS_INT_EQUAL(tc, -1, n);

    /* Read 2 bytes */
    n = rbuf_read(&trbuf1, rbuf, 2); rbuf[2] = 0;
    ABTS_INT_EQUAL(tc, 2, n);
    ABTS_STR_EQUAL(tc, "ab", rbuf);

    /* Read 5 bytes */
    n = rbuf_read(&trbuf1, rbuf, 5); rbuf[5] = 0;
    ABTS_INT_EQUAL(tc, 5, n);
    ABTS_STR_EQUAL(tc, "cdefg", rbuf);

    /* Check basic members */
    ABTS_TRUE(tc, rbuf_is_empty(&trbuf1));
    ABTS_FALSE(tc, rbuf_is_full(&trbuf1));
    ABTS_INT_EQUAL(tc, SIZE_OF_TRBUF1, rbuf_free_bytes(&trbuf1));
    ABTS_INT_EQUAL(tc, 0, rbuf_bytes(&trbuf1));

    /* Read 1 bytes from empty buffer */
    n = rbuf_read(&trbuf1, rbuf, 1);
    ABTS_INT_EQUAL(tc, -1, n);
}

static void rbuf_test2(abts_case *tc, void *data)
{
    int i, n;
    char wbuf[32];
    char rbuf[32];

    for (i = 0; i < 26; i++)
        wbuf[i] = 'a' + i;

    rbuf_init_ext(&trbuf2, SIZE_OF_TRBUF1, trbuf2_ext_buf);

    /* Check basic members */
    ABTS_TRUE(tc, rbuf_is_empty(&trbuf2));
    ABTS_FALSE(tc, rbuf_is_full(&trbuf2));
    ABTS_INT_EQUAL(tc, SIZE_OF_TRBUF2, rbuf_free_bytes(&trbuf2));
    ABTS_INT_EQUAL(tc, 0, rbuf_bytes(&trbuf2));

    /* Write 3 bytes */
    n = rbuf_write(&trbuf2, wbuf, 3);
    ABTS_INT_EQUAL(tc, 3, n);

    /* Check basic members */
    ABTS_FALSE(tc, rbuf_is_empty(&trbuf2));
    ABTS_FALSE(tc, rbuf_is_full(&trbuf2));
    ABTS_INT_EQUAL(tc, SIZE_OF_TRBUF2 - 3, rbuf_free_bytes(&trbuf2));
    ABTS_INT_EQUAL(tc, 3, rbuf_bytes(&trbuf2));

    /* Read 3 bytes */
    n = rbuf_read(&trbuf2, rbuf, 3); rbuf[3] = 0;
    ABTS_INT_EQUAL(tc, 3, n);
    ABTS_STR_EQUAL(tc, "abc", rbuf);

    /* Check basic members */
    ABTS_TRUE(tc, rbuf_is_empty(&trbuf2));
    ABTS_FALSE(tc, rbuf_is_full(&trbuf2));
    ABTS_INT_EQUAL(tc, SIZE_OF_TRBUF2, rbuf_free_bytes(&trbuf2));
    ABTS_INT_EQUAL(tc, 0, rbuf_bytes(&trbuf2));

    /* Write 3 bytes */
    n = rbuf_write(&trbuf2, wbuf, 3);
    ABTS_INT_EQUAL(tc, 3, n);

    /* Write 5 bytes, but only 4 bytes shall be written */
    n = rbuf_write(&trbuf2, wbuf + 3, 5);
    ABTS_INT_EQUAL(tc, 4, n);

    /* Write 1 bytes to full buffer */
    n = rbuf_write(&trbuf2, wbuf, 1);
    ABTS_INT_EQUAL(tc, -1, n);

    /* Read 2 bytes */
    n = rbuf_read(&trbuf2, rbuf, 2); rbuf[2] = 0;
    ABTS_INT_EQUAL(tc, 2, n);
    ABTS_STR_EQUAL(tc, "ab", rbuf);

    /* Read 5 bytes */
    n = rbuf_read(&trbuf2, rbuf, 5); rbuf[5] = 0;
    ABTS_INT_EQUAL(tc, 5, n);
    ABTS_STR_EQUAL(tc, "cdefg", rbuf);

    /* Check basic members */
    ABTS_TRUE(tc, rbuf_is_empty(&trbuf2));
    ABTS_FALSE(tc, rbuf_is_full(&trbuf2));
    ABTS_INT_EQUAL(tc, SIZE_OF_TRBUF2, rbuf_free_bytes(&trbuf2));
    ABTS_INT_EQUAL(tc, 0, rbuf_bytes(&trbuf2));

    /* Read 1 bytes from empty buffer */
    n = rbuf_read(&trbuf2, rbuf, 1);
    ABTS_INT_EQUAL(tc, -1, n);

    n = rbuf_skip_write_pos(&trbuf2, 5);
    ABTS_INT_EQUAL(tc, 5, n);
    n = rbuf_skip_read_pos(&trbuf2, 5);
    ABTS_INT_EQUAL(tc, 5, n);
}

abts_suite *testds(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, pool_test1, NULL);
    abts_run_test(suite, pool_test2, NULL);

    abts_run_test(suite, list_test1, NULL);
    abts_run_test(suite, list_test2, NULL);
    abts_run_test(suite, list_test3, NULL);
    abts_run_test(suite, list_test4, NULL);
    abts_run_test(suite, list_test5, NULL);

    abts_run_test(suite, que_test1, NULL);

    abts_run_test(suite, rbuf_test1, NULL);
    abts_run_test(suite, rbuf_test2, NULL);

    return suite;
}

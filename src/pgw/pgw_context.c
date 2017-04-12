#define TRACE_MODULE _pgw_context

#include "core_debug.h"
#include "core_pool.h"
#include "core_index.h"

#include "gtp_path.h"

#include "pgw_context.h"

static pgw_context_t self;

pool_declare(pgw_pdn_pool, pdn_t, MAX_NUM_OF_PDN);
index_declare(pgw_sess_pool, pgw_sess_t, MAX_NUM_OF_UE);

static int context_initiaized = 0;

status_t pgw_context_init()
{
    pdn_t *pdn = NULL;
    
    d_assert(context_initiaized == 0, return CORE_ERROR,
            "MME context already has been initialized");

    memset(&self, 0, sizeof(pgw_context_t));

    pool_init(&pgw_pdn_pool, MAX_NUM_OF_PDN);
    list_init(&self.pdn_list);

    index_init(&pgw_sess_pool, MAX_NUM_OF_UE);
    list_init(&self.sess_list);

    self.s5c_addr = inet_addr("127.0.0.1");
    self.s5c_port = GTPV2_C_UDP_PORT + 3;
    self.s5c_node.addr = inet_addr("127.0.0.1");
    self.s5c_node.port = GTPV2_C_UDP_PORT + 2;
    list_init(&self.s5c_node.local_list);
    list_init(&self.s5c_node.remote_list);

    self.s5u_addr = inet_addr("127.0.0.1");
    self.s5u_port = GTPV1_U_UDP_PORT + 1;
    self.s5u_node.addr = inet_addr("127.0.0.1");
    self.s5u_node.port = GTPV1_U_UDP_PORT;

    /***********************************************
     * PDN DB */
    pdn = pgw_pdn_add();
    d_assert(pdn, return -1, "Profile context add failed");

    pdn->apn[0] = 0x08;
    strcpy(pdn->apn+1, "internet");

    pdn->paa.gtp_type = GTP_PDN_TYPE_IPV4;
    pdn->paa.ipv4_addr = inet_addr("45.45.45.113"); /* FIXME */

    context_initiaized = 1;

    return CORE_OK;
}

status_t pgw_context_final()
{
    d_assert(context_initiaized == 1, return CORE_ERROR,
            "HyperCell context already has been finalized");

    gtp_xact_delete_all(&self.s5c_node);
    pgw_sess_remove_all();
    pgw_pdn_remove_all();

    d_print("%d not freed in pgw_sess_pool[%d] in PGW-Context\n",
            index_size(&pgw_sess_pool) - pool_avail(&pgw_sess_pool),
            index_size(&pgw_sess_pool));

    index_final(&pgw_sess_pool);
    pool_final(&pgw_pdn_pool);

    context_initiaized = 0;
    
    return CORE_OK;
}

pgw_context_t* pgw_self()
{
    return &self;
}

pdn_t* pgw_pdn_add()
{
    pdn_t *pdn = NULL;

    pool_alloc_node(&pgw_pdn_pool, &pdn);
    d_assert(pdn, return NULL, "HSS-UE context allocation failed");

    memset(pdn, 0, sizeof(pdn_t));

    pdn->id = NEXT_ID(self.pdn_id, 1, 0xffffffff);
    
    list_append(&self.pdn_list, pdn);

    return pdn;
}

status_t pgw_pdn_remove(pdn_t *pdn)
{
    d_assert(pdn, return CORE_ERROR, "Null param");

    list_remove(&self.pdn_list, pdn);
    pool_free_node(&pgw_pdn_pool, pdn);

    return CORE_OK;
}

status_t pgw_pdn_remove_all()
{
    pdn_t *pdn = NULL, *next_pdn = NULL;
    
    pdn = list_first(&self.pdn_list);
    while (pdn)
    {
        next_pdn = list_next(pdn);

        pgw_pdn_remove(pdn);

        pdn = next_pdn;
    }

    return CORE_OK;
}

pdn_t* pgw_pdn_find_by_id(pdn_id_t id)
{
    pdn_t *pdn = NULL;
    
    pdn = list_first(&self.pdn_list);
    while (pdn)
    {
        if (pdn->id == id)
            break;

        pdn = list_next(pdn);
    }

    return pdn;
}

pdn_t* pgw_pdn_find_by_apn(c_int8_t *apn)
{
    pdn_t *pdn = NULL;
    
    pdn = list_first(&self.pdn_list);
    while (pdn)
    {
        if (strcmp(pdn->apn, apn) == 0)
            break;

        pdn = list_next(pdn);
    }

    return pdn;
}

pdn_t* pgw_pdn_first()
{
    return list_first(&self.pdn_list);
}

pdn_t* pgw_pdn_next(pdn_t *pdn)
{
    return list_next(pdn);
}

pgw_sess_t *pgw_sess_add()
{
    pgw_sess_t *sess = NULL;

    index_alloc(&pgw_sess_pool, &sess);
    d_assert(sess, return NULL, "Null param");

    sess->teid = sess->index;  /* derived from an index */

    list_append(&self.sess_list, sess);

    return sess;
}

status_t pgw_sess_remove(pgw_sess_t *sess)
{
    d_assert(sess, return CORE_ERROR, "Null param");

    list_remove(&self.sess_list, sess);
    index_free(&pgw_sess_pool, sess);

    return CORE_OK;
}

status_t pgw_sess_remove_all()
{
    pgw_sess_t *enb = NULL, *next_enb = NULL;
    
    enb = pgw_sess_first();
    while (enb)
    {
        next_enb = pgw_sess_next(enb);

        pgw_sess_remove(enb);

        enb = next_enb;
    }

    return CORE_OK;
}

pgw_sess_t* pgw_sess_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&pgw_sess_pool, index);
}

pgw_sess_t* pgw_sess_find_by_teid(c_uint32_t teid)
{
    return pgw_sess_find(teid);
}

pgw_sess_t* pgw_sess_first()
{
    return list_first(&self.sess_list);
}

pgw_sess_t* pgw_sess_next(pgw_sess_t *enb)
{
    return list_next(enb);
}

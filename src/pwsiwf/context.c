#include "context.h"
#include "ogs-sbi.h"
#include "ogs-app.h"

OGS_POOL(pwsiws_connection_pool, pwsiws_connection_t);
OGS_POOL(pwsiws_warning_pool, pwsiws_warning_t);

int __pwsiws_log_domain;

static pwsiws_context_t self;

void pwsiws_context_init(void) 
{
    memset(&self, 0, sizeof(pwsiws_context_t));
    
    ogs_list_init(&self.connection_list);
    ogs_list_init(&self.warning_list);
    ogs_list_init(&self.pws_iws_list);
    ogs_list_init(&self.pws_iws_list6);
    ogs_list_init(&self.sbcap_list);

    /* Initialize pools */
    ogs_pool_init(&pwsiws_connection_pool, 32);
    ogs_pool_init(&pwsiws_warning_pool, 64);

    self.amf_sbi = NULL;
}

void pwsiws_context_final(void) 
{
    pwsiws_connection_remove_all();
    pwsiws_warning_remove_all();

    if (self.amf_sbi) {
        // TODO: Find the correct way to free the SBI client
        // ogs_sbi_client_free(self.amf_sbi);
        self.amf_sbi = NULL;
    }
}

pwsiws_context_t *pwsiws_self(void) 
{ 
    return &self; 
}

int pwsiws_context_parse_config(void)
{
    // Hardcode SBCAP server configuration for now
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;
    char buf[OGS_ADDRSTRLEN];
    
    // Create SBCAP server configuration
    addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    ogs_assert(addr);
    
    // Set to 127.0.0.199:29168 as configured in YAML
    addr->sa.sa_family = AF_INET;
    addr->sin.sin_addr.s_addr = inet_addr("127.0.0.199");
    addr->sin.sin_port = htons(29168);
    
    node = ogs_socknode_add(&self.sbcap_list, AF_INET, addr, NULL);
    ogs_assert(node);
    
    ogs_info("PWS-IWS SBCAP server configured: [%s]:%d", 
            OGS_ADDR(addr, buf), OGS_PORT(addr));
    ogs_info("PWS-IWS configured as SBCAP server");
    ogs_info("SBCAP server list count: %d", ogs_list_count(&self.sbcap_list));
    
    // Setup AMF SBI client
    ogs_sbi_client_t *amf_client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_http;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *amf_addr = NULL, *amf_addr6 = NULL;
    
    // Parse AMF URI from config or use default
    char *amf_uri = ogs_strdup("http://127.0.0.5:7777"); // Default AMF SBI URI
    
    bool rc = ogs_sbi_getaddr_from_uri(
            &scheme, &fqdn, &fqdn_port, &amf_addr, &amf_addr6, amf_uri);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("Invalid AMF URI [%s]", amf_uri);
        ogs_free(amf_uri);
        return OGS_ERROR;
    }
    
    amf_client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, amf_addr, amf_addr6);
    if (!amf_client) {
        ogs_debug("Creating AMF SBI client");
        amf_client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, amf_addr, amf_addr6);
        if (!amf_client) {
            ogs_error("Failed to create AMF SBI client");
            ogs_free(fqdn);
            ogs_freeaddrinfo(amf_addr);
            ogs_freeaddrinfo(amf_addr6);
            ogs_free(amf_uri);
            return OGS_ERROR;
        }
    }
    
    self.amf_sbi = amf_client;
    
    ogs_info("PWS-IWS AMF SBI client configured: [%s]", amf_uri);
    
    ogs_free(fqdn);
    ogs_freeaddrinfo(amf_addr);
    ogs_freeaddrinfo(amf_addr6);
    ogs_free(amf_uri);
    
    return OGS_OK;
}

int pwsiws_context_nf_info(void) 
{ 
    return OGS_OK; 
}

pwsiws_connection_t *pwsiws_connection_add(ogs_sock_t *sock, ogs_sockaddr_t *addr) 
{ 
    pwsiws_connection_t *connection = NULL;
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(sock);
    ogs_assert(addr);

    ogs_pool_alloc(&pwsiws_connection_pool, &connection);
    if (!connection) {
        ogs_error("Failed to allocate PWS-IWS connection");
        return NULL;
    }

    memset(connection, 0, sizeof(pwsiws_connection_t));

    connection->sctp.sock = sock;
    connection->addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    ogs_assert(connection->addr);
    memcpy(connection->addr, addr, sizeof(ogs_sockaddr_t));

    ogs_list_add(&self.connection_list, connection);

    ogs_info("PWS-IWS connection added: [%s]:%d", 
            OGS_ADDR(connection->addr, buf), OGS_PORT(connection->addr));

    return connection;
}

void pwsiws_connection_remove(pwsiws_connection_t *connection) 
{
    ogs_assert(connection);
    
    if (connection->addr) {
        ogs_free(connection->addr);
    }
    
    ogs_list_remove(&self.connection_list, connection);
    ogs_pool_free(&pwsiws_connection_pool, connection);
}

void pwsiws_connection_remove_all(void) 
{
    pwsiws_connection_t *connection = NULL, *next_connection = NULL;
    
    ogs_list_for_each_safe(&self.connection_list, next_connection, connection) {
        pwsiws_connection_remove(connection);
    }
}

pwsiws_connection_t *pwsiws_connection_find_by_addr(ogs_sockaddr_t *addr) 
{ 
    pwsiws_connection_t *connection = NULL;
    
    ogs_assert(addr);
    
    ogs_list_for_each(&self.connection_list, connection) {
        if (connection->addr && 
            memcmp(connection->addr, addr, sizeof(ogs_sockaddr_t)) == 0) {
            return connection;
        }
    }
    
    return NULL;
}

pwsiws_connection_t *pwsiws_connection_find_by_id(ogs_pool_id_t id) 
{ 
    pwsiws_connection_t *connection = NULL;
    
    ogs_list_for_each(&self.connection_list, connection) {
        if (connection->id == id) {
            return connection;
        }
    }
    
    return NULL;
}

pwsiws_warning_t *pwsiws_warning_add(pwsiws_connection_t *connection) 
{ 
    pwsiws_warning_t *warning = NULL;
    
    ogs_assert(connection);
    
    ogs_pool_alloc(&pwsiws_warning_pool, &warning);
    if (!warning) {
        ogs_error("Failed to allocate PWS-IWS warning");
        return NULL;
    }
    
    memset(warning, 0, sizeof(pwsiws_warning_t));
    
    warning->connection_id = connection->id;
    warning->warning_id = pwsiws_warning_id_alloc();
    warning->message_id = pwsiws_message_id_alloc();
    
    ogs_list_add(&self.warning_list, warning);
    
    ogs_info("PWS-IWS warning added: ID=%d, WarningID=%d, MessageID=%d", 
            warning->id, warning->warning_id, warning->message_id);
    
    return warning;
}

void pwsiws_warning_remove(pwsiws_warning_t *warning) 
{
    ogs_assert(warning);
    
    if (warning->paging_nonuen2message_location) {
        ogs_free(warning->paging_nonuen2message_location);
    }
    
    ogs_list_remove(&self.warning_list, warning);
    ogs_pool_free(&pwsiws_warning_pool, warning);
}

void pwsiws_warning_remove_all(void) 
{
    pwsiws_warning_t *warning = NULL, *next_warning = NULL;
    
    ogs_list_for_each_safe(&self.warning_list, next_warning, warning) {
        pwsiws_warning_remove(warning);
    }
}

pwsiws_warning_t *pwsiws_warning_find_by_id(ogs_pool_id_t id) 
{ 
    pwsiws_warning_t *warning = NULL;
    
    ogs_list_for_each(&self.warning_list, warning) {
        if (warning->id == id) {
            return warning;
        }
    }
    
    return NULL;
}

pwsiws_warning_t *pwsiws_warning_find_by_warning_id(uint32_t warning_id) 
{ 
    pwsiws_warning_t *warning = NULL;
    
    ogs_list_for_each(&self.warning_list, warning) {
        if (warning->warning_id == warning_id) {
            return warning;
        }
    }
    
    return NULL;
}

pwsiws_warning_t *pwsiws_warning_find_by_paging_nonuen2message_location(char *location) 
{ 
    pwsiws_warning_t *warning = NULL;
    
    ogs_assert(location);
    
    ogs_list_for_each(&self.warning_list, warning) {
        if (warning->paging_nonuen2message_location && 
            strcmp(warning->paging_nonuen2message_location, location) == 0) {
            return warning;
        }
    }
    
    return NULL;
}

void pwsiws_warning_set_paging_nonuen2message_location(pwsiws_warning_t *warning, char *location) 
{
    ogs_assert(warning);
    ogs_assert(location);
    
    if (warning->paging_nonuen2message_location) {
        ogs_free(warning->paging_nonuen2message_location);
    }
    
    warning->paging_nonuen2message_location = ogs_strdup(location);
    ogs_assert(warning->paging_nonuen2message_location);
}

uint32_t pwsiws_warning_id_alloc(void) 
{ 
    static uint32_t warning_id = 1;
    return warning_id++;
}

uint32_t pwsiws_message_id_alloc(void) 
{ 
    static uint32_t message_id = 1;
    return message_id++;
} 
#include "ogs-core.h"            // Open5GS logging and utilities
#include "real-mme-connection.h" // Function prototypes
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

static ogs_socket_t real_mme_sock = -1; // Global variable to store the SCTP socket

void connect_to_real_mme(void) {
    struct sockaddr_in server_addr;

    // Avoid reconnecting if already connected
    if (real_mme_sock != -1) {
        ogs_warn("Connection to real MME is already established");
        return;
    }

    // Create SCTP socket
    real_mme_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (real_mme_sock < 0) {
        ogs_error("Failed to create SCTP socket");
        return;
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(36412); // S1AP default port
    inet_pton(AF_INET, "192.168.220.9", &server_addr.sin_addr);

    // Connect to the real MME
    if (connect(real_mme_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ogs_error("Failed to connect to real MME at 192.168.220.9:36412");
        close(real_mme_sock);
        real_mme_sock = -1;
        return;
    }

    ogs_info("Successfully connected to real MME at 192.168.220.9:36412");
}

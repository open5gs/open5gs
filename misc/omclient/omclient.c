#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <yaml.h>	// for read yaml
#include <assert.h> // for read yaml
#include <errno.h>	// socket error handling
#define MAX 80
#define SA struct sockaddr
void func(int sockfd);

typedef struct sockinfo_s {
    int domain;
    int socktype;
    uint16_t port;
    uint32_t addr;
} sockinfo_t;

uint32_t iptoint(unsigned char* sourceString)
{
    unsigned short len = 0;
    unsigned char oct[4] = { 0 }, cnt = 0, cnt1 = 0, i, buf[5];

    len = strlen(sourceString);
    for (i = 0; i < len; i++) {
        if (sourceString[i] != '.') {
            buf[cnt++] = sourceString[i];
        }
        if (sourceString[i] == '.' || i == len - 1) {
            buf[cnt] = '\0';
            cnt = 0;
            oct[cnt1++] = atoi(buf);
        }
    }
    uint32_t result;
    result = (oct[3]<<24) | (oct[2]<<16)| (oct[1]<<8) | oct[0];
    printf("result : %x\n", result);
    printf("Ip Address: %03d. %03d. %03d. %03d\n", oct[0], oct[1], oct[2], oct[3]);
    return result;
}

void func(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter the string : ");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client Exit...\n");
			break;
		}
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s", buff);
	}
}

int main(void)
{
  	yaml_parser_t parser;
	FILE *file = fopen("omclient.yaml", "rb");
  	yaml_document_t document;
  	yaml_node_t *node;
  	int i = 1;
  	int j = 0;
  	int sockfd;
	struct sockaddr_in servaddr;
	sockinfo_t omsock;


	assert(file);
  	assert(yaml_parser_initialize(&parser));
  	yaml_parser_set_input_file(&parser, file);

  	if (!yaml_parser_load(&parser, &document)) {
    	goto done;
  	}

  	while(1) {
    	node = yaml_document_get_node(&document, i);
    	if(!node) break;
    	if(node->type == YAML_SCALAR_NODE) {
      		if (!strcmp(node->data.scalar.value, "addr")) {
      			i++;
      			node = yaml_document_get_node(&document, i);
      			printf( "%s\n", node->data.scalar.value );
      			omsock.addr = iptoint(node->data.scalar.value);      			
      		} else if (!strcmp(node->data.scalar.value, "port")) {
      			i++;
      			node = yaml_document_get_node(&document, i);
      			printf( "%s\n", node->data.scalar.value );
      			omsock.port = atoi(node->data.scalar.value);
      		} else if (!strcmp(node->data.scalar.value, "family")) {
      			i++;
      			node = yaml_document_get_node(&document, i);
      			printf( "%s\n", node->data.scalar.value );
      			if (!strcmp(node->data.scalar.value, "AF_INET"))
      				omsock.domain = AF_INET;
      		} else if (!strcmp(node->data.scalar.value, "socktype")) {
      			i++;
      			node = yaml_document_get_node(&document, i);
      			printf( "%s\n", node->data.scalar.value );
      			if (!strcmp(node->data.scalar.value, "SOCK_STREAM"))
      				omsock.socktype = SOCK_STREAM;
      		}       		
    	}
    	i++;
  	}	
  	yaml_document_delete(&document);
  	done:
  	yaml_parser_delete(&parser);
  	assert(!fclose(file));

  	// socket create and verification
	sockfd = socket(omsock.domain, omsock.socktype, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = omsock.domain;
	servaddr.sin_addr.s_addr = omsock.addr;
	servaddr.sin_port = htons(omsock.port);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the server failed... %s\n", strerror(errno));
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
	exit(1);
}


/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "context.h"
#include <fcntl.h>  
#include <unistd.h>
#define MAX 80

#define SA struct sockaddr

int om_sock_open(void);
void om_sock_core(int connfd);

int om_sock_open(void)
{
    int sockfd;
    struct sockaddr_in servaddr;
    ogs_om_socknode_t *om_socknode;
    
    om_socknode = malloc(sizeof(om_socknode));
    om_socknode = amf_self()->om_sock;
    sockfd = socket(om_socknode->family, SOCK_STREAM, 0);
    
    if (sockfd == -1) {
        ogs_debug("socket creation failed...");
    } else {
        om_socknode->sockfd = sockfd;
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = om_socknode->family;      
    servaddr.sin_addr.s_addr = inet_addr(om_socknode->addr);
    servaddr.sin_port = htons(om_socknode->port);

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
        ogs_debug("socket bind failed...");
    
    if ((listen(sockfd, 5)) != 0)
        ogs_debug("Listen failed...");

    return sockfd;  
}

void om_sock_core(int connfd)
{
    int rv;
    int gnb_cnt = 0;
    char buff[MAX];
    fcntl(connfd, F_SETFL, O_NONBLOCK); //for nonblocking read

    for (;;) {
        bzero(buff, MAX);
        rv = read(connfd, buff, sizeof(buff));   // read the message from client and copy it in buffer

        if (ogs_get_queue(ogs_app()->queue)) {
            ogs_debug("ogs_app()->queue->terminated is set to true\n");
            break;
        }  

        if (rv == -1) {
            /* do nothing */
        } else { 
            if (strncmp("gnb_list", buff, 8) == 0) {
                bzero(buff, MAX);
                if ( amf_gnb_list(&gnb_cnt, buff) ) {
                    write(connfd, buff, sizeof(buff));
                } else {
                    write(connfd, "No data\n", sizeof("No data"));
                }
            }
            else if (strncmp("exit", buff, 4) ==0) {
                ogs_debug("break...\n");
                break;
            } else {
                write(connfd, "RECEIVED\n", sizeof("RECEIVED"));
            }
        }
    }
}


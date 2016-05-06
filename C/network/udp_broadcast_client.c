/**
 * Copyright (C) Laxmikant Rashinkar 2016 LK.Rashinkar@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#include "network.h"

/*
 * This app broadcasts a UDP packet once a second on the first available
 * network interface that is not a looback and polls for a response from
 * the server. It can be used, for example, to discover a server
 *
 * If port number is not specified, it defaults to 45990
 *
 * usage: udp_broadcast_client [port]
 */

int main(int argc, char **argv)
{
    struct sockaddr_in server_addr;

    socklen_t   addr_len;
    char        out_buf[4096];
    char        in_buf[4096];
    char        bc_ip[128];
    int         skt;
    int         rv;
    int         opt_val;
    int         opt_len;
    int         port;

    if (argc == 2)
        port = atoi(argv[1]);
    else
        port = 45990;

    get_broadcast_addr(bc_ip, 128);

    if ((skt = socket(AF_INET, SOCK_DGRAM | O_NONBLOCK, 0)) < 0)
    {
        printf("error creating socket\n");
        return -1;
    }

    memset((void *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, bc_ip, &server_addr.sin_addr);

    /* set socket to use MAC-level broadcast */
    opt_val = 1;
    opt_len = sizeof(int);
    setsockopt(skt, SOL_SOCKET, SO_BROADCAST, (char *) &opt_val, opt_len);

    strcpy(out_buf, "This is the client saying Holla dude!");

    while (1)
    {
        /* send message to server */
        rv = sendto(skt, out_buf, (strlen(out_buf) + 1), 0,
            (struct sockaddr *) &server_addr, sizeof(server_addr));
        if (rv < 0)
        {
            printf("error sending to servier\n");
            return -1;
        }

        usleep(1000000);

        /* wait for message from server */
        addr_len = sizeof(server_addr);
        rv = recvfrom(skt, in_buf, sizeof(in_buf), 0, (struct sockaddr *) &server_addr, &addr_len);
        if (rv < 0)
            continue;

        printf("server says: %s \n", in_buf);
        break;
    }

    close(skt);
    return 0;
}

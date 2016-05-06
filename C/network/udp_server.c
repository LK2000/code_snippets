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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>

#include "network.h"

/*
 * This app listens for UDP packets; as soon as the first one is received,
 * it replies to it and terminates
 *
 * If port number is not specified, it defaults to 45990
 *
 * usage: udp_broadcast_client [port]
 */

int main(int argc, char **argv)
{
    struct sockaddr_in   server_addr;
    struct sockaddr_in   client_addr;
    struct in_addr       client_ip_addr;

    socklen_t   addr_len;
    int         skt;
    int         port;
    char        out_buf[4096];
    char        in_buf[4096];
    int         rv;

    if (argc == 2)
        port = atoi(argv[1]);
    else
        port = 45990;

    if ((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("error opening socket\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    rv = bind(skt, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (rv < 0)
    {
        printf("bind error\n");
        return -1;
    }

    printf("Waiting for data...\n");

    addr_len = sizeof(client_addr);
    rv = recvfrom(skt, in_buf, sizeof(in_buf), 0, (struct sockaddr *) &client_addr, &addr_len);
    if (rv < 0)
    {
        printf("recvfrom failed\n");
        return -1;
    }

    /* get client IP addr */
    memcpy(&client_ip_addr, &client_addr.sin_addr.s_addr, 4);

    /* print client IP addr and port */
    printf("client connected, IP=%s port=%d\n", inet_ntoa(client_ip_addr), ntohs(client_addr.sin_port));

    printf("client says: %s\n", in_buf);

    sprintf(out_buf, "ip=192.168.3.109 port=43222");

    rv = sendto(skt, out_buf, (strlen(out_buf) + 1), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    if (rv < 0)
    {
        printf("error writing to client\n");
        return -1;
    }

    close(skt);
    return(0);
}

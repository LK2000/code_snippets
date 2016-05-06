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

#include "network.h"

/**
 * @brief Print information about network interfaces
 *****************************************************************************/
void print_net_info()
{
    struct ifaddrs     *ifap;
    struct ifaddrs     *ifa;
    struct sockaddr_in *sa;
    char               *addr;

     getifaddrs(&ifap);
     for (ifa = ifap; ifa; ifa = ifa->ifa_next)
     {
         if (ifa->ifa_addr && ifa->ifa_addr->sa_family==AF_INET)
         {
             sa = (struct sockaddr_in *) ifa->ifa_addr;
             addr = inet_ntoa(sa->sin_addr);
             printf("Interface: %s\tAddress=%s\t", ifa->ifa_name, addr);

             sa = (struct sockaddr_in *) ifa->ifa_netmask;
             addr = inet_ntoa(sa->sin_addr);
             printf("Netmask=%s\t", addr);

             sa = (struct sockaddr_in *) ifa->ifa_ifu.ifu_broadaddr;
             addr = inet_ntoa(sa->sin_addr);
             printf("Broadcast=%s\n", addr);
         }
     }

     freeifaddrs(ifap);
}

/**
 * @brief Get IP address of the first non loopback interface
 * @param buf  store IP address here
 * @param len  length of buf
 *****************************************************************************/
void get_ip_addr(char *buf, int len)
{
    struct ifaddrs     *ifap;
    struct ifaddrs     *ifa;
    struct sockaddr_in *sa;

    getifaddrs(&ifap);

    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
        {
            /* skip looback interface */
            if (strcasecmp("lo", ifa->ifa_name) == 0)
                continue;

            sa = (struct sockaddr_in *) ifa->ifa_addr;
            strncpy(buf, inet_ntoa(sa->sin_addr), len);
            buf[len - 1] = 0;
            break;
        }
    }

    freeifaddrs(ifap);
}

/**
 * @brief Get broadcast address of the first non loopback interface
 * @param buf  store broadcast address here
 * @param len  length of buf
 *****************************************************************************/
void get_broadcast_addr(char *buf, int len)
{
    struct ifaddrs     *ifap;
    struct ifaddrs     *ifa;
    struct sockaddr_in *sa;

    getifaddrs(&ifap);

    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
        {
            /* skip looback interface */
            if (strcasecmp("lo", ifa->ifa_name) == 0)
                continue;

            sa = (struct sockaddr_in *) ifa->ifa_ifu.ifu_broadaddr;
            strncpy(buf, inet_ntoa(sa->sin_addr), len);
            buf[len - 1] = 0;
            break;
        }
    }

    freeifaddrs(ifap);
}

/**
 * @brief Connect to specified server on specified port
 * @param skt      return socket descriptor here
 * @param ip_addr  IP of server to connect to
 * @param port     port on which to connect
 * @return 0 on success, -1 on error
 ****************************************************************************/
int connect_to_server(int *skt, char *ip_addr, int port)
{
    struct sockaddr_in server_addr;
    int flags;

    *skt = socket(AF_INET, SOCK_STREAM, 0);
    if (*skt < 0)
        return -1;

    memset((void *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_addr, &server_addr.sin_addr) <= 0)
    {
        close(*skt);
        *skt = -1;
        return -1;
    }

    if (connect(*skt, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        close(*skt);
        *skt = -1;
        return -1;
    }

    flags = fcntl(*skt, F_GETFL, 0);
    fcntl(*skt, F_SETFL, flags | O_NONBLOCK);

    return 0;
}

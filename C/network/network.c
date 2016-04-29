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
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>

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

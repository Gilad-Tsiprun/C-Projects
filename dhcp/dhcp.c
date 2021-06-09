#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <math.h> /* pow */
#include "dhcp.h"
#include "trie.h"

#define INT_BIT_SIZE (32)
#define CHAR_MAX (255)
#define CHAR_BIT_SIZE (8)

struct dhcp
{
    trie_t *trie;
    unsigned int network_ip;
    size_t subnet_mask;
};

#define SUCCESS (0)

/* Status: Sent */


static unsigned int IPToInt(const ip_t ip)
{
    int i = 0;
    unsigned int res = 0;
    size_t mask = 24;

    for (; i < 4; i++)
    {
        res |= (ip[i] << mask);

        mask -= CHAR_BIT_SIZE;
    }
    
    return res;
}

static void IntToIP(const int n, ip_t ip)
{
    int i = 0;
    size_t mask = 24;

    for (; i < 4; i++)
    {
        unsigned char char_mask = CHAR_MAX;

        ip[i] = ((n >> mask) & char_mask);

        mask -= CHAR_BIT_SIZE;
    }
}

static unsigned int GetNetWorkAddress(unsigned int int_subnet_id, size_t wildcard)
{
    unsigned int network_ip = int_subnet_id;
    size_t mask = pow(2, wildcard) - 1;

    assert(wildcard != 0);
    network_ip &= ~(mask); /*nullify all the least signifcant bits of the wildcard*/
    return network_ip;
}

dhcp_t *DhcpCreate(const ip_t subnet_id, size_t subnet_mask_length)
{
    dhcp_t *dhcp = malloc(sizeof(dhcp_t));
    size_t wildcard = INT_BIT_SIZE - subnet_mask_length;
    unsigned int network_ip = GetNetWorkAddress(IPToInt(subnet_id), wildcard);
    unsigned int network = 0;
    unsigned int broadcast = pow(2, wildcard) - 1;
    unsigned int dhcp_server = broadcast - 1;

    assert(subnet_id);

    if (dhcp == NULL)
    {
        return NULL;
    }

    dhcp->subnet_mask = subnet_mask_length;
    dhcp->network_ip = network_ip;
    dhcp->trie = TrieCreate(wildcard);

    if (dhcp->trie == NULL)
    {
        free(dhcp);
        dhcp = NULL;

        return NULL;
    }

    if (TrieInsert(dhcp->trie, network) != SUCCESS) /*allocate network address*/
    {
        TrieDestroy(dhcp->trie);
        free(dhcp);

        return NULL;
    }

    if (TrieInsert(dhcp->trie, broadcast) != SUCCESS) /*allocate broadcast address*/
    {
        TrieDestroy(dhcp->trie);
        free(dhcp);
           
        return NULL;
    }

    if (TrieInsert(dhcp->trie, dhcp_server) != SUCCESS) /*allocate dhcp server address*/
    {
        TrieDestroy(dhcp->trie);
        free(dhcp);
           
        return NULL;
    }

    return dhcp;
}

void DhcpDestroy(dhcp_t *dhcp)
{
    assert(dhcp);

    TrieDestroy(dhcp->trie);

    free(dhcp);
    dhcp = NULL;
}

int DhcpAllocateIp(dhcp_t *dhcp, const ip_t request_ip, ip_t result_ip)
{
    size_t wildcard = 0;
    unsigned int ip_requested = 0;
    unsigned int ip_recieved = 0;
    unsigned int network_ip = 0;
    unsigned int dhcp_ip = 0;
    ip_t network = {0};

    assert(dhcp);

    wildcard = INT_BIT_SIZE - dhcp->subnet_mask;
    network_ip = dhcp->network_ip;
    dhcp_ip = network_ip + pow(2, wildcard) - 2;

    if (!request_ip) /*if null Allocate starting from network ip*/
    {
       IntToIP(network_ip, network);
       return DhcpAllocateIp(dhcp, network, result_ip);
    }
    else
    {
        ip_requested = IPToInt(request_ip);
    }
    
    if (network_ip > ip_requested || dhcp_ip <= ip_requested) /*if ip requested smaller than network or bigger than dhcp server*/
    {
        result_ip = NULL;

        return -1;
    }
    else
    {
        ip_recieved = TrieAvailableKeyFrom(dhcp->trie, ip_requested - network_ip); /*ip in trie saved from end of network_ip*/
    } 

    if (TrieInsert(dhcp->trie, ip_recieved) != SUCCESS)
    {
        return -1;
    }


    IntToIP(ip_recieved + network_ip, result_ip); /*ip in trie saved from end of network_ip*/

    return 0;
}

void DhcpFreeIp(dhcp_t *dhcp, const ip_t ip)
{
    assert(dhcp);
    
    TrieRemove(dhcp->trie, IPToInt(ip) - dhcp->network_ip); /*ip in trie saved from end of network_ip*/
}

size_t DhcpCountFree(const dhcp_t *dhcp)
{
    size_t client_address_count = 0;
    size_t used_clients_count = 0;
    size_t wildcard = 0;
    
    assert(dhcp);

    wildcard = INT_BIT_SIZE - dhcp->subnet_mask;
    client_address_count = pow(2, (wildcard));
    used_clients_count = TrieCountFullKeys(dhcp->trie);

    return (client_address_count - used_clients_count);
}


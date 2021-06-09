#ifndef __DHCP__H__
#define __DHCP__H__

/* 
	Version: 1.0
	last update: 09.07.2020 15:30
	made by: OL89
*/

#include <stddef.h> /* size_t */

typedef struct dhcp dhcp_t;

#define IPV4 4
typedef unsigned char ip_t[IPV4];



/*
	recives subnet_id, and length of the subnet.
	return pointer to dhcp_t, in failure returns null.
	Time complexity: O(1)
*/
dhcp_t *DhcpCreate(const ip_t subnet_id, size_t subnet_mask_length);


void DhcpDestroy(dhcp_t *dhcp);

/*
	The return ip will be the smallest one available,
	that is bigger or equal to the requested one
	if request_ip is NULL, result_ip will be the smallest available ip.
	return non-zero value for failure.
	Time complexity: O(log(n))
*/
int DhcpAllocateIp(dhcp_t *dhcp, const ip_t request_ip, ip_t result_ip);


/*
	Free the given ip to be available
	Time complexity: O(log(n))
*/
void DhcpFreeIp(dhcp_t *dhcp, const ip_t ip);


/*
	return the amount of available ip
	Time complexity: O(n)
*/
size_t DhcpCountFree(const dhcp_t *dhcp);








#endif /* __DHCP__H__ */
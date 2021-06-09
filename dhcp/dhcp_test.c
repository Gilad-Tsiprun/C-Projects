#include <stdio.h> /* printf */
#include "dhcp.h"

#define RED_COLOR ("\033[1;31m")
#define GREEN_COLOR ("\033[1;32m")
#define RESET_COLOR ("\033[0m")

#define GET_ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static void ReportFailure(char *func_name, char *fail_message,char *test_name,
                          size_t *test_case, size_t *bugs)
{
    (*test_case)++;
    (*bugs)++;
    printf("%s not working: %s\n\
    %s, test case faild: %ld\n",func_name,fail_message,test_name,*test_case);

}

static void PrintTestStatus(char *func_name, size_t bugs)
{
    if (bugs)
    {
        printf("%s %snot working%s: %ld bugs found!\n",
                func_name,RED_COLOR,RESET_COLOR,bugs);
    }
    else
    {
        printf("%s %sworks good!%s\n",
                func_name,GREEN_COLOR,RESET_COLOR);
    }
    
}


/* returns 0 if ip1 is equal to ip2,
otherwise returns non-zero */
int CompareIp(ip_t ip1, ip_t ip2)
{
    size_t i = 0;

    for (; i < IPV4; i++)
    {
        if (ip1[i] != ip2[i])
        {
            return 1;
        }
    }
    
    return 0;
}


static void TestCreateDestroy(void)
{
    dhcp_t *dhcp = NULL;
    ip_t subnet = {192,168,111,0};
    size_t mask_len = 24;

    dhcp = DhcpCreate(subnet, mask_len);

    if (dhcp)
    {
        PrintTestStatus("DhcpCreate", 0);
    }
    else
    {
        PrintTestStatus("DhcpCreate", 1);
    }
    
    DhcpDestroy(dhcp);
}


static void TestAllocateIPBasic(void)
{
    size_t test_case = 0;
    size_t bugs = 0;
    size_t i = 0;
    ip_t subnet = {192,168,123,0};
    size_t subnet_mask_len = 24;
    ip_t requests[4] = {{192,168,123,5},{192,168,123,39},{192,168,123,200},{192,168,123,39}};
    ip_t expected[4] = {{192,168,123,5},{192,168,123,39},{192,168,123,200},{192,168,123,40}};

    dhcp_t *dhcp = DhcpCreate(subnet,subnet_mask_len);

    for (i = 0; i < GET_ARR_SIZE(requests); i++)
    {
        ip_t test_ip = {0,0,0,0};
        int status = 0;
        
        status = DhcpAllocateIp(dhcp,requests[i],test_ip);

        if (status)
        {
            ReportFailure("DhcpAllocateIp","return value indicate allocation fail",
                            "TestAllocateIPBasic", &test_case, &bugs);
        }

        if (CompareIp(expected[i],test_ip))
        {
            ReportFailure("DhcpAllocateIp","did not allocate IP correctly",
                            "TestAllocateIPBasic", &test_case, &bugs);
        } 
    }
    
    DhcpDestroy(dhcp);

    PrintTestStatus("DhcpAllocateIp", bugs);
}


static void TestFreeIP(void)
{
    size_t test_case = 0;
    size_t bugs = 0;
    size_t i = 0;
    ip_t subnet = {192,168,123,0};
    size_t subnet_mask_len = 24;
    
    ip_t request = {192,168,123,39};
    ip_t expected[3] = {{192,168,123,39},{192,168,123,40},{192,168,123,39}};
    ip_t test_ip = {0,0,0,0};
    dhcp_t *dhcp = DhcpCreate(subnet,subnet_mask_len);

    DhcpAllocateIp(dhcp,request,test_ip);

    if (CompareIp(expected[i++],test_ip))
    {
        ReportFailure("DhcpAllocateIp","did not allocate IP correctly",
                            "TestFreeIP", &test_case, &bugs);
    }

    DhcpAllocateIp(dhcp,request,test_ip);

    if (CompareIp(expected[i++],test_ip))
    {
        ReportFailure("DhcpAllocateIp","did not allocate IP correctly",
                            "TestFreeIP", &test_case, &bugs);
    }

    DhcpFreeIp(dhcp,request);
    
    DhcpAllocateIp(dhcp,request,test_ip);

    if (CompareIp(expected[i++],test_ip))
    {
        ReportFailure("DhcpFreeIp","did not free requested IP",
                            "TestFreeIP", &test_case, &bugs);
    }

    DhcpDestroy(dhcp);

    PrintTestStatus("DhcpFreeIp", bugs);
}


static void TestCountFree(void)
{
    size_t test_case = 0;
    size_t bugs = 0;
    size_t i = 0;
    ip_t subnet = {192,168,123,0};
    size_t subnet_mask_len = 24;
    size_t expected[] = {253,153};
    dhcp_t *dhcp = DhcpCreate(subnet,subnet_mask_len);

    if (DhcpCountFree(dhcp) != expected[0])
    {
        ReportFailure("DhcpCountFree","wrong return value","TestCountFree",
                        &test_case, &bugs);
    }

    for (i = 0; i < 100; i++)
    {
        ip_t test_ip = {0,0,0,0};
        ip_t expected = {192,168,123,0};
        expected[3] = i + 1;
        
        if (DhcpAllocateIp(dhcp,NULL,test_ip))
        {
            ReportFailure("DhcpAllocateIp","did not allocate IP",
                            "TestCountFree", &test_case, &bugs);
        }
        
        if (CompareIp(test_ip,expected))
        {
            ReportFailure("DhcpAllocateIp","did not allocate IP correctly",
                            "TestCountFree", &test_case, &bugs);
        }
    }

    if (DhcpCountFree(dhcp) != expected[1])
    {
        ReportFailure("DhcpCountFree","wrong return value","TestCountFree",
                        &test_case, &bugs);
    }
    
    DhcpDestroy(dhcp);

    PrintTestStatus("DhcpCountFreeIp", bugs);
}


static void TestStress(void)
{
    size_t test_case = 0;
    size_t bugs = 0;
    size_t i = 0;
    size_t j = 0;
    ip_t subnet = {192,168,42,6};
    size_t subnet_mask_len = 16;
    dhcp_t *dhcp = DhcpCreate(subnet,subnet_mask_len);

    for (i = 0; i < 255; i++)
    {
        ip_t test_ip = {0,0,0,0};
        ip_t expected = {192,168,0,0};
        int status = DhcpAllocateIp(dhcp,NULL,test_ip);
        
        expected[3] = i + 1;

        if (status)
        {
            ReportFailure("DhcpAllocateIp","did not allocate IP",
                        "TestCountFree", &test_case, &bugs);
        }

        if (CompareIp(test_ip,expected))
        {
            ReportFailure("DhcpAllocateIp","did not allocate IP correctly",
                            "TestCountFree", &test_case, &bugs);
        }
    }
    

    for (i = 1; i < 256; i++)
    {
        for (j = 0; j < 256; j++)
        {
            ip_t test_ip = {0,0,0,0};
            ip_t expected = {192,168,0,0};
            int status = DhcpAllocateIp(dhcp,NULL,test_ip);
            
            expected[2] = i;
            expected[3] = j;          

            if (status)
            {
                if ( i == 255 && (j == 254 || j == 255))
                {
                    continue;
                }
                else
                {
                ReportFailure("DhcpAllocateIp","did not allocate IP",
                            "TestCountFree", &test_case, &bugs);
                }
            }
            
            if (CompareIp(test_ip,expected))
            {
                ReportFailure("DhcpAllocateIp","did not allocate IP correctly",
                                "TestCountFree", &test_case, &bugs);
            }
        }
    }

    DhcpDestroy(dhcp);

    PrintTestStatus("Dhcp Stress Test", bugs);
}


int main()
{
    TestCreateDestroy();
    TestAllocateIPBasic();
    TestFreeIP();
    TestCountFree();
    TestStress();

    return 0;
}
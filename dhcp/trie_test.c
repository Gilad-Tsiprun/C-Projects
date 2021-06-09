#include <stdio.h> /* printf */
#include <string.h> /* strcmp */
#include "trie.h"

#define RED_COLOR ("\033[1;31m")
#define GREEN_COLOR ("\033[1;32m")
#define RESET_COLOR ("\033[0m")

#define GET_ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

void ReportFailure(char *func_name, char *fail_message,char *test_name,
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


static void TestCreateDestroy(void)
{
    trie_t *trie = NULL;

    trie = TrieCreate();

    if (trie)
    {
        PrintTestStatus("TrieCreate",0);
    }
    else
    {
        PrintTestStatus("TrieCreate",1);
    }
    
    TrieDestroy(trie);
}


static void TestPushRemove(void)
{
    size_t test_case = 0;
    size_t bugs = 0;
    char *inputs[] = {"1100","0011","1101","1110"};
    trie_t *trie = TrieCreate();
    size_t i = 0;

    for (i = 0; i < GET_ARR_SIZE(inputs); i++)
    {
        int status  = TriePush(trie,inputs[i]);

        if (status)
        {
            ReportFailure("TriePush","did not insert data to trie","TestPushRemove",
                            &test_case, &bugs);
            printf("\tstatus = %d\n",status);
        }
    }

    for (i = 0; i < GET_ARR_SIZE(inputs); i++)
    {
        int status  = TriePush(trie,inputs[i]);

        if (status != 0)
        {
            ReportFailure("TriePush","wrong return value","TestPushRemove",
                            &test_case, &bugs);
            printf("\tstatus = %d\n",status);
        }
    }

    TrieRemove(trie, "1101");
    TrieRemove(trie, "1110");
    
    TrieDestroy(trie);
    PrintTestStatus("TriePush and TrieRemove",bugs);

}


static void TestCount(void)
{
    size_t test_case = 0;
    size_t bugs = 0;
    char *inputs[] = {"0001","1011","1000","1001","0101"};
    trie_t *trie = TrieCreate();
    size_t i = 0;

    if (TrieCount(trie) != 0)
    {
        ReportFailure("TrieCount","did not return 0 for an empty trie",
                        "TestCount",&test_case,&bugs);
    }
    
    for (i = 0; i < GET_ARR_SIZE(inputs); i++)
    {
        TriePush(trie,inputs[i]);
    }

    if (TrieCount(trie) != 5)
    {
        ReportFailure("TrieCount","wrong return value",
                        "TestCount",&test_case,&bugs);
    }
    
    i = 0;
    TrieRemove(trie, inputs[i++]);
    TrieRemove(trie, inputs[i++]);
    
    if (TrieCount(trie) != 3)
    {
        ReportFailure("TrieRemove","did not remove element from trie",
                        "TestCount",&test_case,&bugs);
    }

    TrieRemove(trie, inputs[i++]);
    TrieRemove(trie, inputs[i++]);
    TrieRemove(trie, inputs[i++]);
    
    if (TrieCount(trie) != 0)
    {
        ReportFailure("TrieCount","did not return 0 for an empty trie",
                        "TestCount",&test_case,&bugs);
    }

    for (i = 0; i < GET_ARR_SIZE(inputs); i++)
    {
        TriePush(trie,inputs[i]);
    }

    if (TrieCount(trie) != 5)
    {
        ReportFailure("TrieCount","wrong return value",
                        "TestCount",&test_case,&bugs);
    }

    TrieDestroy(trie);
    PrintTestStatus("TrieCount",bugs);
}


static void TestNextAvailable(void)
{
    size_t test_case = 0;
    size_t bugs = 0;
    char *inputs[] = {"0001","1011","1000","1001","0101","0111"};
    trie_t *trie = TrieCreate();
    size_t i = 0;
    char buffer_1[] = "0010";
    char buffer_2[] = "0001";
    char buffer_3[] = "0000";
    char buffer_4[] = "1011";
    char buffer_5[] = "0111";
    char buffer_6[] = "1111";
    char *expected[] = {"0010","0010","0000","1100","1010"};
    int status = 0;

    for (i = 0; i < GET_ARR_SIZE(inputs); i++)
    {
        TriePush(trie,inputs[i]);
    }

    i = 0;
    
    status = TrieNextAvailable(trie,buffer_1);

    if (strcmp(buffer_1, expected[i++]))
    {
        ReportFailure("TrieNextAvailable","wrong return value",
                        "TestNextAvailable",&test_case, &bugs);
    }
    
    if (status != SUCCESS)
    {
        ReportFailure("TrieNextAvailable","wrong return status",
                        "TestNextAvailable",&test_case, &bugs);
    }
    
    status = TrieNextAvailable(trie,buffer_2);
    
    if (strcmp(buffer_2, expected[i++]))
    {
        ReportFailure("TrieNextAvailable","wrong return value",
                        "TestNextAvailable",&test_case, &bugs);
    }

    if (status != SUCCESS)
    {
        ReportFailure("TrieNextAvailable","wrong return status",
                        "TestNextAvailable",&test_case, &bugs);
    }

    status = TrieNextAvailable(trie,buffer_3);
        
    if (strcmp(buffer_3, expected[i++]))
    {
        ReportFailure("TrieNextAvailable","wrong return value",
                        "TestNextAvailable",&test_case, &bugs);
    }

    if (status != SUCCESS)
    {
        ReportFailure("TrieNextAvailable","wrong return status",
                        "TestNextAvailable",&test_case, &bugs);
    }

    status = TrieNextAvailable(trie,buffer_4);
    
    if (strcmp(buffer_4, expected[i++]))
    {
        ReportFailure("TrieNextAvailable","wrong return value",
                        "TestNextAvailable",&test_case, &bugs);
    }

    if (status != SUCCESS)
    {
        ReportFailure("TrieNextAvailable","wrong return status",
                        "TestNextAvailable",&test_case, &bugs);
    }

    status = TrieNextAvailable(trie,buffer_5);
    
    if (strcmp(buffer_5, expected[i++]))
    {
        ReportFailure("TrieNextAvailable","wrong return value",
                        "TestNextAvailable",&test_case, &bugs);
    }

    if (status != SUCCESS)
    {
        ReportFailure("TrieNextAvailable","wrong return status",
                        "TestNextAvailable",&test_case, &bugs);
    }

    TriePush(trie,"1111");

    status = TrieNextAvailable(trie,buffer_6);
    
    if (strcmp(buffer_6, "1111"))
    {
        ReportFailure("TrieNextAvailable","wrong return value",
                        "TestNextAvailable",&test_case, &bugs);
    }

    if (status != 0)
    {
        ReportFailure("TrieNextAvailable","wrong return status",
                        "TestNextAvailable",&test_case, &bugs);
    }

    TrieDestroy(trie);
    PrintTestStatus("TrieNextAvailable",bugs);
}


static void TestNextAvailableStress(void)
{
    size_t test_case = 0;
    size_t bugs = 0;
    char *inputs[] = {"0000","0001","0010","0011","0100","0101","0110","0111","1000",
                        "1001","1010","1011","1100","1101","1110"};
    trie_t *trie = TrieCreate();
    size_t i = 0;
    char buffer_1[] = "0000";
    char buffer_2[] = "0000";
    char buffer_3[] = "0000";
    char *expected[] = {"1111","0111"};
    int status = 0;

    for (i = 0; i < GET_ARR_SIZE(inputs); i++)
    {
        TriePush(trie,inputs[i]);
    }

    i = 0;

    status = TrieNextAvailable(trie,buffer_1);

    if (strcmp(buffer_1, expected[i++]))
    {
        ReportFailure("TrieNextAvailable","wrong return value",
                        "TestNextAvailableStress",&test_case, &bugs);
    }
    
    if (status != SUCCESS)
    {
        ReportFailure("TrieNextAvailable","wrong return status",
                        "TestNextAvailableStress",&test_case, &bugs);
    }

    TriePush(trie,buffer_1);

    status = TrieNextAvailable(trie,buffer_2);
    
    if (status != 0)
    {
        ReportFailure("TrieNextAvailable","wrong return status",
                        "TestNextAvailable",&test_case, &bugs);
    }

    TrieRemove(trie, "0111");

    status = TrieNextAvailable(trie,buffer_3);

    if (strcmp(buffer_3, expected[i++]))
    {
        ReportFailure("TrieNextAvailable","wrong return value",
                        "TestNextAvailableStress",&test_case, &bugs);
    }
    
    if (status != SUCCESS)
    {
        ReportFailure("TrieNextAvailable","wrong return status",
                        "TestNextAvailableStress",&test_case, &bugs);
    }


    TrieDestroy(trie);
    PrintTestStatus("TrieNextAvailable - stress test",bugs);

}


int main()
{
    TestCreateDestroy();
    TestPushRemove();
    TestCount();
    TestNextAvailable();
    TestNextAvailableStress();

    return 0;
}
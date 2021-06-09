/*
	status: initial file
	tested: N/A
	reviewed by: not sent
*/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <string.h> /* strcmp, strlen, strcpy */

#include "knight_tour.h"

#define NUM_OF_BUCKETS 10

typedef enum status
{
	SUCCESS,
	FAIL
}status_t;

#define BUFFER_SIZE 8

#define NUM_OF_ARRAY_ELM(array) (sizeof(array)/sizeof(array[0]))

#define TEST_STATUS(func, str) ((func != 0) ? PrintFail(str) : PrintPass(str))
#define TEST_VAL(func, str) ((func != 1) ? PrintFail(str) : PrintPass(str))

static void PrintFail(char *str)
{
	printf("\033[0;31m");
	printf ("%s Failed \n", str);
	printf("\033[0m");
}


static void PrintPass(char *str)
{
	printf("\033[1;32m");
	printf ("%s pass \n", str);
	printf("\033[0m");
}


int HasOnlyUniquePathPoints(point_t *tour_path)
{
	size_t i = 0;

	for (; i < BOARD_SIZE; i++)
	{
		size_t j = i + 1;

		for (; j < BOARD_SIZE; j++)
		{
			if ((tour_path[i].col == tour_path[j].col) &&
					(tour_path[i].row == tour_path[j].row))
			{
				return 0;
			}
		}
	}

	return 1;
}


static point_t IndexToPoint(size_t index)
{
    point_t point = {0};

    point.col = index % BOARD_DIMENTION;
    point.row = index / BOARD_DIMENTION;

    return point;
}


void PrintThePoints(point_t *result_path)
{
    size_t i = 0;
    int board_index[BOARD_SIZE] = {0};

    for (i = 0; i < BOARD_SIZE; i++)
    {
        board_index[result_path->row * BOARD_DIMENTION + result_path->col] = i;
        result_path++;
    }

    for (i = 0; i < BOARD_SIZE; i++)
    {
        if((i % BOARD_DIMENTION) == 0)
        {
            printf("\n");
        }
        if(board_index[i] < 10) 
        {
            printf(" ");
        }
        printf("  %d  ", board_index[i]);
    }
}


int main()
{
	size_t i = 0;

	for (; i < 64; i++)
	{
		point_t tour_path[BOARD_SIZE] = {0};
		point_t start = IndexToPoint(i);

		printf("\nKnightTour from index %lu\n", i);
		
		TEST_STATUS(KnightTour(start, 1, tour_path), "KnightTour");
		TEST_VAL(HasOnlyUniquePathPoints(tour_path), "HasOnlyUniquePathPoints");
		PrintThePoints(tour_path); 
	}

	return 0;
}
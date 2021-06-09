#include <math.h> /*log*/
#include <stdlib.h> /*qsort*/
#include <time.h> /*time*/
#include "knight_tour.h"
#include "bit_array.h"

#define BOARD_DIMENSION (64)
#define POINT_TO_INDEX(point) ((point.row * 8) + (point.col))
#define INDEX_TO_POINT_ROW(index) ((index) / 8)
#define INDEX_TO_POINT_COL(index) ((index) % 8)
#define SUCCESS (0)

unsigned long Log2(unsigned long base)
{
    return log(base) / log(2);
}

typedef struct next_available_moves
{
    size_t index;
    bitarray_t available_moves;
}next_available_moves_t;

int CmpMoveIndex(const void *index1, const void *index2)
{
    next_available_moves_t move1 = (*(next_available_moves_t *)index1);
    next_available_moves_t move2 = (*(next_available_moves_t *)index2);

    return ((int)BitArrayCountOn(move1.available_moves) - (int)BitArrayCountOn(move2.available_moves));
}


static bitarray_t PossibleMoves(size_t index)
{
    long row[] = {1, 1, 2, 2, -1, -1, -2, -2}; 
    long col[] = {2, -2, 1, -1, 2, -2, 1, -1}; 

    point_t point = {0};
    bitarray_t moves = {0};
    size_t i = 0;

    point.row = INDEX_TO_POINT_ROW(index);
    point.col = INDEX_TO_POINT_COL(index);

    for (; i < BOARD_DIMENTION; i++)
    {
        point_t tmp = {0};

        tmp.row = (long)point.row + row[i];
        tmp.col = (long)point.col + col[i];

        if ((tmp.row < BOARD_DIMENTION) && (tmp.col < BOARD_DIMENTION))
        {
            moves = BitArraySetOn(moves, POINT_TO_INDEX(tmp));
        }
    }

    return moves;
}

static bitarray_t UpdateAvailableMoves(bitarray_t available_moves, bitarray_t move_history)
{
    available_moves |= move_history;
    available_moves ^= move_history;

    return available_moves;
}

bitarray_t GetFirstAvailableMove(bitarray_t available_moves) 
{ 
    return Log2(available_moves & -available_moves); 
} 

void AvailableMovesArr(next_available_moves_t available_moves_arr[], 
                                        bitarray_t available_moves, bitarray_t move_history)
{
    size_t i = 0;

    while (available_moves)
    {
        bitarray_t next_move_available_moves = {0};

        available_moves_arr[i].index = GetFirstAvailableMove(available_moves);

        next_move_available_moves = PossibleMoves(available_moves_arr[i].index);
        next_move_available_moves = UpdateAvailableMoves(next_move_available_moves, move_history);
        available_moves_arr[i].available_moves = next_move_available_moves;

        available_moves = BitArraySetOff(available_moves, available_moves_arr[i].index);
        i++;
    }
}

int RecKnightTour(size_t current_move,  bitarray_t move_history, time_t timeout_sec, point_t tour_path_arr[BOARD_SIZE])
{
    size_t next_move = 0;
    size_t i = 0;
    size_t available_moves_count = 0;
    bitarray_t available_moves = 0;
    next_available_moves_t next_moves_arr[8] = {0};
    int status = 1;
    time_t curr_time = time(NULL);

    move_history = BitArraySetOn(move_history, current_move);

    tour_path_arr[0].col = INDEX_TO_POINT_COL(current_move); 
    tour_path_arr[0].row = INDEX_TO_POINT_ROW(current_move);

    available_moves = PossibleMoves(current_move);
    available_moves = UpdateAvailableMoves(available_moves, move_history);

    available_moves_count = BitArrayCountOn(available_moves);
    AvailableMovesArr(next_moves_arr, available_moves, move_history);
    qsort(next_moves_arr, available_moves_count, sizeof(next_available_moves_t), CmpMoveIndex);

    if (BitArrayCountOn(move_history) == BOARD_SIZE)
    {
        status = SUCCESS;
    }

    while (status && i < available_moves_count && curr_time < timeout_sec)
    {
        curr_time = time(NULL);
        next_move = next_moves_arr[i].index;
        
        status = RecKnightTour(next_move, move_history, timeout_sec, tour_path_arr + 1);

        i++;
    }

    return status;
}


int KnightTour(point_t start_point, size_t timeout_sec, 
                                point_t tour_path[BOARD_SIZE])
{
    bitarray_t move_history = 0;
    size_t start_move = POINT_TO_INDEX(start_point);

    return RecKnightTour(start_move, move_history, timeout_sec + time(NULL), tour_path);
}
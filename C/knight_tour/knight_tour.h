#ifndef __KNIGHT_TOUR_H__
#define __KNIGHT_TOUR_H__

/* 
	Version: 1.1
	Last update: 14.07.2020 16:00
	Made by: OL89
*/

#include <stddef.h> /* size_t */

#define BOARD_DIMENTION (8)
#define BOARD_SIZE (BOARD_DIMENTION * BOARD_DIMENTION)

typedef struct point
{
    size_t row;
    size_t col;
} point_t;


/*
	recives start point, range of row and col of the point
    is 1 to BOARD_DIMENTION.
    the board starts at leftmost top position.
    timeout_sec is the limited time for the function to run.
    KnightTour puts the found path in tour_path and
    returns non-zero if not found.
*/
int KnightTour(point_t start_point, size_t timeout_sec, 
                                point_t tour_path[BOARD_SIZE]);


#endif /* __KNIGHT_TOUR_H__ */
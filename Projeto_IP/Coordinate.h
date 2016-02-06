#ifndef COORDINATE_H_INCLUDED
#define COORDINATE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// Estruturas-base
typedef enum Direction
{
    LEFT, RIGHT, DOWN, UP
} Direction;

typedef struct Coord
{
    double x, y;
    Direction direction;
    struct Coord *next;

} Coord;

/* Funções para a Coordinate */
Coord *createCoordinate(double x, double y);
int  appendCoordinate (Coord *coord, double x, double y);
void destroyCoordList(Coord *coord);

double distanceBetween(double x1, double y1, double x2, double y2);


#endif // COORDINATE_H_INCLUDED

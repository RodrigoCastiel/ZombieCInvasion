#include "Coordinate.h"

// Construtor
Coord *createCoordinate(double x, double y)
{
    Coord *coord = (Coord *)malloc(sizeof(Coord));

    if(coord != NULL)
    {
        coord->x = x;
        coord->y = y;
        coord->next = NULL;
    }

    return coord;
}

// Insere no final da lista encadeada uma coordenada
int appendCoordinate(Coord *coord, double x, double y)
{
    int inserted = 0;

    if(coord != NULL)
    {
        while(coord->next != NULL)
            coord = coord->next;

        coord->next = createCoordinate(x, y);

        if(coord->next != NULL)
            inserted = 1;
    }

    return inserted;
}

// Liberar a lista recursivamente
void destroyCoordList(Coord *coord)
{
    if(coord != NULL)
    {
        if(coord->next != NULL)
            destroyCoordList(coord->next);

        free(coord);
    }
}

double distanceBetween(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}













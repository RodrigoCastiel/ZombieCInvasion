#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Coordinate.h"


#define     FIELD_WIDTH     16
#define     FIELD_HEIGHT    12

#define     BAR_WIDTH      300
#define     BAR_HEIGHT     600
#define     TILE_SIZE       50

/* Tipo Estruturados */

typedef struct Enemy
{
    BITMAP *sheet;
    Coord *centre;
    Coord *dest;
    Direction orientation;
    double speed, elapsedDistance;

    char type;          // Se é normal, resistente à splash, forte, ...
    int hp, def;        // Vida, defesa
    int level, money;
    int slowTax, poisonTax, maxHP;
    int blenderTax;

} Enemy;


// Assinaturas:

//inline void destroyBitmap(BITMAP **bitmap);

/* Funções para Enemy*/
Enemy *createEnemy(Coord *path, BITMAP *spriteSheet, char type, int level);
void drawEnemy(Enemy *enemy, BITMAP *buffer, BITMAP *auxiliar, unsigned int tick);
int  moveEnemy(Enemy *enemy);


#endif // ENEMY_H_INCLUDED

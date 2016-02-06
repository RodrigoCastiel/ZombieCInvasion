#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <allegro.h>

#include "Enemy.h"
#include  "Wave.h"
#include <math.h>

#define     FIELD_WIDTH     16
#define     FIELD_HEIGHT    12

#define     BAR_WIDTH      300
#define     BAR_HEIGHT     600
#define     TILE_SIZE       50

/////////////////////////////////////////////////////////////

/* Tipo Estruturados */


typedef struct Field    // O Mapa
{
    int tile[FIELD_HEIGHT][FIELD_WIDTH];
    Coord  *path;
    Wave   *lastActivetedEnemy;

    BITMAP *sheet;  // O bitmap único para todos os elementos do cenário
    BITMAP *buffer; // Um buffer para colar as pequenas imagens temporariamente
    BITMAP *fog;
    Wave  **waves;  // Vetor de ondas de zoombies
    int currentWave, waveNumber;

} Field;


// Assinaturas:

void destroyBitmap(BITMAP **bitmap);

/* Funções para field */
Field *createField(FILE *inputFile, BITMAP *spriteSheet, BITMAP *fog);
void  destroyField (Field *field);
int   findPath(Field *field);
int createEnemies(Field *field, FILE *inputFile, BITMAP *normalSheet, BITMAP *resistentSheet, BITMAP *fastSheet, BITMAP *strongSheet, BITMAP *immuneSheet, BITMAP *bossSheet);

void drawFieldTerrain(Field  *field, BITMAP *buffer);
void drawActiveEnemies(Wave*, BITMAP *buffer, BITMAP *auxiliar, unsigned int tick);
void drawFieldObjects(Field  *field, BITMAP *buffer, unsigned int tick);
void spawnEnemies(Field *field, unsigned int tick);

#endif // FIELD_H_INCLUDED

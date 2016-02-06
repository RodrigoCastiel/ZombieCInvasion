#ifndef WAVE_H_INCLUDED
#define WAVE_H_INCLUDED

#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Enemy.h"
#include "GraphicsAuxiliar.h"

// Onda de inimigos: uma lista encadeada
typedef struct Wave
{
    Enemy *enemy;
    unsigned int timeToNext;
    int isEnabled;
    struct Wave *next;
    struct Wave *prev;
} Wave;

/* Funções para Wave */
Wave *createWaveNode(Enemy *enemy, unsigned int timeToNext);
Wave* appendWaveNode(Wave *wave, Enemy *enemy, unsigned int timeToNext);
void destroyWave(Wave *wave);
int     moveWave(Wave *wave);
void   printWave(Wave *wave);

void selectEnemyByMouse(Wave *wave);
void drawInfoPanel(Wave *wave, BITMAP *buffer);

int isWaveFinished(Wave *wave);

#endif // WAVE_H_INCLUDED

#ifndef GRAPHICSAUXILIAR_H_INCLUDED
#define GRAPHICSAUXILIAR_H_INCLUDED

#include <allegro.h>
#include <stdio.h>

// Essa estrutura vai poupar um pouco de trabalho para menus e o jogo
typedef struct Button
{
    BITMAP *clickedImage, *defaultImage, *notEnabledImage;
    int x, y, w, h; // coordinates
    int isClicked, isOccluded, isEnabled;
} Button;

typedef struct Log
{
    FILE *outputFile;
    int *towerMatrix[5];
    int enemiesVector[5];
    int spentMoney, waveNumber;
} Log;

///////////////////////////////////////////////////////////////////////

int isCoordinateInFrame(int xc, int yc, int x, int y, int w, int h); // Retorna TRUE se a coordenada (xc, yc) se a coordenada estiver no frame definido por x, y, w, h
Button *createButton(BITMAP *clickedImage, BITMAP *defaultImage, BITMAP *notEnabledImage, int x, int y); // Construtor
int updateButtonState(Button *button); // Retorna TRUE se tiver sido acionado e FALSE se não
void drawButton(Button *button, BITMAP *buffer);

///////////////////////////////////////////////////////////////////////

void draw_trans_circle(BITMAP *buffer, int x,  int y, int radius, int color, int translevel);

///////////////////////////////////////////////////////////////////////

Log *createLog(const char *fileName, int waveNumber);
void printTowerMatrix(Log *log);
void addKilledEnemy(Log *log, char type);
void writeKilledEnemies(Log *log);
void destroyLog(Log *log);

#endif // GRAPHICSAUXILIAR_H_INCLUDED

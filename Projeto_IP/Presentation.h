#ifndef PRESENTATION_H_INCLUDED
#define PRESENTATION_H_INCLUDED

#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "GraphicsAuxiliar.h"
#include "Global.h"
#include "Field.h"

#define          EXIT    1
#define          PLAY    2


int menu(BITMAP *mouse, BITMAP *buffer);
void showGameOver(BITMAP *buffer, int score);
void showWinnerScreen(BITMAP *buffer);

#endif // PRESENTATION_H_INCLUDED

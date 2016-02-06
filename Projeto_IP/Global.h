#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <allegro.h>

extern volatile unsigned int tick;
extern FONT *digitalFont;

void updateTimer();

#endif // GLOBAL_H_INCLUDED

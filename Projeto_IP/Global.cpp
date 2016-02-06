#include "Global.h"

volatile unsigned int tick;
FONT *digitalFont;

void updateTimer()
{
    tick++;
}

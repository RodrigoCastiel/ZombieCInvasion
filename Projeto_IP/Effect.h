#ifndef EFFECT_H_INCLUDED
#define EFFECT_H_INCLUDED

#include <allegro.h>
#include <stdlib.h>
#include <time.h>

#define EFFECT_EXPLOSION 0

typedef struct Effect
{
    double x, y, dx, dy;
    int currentFrame, w, h;
    int type;
    unsigned last_tick, changeTime;

    struct Effect *next, *previous;
} Effect;

Effect *createEffect(double x, double y, int w, int h, int type, unsigned last_tick);
Effect *insertEffect(Effect *list, double x, double y, int w, int h, int type, int last_tick);
Effect *removeEffect(Effect *list, Effect *effect);
void destroyEffectList(Effect *list);
int moveAndUpdateEffect(Effect *effect, unsigned tick);



#endif // EFFECT_H_INCLUDED

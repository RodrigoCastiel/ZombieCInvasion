#include "Effect.h"


Effect *createEffect(double x, double y, int w, int h, int type, int last_tick)
{
    Effect *effect = (Effect*)malloc(sizeof(Effect));

    if(effect != NULL)
    {
        effect->x = x;
        effect->y = y;
        effect->dx = ((double)(rand() % 150 + 50))/100.0;
        effect->dy = ((double)(rand() % 75 + 35))/100.0;

        effect->w = w;
        effect->h = h;

        effect->type = type;
        effect->last_tick = last_tick;
        effect->changeTime = 1;
        effect->currentFrame = 0;

        effect->next = NULL;
        effect->previous = NULL;
    }

    return effect;
}

Effect *insertEffect(Effect *list, double x, double y, int w, int h, int type, int last_tick)
{
    Effect *effect = createEffect(x, y, w, h, type, last_tick);

    if(effect != NULL)
    {
        if(list != NULL)
        {
            effect->next = list;
            list->previous = effect;
        }
    }

    else
    {
        effect = list;
    }

    return effect;
}

Effect *removeEffect(Effect *list, Effect *effect)
{
    if(effect->next == effect) // Lista com um único elemento
    {
        list = NULL;
        free(effect); // Liberar o efeito
    }

    else // Lista com tamanho maior que 1
    {
        if(list == effect) // Remover o primeiro elemento da lista
        {
            list = effect->next;
        }

        else if(effect->next == NULL)
        {
            effect->previous->next = NULL;
        }

        else
        {
            effect->previous->next = effect->next;
            effect->next->previous = effect->previous;
        }

        free(effect); // Liberar o efeito
    }

    return list;
}

void destroyEffectList(Effect *list)
{
    if(list != NULL)
    {
        destroyEffectList(list->next);
        free(list);
    }
}

int moveAndUpdateEffect(Effect *effect, unsigned tick)
{
    int finished = FALSE;

    effect->x += effect->dx;
    effect->y -= effect->dy;

    if((tick - effect->last_tick) >= effect->changeTime)
    {
        effect->last_tick = tick;
        effect->currentFrame++;
        finished = (effect->currentFrame ==  effect->w * effect->h);
    }

    return finished;
}



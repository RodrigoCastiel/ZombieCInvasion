#include "Wave.h"


////////////////////////////////////////////////////////////////////////

/* Funções para onda de zoombies */

// Construtor
Wave *createWaveNode(Enemy *enemy, unsigned int timeToNext)
{
    Wave *waveNode = (Wave *)malloc(sizeof(Wave));

    if(waveNode != NULL)
    {
        waveNode->enemy = enemy;
        waveNode->timeToNext = timeToNext;
        waveNode->isEnabled = FALSE;

        waveNode->prev = NULL;
        waveNode->next = NULL;
    }

    return waveNode;
}

// Inserção de um inimigo na onda de inimigos
Wave* appendWaveNode(Wave *wave, Enemy *enemy, unsigned int timeToNext)
{
    Wave *node = createWaveNode(enemy, timeToNext), *w = wave;

    if(wave != NULL) // Lista nao vazia
    {
        while(w->next != NULL)
            w = w->next;

        w->next = node;

        if(node != NULL)
        {
            node->prev = w;
        }
    }

    else // Lista vazia
    {
        wave = node;
    }

    return wave;
}

// Destrutor recursivo
void destroyWave(Wave *wave)
{
    if(wave != NULL)
    {
        destroyWave(wave->next);
        free(wave);
    }
}

void printWave(Wave *wave)
{
    if(wave != NULL)
    {
        printf("<%c%d %d> ", wave->enemy->type, wave->enemy->level, wave->timeToNext);
        printWave(wave->next);
    }
}

// Retorna a quantidade de zombies que chegaram no destino
int moveWave(Wave *wave)
{
    Wave *node;
    int zombieArrived = FALSE, zombieQuantity = 0;

    if(wave != NULL)
    {
        node = wave;

        while((node != NULL)) // Percorrer a lista encadeada de inimigos (onda)
        {
            if(node->isEnabled)
            {
                zombieArrived = moveEnemy(node->enemy);
                node->enemy->slowTax = 0;

                if(zombieArrived)
                {
                    node->isEnabled = FALSE;
                    zombieQuantity++;
                }
            }

            node = node->next;
        }
    }

    return zombieQuantity;
}

// Desenhar as informações de vida dos inimigos
void drawInfoPanel(Wave *wave, BITMAP *buffer)
{
    int width, x, y;

    while(wave != NULL)
    {
        if(wave->isEnabled)
        {
            width = ((double)wave->enemy->hp/wave->enemy->maxHP) * TILE_SIZE;
            x = (int) wave->enemy->centre->x - TILE_SIZE/2;
            y = (int) wave->enemy->centre->y - TILE_SIZE/2;

            rectfill(buffer, x, y - 10, x + TILE_SIZE, y - 7, makecol(166, 0, 0));

            if(wave->enemy->type == 'B')
            {
                rectfill(buffer, x, y - 10, x + width, y - 7, makecol(24, 10, 140));
            }

            else
            {
                rectfill(buffer, x, y - 10, x + width, y - 7, makecol(0, 157, 0));
            }

            rect(buffer, x, y - 10, x + TILE_SIZE, y - 7, makecol(0, 0, 0));
        }

        wave = wave->next;
    }
}

int isWaveFinished(Wave *wave)
{
    int finished = TRUE;

    while((wave != NULL) && finished)
    {
        finished = !(wave->isEnabled);
        wave = wave->next;
    }

    return finished;
}























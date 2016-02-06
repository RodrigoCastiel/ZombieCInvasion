#include "Enemy.h"

// Construtor
Enemy *createEnemy(Coord *path, BITMAP *spriteSheet, char type, int level)
{
    Enemy *enemy = (Enemy *)calloc(1, sizeof(Enemy));

    if(enemy != NULL)
    {
        enemy->sheet  = spriteSheet;
        enemy->centre = createCoordinate(path->x, path->y); // Salvando a coordenata atual do centro
        enemy->dest   = path->next; // Salvar a proxima coordenada
        enemy->elapsedDistance = 0.0;
        enemy->blenderTax = 255;

        if(enemy->centre != NULL)
        {
            level--;

            enemy->level = level;
            enemy->type  = type;
            enemy->slowTax   = 0;
            enemy->poisonTax = 0;
            enemy->orientation = UP;

            if(type == 'N') // Normal
            {
                enemy->hp  = 100 + 50 * level;
                enemy->def = 4 * level;
                enemy->speed = 1 + 0.2 * (double)level;
                enemy->money = 35 + 10 * level;

                if(enemy->speed > 2.3) // O máximo de velocidade é 2.0
                    enemy->speed = 2.3;
            }

            else if(type == 'R') // Resistente a splash
            {
                enemy->hp  = 120 + 60 * level;
                enemy->def = 4 + 5 * level;
                enemy->speed = 1 + 0.2 * (double)level;
                enemy->money = 40 + 15 * level;

                if(enemy->speed > 2) // O máximo de velocidade é 2.0
                    enemy->speed = 2.0;
            }

            else if(type == 'S') // Forte
            {
                enemy->hp  = 150 + 100 * level;
                enemy->def = 5 + 6 * level;
                enemy->speed = 0.8 + 0.15 * (double)level;
                enemy->money = 40 + 30 * level;

                if(enemy->speed > 1.5) // O máximo de velocidade é 1.5
                    enemy->speed = 1.5;
            }

            else if(type == 'F') // Rápido
            {
                enemy->hp  = 80 + 40 * level;
                enemy->def = 2 + 3 * level;
                enemy->speed = 1.5 + 0.3 * (double)level;
                enemy->money = 30 + 13 * level;

                if(enemy->speed > 5.0) // O máximo de velocidade é 3.0
                    enemy->speed = 5.0;
            }

            else if(type == 'I') // Imune
            {
                enemy->hp  = 90 + 45 * level;
                enemy->def = 2 + 4 * level;
                enemy->speed = 1 + 0.2 * (double)level;
                enemy->money = 30 + 16 * level;

                if(enemy->speed > 2.5) // O máximo de velocidade é 2.0
                    enemy->speed = 2.5;
            }

            else if(type == 'B') // Boss
            {
                enemy->hp  = 150 + 100 * level;
                enemy->def = 10 + 4 * level;
                enemy->speed = 1 + 0.2 * (double)level;
                enemy->money = 400 + 100 * level;

                if(enemy->speed > 2.0) // O máximo de velocidade é 2.0
                    enemy->speed = 2.0;
            }

            enemy->maxHP = enemy->hp;
        }
    }

    return enemy;
}

void drawEnemy(Enemy *enemy, BITMAP *buffer, BITMAP *auxiliar, unsigned int tick)
{
    if(enemy != NULL)
    {
        if(enemy->hp > 0)
        {
            masked_blit(enemy->sheet, buffer, ((int)(enemy->speed * tick)/(10) % 2) * TILE_SIZE, (enemy->orientation * TILE_SIZE),
                    (enemy->centre->x - TILE_SIZE/2), (enemy->centre->y - TILE_SIZE/1.5),
                    TILE_SIZE, TILE_SIZE);
        }

        else
        {
            set_trans_blender(255, 255, 255, enemy->blenderTax);
            blit(enemy->sheet, auxiliar, ((int)(enemy->speed * tick)/(10) % 2) * TILE_SIZE, (4 * TILE_SIZE),
                        0, 0, TILE_SIZE, TILE_SIZE);
            draw_trans_sprite(buffer, auxiliar, (enemy->centre->x - TILE_SIZE/2), (enemy->centre->y - TILE_SIZE/1.5));

            if(enemy->blenderTax > 0)
            {
                enemy->blenderTax--;
            }

        }
    }
}

// Move o inimigo pela trajetória salva por ele : retorna 1 se ele chegou no final do caminho e 0 caso contrário
int moveEnemy(Enemy *enemy)
{
    double ds; // Deslocamento no movimento
    double ex; // Excedente da trilha
    int arrived = 0;

    if((enemy != NULL) && (enemy->dest != NULL))
    {
        ds = ((((enemy->speed * (100 - enemy->slowTax)/100)* (double)TILE_SIZE) * 3)/100.0);
        enemy->elapsedDistance += ds;

        if(enemy->dest->x > enemy->centre->x) // Mover para a direita
        {
            enemy->centre->x += ds;
            ex = enemy->centre->x - enemy->dest->x; // Calculando o excendente
            enemy->orientation = RIGHT;

            if((ex >= 0) && (enemy->dest->next != NULL)) // Ultrapassou, ao andar ds
            {
                if((enemy->dest->next->y == enemy->centre->y)) // ainda para a direita
                {
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = RIGHT;
                }

                else if(enemy->dest->next->y < enemy->centre->y) // para cima
                {
                    enemy->centre->y -= ex; // Compensar o excedente
                    enemy->centre->x = enemy->dest->x;
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = UP;
                }

                else if(enemy->dest->next->y > enemy->centre->y) // para baixo
                {
                    enemy->centre->y += ex; // Compensar o excedente
                    enemy->centre->x = enemy->dest->x;
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = DOWN;
                }
            }

            else if((ex >= 0) && (enemy->dest->next == NULL)) // Chegou no destino
            {
                arrived = 1;
            }
        }

        else if(enemy->dest->x < enemy->centre->x) // Mover para a esquerda
        {
            enemy->centre->x -= ds;
            ex = enemy->dest->x - enemy->centre->x; // Calculando o excendente
            enemy->orientation = LEFT;

            if((ex >= 0) && (enemy->dest->next != NULL)) // Ultrapassou, ao andar ds
            {
                if((enemy->dest->next->y == enemy->centre->y)) // ainda para a esquerda
                {
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = LEFT;
                }

                else if(enemy->dest->next->y < enemy->centre->y) // para cima
                {
                    enemy->centre->y -= ex; // Compensar o excedente
                    enemy->centre->x = enemy->dest->x;
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = UP;
                }

                else if(enemy->dest->next->y > enemy->centre->y) // para baixo
                {
                    enemy->centre->y += ex; // Compensar o excedente
                    enemy->centre->x = enemy->dest->x;
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = DOWN;
                }
            }

            else if((ex >= 0) && (enemy->dest->next == NULL)) // Chegou no destino
            {
                arrived = 1;
            }
        }

        else if(enemy->dest->y > enemy->centre->y) // Mover para baixo
        {
            enemy->centre->y += ds;
            ex = enemy->centre->y - enemy->dest->y;
            enemy->orientation = DOWN;

            if((ex >= 0) && (enemy->dest->next != NULL))
            {
                if(enemy->centre->x == enemy->dest->next->x) // continuar descendo
                {
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = DOWN;
                }

                else if(enemy->centre->x > enemy->dest->next->x) // Ir para esquerda, depois
                {
                    enemy->centre->y = enemy->dest->y;
                    enemy->centre->x -= ex; // Compensar o excedente
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = LEFT;
                }

                else if(enemy->centre->x < enemy->dest->next->x) // Ir para a direita, depois
                {
                    enemy->centre->y = enemy->dest->y;;
                    enemy->centre->x += ex; // Compensar o excedente
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = RIGHT;
                }
            }

            else if((ex >= 0) && (enemy->dest->next == NULL)) // Chegou no destino
            {
                arrived = 1;
            }
        }

        else if(enemy->dest->y < enemy->centre->y) // Mover para cima
        {
            enemy->centre->y -= ds;
            ex = enemy->dest->y - enemy->centre->y;
            enemy->orientation = UP;

            if((ex >= 0) && (enemy->dest->next != NULL))
            {
                if(enemy->centre->x == enemy->dest->next->x) // continuar subindo
                {
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = UP;
                }

                else if(enemy->centre->x > enemy->dest->next->x) // Ir para esquerda, depois
                {
                    enemy->centre->y = enemy->dest->y;
                    enemy->centre->x -= ex; // Compensar o excedente
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = LEFT;
                }

                else if(enemy->centre->x < enemy->dest->next->x) // Ir para a direita, depois
                {
                    enemy->centre->y = enemy->dest->y;
                    enemy->centre->x += ex; // Compensar o excedente
                    enemy->dest = enemy->dest->next;
                    enemy->orientation = RIGHT;
                }
            }

            else if((ex >= 0) && (enemy->dest->next == NULL)) // Chegou no destino
            {
                arrived = 1;
            }
        }
    }

    return arrived;
}




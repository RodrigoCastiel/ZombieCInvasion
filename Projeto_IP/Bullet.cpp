#include "Bullet.h"

// Construtor
Bullet *createBullet(BulletType type, double x, double y, double speed, double explosionRange, double explosionDamage, double damage, Wave *target)
{
    Bullet *bullet = (Bullet*)malloc(sizeof(Bullet));

    if(bullet != NULL)
    {
        bullet->x = x;
        bullet->y = y;
        bullet->type = type;
        bullet->speed = speed;
        bullet->target = target;
        bullet->damage = damage;
        bullet->explosionRange  = explosionRange;
        bullet->explosionDamage = explosionDamage;

        bullet->next     = NULL;
        bullet->previous = NULL;
    }

    return bullet;
}

// Move a bala - retorna 1 se conseguiu chegar na posião do inimigo, mas o inimigo morreu. Retorna 2 se chegou e o inimigo não morreu
int moveBullet(Bullet *bullet)
{
    int result;

    // Achar o novo ângulo
    if(bullet->target->isEnabled) // Inimigo ainda vivo
    {
        if(distanceBetween(bullet->x, bullet->y, bullet->target->enemy->centre->x, bullet->target->enemy->centre->y) >= TILE_SIZE/3) // Ainda não chegou
        {
            bullet->angle = atan2(bullet->target->enemy->centre->y - bullet->y, bullet->target->enemy->centre->x - bullet->x); // Achar o ângulo do novo vetor velocidade

            bullet->x += bullet->speed * cos(bullet->angle);
            bullet->y += bullet->speed * sin(bullet->angle);

            bullet->angle = (180.0 * bullet->angle)/3.141592 - 90.0;

            bullet->angle += 180;
            bullet->angle = (int)((bullet->angle * 255.0)/360.0);

            result = 0;
        }

        else // Chegou
        {
            result = 2;
        }

    }

    else // Inimigo morreu
    {
        if(distanceBetween(bullet->x, bullet->y, bullet->target->enemy->centre->x, bullet->target->enemy->centre->y) >= TILE_SIZE/3) // Ainda não chegou
        {
            bullet->angle = atan2(bullet->target->enemy->centre->y - bullet->y, bullet->target->enemy->centre->x - bullet->x); // Achar o ângulo do novo vetor velocidade

            bullet->x += bullet->speed * cos(bullet->angle);
            bullet->y += bullet->speed * sin(bullet->angle);

            bullet->angle = (180.0 * bullet->angle)/3.141592 - 90.0;

            bullet->angle += 180;
            bullet->angle = (int)((bullet->angle * 255.0)/360.0);

            result = 0;
        }

        else // Chegou
        {
            result = 1;
        }
    }

    return result;
}


// Adiciona no início da lista encadeada de balas uma nova bala
Bullet *insertBullet(Bullet *list, BulletType type, double x, double y, double speed, double explosionRange, double explosionDamage, double damage, Wave *target)
{
    Bullet *bullet = createBullet(type, x, y, speed, explosionRange, explosionDamage, damage, target);

    if(bullet != NULL) // Se não houver erro de alocação
    {
        if(list == NULL) // Lista vazia
        {
            list = bullet;
        }

        else // Lista não vazia - encaixar o novo nó
        {
            bullet->next = list;
            list->previous = bullet;
            list = bullet;
        }
    }

    return list;
}

Bullet *removeBullet(Bullet *list, Bullet *bullet)
{
    if(bullet->next == bullet) // Lista com um único elemento
    {
        list = NULL;
        free(bullet); // Liberar a bala
    }

    else // Lista com tamanho maior que 1
    {
        if(list == bullet) // Remover o primeiro elemento da lista
        {
            list = bullet->next;
        }

        else if(bullet->next == NULL)
        {
            bullet->previous->next = NULL;
        }

        else
        {
            bullet->previous->next = bullet->next;
            bullet->next->previous = bullet->previous;
        }

        free(bullet); // Liberar a bala
    }

    return list;
}

void destroyBulletList(Bullet *list)
{
    if(list != NULL)
    {
        destroyBulletList(list->next);
        free(list);
    }
}











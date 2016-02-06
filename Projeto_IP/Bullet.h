#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <allegro.h>
#include <math.h>
#include <stdio.h>

#include "GraphicsAuxiliar.h"
#include "Wave.h"

typedef enum BulletType
{
    BULLET_NORMAL, BULLET_SPLASH
} BulletType;

typedef struct Bullet // Nó para uma lista duplamente encadeada de bullets
{
    double x, y, speed, angle, explosionRange, explosionDamage, damage;
    BulletType type;
    Wave *target;

    struct Bullet *next;
    struct Bullet *previous;

} Bullet;

Bullet *createBullet(BulletType type, double x, double y, double speed, double explosionRange, double explosionDamage, double damage, Wave *target);
Bullet *insertBullet(Bullet *list, BulletType type, double x, double y, double speed, double explosionRange, double explosionDamage, double damage, Wave *target); // Adiciona no final da lista encadeada de balas uma nova bala
Bullet *removeBullet(Bullet *list, Bullet *bullet); // Remove  a bala
void destroyBulletList(Bullet *list);

int moveBullet(Bullet *bullet);

#endif // BULLET_H_INCLUDED

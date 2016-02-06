#ifndef TOWER_H_INCLUDED
#define TOWER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <allegro.h>
#include "Global.h"
#include "Wave.h"
#include "GraphicsAuxiliar.h"
#include "Bullet.h"

#define     TILE_SIZE       50
#define    FIELD_WIDTH      16
#define    FIELD_HEIGHT     12

#define     PRIORITY_NEAR   0
#define     PRIORITY_MAXHP  1
#define     PRIORITY_MINHP  2
#define     PRIORITY_DIST   3

typedef enum TowerType
{
    NORMAL, SPLASH, SLOW, POISON, BEACON
} TowerType;

typedef struct Tower
{
    TowerType type;
    struct Tower *beacon;
    BITMAP *img;
    Wave *target;
    Wave *slowTargets[5];

    double range, x, y, damage, explosionRange, explosionDamage;
    double sellCost, upgradeCost;
    int rank, period;
    int slowTax, priority;
    int isSelected;
    unsigned int last_tick;

} Tower;

typedef struct TowerButton
{
    BITMAP *clickedImage, *defaultImage, *notEnabledImage;
    int x, y, w, h; // coordinates
    int isClicked, isOccluded, isEnabled, isTriggered, cost;
    double range;
    TowerType type;
} TowerButton;


TowerButton *createTowerButton(BITMAP *clickedImage, BITMAP *defaultImage, BITMAP *notEnabled, TowerType type); // Construtor
void updateTowerButtonState(TowerButton *button); // Retorna TRUE se tiver sido acionado e FALSE se não
void drawTowerButton(TowerButton *button, BITMAP *buffer);

// Assinaturas:
Tower *createTower(TowerType type, double x, double y, int priority);
void upgradeTower(Tower *tower);


#endif // TOWER_H_INCLUDED

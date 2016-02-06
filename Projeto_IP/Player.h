#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <allegro.h>
#include <stdio.h>

#include "GraphicsAuxiliar.h"
#include "Tower.h"
#include "Bullet.h"
#include "Effect.h"
#include "Enemy.h"
#include "Global.h"

typedef struct Player
{
    Tower **towers;     // Vetor de torres
    int towerQuantity;  // Comprimento do vetor
    Bullet *bulletList; // Listas encadeadas de balas
    Effect *effectList;
    int priorities[3];

    BITMAP *towerSheet, *smokeEffectImage;
    BITMAP *normalBulletImg, *splashBulletImg;

    int life;
    int money;
    int score;

} Player;

Player* createPlayer(FILE *inputFile, BITMAP *towerSheet, BITMAP *normalBulletImg, BITMAP *splashBulletImg, BITMAP *smokeEffectImage);
void destroyPlayer(Player *player);

void drawPlayerInformation(Player *player, BITMAP *buffer);
void drawTowers(Player *player, BITMAP *buffer);
void drawAndMoveBullets(Player *player, Wave *wave, Log *log, BITMAP *buffer, unsigned tick);
void drawAndUpdateEffects(Player *player, BITMAP *buffer, BITMAP *effectSprite, unsigned tick);

void  addTower(Player *player, TowerType type, int line, int column, int last_tick); // Adiciona uma torre para o jogador
void sellTower(Player *player, Tower *tower, int tile[][FIELD_WIDTH]);
void selectTowerByMouse(Player *player, Tower **selectedTower);
void searchBestBeaconInRange(Player *player, Tower *tower);
void redirectTower(Tower *tower, Wave *wave, Player *player);
void attackEnemies(Player *player, Tower *tower, Wave *wave, unsigned int last_tick);

Wave *searchNearestEnemy   (Tower *tower, Wave *wave);
Wave *searchFirstEnemy     (Tower *tower, Wave *wave);
Wave *searchBiggestHPEnemy (Tower *tower, Wave *wave);
Wave *searchSmallestHPEnemy(Tower *tower, Wave *wave);

#endif // PLAYER_H_INCLUDED

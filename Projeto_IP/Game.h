#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "GraphicsAuxiliar.h"
#include "Presentation.h"
#include "Player.h"
#include "Global.h"
#include "Field.h"

#define          EXIT    1
#define          PLAY    2
#define         ERROR    3
#define    FILE_ERROR    4

#define       WIN    0
#define GAME_OVER    1
#define      PLAY    2

/* Funções para a estrutura jogo */
typedef struct Game
{
    // Mecânica
    Field  *field;
    Player *player;
    Log *outputLog;

    // Ponteiros para estruturas da interface gráfica
    Tower  *selectedTower;
    Button *sellButton, *upgradeButton, *nextWaveButton;
    TowerButton *towerButtons[5],  *selectedTowerButton;

    // Imagens
    BITMAP *scenerySpriteSheet, *fog;
    BITMAP *lateralBar;
    BITMAP *normalBulletImg, *splashBulletImg;
    BITMAP *normalSheet, *resistentSheet, *fastSheet, *strongSheet, *immuneSheet, *bossSheet;
    BITMAP *towerSheet,  *towerSheetOff,  *towerSheetOn;
    BITMAP *sellButtonClickedImage,     *sellButtonDefaultImage;
    BITMAP *upgradeButtonClickedImage,  *upgradeButtonDefaultImage;
    BITMAP *nextWaveButtonClickedImage, *nextWaveButtonDefaultImage;
    BITMAP *auxiliarBitmap, *effectSprite, *smokeEffectImage;

} Game;

// Assinaturas:
Game *createGame(FILE *inputFile);
void destroyGame(Game *game);
int  executeGame(const char *fileDirectory, BITMAP *buffer, BITMAP *defaultCursor);

void updateButtonsState(Game *game);
void drawAndUpdateGUI(Game *game, BITMAP *buffer, TowerButton **selectedTowerButton);
void drawGameFrame(Game *game, BITMAP *buffer, BITMAP *defaultCursor);
void drawAndUpdateTowers(Game *game);

#endif // GAME_H_INCLUDED

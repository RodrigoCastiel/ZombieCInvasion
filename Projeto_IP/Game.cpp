#include "Game.h"

// Destrutor
void destroyGame(Game *game)
{
    int i;

    if(game != NULL)
    {
        // Deletar as imagens
        destroyBitmap(&game->scenerySpriteSheet);
        destroyBitmap(&game->lateralBar);
        destroyBitmap(&game->fog);

        destroyBitmap(&game->sellButtonClickedImage);
        destroyBitmap(&game->sellButtonDefaultImage);

        destroyBitmap(&game->upgradeButtonClickedImage);
        destroyBitmap(&game->upgradeButtonDefaultImage);

        destroyBitmap(&game->nextWaveButtonClickedImage);
        destroyBitmap(&game->nextWaveButtonDefaultImage);

        destroyBitmap(&game->normalBulletImg);
        destroyBitmap(&game->splashBulletImg);
        destroyBitmap(&game->effectSprite);

        destroyBitmap(&game->towerSheet);
        destroyBitmap(&game->towerSheetOn);
        destroyBitmap(&game->towerSheetOff);

        destroyBitmap(&game->normalSheet);
        destroyBitmap(&game->fastSheet);
        destroyBitmap(&game->resistentSheet);
        destroyBitmap(&game->immuneSheet);
        destroyBitmap(&game->bossSheet);
        destroyBitmap(&game->strongSheet);
        destroyBitmap(&game->auxiliarBitmap);

        // Tudo relativo ao cenário e jogador
        destroyField (game->field);
        destroyPlayer(game->player);
        destroyLog(game->outputLog);

        for(i = 0; i < 5; i++) // Todos os botões
        {
            if(game->towerButtons[i] != NULL)
            {
                free(game->towerButtons[i]);
            }
        }
    }
}

// Construtor
Game *createGame(FILE *inputFile)
{
    Game *game = (Game *)malloc(sizeof(Game));
    int error = 0, i;

    if(game !=  NULL)
    {
        // Carregar sprites
        game->scenerySpriteSheet = load_bitmap("Sprites/Scenery/ScenerySheet.bmp", NULL);
        game->lateralBar         = load_bitmap("Sprites/ToolBar/LateralBar.bmp"  , NULL);
        game->fog                = load_bitmap("Sprites/Scenery/Fog.bmp"         , NULL);
        game->effectSprite       = load_bitmap("Sprites/Effect/Explosion.bmp"    , NULL);
        game->smokeEffectImage   = load_bitmap("Sprites/Effect/SmokeEffect.bmp"  , NULL);

        game->towerSheet         = load_bitmap("Sprites/Scenery/Towers.bmp"      , NULL);
        game->towerSheetOn       = load_bitmap("Sprites/Scenery/TowersOn.bmp"    , NULL);
        game->towerSheetOff      = load_bitmap("Sprites/Scenery/TowersOff.bmp"   , NULL);

        game->normalBulletImg = load_bitmap("Sprites/Scenery/NormalBullet.bmp", NULL);
        game->splashBulletImg = load_bitmap("Sprites/Scenery/SplashBullet.bmp", NULL);

        game->sellButtonClickedImage = load_bitmap("Sprites/Toolbar/SellButtonClickedImage.bmp", NULL);
        game->sellButtonDefaultImage = load_bitmap("Sprites/Toolbar/SellButtonDefaultImage.bmp", NULL);

        game->upgradeButtonClickedImage = load_bitmap("Sprites/Toolbar/UpgradeButtonClickedImage.bmp", NULL);
        game->upgradeButtonDefaultImage = load_bitmap("Sprites/Toolbar/UpgradeButtonDefaultImage.bmp", NULL);

        game->nextWaveButtonClickedImage = load_bitmap("Sprites/Toolbar/NextWaveButtonClickedImage.bmp", NULL);
        game->nextWaveButtonDefaultImage = load_bitmap("Sprites/Toolbar/NextWaveButtonDefaultImage.bmp", NULL);

        game->normalSheet        = load_bitmap("Sprites/Enemy/Normal.bmp"   , NULL);
        game->fastSheet          = load_bitmap("Sprites/Enemy/Fast.bmp"     , NULL);
        game->resistentSheet     = load_bitmap("Sprites/Enemy/Resistent.bmp", NULL);
        game->immuneSheet        = load_bitmap("Sprites/Enemy/Immune.bmp"   , NULL);
        game->strongSheet        = load_bitmap("Sprites/Enemy/Strong.bmp"   , NULL);
        game->bossSheet          = load_bitmap("Sprites/Enemy/Boss.bmp"   , NULL);


        if(((game->scenerySpriteSheet != NULL) // Se não houver erro de alocação ou de carregamento de imagem
        && (game->lateralBar   != NULL)
        && (game->fog          != NULL)

        && (game->sellButtonClickedImage    != NULL)
        && (game->sellButtonDefaultImage    != NULL)
        && (game->upgradeButtonClickedImage != NULL)
        && (game->upgradeButtonDefaultImage != NULL)
        && (game->nextWaveButtonClickedImage != NULL)
        && (game->nextWaveButtonDefaultImage != NULL)
        && (game->normalBulletImg != NULL)
        && (game->splashBulletImg != NULL)
        && (game->effectSprite     != NULL)
        && (game->smokeEffectImage != NULL)

        && (game->towerSheet   != NULL)
        && (game->towerSheetOn != NULL)
        && (game->towerSheetOff  != NULL)
        && (game->normalSheet    != NULL)
        && (game->fastSheet        != NULL)
        && (game->resistentSheet   != NULL)
        && (game->immuneSheet   != NULL)
        && (game->bossSheet   != NULL)
        && (game->strongSheet   != NULL)))
        {
            // Carregar o campo e o jogador a partir do arquivo de entrada o arquivo de entrada
            game->field = createField(inputFile, game->scenerySpriteSheet, game->fog); // Carregar o campo a partir do arquivo
            game->player = createPlayer(inputFile, game->towerSheet, game->normalBulletImg, game->splashBulletImg, game->smokeEffectImage); // Carregar os dados do jogador

            // Alocar os botões
            game->sellButton     = createButton(game->sellButtonClickedImage,     game->sellButtonDefaultImage,      NULL, FIELD_WIDTH * TILE_SIZE +  10, 530);
            game->upgradeButton  = createButton(game->upgradeButtonClickedImage,  game->upgradeButtonDefaultImage,   NULL, FIELD_WIDTH * TILE_SIZE + 150, 530);
            game->nextWaveButton = createButton(game->nextWaveButtonClickedImage, game->nextWaveButtonDefaultImage, NULL, FIELD_WIDTH * TILE_SIZE  + 30 , 335);

            game->selectedTower       = NULL;
            game->selectedTowerButton = NULL;
            game->auxiliarBitmap = create_bitmap(TILE_SIZE, TILE_SIZE);

            if(game->player != NULL
            && game->field  != NULL
            && game->sellButton     != NULL
            && game->upgradeButton  != NULL
            && game->nextWaveButton != NULL
            && createEnemies(game->field, inputFile, game->normalSheet, game->resistentSheet, game->fastSheet, game->strongSheet, game->immuneSheet, game->bossSheet)
            && game->auxiliarBitmap != NULL)
            {
                game->sellButton->isEnabled     = FALSE;
                game->upgradeButton->isEnabled  = FALSE;
                game->nextWaveButton->isEnabled = TRUE;
                error = ((game->outputLog = createLog("LOG.out", game->field->waveNumber)) == NULL);

                for(i = 0; i < 5; i++) // Carregar os botões das torres
                {
                    game->towerButtons[i] = createTowerButton(game->towerSheetOn, game->towerSheet, game->towerSheetOff, (TowerType)i);

                    if(game->towerButtons[i] == NULL)
                    {
                        error = 1;
                        printf("Seu computador esta com a memoria cheia. Finalizando o programa. \n");
                    }
                }
            }

            else
            {
                error = 1;
                printf("Seu computador esta com a memoria cheia. Finalizando o programa. \n");
            }
        }

        else
        {
            error = 1;
            printf("Erro de localizacao de imagem. Por favor baixe o .rar do programa novamente e verifique se a pasta Sprites esta completa. \n");
        }

        if(error)
        {
            destroyGame(game);
            game = NULL;
        }

        fclose(inputFile);
    }

    return game;
}

void drawAndUpdateGUI(Game *game, BITMAP *buffer)
{
    int i, mouse_i = mouse_y/TILE_SIZE, mouse_j = mouse_x/TILE_SIZE;
    static const char *towerNames[] = {"NORMAL", "SPLASH", "SLOW", "POISON", "BEACON"};
    static const char *priority[]   = {"MAIS PERTO", "MAX HP", "MIN HP", "PRIMEIRO"};

    // Atualizar e desenhar onde está a torre escolhida para compra
    if((mouse_x < FIELD_WIDTH * TILE_SIZE)) // Se o cursor estiver no campo
    {
        if(game->selectedTowerButton != NULL && mouse_b & 2) // e está com alguma torre (para compra) selecionada, mas a pertando o botão direito do mouse
        {
            game->selectedTowerButton->isTriggered = FALSE;
            game->selectedTowerButton = NULL;
        }

        if(game->selectedTowerButton != NULL) // e está com alguma torre (para compra) selecionada
        {
            masked_blit(game->selectedTowerButton->defaultImage, buffer, game->selectedTowerButton->type * TILE_SIZE, 0, mouse_j * (TILE_SIZE), mouse_i * (TILE_SIZE), TILE_SIZE, TILE_SIZE);

            if((game->field->tile[mouse_i][mouse_j] <= 0) && (game->field->tile[mouse_i][mouse_j] >= -2)) // Se é possível adicionar a torre no tile onde o mouse está
            {
                rect(buffer, mouse_j * TILE_SIZE, mouse_i * TILE_SIZE, (mouse_x/TILE_SIZE) * TILE_SIZE + TILE_SIZE, mouse_i * TILE_SIZE + TILE_SIZE, makecol(0, 255, 0));
                draw_trans_circle(buffer, mouse_j * TILE_SIZE + TILE_SIZE/2, mouse_i * TILE_SIZE + TILE_SIZE/2, game->selectedTowerButton->range, makecol(0, 255, 0), 50);

                if(mouse_b & 1)
                {
                    game->field->tile[mouse_i][mouse_j] = -4;
                    addTower(game->player, game->selectedTowerButton->type, mouse_i, mouse_j, tick);

                    game->outputLog->spentMoney += game->player->towers[game->player->towerQuantity - 1]->sellCost * 2;

                    // Adicionar a nova torre à matriz de torres do log
                    if(game->field->currentWave == -1)
                    {
                        game->outputLog->towerMatrix[(int)game->selectedTowerButton->type][0]++;
                    }

                    else
                    {
                        game->outputLog->towerMatrix[(int)game->selectedTowerButton->type][game->field->currentWave]++;
                    }

                    game->selectedTowerButton->isTriggered = FALSE;
                    game->selectedTowerButton = NULL;
                }
            }
            else
            {
                rect(buffer, mouse_j * TILE_SIZE, mouse_i * TILE_SIZE, (mouse_x/TILE_SIZE) * TILE_SIZE + TILE_SIZE, mouse_i * TILE_SIZE + TILE_SIZE, makecol(255, 0, 0));
                draw_trans_circle(buffer, mouse_j * TILE_SIZE + TILE_SIZE/2, mouse_i * TILE_SIZE + TILE_SIZE/2, game->selectedTowerButton->range, makecol(255, 0, 0), 50);
            }
        }
    }

    if(game->selectedTower != NULL)
    {
        masked_blit(game->player->towerSheet, buffer, game->selectedTower->type * TILE_SIZE, 0, game->selectedTower->x - TILE_SIZE/2, game->selectedTower->y - TILE_SIZE/2, TILE_SIZE, TILE_SIZE);
        draw_trans_circle(buffer, game->selectedTower->x, game->selectedTower->y, game->selectedTower->range, makecol(255, 10, 20), 50);
        rect(buffer, game->selectedTower->x - TILE_SIZE/2, game->selectedTower->y - TILE_SIZE/2,
             game->selectedTower->x + TILE_SIZE/2, game->selectedTower->y + TILE_SIZE/2
             , makecol(0, 0, 255));

        if(game->selectedTower->type < 3)
        {
            // Mudar prioridade das torres do tipo da torre selecionada
            if(key[KEY_1])
            {
                game->player->priorities[(int)game->selectedTower->type] = PRIORITY_NEAR;
            }

            else if(key[KEY_2])
            {
                game->player->priorities[(int)game->selectedTower->type] = PRIORITY_DIST;
            }

            else if(key[KEY_3])
            {
                game->player->priorities[(int)game->selectedTower->type] = PRIORITY_MINHP;
            }

            else if(key[KEY_4])
            {
                game->player->priorities[(int)game->selectedTower->type] = PRIORITY_MAXHP;
            }
        }
    }

    blit(game->lateralBar, buffer, 0, 0, TILE_SIZE * FIELD_WIDTH, 0, BAR_WIDTH, BAR_HEIGHT);

    for(i = 0; (i < 5); i++) // Atualizar e desenhar os estados dos botões de compra de torres
    {
        if(game->player->money < game->towerButtons[i]->cost) // Checar se o jogador tem dinheiro para a torre atual
        {
            game->towerButtons[i]->isEnabled = FALSE; // se não tiver, desativar o botão
        }

        else
        {
            game->towerButtons[i]->isEnabled = TRUE;
        }

        updateTowerButtonState(game->towerButtons[i]);

        if(game->towerButtons[i]->isTriggered) // Se o botão estiver acionado
        {
            if((game->selectedTowerButton != NULL) && (game->selectedTowerButton != game->towerButtons[i]))
            {
                game->selectedTowerButton->isTriggered = FALSE;
            }

            game->selectedTowerButton = game->towerButtons[i];
        }

        if(game->towerButtons[i]->isTriggered)
        {
            textprintf_centre_ex(buffer, digitalFont, TILE_SIZE * FIELD_WIDTH + game->lateralBar->w/2, 164, makecol(200, 200, 200), -1, "Compra: %s", towerNames[(int)game->selectedTowerButton->type]);
        }

        if((game->towerButtons[i]->isClicked) && (game->selectedTower != NULL)) // De-selecionar a torre
        {
            game->selectedTower->isSelected = FALSE;
            game->selectedTower = NULL;
        }

        drawTowerButton(game->towerButtons[i], buffer);
    }

    if(game->selectedTower != NULL)
    {
        if(game->selectedTower->beacon != NULL)
        {
            draw_trans_circle(buffer, game->selectedTower->beacon->x, game->selectedTower->beacon->y, game->selectedTower->beacon->range, makecol(18, 200, 130), 25);
            textprintf_centre_ex(buffer, digitalFont, game->selectedTower->beacon->x, game->selectedTower->beacon->y - TILE_SIZE, makecol(144, 144, 0), -1, "Beacon Rank: %d", game->selectedTower->beacon->rank);
        }

        textprintf_centre_ex(buffer, digitalFont, TILE_SIZE * FIELD_WIDTH + game->lateralBar->w/2, 164, makecol(200, 200, 200), -1, "Torre: %s.", towerNames[(int)game->selectedTower->type]);
        textprintf_ex(buffer, digitalFont, TILE_SIZE * FIELD_WIDTH + 28, 200, makecol(255, 255, 0), -1, "Rank:   %2d" , game->selectedTower->rank + 1);
        textprintf_ex(buffer, digitalFont, TILE_SIZE * FIELD_WIDTH + 28, 225, makecol(255, 255, 0), -1, "Alcance:   %.2f", game->selectedTower->range / TILE_SIZE);
        textprintf_ex(buffer, digitalFont, TILE_SIZE * FIELD_WIDTH + 28, 250, makecol(255, 255, 0), -1, "Venda:     %.2f", game->selectedTower->sellCost);

        if(((game->selectedTower->type != BEACON) && (game->selectedTower->rank < 4))
        || ((game->selectedTower->type == BEACON)  && (game->selectedTower->rank < 2)))
        {
            textprintf_ex(buffer, digitalFont, TILE_SIZE * FIELD_WIDTH + 28, 275, makecol(255, 255, 0), -1, "Upgrade:   %.2f", game->selectedTower->upgradeCost);
        }

        if((game->selectedTower->type == NORMAL) || (game->selectedTower->type == SPLASH) || (game->selectedTower->type == SLOW))
        {
            textprintf_ex(buffer, digitalFont, TILE_SIZE * FIELD_WIDTH + 28, 300, makecol(255, 255, 0), -1, "Alvo: %s", priority[(int)game->selectedTower->priority]);
        }
    }

    if(game->field->currentWave >= 0)
    {
        drawInfoPanel(game->field->waves[game->field->currentWave], buffer);

    }

    drawButton(game->sellButton   , buffer);
    drawButton(game->upgradeButton, buffer);
    drawButton(game->nextWaveButton, buffer);
}

void drawGameFrame(Game *game, BITMAP *buffer, BITMAP *defaultCursor)
{
    drawFieldTerrain(game->field, buffer);

    if(game->field->currentWave >= 0)
    {
        drawActiveEnemies(game->field->waves[game->field->currentWave], buffer, game->auxiliarBitmap, tick);
    }

    drawFieldObjects(game->field, buffer, tick);
    drawTowers(game->player, buffer);
    drawAndMoveBullets(game->player, game->field->waves[game->field->currentWave], game->outputLog, buffer, tick);
    drawAndUpdateEffects(game->player, buffer, game->effectSprite, tick);

    // Desenhar neblina por cima dos objetos e do cenário
    set_trans_blender(255, 255, 255, 115);
    draw_trans_sprite(buffer, game->field->fog, ((tick * 2) % 800), 0);
    draw_trans_sprite(buffer, game->field->fog, ((tick * 2) % 800) - 800, 0);

    // Tudo a partir daqui ficará destacado, pois estará em cima da neblina
    drawAndUpdateGUI(game, buffer);
    drawPlayerInformation(game->player, buffer);

    draw_sprite(buffer, defaultCursor, mouse_x, mouse_y);
    textprintf_ex(buffer, digitalFont, 10, FIELD_HEIGHT*TILE_SIZE - TILE_SIZE/2, makecol(255, 255, 0), -1, "Wave: %d/%d.", game->field->currentWave + 1, game->field->waveNumber);
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}

void updateButtonsState(Game *game)
{
    int i;

    if(updateButtonState(game->nextWaveButton))
    {
        if(game->field->currentWave < game->field->waveNumber - 1)
        {
            game->field->currentWave++;
            game->field->lastActivetedEnemy = game->field->waves[game->field->currentWave];
            game->field->waves[game->field->currentWave]->isEnabled = TRUE;
            game->nextWaveButton->isEnabled = FALSE;
        }
    }

    if(updateButtonState(game->sellButton)) // Venda - foi acionado
    {
        // Vender torre
        sellTower(game->player, game->selectedTower, game->field->tile);
        game->selectedTower = NULL;
    }

    if(updateButtonState(game->upgradeButton)) // Upgrade - foi acionado
    {
        // Fazer upgrade
        game->outputLog->spentMoney += game->selectedTower->upgradeCost;
        game->outputLog->spentMoney += game->selectedTower->upgradeCost;
        game->player->money -= game->selectedTower->upgradeCost;
        upgradeTower(game->selectedTower);

        if(game->selectedTower->type == BEACON)
        {
            for(i = 0; i < (game->player->towerQuantity); i++) // Para cada torre, verificar se ela está dentro da range da torre beacon em que foi feito o upgrade
            {
                if((game->player->towers[i]->type != BEACON)  && (distanceBetween(game->player->towers[i]->x, game->player->towers[i]->y, game->selectedTower->x, game->selectedTower->y) <= game->selectedTower->range))
                {
                    // Torre dentro da range
                    if((game->player->towers[i]->beacon == NULL) || (game->player->towers[i]->beacon->rank < game->selectedTower->rank))
                    {
                        game->player->towers[i]->beacon = game->selectedTower;
                    }
                }
            }
        }
    }

    if(game->selectedTower != NULL) // Se a torre estiver selecionada,
    {
        game->sellButton->isEnabled = TRUE; // Habilitar o botão de compra

        // Checar se o botão de upgrade pode ser habilitado (seja por conta do dinheiro ou o rank da torre)
        if((((game->selectedTower->type != BEACON) && (game->selectedTower->rank < 4))
        ||  ((game->selectedTower->type == BEACON)  && (game->selectedTower->rank < 2)))
        &&  (game->selectedTower->upgradeCost) <= game->player->money)
        {
            game->upgradeButton->isEnabled = TRUE;
        }

        else
        {
            game->upgradeButton->isEnabled = FALSE;
        }
    }

    else
    {
        game->sellButton->isEnabled = FALSE;
        game->upgradeButton->isEnabled = FALSE;
    }
}

int executeGame(const char *fileDirectory, BITMAP *buffer, BITMAP *defaultCursor)
{
    FILE *inputFile = fopen(fileDirectory, "r");
    MIDI* backgroundSound = load_midi("Sounds/Game/rach_op30_3.mid");
    Game *game = createGame(inputFile);
    Wave *node;

    unsigned int last_tick = 0;
    int result = PLAY, i;

    play_midi(backgroundSound, -1);

    if((game != NULL) && (inputFile != NULL)) // Tudo ok para iniciar o jogo
    {
        tick = 0;

        while(!key[KEY_ESC] && (result == PLAY))
        {
            if(tick != last_tick)
            {
                // Display
                clear(buffer);
                drawGameFrame(game, buffer, defaultCursor);

                // Atualizar
                if(game->selectedTowerButton == NULL)
                {
                    selectTowerByMouse(game->player, &game->selectedTower);
                }

                if(game->field->currentWave != -1)
                {
                    spawnEnemies(game->field, tick);
                    game->player->life -= moveWave(game->field->waves[game->field->currentWave]);

                    for(i = 0; i < game->player->towerQuantity; i++)
                    {
                        redirectTower(game->player->towers[i], game->field->waves[game->field->currentWave], game->player);
                        attackEnemies(game->player, game->player->towers[i], game->field->waves[game->field->currentWave], tick);

                        for(node = game->field->waves[game->field->currentWave]; node != NULL; node = node->next) // Zerar a taxa de slow dos inimigos
                        {
                            if(node->isEnabled)
                            {
                                node->enemy->hp -= node->enemy->poisonTax;

                                if(node->enemy->hp <= 0) // Inimigo morreu
                                {
                                    node->isEnabled = FALSE;
                                    game->player->money += node->enemy->money;
                                }

                                node->enemy->poisonTax = 0;
                            }
                        }
                    }

                    if((game->field->currentWave < game->field->waveNumber - 1) && (isWaveFinished(game->field->waves[game->field->currentWave])))
                    {
                        game->nextWaveButton->isEnabled = TRUE;
                    }

                    else if((game->field->currentWave == game->field->waveNumber - 1) && (isWaveFinished(game->field->waves[game->field->currentWave])))
                    {
                        result = WIN;
                    }
                }

                updateButtonsState(game);

                if(key[KEY_R])
                {
                    result = WIN;
                }

                // Para fins de teste
                if(key[KEY_UP])
                {
                    game->player->money += 10;
                }

                else if(key[KEY_DOWN])
                {
                    game->player->money -= 10;
                }

                if(key[KEY_RIGHT])
                {
                    game->player->life++;
                }

                else if(key[KEY_LEFT])
                {
                    game->player->life--;
                }
//
                if(game->player->life <= 0) // Perdeu
                {
                    game->player->life = 0;
                    result = GAME_OVER;
                }

                if(result != PLAY)
                {
                    if(result == WIN)
                    {
                        fprintf(game->outputLog->outputFile, "O jogador Venceu!\n");
                    }
                    else if(result == GAME_OVER)
                    {
                        fprintf(game->outputLog->outputFile, "O jogador Perdeu!\n");
                    }

                    if(game->field->currentWave == -1)
                    {
                        game->field->currentWave = 0;
                    }

                    fprintf(game->outputLog->outputFile, "%d\n", game->field->currentWave + 1);
                    writeKilledEnemies(game->outputLog);

                    fprintf(game->outputLog->outputFile, "%d\n", game->outputLog->spentMoney);
                    fprintf(game->outputLog->outputFile, "%d\n", game->player->money);
                    fprintf(game->outputLog->outputFile, "%d\n", game->player->life);
                    printTowerMatrix(game->outputLog);
                    // Escrever o log
                }

                last_tick = tick;
            }
        }

        stop_midi();
        destroy_midi(backgroundSound);

        if(result == GAME_OVER)
        {
            showGameOver(buffer, 0);
        }

        else if(result == WIN)
        {
            showWinnerScreen(buffer);
            printf("win! \n");
        }

        destroyGame(game);
        fclose(inputFile);
        result = GAME_OVER;
    }

    else if(inputFile == NULL) // Erro do arquivo
    {
        result = FILE_ERROR;
    }

    else
        result = ERROR;

    return result;
}



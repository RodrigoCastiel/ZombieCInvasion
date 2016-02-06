#include "Player.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Player* createPlayer(FILE *inputFile, BITMAP *towerSheet, BITMAP *normalBulletImg, BITMAP *splashBulletImg, BITMAP *smokeEffectImage)
{
    Player* player = (Player*)malloc(sizeof(Player));
    int i;

    if(player != NULL)
    {
        player->towers = NULL;
        player->towerQuantity = 0;
        player->towerSheet = towerSheet;
        player->bulletList = NULL;
        player->effectList = NULL;
        player->normalBulletImg = normalBulletImg;
        player->splashBulletImg = splashBulletImg;
        player->smokeEffectImage = smokeEffectImage;

        fscanf(inputFile, "%d", &player->life);
        fscanf(inputFile, "%d", &player->money);

        for(i = 0; i < 3; i++)
        {
            player->priorities[i] = PRIORITY_NEAR;
        }
    }

    return player;
}

// Destrutor
void destroyPlayer(Player *player)
{
    int i;

    if(player != NULL)
    {
        if(player->towers != NULL) // Deletar as torres e o vetor de torres
        {
            for(i = 0; i < player->towerQuantity; i++)
            {
                if(player->towers[i] != NULL)
                {
                    free(player->towers[i]);
                }
            }

            free(player->towers);
            destroyBulletList(player->bulletList);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawPlayerInformation(Player *player, BITMAP *buffer)
{
    textprintf_ex(buffer, digitalFont, TILE_SIZE * FIELD_WIDTH + 60,  118, makecol(0, 255, 40)  , -1, "%d", player->life);
    textprintf_ex(buffer, digitalFont, TILE_SIZE * FIELD_WIDTH + 173, 118, makecol(255, 255, 25), -1, "%d", player->money);
}

void drawAndMoveBullets(Player *player, Wave *wave, Log *log, BITMAP *buffer, unsigned tick)
{
    Bullet *bullet = player->bulletList, *next;
    Wave *node = NULL;
    int result = 0, random;
    double damage;

    if(player->bulletList != NULL)
    {
        while(bullet != NULL)
        {
            result = moveBullet(bullet); // Mover a bala e verificar o que aconteceu
            next = bullet->next;

            if(result)
            {
                if(bullet->type == BULLET_SPLASH) // Explodir o que tem em volta
                {
                    // Nova explos'ao - adicionar novo efeito
                    player->effectList = insertEffect(player->effectList, bullet->x, bullet->y, 5, 5, EFFECT_EXPLOSION, tick);

                    // Procurar os inimigos que estão em volta
                    for(node = wave; node != NULL; node = node->next)
                    {
                        if((node != bullet->target) && (node->isEnabled) && (node->enemy->type != 'R')
                        && (distanceBetween(node->enemy->centre->x, node->enemy->centre->y, bullet->x, bullet->y) <= bullet->explosionRange))
                        {
                            node->enemy->hp -= bullet->damage * (((double)bullet->explosionDamage)/100.0);

                            if(node->enemy->hp <= 0) // Inimigo morreu
                            {
                                node->isEnabled = FALSE;
                                player->money += bullet->target->enemy->money;
                                addKilledEnemy(log, bullet->target->enemy->type);
                            }
                        }
                    }
                }
            }

            if(result == 2) // Atingiu um inimigo vivo
            {
                random = rand() % 100;

                // O dano que o alvo levará ao ser atingido será o dano do projétil menos a defesa do alvo, tendo um dano mínimo de 5pts
                if((random % 20 == 0)) // Crítico
                {
                    damage = (bullet->damage - bullet->target->enemy->def) * 1.3;
                }

                else if((random % 33 == 0)) // Fraco
                {
                    damage = (bullet->damage - bullet->target->enemy->def) * 0.8;
                }

                else
                {
                    damage = (bullet->damage - bullet->target->enemy->def);
                }

                if(damage < 5)
                {
                    damage = 5;
                }

                bullet->target->enemy->hp -= damage;

                if(bullet->target->enemy->hp <= 0) // Inimigo morreu
                {
                    bullet->target->isEnabled = FALSE;
                    player->money += bullet->target->enemy->money;
                    addKilledEnemy(log, bullet->target->enemy->type);
                }

                player->bulletList = removeBullet(player->bulletList, bullet); // Removê-lo da lista
                // Aplicar algum efeito
            }

            else if(result == 1) // Chegou na posição do inimigo, mas ele está morto
            {
                player->bulletList = removeBullet(player->bulletList, bullet); // Removê-lo da lista
                // Aplicar algum efeito
            }

            if(bullet->type == BULLET_NORMAL)
            {
                pivot_sprite(buffer, player->normalBulletImg, bullet->x - player->normalBulletImg->w/2, bullet->y - player->normalBulletImg->h/2,
                              player->normalBulletImg->w/2, player->normalBulletImg->h/2, ftofix((tick * 15) % 360));
            }

            else
            {
                //draw_sprite(buffer, player->splashBulletImg, bullet->x - player->splashBulletImg->w/2, bullet->y - player->splashBulletImg->h/2);
                pivot_sprite(buffer, player->splashBulletImg, bullet->x - player->splashBulletImg->w/2, bullet->y - player->splashBulletImg->h/2,
                              player->splashBulletImg->w/2, player->splashBulletImg->h/2,  itofix((int)bullet->angle));
               // circle(buffer, bullet->x, bullet->y, bullet->explosionRange, makecol(255, 0, 0));
            }

            bullet = next;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Busca a melhor torre beacon que pode influenciar a torre passada
void searchBestBeaconInRange(Player *player, Tower *tower)
{
    Tower *bestBeacon = NULL;
    int i;

    for(i = 0; i < player->towerQuantity; i++)
    {
        if((player->towers[i] != tower) && (player->towers[i]->type == BEACON)
        && ( distanceBetween(player->towers[i]->x, player->towers[i]->y, tower->x, tower->y) <= player->towers[i]->range))
        {
            if((bestBeacon == NULL) || player->towers[i]->rank > tower->rank) // Salvar sempre a melhor
            {
                bestBeacon = player->towers[i];
            }
        }
    }

    tower->beacon = bestBeacon;
}

// Adicionar a torre comprada ao vetor de torres
void  addTower(Player *player, TowerType type, int line, int column, int last_tick)
{
    Tower **aux, *newTower;
    int i;

    aux = (Tower**)realloc(player->towers, sizeof(Tower*) * (player->towerQuantity + 1));

    if(aux != NULL) // Se não houver erro de alocação,
    {
        player->towers = aux;
        newTower = createTower(type, (double)column * TILE_SIZE + (double)TILE_SIZE/2, (double)line * TILE_SIZE + (double)TILE_SIZE/2, player->priorities[(int)type]);

        if(newTower != NULL) // Se não houver (novamente) erro de alocação:
        {
            newTower->last_tick = last_tick;

            if(type == BEACON) // Verificar se, dentro da range da torre criada, há uma torre que possa ser melhorada
            {
                for(i = 0; i < (player->towerQuantity); i++) // Para cada torre, verificar se ela está dentro da range da nova torre beacon
                {
                    if((player->towers[i]->type != BEACON)  && (distanceBetween(player->towers[i]->x, player->towers[i]->y, newTower->x, newTower->y) <= newTower->range))
                    {
                        // Torre dentro da range
                        if((player->towers[i]->beacon == NULL) || (player->towers[i]->beacon->rank < newTower->rank))
                        {
                            player->towers[i]->beacon = newTower;
                        }
                    }
                }
            }

            else // Verificar se há alguma beacon no range da nova torre para reforçá-la (a com o rank mais alto)
            {
                searchBestBeaconInRange(player, newTower);
            }

            player->money -= newTower->sellCost * 2; // Gastar dinheiro
        }

        player->towerQuantity++;
        player->towers[player->towerQuantity - 1] = newTower;
    }
}

void sellTower(Player *player, Tower *tower, int tile[][FIELD_WIDTH])
{
    int i, j;
    Tower **aux;

    for(i = 0; player->towers[i] != tower; i++); // Procurar a torre a ser vendida

    for(j = i; j < player->towerQuantity - 1; j++) // Deslocar as torres para deletar o último
    {
        player->towers[j] = player->towers[j + 1];
    }

    aux = (Tower**)realloc(player->towers, sizeof(Tower*) * (player->towerQuantity - 1));

    if((aux != NULL) || (player->towerQuantity == 1))
    {
        player->towerQuantity--;
        tile[((int)tower->y)/ TILE_SIZE][((int)tower->x)/ TILE_SIZE] = 0;

        player->money += tower->sellCost;
        player->towers = aux;

        if(tower->type == BEACON) // Se a torre deletada for beacon, será necessário procurar outra torre beacon para cada torre (que estava sendo influenciada por ela)
        {
            for(i = 0; i < player->towerQuantity; i++)
            {
                if(player->towers[i]->type != BEACON)
                {
                    // printf("");
                    searchBestBeaconInRange(player, player->towers[i]);
                }
            }
        }

        free(tower);
    }
}


void drawTowers(Player *player, BITMAP *buffer)
{
    int i, j;

    for(i = 0; i < player->towerQuantity; i++) // Para cada tipo, cortar a imagem correspondente
    {
        if(!player->towers[i]->isSelected)
        {
            masked_blit(player->towerSheet, buffer, player->towers[i]->type * TILE_SIZE, 0, player->towers[i]->x - TILE_SIZE/2, player->towers[i]->y - TILE_SIZE/2, TILE_SIZE, TILE_SIZE);
        }

        if(player->towers[i]->type == SLOW)
        {
            //stretch_sprite(buffer, player->smokeEffectImage, (int)player->towers[i]->x - player->towers[i]->range, player->towers[i]->y - player->towers[i]->range, player->towers[i]->range * 2, player->towers[i]->range * 2);

            for(j = 0; j < player->towers[i]->rank + 1; j++)
            {
                if(player->towers[i]->slowTargets[j] != NULL)
                {
                    line(buffer, player->towers[i]->x, player->towers[i]->y, player->towers[i]->slowTargets[j]->enemy->centre->x, player->towers[i]->slowTargets[j]->enemy->centre->y, 0xFFFFFF);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Salava o endereço da torre selecionada pelo mouse (verificando-se a coordenada do mouse no quadrado)
void selectTowerByMouse(Player *player, Tower **selectedTower)
{
    int i, selected = FALSE, clicked = FALSE;
    int mouse_i = mouse_y/TILE_SIZE, mouse_j = mouse_x/TILE_SIZE;

    for(i = 0; (i < player->towerQuantity) && !selected; i++) // Para cada torre,
    {
        // verifica se, ao apertar o primeiro botão do mouse, o cursor está no mesmo tile que a torre i
        if((mouse_b & 1))
        {
            clicked = TRUE;

            if((mouse_j == (((int)player->towers[i]->x)/TILE_SIZE)) && ((mouse_i == (((int)player->towers[i]->y)/TILE_SIZE))))
            {
                if(*selectedTower != NULL) // Se já tiver selecionado alguma torre, de-selecionar
                {
                    (*selectedTower)->isSelected = FALSE;
                }

                *selectedTower = player->towers[i];
                player->towers[i]->isSelected = TRUE;
                selected = TRUE;
            }
        }
    }

    // Apertou fora dos tile de qualquer torre, mas ainda dentro do campo
    if(clicked && !selected && mouse_x < FIELD_WIDTH * TILE_SIZE)
    {
        if(*selectedTower != NULL) // Se já tiver selecionado alguma torre, de-selecionar
        {
            (*selectedTower)->isSelected = FALSE;
            (*selectedTower) = NULL;
        }
    }
}

void redirectTower(Tower *tower, Wave *wave, Player *player)
{
    Wave *target = NULL;
    int i, slowTax;

    if(tower->type == NORMAL)
    {
        // Se a torre ainda não tem alvo ou o alvo que tinha morreu ou o usuário redirecionou, procurar!!
        if((tower->priority != player->priorities[(int)tower->type]) || (tower->target == NULL) || (!tower->target->isEnabled) || (distanceBetween(tower->x, tower->y, tower->target->enemy->centre->x, tower->target->enemy->centre->y) > tower->range))
        {
            tower->priority = player->priorities[(int)tower->type];

            if(tower->priority == PRIORITY_NEAR)
            {
                target = searchNearestEnemy(tower, wave);
            }

            else if(tower->priority == PRIORITY_DIST)
            {
                target = searchFirstEnemy(tower, wave);
            }

            else if(tower->priority == PRIORITY_MINHP)
            {
                target = searchSmallestHPEnemy(tower, wave);
            }

            else if(tower->priority == PRIORITY_MAXHP)
            {
                target = searchBiggestHPEnemy(tower, wave);
            }

            tower->target = target;
        }
    }

    else if(tower->type == SPLASH)
    {
        // Se a torre ainda não tem alvo ou o alvo que tinha morreu ou o usuário redirecionou, procurar!!
        if((tower->priority != player->priorities[(int)tower->type]) || (tower->target == NULL) || (!tower->target->isEnabled) || (distanceBetween(tower->x, tower->y, tower->target->enemy->centre->x, tower->target->enemy->centre->y) > tower->range))
        {
            tower->priority = player->priorities[(int)tower->type];

            if(tower->priority == PRIORITY_NEAR)
            {
                target = searchNearestEnemy(tower, wave);
            }

            else if(tower->priority == PRIORITY_DIST)
            {
                target = searchFirstEnemy(tower, wave);
            }

            else if(tower->priority == PRIORITY_MINHP)
            {
                target = searchSmallestHPEnemy(tower, wave);
            }

            else if(tower->priority == PRIORITY_MAXHP)
            {
                target = searchBiggestHPEnemy(tower, wave);
            }

            tower->target = target;
        }
    }

    else if(tower->type == SLOW)
    {
        for(i = 0; i < tower->rank + 1; i++)
        {
            target = tower->slowTargets[i];

            if((tower->priority != player->priorities[(int)tower->type]) || (tower->slowTargets[i] == NULL) || (!tower->slowTargets[i]->isEnabled) || (distanceBetween(tower->x, tower->y, tower->slowTargets[i]->enemy->centre->x, tower->slowTargets[i]->enemy->centre->y) > tower->range))
            {
                tower->priority = player->priorities[(int)tower->type];
                tower->slowTargets[i] = NULL;

                // Procurar pelo novo inimigo, de acordo com a prioridade
                if(tower->priority == PRIORITY_NEAR)
                {
                    target = searchNearestEnemy(tower, wave);
                }

                else if(tower->priority == PRIORITY_DIST)
                {
                    target = searchFirstEnemy(tower, wave);
                }

                else if(tower->priority == PRIORITY_MINHP)
                {
                    target = searchSmallestHPEnemy(tower, wave);
                }

                else if(tower->priority == PRIORITY_MAXHP)
                {
                    target = searchBiggestHPEnemy(tower, wave);
                }

                tower->slowTargets[i] = target;
            }

            if((target != NULL))
            {
                if(tower->beacon != NULL)
                {
                    slowTax = tower->slowTax + 5 * (tower->beacon->rank + 1);
                }

                else
                {
                    slowTax = 35;
                }

                if(target->enemy->slowTax < slowTax) // Deixar o inimigo com slow, se ele já não tiver uma slow mais forte
                {
                    target->enemy->slowTax = slowTax;
                }
            }
        }
    }


}

void attackEnemies(Player *player, Tower *tower, Wave *wave, unsigned int tick)
{
    Wave *target = tower->target;

    if((tower->type == NORMAL) && (tick - tower->last_tick >= 16) && (target != NULL))
    {
        tower->last_tick = tick;

        if(tower->beacon == NULL)
        {
            player->bulletList = insertBullet(player->bulletList, BULLET_NORMAL, tower->x, tower->y, 0.12 * TILE_SIZE, 0, 0, tower->damage, target);
        }

        else
        {
            player->bulletList = insertBullet(player->bulletList, BULLET_NORMAL, tower->x, tower->y, 0.12 * TILE_SIZE, 0, 0, tower->damage * (1.0 + (5 * tower->beacon->rank + 5)/100.0), target);
        }
    }

    else if((tower->type == SPLASH) && (tick - tower->last_tick >= 22) && (target != NULL))
    {
        tower->last_tick = tick;

        if(tower->beacon == NULL)
        {
            player->bulletList = insertBullet(player->bulletList, BULLET_SPLASH, tower->x, tower->y, 0.12 * TILE_SIZE, tower->explosionRange, tower->explosionDamage, tower->damage, target);
        }

        else
        {
            player->bulletList = insertBullet(player->bulletList, BULLET_SPLASH, tower->x, tower->y, 0.12 * TILE_SIZE, tower->explosionRange + 5 * (tower->beacon->rank + 1), tower->explosionDamage, tower->damage * (1.0 + (5 * tower->beacon->rank + 5)/100.0), target);
        }
    }

    else if((tower->type == POISON) && (tick - tower->last_tick >= 3))
    {
        tower->last_tick = tick;

        while(wave != NULL) // Verificar se cada inimigo em jogo está na range da torre (para perder hp)
        {
            if((wave->isEnabled) && (wave->enemy->type != 'I') && ((distanceBetween(tower->x, tower->y, wave->enemy->centre->x, wave->enemy->centre->y)) <= tower->range))
            {
                // Verificar se a taxa de poison de outra torre no inimgo é menor. Se for menor, a torre atual vai modificar a taxa de slow do inimigo
                if((tower->beacon != NULL) && (wave->enemy->poisonTax < (tower->damage + tower->beacon->rank + 1)))
                {
                    wave->enemy->poisonTax = (tower->damage + tower->beacon->rank + 1);
                }

                else if(wave->enemy->poisonTax < tower->damage)
                {
                    wave->enemy->poisonTax = tower->damage;
                }
            }

            wave = wave->next;
        }

        tower->last_tick = tick;
    }

}

Wave *searchNearestEnemy(Tower *tower, Wave *wave)
{
    double d, minimum = FIELD_WIDTH * TILE_SIZE;
    Wave *nearest = NULL;
    int alreadyChosen, i;

    while(wave != NULL) // Verificar se cada inimigo em jogo está na range da torre (para perder hp)
    {
        if((wave->isEnabled) && ((tower->type == SLOW && wave->enemy->type != 'I') || ((tower->type != SLOW))) ) // Armazenar o inimigo com a menor distância (se ele já não tiver sido escolhido, no caso da torre slow)
        {
            d = distanceBetween(tower->x, tower->y, wave->enemy->centre->x, wave->enemy->centre->y); // Achar a distância entre o inimigo atual e a torre

            if(d <= tower->range)
            {
                if(d < minimum)
                {
                    alreadyChosen = FALSE;

                    if(tower->type == SLOW)
                    {
                        for(i = 0; (i < tower->rank + 1) && !alreadyChosen; i++)
                        {
                            alreadyChosen = (tower->slowTargets[i] == wave);
                        }
                    }

                    if(!alreadyChosen) // Checar se o inimigo atual já foi escolhido
                    {
                        nearest = wave;
                        minimum = d;
                    }
                }
            }
        }

        wave = wave->next;
    }

    return nearest;
}

Wave *searchBiggestHPEnemy(Tower *tower, Wave *wave)
{
    Wave *maxHPEnemy = NULL;
    int maxHP = 0, alreadyChosen, i;
    double d;

    while(wave != NULL) // Verificar se cada inimigo em jogo está na range da torre (para perder hp)
    {
        // Armazenar o inimigo com a menor distância
        if((wave->isEnabled) && ((tower->type == SLOW && wave->enemy->type != 'I') || ((tower->type != SLOW))) )
        {
            d = distanceBetween(tower->x, tower->y, wave->enemy->centre->x, wave->enemy->centre->y); // Achar a distância entre o inimigo atual e a torre

            if(d <= tower->range)
            {
                if((wave->enemy->hp > maxHP))
                {
                    alreadyChosen = FALSE;

                    if(tower->type == SLOW)
                    {
                        for(i = 0; (i < tower->rank + 1) && !alreadyChosen; i++)
                        {
                            alreadyChosen = (tower->slowTargets[i] == wave);
                        }
                    }

                    if(!alreadyChosen) // Checar se o inimigo atual já foi escolhido
                    {
                        maxHPEnemy = wave;
                        maxHP = wave->enemy->hp;
                    }
                }
            }
        }

        wave = wave->next;
    }

    return maxHPEnemy;
}

Wave *searchSmallestHPEnemy(Tower *tower, Wave *wave)
{
    Wave *minHPEnemy = NULL;
    int minHP = -1, alreadyChosen, i;
    double d;

    while(wave != NULL) // Verificar se cada inimigo em jogo está na range da torre (para perder hp)
    {
        // Armazenar o inimigo com a menor distância
        if((wave->isEnabled) && ((tower->type == SLOW && wave->enemy->type != 'I') || ((tower->type != SLOW))) )
        {
            d = distanceBetween(tower->x, tower->y, wave->enemy->centre->x, wave->enemy->centre->y); // Achar a distância entre o inimigo atual e a torre

            if(d <= tower->range)
            {
                if((wave->enemy->hp < minHP) || (minHP == -1))
                {
                    alreadyChosen = FALSE;

                    if(tower->type == SLOW)
                    {
                        for(i = 0; (i < tower->rank + 1) && !alreadyChosen; i++)
                        {
                            alreadyChosen = (tower->slowTargets[i] == wave);
                        }
                    }

                    if(!alreadyChosen) // Checar se o inimigo atual já foi escolhido
                    {
                        minHPEnemy = wave;
                        minHP = wave->enemy->hp;
                    }
                }
            }
        }

        wave = wave->next;
    }

    return minHPEnemy;
}

Wave *searchFirstEnemy (Tower *tower, Wave *wave)
{
    Wave *firstEnemy = NULL;
    double d, maxElapsedDist = 0.0;
    int alreadyChosen, i;

    while(wave != NULL) // Verificar se cada inimigo em jogo está na range da torre (para perder hp)
    {
        // Armazenar o inimigo com a menor distância
        if((wave->isEnabled) && ((tower->type == SLOW && wave->enemy->type != 'I') || ((tower->type != SLOW))) )
        {
            d = distanceBetween(tower->x, tower->y, wave->enemy->centre->x, wave->enemy->centre->y); // Achar a distância entre o inimigo atual e a torre

            if(d <= tower->range)
            {
                if(wave->enemy->elapsedDistance > maxElapsedDist)
                {
                    alreadyChosen = FALSE;

                    if(tower->type == SLOW)
                    {
                        for(i = 0; (i < tower->rank + 1) && !alreadyChosen; i++)
                        {
                            alreadyChosen = (tower->slowTargets[i] == wave);
                        }
                    }

                    if(!alreadyChosen) // Checar se o inimigo atual já foi escolhido
                    {
                        firstEnemy = wave;
                        maxElapsedDist = wave->enemy->elapsedDistance;
                    }
                }
            }
        }

        wave = wave->next;
    }


    return firstEnemy;
}

void drawAndUpdateEffects(Player *player, BITMAP *buffer, BITMAP *effectSprite, unsigned tick)
{
    Effect *node = player->effectList, *next = NULL;
    int result;

    while(node != NULL)
    {
        result = moveAndUpdateEffect(node, tick);
        next = node->next;

        if(result) // Se tiver terminado
        {
            player->effectList = removeEffect(player->effectList, node);
        }

        else
        {
            masked_blit(effectSprite, buffer, (node->currentFrame % node->w) * 64, (node->currentFrame / node->w) * 64, node->x - 32, node->y - 32, 64, 64);
        }

        node = next;
    }
}










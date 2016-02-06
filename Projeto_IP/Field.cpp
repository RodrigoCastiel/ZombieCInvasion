#include "Field.h"

///////////////////////////////////////////////////////////////////////////////////////////////

// Essa função é só para não ter que toda hora ficar checando se o bitmap é NULL
inline void destroyBitmap(BITMAP **bitmap)
{
    if((bitmap != NULL) && (*bitmap != NULL))
    {
        destroy_bitmap(*bitmap);
        *bitmap = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Funções para Field */

// Destrutor
void destroyField(Field *field)
{
    int i;

    if(field != NULL)
    {
        destroyBitmap(&field->buffer);
        destroyCoordList(field->path);

        if(field->waves != NULL)
        {
            for(i = 0; i < (field->waveNumber); i++) // Deletar as listas encadeadas de inimigos
            {
                destroyWave(field->waves[i]);
            }

            free(field->waves);
        }

        free(field);
    }
}

// Salva o caminho dos inimigos nos tiles do campo // Retorna 1 se conseguir achar e 0 caso contrário
int findPath(Field *field)
{
    int i, j, last_i, last_j; // i, j: contadores // last_i, last_j guardam a posição da última iteração
    int l = 0, c = 0, error = 0;      // l, c: coordenadas do 2 (do início)

    if(field != NULL)
    {   // Achar o início do caminho
        for(i = 0; (i < FIELD_HEIGHT) && (field->tile[l][c] != 2); i++)
        {
            for(j = 0; (j < FIELD_WIDTH) && (field->tile[l][c] != 2); j++)
            {
                if(field->tile[i][j] == 2) // Achou!
                {
                    l = i;
                    c = j;
                }
            }
        }

        field->path = createCoordinate((double)c * TILE_SIZE + (double)TILE_SIZE/2, // Salva a primeira coordenada
                                       (double)l * TILE_SIZE + (double)TILE_SIZE/2);
        last_i = i = l;
        last_j = j = c;

        // Descobrir qual é a primeira casa depois do início
        if((l > 0) && (field->tile[l - 1][c] == 1)) // Caso 1 - trilho em cima
            i = (l - 1);

        else if((l < FIELD_HEIGHT - 1) && (field->tile[l + 1][c] == 1)) // Caso 2 - trilho em baixo
            i = (l + 1);

        else if((c < FIELD_WIDTH - 1) && (field->tile[l][c + 1] == 1)) // Caso 3 - trilho à direita
            j = (c + 1);

        else if((c > 0) && (field->tile[l][c - 1] == 1)) // Caso 4 - trilho à esquerda
            j = (c - 1);

        // Agora, achar o caminho. Ou seja, começar do 2 e chegar no 3, seguindo a trilha de 1's
        while((field->tile[i][j] != 3))
        {
            error = !(appendCoordinate(field->path, // Inserir uma nova coordenada no final do caminho
                                       (double)j * TILE_SIZE + (double)TILE_SIZE/2,   // x do centro
                                       (double)i * TILE_SIZE + (double)TILE_SIZE/2)); // y do centro
            if(!error)
            {
                if((i > 0) && (last_i != (i - 1)) && ((field->tile[i - 1][j] == 1) || (field->tile[i - 1][j] == 3))) // Caso 1 - vai para o trilho em cima
                {
                    last_i = i;
                    last_j = j;
                    i--;
                }

                else if((i < FIELD_HEIGHT - 1) && (last_i != (i + 1) && ((field->tile[i + 1][j] == 1) || (field->tile[i + 1][j] == 3)))) // Caso 2 - vai para o trilho em baixo
                {
                    last_i = i;
                    last_j = j;
                    i++;
                }

                else if((j < FIELD_WIDTH - 1) && (last_j != (j + 1) && ((field->tile[i][j + 1] == 1) || (field->tile[i][j + 1] == 3)))) // Caso 3 - trilho à direita
                {
                    last_i = i;
                    last_j = j;
                    j++;
                }

                else if((j > 0) && (last_j != (j - 1)) && ((field->tile[i][j - 1] == 1) || (field->tile[i][j - 1] == 3))) // Caso 4 - trilho à esquerda
                {
                    last_i = i;
                    last_j = j;
                    j--;
                }
            }
        }

        error = !(appendCoordinate(field->path, // Inserir a última coordenada no final do caminho
                                       j * TILE_SIZE + (double)TILE_SIZE/2,   // x do centro
                                       i * TILE_SIZE + (double)TILE_SIZE/2)); // y do centro
    }

    return !error;
}

// Construtor
Field *createField(FILE *inputFile, BITMAP *spriteSheet, BITMAP *fog)
{
    Field *field = (Field *)malloc(sizeof(Field));
    int error = 0, i, j;

    if(field != NULL) // Inicializar
    {
        field->sheet = spriteSheet;
        field->fog   = fog;
        field->waveNumber  = 0;
        field->currentWave = -1;
        field->waves = NULL;
        field->lastActivetedEnemy = NULL;

        for(i = 0; (i < FIELD_HEIGHT); i++)
        {
            for(j = 0; (j < FIELD_WIDTH); j++)
            {
                fscanf(inputFile, "%d", &field->tile[i][j]);

                if((field->tile[i][j] == 0) && ((rand() % 2) == 0)) // Se for espaço livre
                {
                    // Sortear um objeto aleatório (ou não)
                    field->tile[i][j] = - rand() % 3;
                }
            }
        }

        field->buffer = create_bitmap(TILE_SIZE, TILE_SIZE);
        error = (field->buffer == NULL);

        if(error)
        {
            destroyField(field);
            field = NULL;
        }

        else // Montar o caminho logo
        {
            error = !findPath(field);
        }
    }

    return field;
}


// Desenha apenas o background, pista, origem e destino
void drawFieldTerrain(Field *field, BITMAP *buffer)
{
    int i, j, x, y;

    if(field != NULL)
    {
        for(i = 0; i < FIELD_HEIGHT; i++) // Percorrer a matriz para salvar cada elemento
        {
            for(j = 0; j < FIELD_WIDTH; j++)
            {
                x = j * TILE_SIZE;
                y = i * TILE_SIZE;

                blit(field->sheet, field->buffer, 3 * TILE_SIZE, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                draw_sprite(buffer, field->buffer, x, y);

                if(field->tile[i][j] == 1) // Pista
                {
                    if((i > 0) && (i < FIELD_HEIGHT - 1)
                       && (field->tile[i - 1][j] > 0)
                       && (field->tile[i + 1][j] > 0)) // Pista vertical retilínia
                    {
                        blit(field->sheet, field->buffer, 0, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                        draw_sprite(buffer, field->buffer, x, y);
                    }

                    else if((j > 0) && (j < FIELD_WIDTH - 1)
                       && (field->tile[i][j - 1] > 0)
                       && (field->tile[i][j + 1] > 0)) // Pista horizontal retílinia
                    {
                        blit(field->sheet, field->buffer, 0, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                        rotate_sprite(buffer, field->buffer, x, y, itofix(64));
                    }

                    else if((j < FIELD_WIDTH - 1)
                        && (i < FIELD_HEIGHT - 1)
                        && (field->tile[i][j + 1] > 0)
                        && (field->tile[i + 1][j] > 0)) // Borda: baixo e direita
                    {
                        blit(field->sheet, field->buffer, 1 * TILE_SIZE, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                        draw_sprite(buffer, field->buffer, x, y);
                    }

                    else if((i < FIELD_HEIGHT - 1)
                        &&  (j > 0)
                        &&  (field->tile[i][j - 1] > 0)
                        &&  (field->tile[i + 1][j] > 0)) // Borda: baixo e esquerda
                    {
                        blit(field->sheet, field->buffer, 1 * TILE_SIZE, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                        rotate_sprite(buffer, field->buffer, x, y, itofix(64));
                    }

                    else if((i > 0)
                        && (j < FIELD_WIDTH - 1)
                        && (field->tile[i][j + 1] > 0)
                        && (field->tile[i - 1][j] > 0)) // Borda: cima e direita
                    {
                        blit(field->sheet, field->buffer, 1 * TILE_SIZE, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                        rotate_sprite(buffer, field->buffer, x, y, itofix(-64));
                    }

                    else if((i > 0)
                        &&  (j > 0)
                        &&  (field->tile[i - 1][j] > 0)
                        &&  (field->tile[i][j - 1] > 0)) // Borda: cima e esquerda
                    {
                        blit(field->sheet, field->buffer, 1 * TILE_SIZE, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                        draw_sprite_vh_flip(buffer, field->buffer, x, y);
                    }
                }

                else if(field->tile[i][j] == 2) // Origem do mapa
                {
                    blit(field->sheet, field->buffer, 4 * TILE_SIZE, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                    draw_sprite(buffer, field->buffer, x, y);
                }

                else if(field->tile[i][j] == 3) // Destino
                {
                    blit(field->sheet, field->buffer, 2 * TILE_SIZE, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                    draw_sprite(buffer, field->buffer, x, y);
                }
            }
        }
    }
}

void drawActiveEnemies(Wave *wave, BITMAP *buffer, BITMAP *auxiliar, unsigned int tick)
{
    if(wave != NULL)
    {
        if((wave->enemy->type == 'R') || (wave->enemy->hp <= 0))
        {
            if(wave->isEnabled || (!wave->isEnabled && wave->enemy->hp <= 0))
            {
                drawEnemy(wave->enemy, buffer, auxiliar, tick);
            }

            drawActiveEnemies(wave->next, buffer, auxiliar, tick);
        }

        else
        {
            drawActiveEnemies(wave->next, buffer, auxiliar, tick);

            if(wave->isEnabled || (!wave->isEnabled && wave->enemy->hp <= 0))
            {
                drawEnemy(wave->enemy, buffer, auxiliar, tick);
            }
        }
    }
}

void drawFieldObjects(Field *field, BITMAP *buffer, unsigned int tick)
{
    int i, j, x, y, index;

    if(field != NULL)
    {
        for(i = 0; i < FIELD_HEIGHT; i++)
        {
            for(j = 0; j < FIELD_WIDTH; j++)
            {
                x = j * TILE_SIZE;
                y = i * TILE_SIZE;

                if(field->tile[i][j] == -1) //  (Dead Tree)
                {
                    if((tick/(73 + i - j) % 2 == 0))
                        index = 5 + (i + j) % 2;

                    else if((tick/(71 + i + j) % 2 == 0))
                        index = 6 - (i + j) % 2;

                    else
                        index = 6;

                    blit(field->sheet, field->buffer, index * TILE_SIZE, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                    draw_sprite(buffer, field->buffer, x, y);
                }

                else if(field->tile[i][j] == -2) //  (Rock)
                {
                    blit(field->sheet, field->buffer, 7 * TILE_SIZE, 0, 0, 0, TILE_SIZE, TILE_SIZE);
                    draw_sprite(buffer, field->buffer, x, y);
                }
            }
        }
    }
}

int createEnemies(Field *field, FILE *inputFile, BITMAP *normalSheet, BITMAP *resistentSheet, BITMAP *fastSheet, BITMAP *strongSheet, BITMAP *immuneSheet, BITMAP *bossSheet)
{
    Enemy *enemy = NULL;
    int error = 0, i, level, timeToNext;
    char c, enemyType;

    if(field != NULL)
    {
        fscanf(inputFile, "%d", &field->waveNumber);
        fgetc(inputFile);

        field->waves = (Wave**)calloc(field->waveNumber, sizeof(Wave *)); // Alocar o vetor de ondas

        if(field->waves != NULL) // Não houve erro
        {
            for(i = 0; i < field->waveNumber; i++)
            {
                do
                {
                    fscanf(inputFile, "%c%d%c", &enemyType, &level, &c);

                    timeToNext = 0;

                    if(c == ' ') // Ainda há um próximo inimigo na linha
                    {
                        fscanf(inputFile, "%d", &timeToNext);
                        fgetc(inputFile);
                    }

                    // Criar o inimigo
                    if(enemyType == 'N')
                    {
                        enemy = createEnemy(field->path, normalSheet, 'N', level);
                    }

                    else if(enemyType == 'R')
                    {
                        enemy = createEnemy(field->path, resistentSheet, 'R', level);
                    }

                    else if(enemyType == 'S')
                    {
                        enemy = createEnemy(field->path, strongSheet, 'S', level);
                    }

                    else if(enemyType == 'F')
                    {
                        enemy = createEnemy(field->path, fastSheet, 'F', level);
                    }

                    else if(enemyType == 'I')
                    {
                        enemy = createEnemy(field->path, immuneSheet,'I', level);
                    }

                    else if(enemyType == 'B')
                    {
                        enemy = createEnemy(field->path, bossSheet,'B', level);
                    }

                    field->waves[i] = appendWaveNode(field->waves[i], enemy, timeToNext);

                } while((c == ' ') && !feof(inputFile));
            }

//            field->lastActivetedEnemy = field->waves[0];
//            field->lastActivetedEnemy->isEnabled = TRUE;
        }

        else // Houve erro
        {
            error = 1;
        }
    }

    return !error;
}


void spawnEnemies(Field *field, unsigned int tick)
{
    static unsigned int last_activation = tick;

    if((field->lastActivetedEnemy->next != NULL) && (tick - last_activation) >= field->lastActivetedEnemy->timeToNext)
    {
        field->lastActivetedEnemy = field->lastActivetedEnemy->next;
        field->lastActivetedEnemy->isEnabled = TRUE;
        last_activation = tick;
    }

    else if(field->lastActivetedEnemy->next == NULL)
    {
        last_activation = tick;
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


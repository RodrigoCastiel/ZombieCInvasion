#include "GraphicsAuxiliar.h"

// A seguinte função é para eliminar a preguiça de escrever a condição gigante
// Retorna TRUE se a coordenada (xc, yc) se a coordenada estiver no frame definido por x, y, w, h
inline int isCoordinateInFrame(int xc, int yc, int x, int y, int w, int h)
{
    return (xc >= x) && (xc <= (x + w)) && (yc >= y) && (yc <= y + h);
}

//////////////////////////////////////////////////////////////////////////////

/* Estrutura Button: */

// Construtor
Button *createButton(BITMAP *clickedImage, BITMAP *defaultImage, BITMAP *notEnabledImage, int x, int y)
{
    Button *button = (Button *)malloc(sizeof(Button));

    if(button != NULL)
    {
        button->x = x;
        button->y = y;

        button->clickedImage = clickedImage;
        button->defaultImage = defaultImage;
        button->notEnabledImage = notEnabledImage;

        button->isClicked  = FALSE;
        button->isOccluded = FALSE;
        button->isEnabled  =  TRUE;

        button->w = button->defaultImage->w;
        button->h = button->defaultImage->h;
    }

    return button;
}

int updateButtonState(Button *button)
{
    int triggered = FALSE;

    if(button->isEnabled)
    {
        if(isCoordinateInFrame(mouse_x, mouse_y, button->x, button->y, button->w, button->h)) // O mouse está dentro por cima do botão
        {
            button->isOccluded = TRUE;

            if(mouse_b && !button->isClicked)
            {
                button->isClicked = TRUE;
            }

            else if(!mouse_b && button->isClicked)
            {
                triggered = TRUE;
                button->isClicked = FALSE;
            }
        }

        else
        {
            button->isOccluded = FALSE;

            if(!mouse_b)
            {
                button->isClicked = FALSE;
            }
        }
    }

    return triggered;
}

void drawButton(Button *button, BITMAP *buffer)
{
    if(button != NULL)
    {
        if(button->isEnabled)
        {
            if(button->isClicked && button->isOccluded)
            {
                draw_sprite(buffer, button->clickedImage, button->x, button->y);
            }

            else
            {
                draw_sprite(buffer, button->defaultImage, button->x, button->y);
            }
        }

        else if(button->notEnabledImage != NULL)
        {
            draw_sprite(buffer, button->notEnabledImage, button->x, button->y);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_trans_circle(BITMAP *buffer, int x,  int y, int radius, int color, int translevel)
{
    drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
    set_trans_blender(255, 255, 255, translevel);
    circlefill(buffer, x, y, radius, color);
    drawing_mode(DRAW_MODE_SOLID, 0, 0, 0);
    circle(buffer, x, y, radius, color);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Log *createLog(const char *fileName, int waveNumber)
{
    Log *log = (Log*)malloc(sizeof(Log));
    int i, error = FALSE;

    if(log != NULL)
    {
        error = ((log->outputFile = fopen(fileName, "w")) == NULL);
        log->spentMoney = 0;
        log->waveNumber = waveNumber;

        if(!error)
        {
            for(i = 0; i < 5; i++)
            {
                log->enemiesVector[i] = 0;
                error = ((log->towerMatrix[i] = (int *)calloc(waveNumber, sizeof(int))) == NULL);
            }
        }

        if(error)
        {
            destroyLog(log); // Liberar tudo
        }
    }

    return log;
}

void printTowerMatrix(Log *log)
{
    int i, j;

    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < log->waveNumber; j++)
        {
            if(j != log->waveNumber - 1)
            {
                fprintf(log->outputFile, "%d\t", log->towerMatrix[i][j]);
            }

            else
            {
                fprintf(log->outputFile, "%d", log->towerMatrix[i][j]);
            }
        }

        fprintf(log->outputFile, "\n");
    }
}

void addKilledEnemy(Log *log, char type)
{
    if(type == 'N')
    {
        log->enemiesVector[0]++;
    }

    else if(type == 'R')
    {
        log->enemiesVector[1]++;
    }

    else if(type == 'S')
    {
        log->enemiesVector[2]++;
    }

    else if(type == 'F')
    {
        log->enemiesVector[3]++;
    }

    else
    {
        log->enemiesVector[4]++;
    }
}

void writeKilledEnemies(Log *log)
{
    fprintf(log->outputFile, "N: %d \n", log->enemiesVector[0]);
    fprintf(log->outputFile, "R: %d \n", log->enemiesVector[1]);
    fprintf(log->outputFile, "S: %d \n", log->enemiesVector[2]);
    fprintf(log->outputFile, "F: %d \n", log->enemiesVector[3]);
    fprintf(log->outputFile, "I: %d \n", log->enemiesVector[4]);
}

void destroyLog(Log *log)
{
    int i;

    if(log != NULL)
    {
        for(i = 0; i < 5; i++)
        {
            if(log->towerMatrix[i])
            {
                free(log->towerMatrix[i]);
            }
        }

        if(log->outputFile != NULL)
        {
            fclose(log->outputFile);
        }

        free(log);
    }
}





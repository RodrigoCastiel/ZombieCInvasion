#include "Tower.h"


//////////////////////////////////////////////////////////////////////////////

/* Estrutura Button: */

// Construtor
TowerButton *createTowerButton(BITMAP *clickedImage, BITMAP *defaultImage, BITMAP *notEnabledImage, TowerType type)
{
    TowerButton *button = (TowerButton *)malloc(sizeof(TowerButton));
    int costs[] = {80, 200, 140, 160, 400};
    double ranges[] = {2.0, 1.5, 1.5, 1.5, 2.0};

    if(button != NULL)
    {
        button->x = 10 + FIELD_WIDTH * TILE_SIZE + type * (TILE_SIZE + 5);
        button->y = 447;

        button->clickedImage = clickedImage;
        button->defaultImage = defaultImage;
        button->notEnabledImage = notEnabledImage;

        button->cost  = costs [(int)type];
        button->range = ranges[(int)type] * TILE_SIZE;

        button->isClicked  = FALSE;
        button->isClicked  = FALSE;
        button->isTriggered = FALSE;
        button->isEnabled  = TRUE;
        button->type       =  type;

        button->w = TILE_SIZE;
        button->h = TILE_SIZE;
    }

    return button;
}

void updateTowerButtonState(TowerButton *button)
{
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
                button->isTriggered = TRUE;
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
}

void drawTowerButton(TowerButton *button, BITMAP *buffer)
{
    if(button != NULL)
    {
        if(button->isEnabled)
        {
            if((button->isTriggered) || (button->isOccluded && button->isClicked))
            {
                masked_blit(button->clickedImage, buffer, button->type * TILE_SIZE, 0, button->x, button->y, TILE_SIZE, TILE_SIZE);
            }

            else
            {
                masked_blit(button->defaultImage, buffer, button->type * TILE_SIZE, 0, button->x, button->y, TILE_SIZE, TILE_SIZE);
            }
        }

        else
        {
            masked_blit(button->notEnabledImage, buffer, button->type * TILE_SIZE, 0, button->x, button->y, TILE_SIZE, TILE_SIZE);
        }
    }
}


// Construtor - inicializa o custo com o custo de revenda
Tower *createTower(TowerType type, double x, double y, int priority)
{
    Tower *tower = (Tower *)malloc(sizeof(Tower));
    int i;

    if(tower != NULL)
    {
        tower->type = type;
        tower->rank = 0;
        tower->last_tick = 0;
        tower->isSelected = FALSE;
        tower->beacon = NULL;
        tower->damage  = 0.0;
        tower->priority = priority;
        tower->explosionRange = TILE_SIZE;
        tower->explosionDamage = 40;
        tower->target = NULL;

        tower->x = x;
        tower->y = y;

        for(i = 0; i < 5; i++)
        {
            tower->slowTargets[i] = NULL;
        }

        if(type == NORMAL)
        {
            tower->damage  = 25;
            tower->range   = 2.0 * TILE_SIZE;
            tower->slowTax = 0;
            tower->period  = 16;
            tower->sellCost = 40;
            tower->upgradeCost = 120;
        }

        else if(type == SPLASH)
        {
            tower->damage  = 40;
            tower->range   = 1.5 * TILE_SIZE;
            tower->slowTax = 0;
            tower->period  = 22;
            tower->sellCost = 100;
            tower->upgradeCost = 300;
        }

        else if(type == SLOW)
        {
            tower->range   = 1.5 * TILE_SIZE;
            tower->slowTax = 35;
            tower->period  = 0;
            tower->sellCost = 70;
            tower->upgradeCost = 210;
        }

        else if(type == POISON)
        {
            tower->damage  = 1;
            tower->range   = 1.5 * TILE_SIZE;
            tower->slowTax = 0;
            tower->period  = 3;
            tower->sellCost = 80;
            tower->upgradeCost = 240;
        }

        else if(type == BEACON)
        {
            tower->range   = 2.0 * TILE_SIZE;
            tower->slowTax = 0;
            tower->period  = 0;
            tower->sellCost = 200;
            tower->upgradeCost = 600;
        }

//        tower->range *= 10.0;
    }

    return tower;
}

void upgradeTower(Tower *tower)
{
    if(tower->type == NORMAL)
    {
        tower->damage += 20;
        tower->range  += 0.3 * TILE_SIZE;
    }

    else if(tower->type == SPLASH)
    {
        tower->damage += 35;
        tower->explosionDamage += 15;
        tower->range  += 0.25 * TILE_SIZE;
    }

    else if(tower->type == SLOW)
    {
        tower->range  += 0.25 * TILE_SIZE;
    }

    else if(tower->type == POISON)
    {
        tower->damage += 1;
        tower->range  += 0.2 * TILE_SIZE;
    }

    else // BEACON
    {
        tower->range += 0.5 * TILE_SIZE;
    }

    tower->rank++;
    tower->sellCost += tower->upgradeCost/2;
    tower->upgradeCost *= 1.5;
}
















#include "Presentation.h"

int menu(BITMAP *mouse, BITMAP *buffer) // Menu do jogo - Retorna 1 se for para iniciar o jogo ou 0 para sair
{
    // Carregar as imagens
    BITMAP* onMousePlay = load_bitmap("Sprites/Menu/OnMousePlayButton.bmp", NULL);
    BITMAP* onMouseExit = load_bitmap("Sprites/Menu/OnMouseExitButton.bmp", NULL);
    BITMAP* defaultPlay = load_bitmap("Sprites/Menu/DefaultPlayButton.bmp", NULL);
    BITMAP* defaultExit = load_bitmap("Sprites/Menu/DefaultExitButton.bmp", NULL);
    BITMAP* background  = load_bitmap("Sprites/Menu/MenuBackground.bmp"   , NULL);
    MIDI* backgroundSound = load_midi("Sounds/Menu/lavender-town.mid");
    Button *playButton, *exitButton;

    unsigned int last_tick = 0;
    int option = 0, play = FALSE, exit_game = FALSE;

    tick = 0;
    // Não houve erro de alocação ou de carregamento da imagem
    if((onMousePlay != NULL) && (onMouseExit != NULL) && (defaultPlay != NULL) && (defaultExit != NULL) && (backgroundSound != NULL))
    {
        playButton = createButton(onMousePlay, defaultPlay, NULL, (SCREEN_W/2 - (onMousePlay->w)/2), SCREEN_H/2);
        exitButton = createButton(onMouseExit, defaultExit, NULL, (SCREEN_W/2 - (onMousePlay->w)/2), SCREEN_H/2 + onMousePlay->h);
        play_midi(backgroundSound, -1);

        if((playButton != NULL) && (exitButton != NULL)) // Não aguento mais colocar if para erro de alocação!!!
        {
            do
            {
                if(tick != last_tick) // Se se passaram 30ms
                {
                    draw_sprite(buffer, background, 0, 0); // tela de fundo a ser customizada
                    option = updateButtonState(playButton);

                    if(!option) // Se o usuário não tiver escolhido jogar
                    {
                        option = updateButtonState(exitButton) || key[KEY_ESC];

                        if(option)
                        {
                            option = EXIT;
                        }
                    }

                    else
                    {
                        option = PLAY;
                    }

                    if(key[KEY_ENTER])
                    {
                        option = PLAY;
                    }

                    drawButton(playButton, buffer);
                    drawButton(exitButton, buffer);
                    draw_sprite(buffer, mouse, mouse_x, mouse_y);
                    draw_sprite(screen, buffer, 0, 0);

                    last_tick = tick; // Salvar o tick atual
                }
            }
            while(!option);
        }
    }

    else
    {
        printf("ERRO DE CARREGAMENTO DE IMAGEM! \n\n\n");
        option = -1; // Retornar erro
    }

    if(play == TRUE)
    {
        option = PLAY;
    }


    else if(exit_game == TRUE)
    {
        option = EXIT;
    }

    // Des-inicialização
    destroyBitmap(&onMousePlay);
    destroyBitmap(&onMouseExit);
    destroyBitmap(&defaultPlay);
    destroyBitmap(&defaultPlay);
    destroyBitmap(&background);

    // Liberar os botões!!

    return option;
}

// Exibe o game over
void showGameOver(BITMAP *buffer, int score)
{
    double dx = 0.0, dy = 0.0;

    BITMAP *background = load_bitmap("Sprites/Menu/GameOver.bmp", NULL);
    unsigned int last_tick = tick = 0;

    if(background != NULL)
    {
        while(!key[KEY_ENTER])
        {
            if(tick != last_tick)
            {
                clear(buffer);
                blit(background, buffer, 0, 0, (SCREEN_W - background->w)/2, 0, SCREEN_W, SCREEN_H);
                //textprintf_centre_ex(buffer, digitalFont, rand() % 1100, rand() % 100, makecol(255, 255, 225500), -1, "Alanzinho Viadinho");
                textprintf_centre_ex(buffer, digitalFont, SCREEN_W/2, SCREEN_H/2, makecol(255, 255, 0), -1, "Pressione ENTER para continuar.");
                textprintf_centre_ex(buffer, digitalFont, SCREEN_W/2 + dx, SCREEN_H/2 + dy, makecol(255, 255, 0), -1, "Pressione ENTER para continuar.");
                //textprintf_centre_ex(buffer, digitalFont, SCREEN_W/2 + 2, SCREEN_H/2 + 2, makecol(255, 255, 0), -1, "Pressione ENTER para continuar.");
                blit(buffer,     screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

                last_tick = tick;

                dx += 0.01;
                dy += 0.01;
            }
        }

        while(key[KEY_ENTER])
        {
            if(tick != last_tick)
            {
                clear(buffer);
                blit(background, buffer, 0, 0, (SCREEN_W - background->w)/2, 0, SCREEN_W, SCREEN_H);
                //textprintf_centre_ex(buffer, digitalFont, rand() % 1100, rand() % 100, makecol(255, 255, 225500), -1, "Alanzinho Viadinho");
                textprintf_centre_ex(buffer, digitalFont, SCREEN_W/2, SCREEN_H/2, makecol(255, 255, 0), -1, "Pressione ENTER para continuar.");
                textprintf_centre_ex(buffer, digitalFont, SCREEN_W/2 + dx, SCREEN_H/2 + dy, makecol(255, 255, 0), -1, "Pressione ENTER para continuar.");
                blit(buffer,     screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

                last_tick = tick;
            }
        }
    }

    destroyBitmap(&background);
}

void showWinnerScreen(BITMAP *buffer)
{
    BITMAP *background = load_bitmap("Sprites/Menu/WinnerScreen.bmp", NULL);
    unsigned int last_tick = tick = 0;

    if(background != NULL)
    {
        while(!key[KEY_ESC])
        {
            if(tick != last_tick)
            {
                clear(buffer);
                blit(background, buffer, 0, 0, (SCREEN_W - background->w)/2, 0, SCREEN_W, SCREEN_H);
                blit(buffer,     screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

                last_tick = tick;
            }
        }
    }

    destroyBitmap(&background);
}
















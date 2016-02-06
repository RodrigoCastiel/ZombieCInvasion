#include <allegro.h>

#include "Presentation.h"
#include "Global.h"
#include "Game.h"

#define          EXIT    1
#define          PLAY    2
#define         ERROR    3
#define    FILE_ERROR    4

#define       WIN    0
#define GAME_OVER    1


/////////////////////////////////////////////////////////////////////////////
/* Funções de inicialização */

inline void init()
{
	allegro_init();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, TILE_SIZE * 16 + BAR_WIDTH, TILE_SIZE * 12, 0, 0);
	set_window_title("Zombie CInvasion 1.0");

    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	install_timer();
	install_keyboard();
	install_mouse();
}

inline void deinit()
{
	clear_keybuf();
	allegro_exit();
}

/////////////////////////////////////////////////////////////////////////////
/* Input Functions */

int main()
{
    srand(time(NULL));

    // Inicializacao do allegro e fontes
    init();
    install_int(updateTimer, 30);
    LOCK_FUNCTION(updateTimer);
    LOCK_VARIABLE(tick);
    digitalFont = load_font("Fonts/diablopcx.pcx", NULL, NULL);

    // Inicializacao
    int menuOption = 0;
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *defaultCursor = load_bitmap("Sprites/Cursor/DefaultCursor.bmp", NULL);

    // Laço do programa
    do
    {
        menuOption = menu(defaultCursor, buffer);

        clear(buffer);

        if(menuOption == PLAY)
        {
            textout_centre_ex(buffer, digitalFont, "Loading...", SCREEN_W / 2, SCREEN_H / 2, makecol(255, 255, 255), makecol(0, 0, 0));
            blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

            executeGame("entrada.in", buffer, defaultCursor);
        }

    } while(menuOption != EXIT);

    destroyBitmap(&defaultCursor);
    destroyBitmap(&buffer);
    destroy_font(digitalFont);

    deinit();

	return 0;
}
END_OF_MAIN()

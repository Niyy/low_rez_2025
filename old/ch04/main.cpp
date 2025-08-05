#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>

using std::cout;
using std::endl;

struct Player 
{
    float x;
    float y;
};

int* gFrameBuffer;
SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
SDL_Texture* gSDLTexture;
static int gDone;
const int WINDOW_WIDTH = 1920 / 2;
const int WINDOW_HEIGHT = 1080 / 2;
Player g_player;

bool update()
{
    SDL_Event e;

    if (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            return false;
        }
        if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE)
        {
            return false;
        }
        if(e.type == SDL_EVENT_MOUSE_MOTION)
        {
            g_player.x = e.motion.x; 
            g_player.y = e.motion.y;

            cout << "mouse: [" << g_player.x << "," << g_player.y << "]" << endl;
        }
    }

    char* pix;
    int pitch;

    SDL_LockTexture(gSDLTexture, NULL, (void**)&pix, &pitch);
    for (int i = 0, sp = 0, dp = 0; i < WINDOW_HEIGHT; i++, dp += WINDOW_WIDTH, sp += pitch)
        memcpy(pix + sp, gFrameBuffer + dp, WINDOW_WIDTH * 4);

    SDL_UnlockTexture(gSDLTexture);  
    SDL_RenderTexture(gSDLRenderer, gSDLTexture, NULL, NULL);
    SDL_RenderPresent(gSDLRenderer);
    SDL_Delay(1);
    return true;
}

void putpixel(int x, int y, int color) 
{
    if(
            x < 0 ||
            y < 0 ||
            x >= WINDOW_WIDTH ||
            y >= WINDOW_HEIGHT
      )
    {
        return;
    }

    gFrameBuffer[y * WINDOW_WIDTH + x] = color;
}


const unsigned char sprite[] =
{
    0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
    0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
    0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
    0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0
};

void drawsprite(int x, int y, unsigned int color)
{
    int i, j, c, yofs;
    yofs = y * WINDOW_WIDTH + x;
    for (i = 0, c = 0; i < 16; i++)
    {
        for (j = 0; j < 16; j++, c++)
        {
            if (sprite[c])
            {
                gFrameBuffer[yofs + j] = color;
            }
        }
        yofs += WINDOW_WIDTH;
    }
}

void init()
{
    for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++)
        gFrameBuffer[i] = 0xff000000;

    for (int i = 0; i < WINDOW_WIDTH; i++)
    {
        int p = (int)((sin((i + 3247) * 0.02) * 0.3 +
                    sin((i + 2347) * 0.04) * 0.1 +
                    sin((i + 4378) * 0.01) * 0.6) * 100 + (WINDOW_HEIGHT * 2 / 3));
        int pos = p * WINDOW_WIDTH + i;
        for (int j = p; j < WINDOW_HEIGHT; j++)
        {
            gFrameBuffer[pos] = 0xff007f00;
            pos += WINDOW_WIDTH;
        }
    }
}

void newsnow()
{
    for (int i = 0; i < 8; i++)
    {
        int color = 0xff;
        color = color << 8;
        color += rand() * 0xff;
        color = color << 8;
        color += rand() * 0xff;
        color = color << 8;
        color += rand() * 0xff;

        gFrameBuffer[rand() % (WINDOW_WIDTH - 2) + 1] = color;
    }
}

void snowfall()
{
    for (int j = WINDOW_HEIGHT - 2; j >= 0; j--)
    {
        int ypos = j * WINDOW_WIDTH;
        for (int i = 1; i < WINDOW_WIDTH - 1; i++)
        {
            float relative_x = g_player.x - i;
            float relative_y = g_player.y - j;
            float radius = 0;
            
            relative_x *= relative_x;
            relative_y *= relative_y;

            radius = relative_x + relative_y;
            bool no_move_on = 
            (
                gFrameBuffer[ypos + i] != 0xff000000 && gFrameBuffer[ypos + i] != 0xff007f00
            );

            if (no_move_on && radius >= 400)
            {
                int held_color = gFrameBuffer[ypos + i];

                if (gFrameBuffer[ypos + i + WINDOW_WIDTH] == 0xff000000)
                {
                    gFrameBuffer[ypos + i + WINDOW_WIDTH] = held_color;
                    gFrameBuffer[ypos + i] = 0xff000000;
                }
                else if (gFrameBuffer[ypos + i + WINDOW_WIDTH - 1] == 0xff000000)
                {
                    gFrameBuffer[ypos + i + WINDOW_WIDTH - 1] = held_color;
                    gFrameBuffer[ypos + i] = 0xff000000;
                }
                else if (gFrameBuffer[ypos + i + WINDOW_WIDTH + 1] == 0xff000000)
                {
                    gFrameBuffer[ypos + i + WINDOW_WIDTH + 1] = held_color;
                    gFrameBuffer[ypos + i] = 0xff000000;
                }
                else if (rand() > 0.75)
                {
                    if (gFrameBuffer[ypos + i + WINDOW_WIDTH - 2] == 0xff000000)
                    {
                        gFrameBuffer[ypos + i + WINDOW_WIDTH - 2] = held_color;
                        gFrameBuffer[ypos + i] = 0xff000000;
                    }
                    else if (gFrameBuffer[ypos + i + WINDOW_WIDTH + 2] == 0xff000000)
                    {
                        gFrameBuffer[ypos + i + WINDOW_WIDTH + 2] = held_color;
                        gFrameBuffer[ypos + i] = 0xff000000;
                    }
                }
            }
        }
    }
}

void render(Uint64 aTicks)
{
    newsnow();
    snowfall();
}

void loop()
{
    if (!update())
    {
        gDone = 1;
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
    }
    else
    {
        render(SDL_GetTicks());
    }
}

int main(int argc, char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
        return -1;
    }

    gFrameBuffer = new int[WINDOW_WIDTH * WINDOW_HEIGHT];
    gSDLWindow = SDL_CreateWindow("SDL3 window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (!gFrameBuffer || !gSDLWindow || !gSDLRenderer || !gSDLTexture)
        return -1;

    gDone = 0;

    init();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    while (!gDone)
    {
        loop();
    }
#endif

    SDL_DestroyTexture(gSDLTexture);
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();

    return 0;
}

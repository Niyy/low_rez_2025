#define SDL_MAIN_USE_CALLBACKS
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>

using std::cout;
using std::endl;
using std::map;


int g_width = 1920 / 2;
int g_height = 1080 / 2;
SDL_Window* g_window;
SDL_Renderer* g_renderer;


SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv);
SDL_AppResult SDL_AppIterate(void *appstate);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
void SDL_AppQuit(void *appstate, SDL_AppResult result);


SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) 
{
    cout << "hello there" << endl;
    
    g_window = SDL_CreateWindow("My Game", g_width, g_height, 0);

    if(!g_window)
    {
        cout << "ERROR::WINDOW_CREATION" << endl;
        return SDL_APP_FAILURE;
    }

    g_renderer = SDL_CreateRenderer(g_window, NULL);

    if(!g_renderer)
    {
        cout << "ERROR::RENDERER_CREATION" << endl;
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_RenderClear(g_renderer);
    SDL_RenderPresent(g_renderer);
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if(event->type == SDL_EVENT_KEY_UP && event->key.key == SDLK_ESCAPE)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
}

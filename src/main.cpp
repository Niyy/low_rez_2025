#define SDL_MAIN_USE_CALLBACKS
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>
//#include <SDL3/SDL_render.h>
#include "game.h"

using std::cout;
using std::endl;
using std::map;
using std::string;


int g_width = 1920 / 2;
int g_height = 1080 / 2;
Game g_game;
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
    int rect_count = 0;
    int offset_x = (g_width / 2) - (g_game.grid_step * (g_game.grid_w / 2));
    int offset_y = (g_height / 2) - (g_game.grid_step * (g_game.grid_h / 2));
    SDL_FRect l_rect;
    SDL_Time ticks;

    if(!SDL_GetCurrentTime(&ticks))
    {
        cout << "ERROR::COULD_NOT_GET_TIME" << endl;
        return SDL_APP_FAILURE;
    }
    
    {
        // handle mouse rect
        l_rect.w = 10;
        l_rect.h = 10;

        SDL_GetMouseState(&l_rect.x, &l_rect.y);

        l_rect.x -= (float)(l_rect.w * 0.5f);
        l_rect.y -= (float)(l_rect.h * 0.5f);
        rect_count++;
    }

    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);
    SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);

    for(int x_index = 0; x_index < g_game.grid_h; x_index++)
    {            
        for(int y_index = 0; y_index < g_game.grid_w; y_index++)
        {
            bool bound_x = false;
            bool bound_y = false;
            SDL_FRect cur_rect;

            cur_rect.x = (x_index * g_game.grid_step) + offset_x;
            cur_rect.y = (y_index * g_game.grid_step) + offset_y;
            cur_rect.w = g_game.grid_step;
            cur_rect.h = g_game.grid_step;

            bound_x = 
                cur_rect.x < l_rect.x && 
                (cur_rect.x + g_game.grid_step) >= l_rect.x;
            bound_y = 
                cur_rect.y < l_rect.y && 
                (cur_rect.y + g_game.grid_step) >= l_rect.y;

            if(bound_x && bound_y)
            {
                cur_rect.y -= 8;
            }

            SDL_RenderRect(g_renderer, &cur_rect);
        }
    }

    SDL_RenderRect(g_renderer, &l_rect);

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

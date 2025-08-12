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
#include <SDL3_image/SDL_image.h>
#include "game.h"

using std::cout;
using std::endl;
using std::map;
using std::string;


const int TEXTURE_SIZE = 64;
const int g_width = 1920 / 2;
const int g_height = 1080 / 2;
static SDL_Texture *texture = NULL;
static SDL_Texture *texture_01 = NULL;
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

    texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_SIZE, TEXTURE_SIZE);
    if (!texture) {
        SDL_Log("Couldn't create streaming texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    texture_01 = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, TEXTURE_SIZE, TEXTURE_SIZE);
    if (!texture_01) {
        SDL_Log("Couldn't create streaming texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    // Set the texture scale mode to nearest
    if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
        // Handle error
        SDL_Log("Failed to set texture scale mode: %s", SDL_GetError());
    }
    if (!SDL_SetTextureScaleMode(texture_01, SDL_SCALEMODE_NEAREST)) {
        // Handle error
        SDL_Log("Failed to set texture scale mode: %s", SDL_GetError());
    }

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate)
{
    int offset_x = (g_width / 2) - (g_game.grid_step * (g_game.grid_w / 2));
    int offset_y = (g_height / 2) - (g_game.grid_step * (g_game.grid_h / 2));
    SDL_FRect l_rect;
    SDL_Time ticks;
    SDL_FRect dst_rect, src_rect;
    const Uint64 now = SDL_GetTicks();
    SDL_Surface *surface = NULL;
    SDL_Texture *texture_02 = IMG_LoadTexture(g_renderer, "assets/cursor.png");
    SDL_SetTextureScaleMode(texture_02, SDL_SCALEMODE_NEAREST);

    /* we'll have some color move around over a few seconds. */
    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;
    SDL_GetMouseState(&l_rect.x, &l_rect.y);

    /* To update a streaming texture, you need to lock it first. This gets you access to the pixels.
       Note that this is considered a _write-only_ operation: the buffer you get from locking
       might not acutally have the existing contents of the texture, and you have to write to every
       locked pixel! */

    /* You can use SDL_LockTexture() to get an array of raw pixels, but we're going to use
       SDL_LockTextureToSurface() here, because it wraps that array in a temporary SDL_Surface,
       letting us use the surface drawing functions instead of lighting up individual pixels. */
    if (SDL_LockTextureToSurface(texture, NULL, &surface)) {
        SDL_Rect r;
        SDL_FillSurfaceRect(surface, NULL, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 255, 255, 255));
        r.w = TEXTURE_SIZE;
        r.h = TEXTURE_SIZE / 10;
        r.x = 0;
        r.y = (int) (((float) (TEXTURE_SIZE - r.h)) * ((scale + 1.0f) / 2.0f));
        SDL_FillSurfaceRect(surface, &r, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 0, 255, 0));  /* make a strip of the surface green */
        SDL_UnlockTexture(texture);  /* upload the changes (and frees the temporary surface)! */
    }
    if (SDL_LockTextureToSurface(texture_01, NULL, &surface)) {
        SDL_Rect r;
        r.w = 10;
        r.h = 10;
        r.x = 0;
        r.y = 0;
        SDL_FillSurfaceRect(surface, &r, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 255, 0, 0));
        SDL_UnlockTexture(texture_01);  /* upload the changes (and frees the temporary surface)! */
    }

    

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(g_renderer, 66, 66, 66, SDL_ALPHA_OPAQUE);  /* grey, full alpha */
    SDL_RenderClear(g_renderer);  /* start with a blank canvas. */

    /* Just draw the static texture a few times. You can think of it like a
       stamp, there isn't a limit to the number of times you can draw with it. */

    /* Center this one. It'll draw the latest version of the texture we drew while it was locked. */
    dst_rect.x = g_width * 0.25f;
    dst_rect.y = 5;
    dst_rect.w = dst_rect.h = g_height - 10;
    SDL_RenderTexture(g_renderer, texture, NULL, &dst_rect);
    SDL_RenderTexture(g_renderer, texture_01, NULL, &dst_rect);
    dst_rect.x = ((int)((l_rect.x - dst_rect.w) / (dst_rect.w / 64)));
    dst_rect.y = ((int)(l_rect.y / dst_rect.h));
    cout << dst_rect.x << endl;
    dst_rect.w = dst_rect.h = (g_height - 10) / 8;
    SDL_RenderTexture(g_renderer, texture_02, NULL, &dst_rect);
//    if(!SDL_GetCurrentTime(&ticks))
//    {
//        cout << "ERROR::COULD_NOT_GET_TIME" << endl;
//        return SDL_APP_FAILURE;
//    }
//    
//    {
//        // handle mouse rect
//        l_rect.w = 10;
//        l_rect.h = 10;
//
//        SDL_GetMouseState(&l_rect.x, &l_rect.y);
//
//        l_rect.x -= (float)(l_rect.w * 0.5f);
//        l_rect.y -= (float)(l_rect.h * 0.5f);
//    }
//
//    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
//    SDL_RenderClear(g_renderer);
//    SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);
//
//    for(int x_index = 0; x_index < g_game.grid_h; x_index++)
//    {            
//        for(int y_index = 0; y_index < g_game.grid_w; y_index++)
//        {
//            bool bound_x = false;
//            bool bound_y = false;
//            SDL_FRect cur_rect;
//
//            cur_rect.x = (x_index * g_game.grid_step) + offset_x;
//            cur_rect.y = (y_index * g_game.grid_step) + offset_y;
//            cur_rect.w = g_game.grid_step;
//            cur_rect.h = g_game.grid_step;
//
//            bound_x = 
//                cur_rect.x < l_rect.x && 
//                (cur_rect.x + g_game.grid_step) >= l_rect.x;
//            bound_y = 
//                cur_rect.y < l_rect.y && 
//                (cur_rect.y + g_game.grid_step) >= l_rect.y;
//
//            if(bound_x && bound_y)
//            {
//                cur_rect.y -= 8;
//            }
//
//            SDL_RenderRect(g_renderer, &cur_rect);
//        }
//    }
//
//    SDL_RenderRect(g_renderer, &l_rect);

    SDL_RenderPresent(g_renderer);
    SDL_DestroyTexture(texture_02);
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
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_01);
}

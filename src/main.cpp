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
#include "game.hpp"

using std::cout;
using std::endl;
using std::map;
using std::string;

// Definitions
const int TEXTURE_SIZE = 64;
const int g_width = 1920 / 2;
const int g_height = 1080 / 2;
const int g_viewport = 448;
const int g_viewport_step = g_viewport / 64;
static SDL_Texture *texture = NULL;
static SDL_Texture *texture_01 = NULL;
Game g_game;
SDL_Window* g_window;
SDL_Renderer* g_renderer;
static map<string, SDL_Texture*> g_textures;


SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv);
SDL_AppResult SDL_AppIterate(void *appstate);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
void SDL_AppQuit(void *appstate, SDL_AppResult result);
void load_textures();
void destroy_textures();
void screen_to_world(float* x, float* y, SDL_FRect* viewport_rect);


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

    load_textures();

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
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture_02 = IMG_LoadTexture(g_renderer, "assets/cursor.png");
    SDL_Texture *dst_texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TEXTURE_SIZE * 2, TEXTURE_SIZE * 2);    
    SDL_SetTextureScaleMode(texture_02, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(dst_texture, SDL_SCALEMODE_NEAREST);

    /* we'll have some color move around over a few seconds. */
    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

    SDL_GetMouseState(&l_rect.x, &l_rect.y);
    screen_to_world(&l_rect.x, &l_rect.y, &dst_rect);

    /* To update a streaming texture, you need to lock it first. This gets you access to the pixels.
       Note that this is considered a _write-only_ operation: the buffer you get from locking
       might not acutally have the existing contents of the texture, and you have to write to every
       locked pixel! */

    /* You can use SDL_LockTexture() to get an array of raw pixels, but we're going to use
       SDL_LockTextureToSurface() here, because it wraps that array in a temporary SDL_Surface,
       letting us use the surface drawing functions instead of lighting up individual pixels. */
    if (SDL_LockTextureToSurface(texture, NULL, &surface)) {
        SDL_Rect r;
        SDL_FillSurfaceRect(surface, NULL, SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL, 0, 0, 0));
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
    if(!SDL_SetRenderTarget(g_renderer, dst_texture))
    {
        cout << "ERROR::SET_RENDER_TARGET::" << SDL_GetError() << endl;
    }
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = dst_rect.h = TEXTURE_SIZE;
    SDL_RenderTexture(g_renderer, texture, NULL, &dst_rect);
    SDL_RenderTexture(g_renderer, texture_01, NULL, &dst_rect);
    dst_rect.x = l_rect.x - 3;
    dst_rect.y = l_rect.y - 3;
    dst_rect.w = dst_rect.h = g_textures["cursor"]->w;
    SDL_RenderTexture(g_renderer, g_textures["cursor"], NULL, &dst_rect);
//    dst_rect.x = ((int)((l_rect.x - dst_rect.x) / g_viewport) * g_viewport) + dst_rect.x;
//    dst_rect.y = ((int)((l_rect.y - dst_rect.y) / g_viewport) * g_viewport) + dst_rect.y;
//    cout << dst_rect.x << endl;
//    dst_rect.w = dst_rect.h = (g_height - 10) / 8;
//    SDL_RenderTexture(g_renderer, texture_02, NULL, &dst_rect);

    dst_rect.x = dst_rect.y = 0;
    dst_rect.w = dst_rect.h = g_viewport;
    src_rect.x = 0;
    src_rect.w = src_rect.h = TEXTURE_SIZE;
    SDL_SetRenderTarget(g_renderer, NULL);
    SDL_RenderTexture(g_renderer, dst_texture, &src_rect, &dst_rect);

    SDL_RenderPresent(g_renderer);
    SDL_DestroyTexture(texture_02);
    SDL_DestroyTexture(dst_texture);
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
    cout << "cleaning up" << endl;
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture_01);
    destroy_textures();
}


void screen_to_world(float* x, float* y, SDL_FRect* viewport_rect)
{
    *x = ((int)((*x - viewport_rect->x) / g_viewport_step));
    *y = ((int)((*y - viewport_rect->y) / g_viewport_step));
}


void load_textures()
{
    g_textures["cursor"] = IMG_LoadTexture(g_renderer, "assets/cursor.png");
    g_textures["crate"] = IMG_LoadTexture(g_renderer, "assets/crate.png");
    g_textures["actor"] = IMG_LoadTexture(g_renderer, "assets/actor.png");
}


void destroy_textures()
{
    map<string, SDL_Texture*>::iterator it;
    for (it = g_textures.begin(); it != g_textures.end(); ++it)
    {
        SDL_DestroyTexture(it->second);
    }
}

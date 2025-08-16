#define SDL_MAIN_USE_CALLBACKS
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3_image/SDL_image.h>
#include "game.hpp"
#include "game_states.hpp"
#include "map.hpp"

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::tuple;

struct Mouse
{
    float raw_x, 
          raw_y;
    int x,
        y;
    bool held;
    Uint32 event;
};

// Definitions p.s. I know this should be wrapped in a class, but it is a game jam.
const int TEXTURE_SIZE = 64;
const int g_width = 1920 / 2;
const int g_height = 1080 / 2;
const int g_viewport = 512;
const int g_viewport_step = g_viewport / 64;
const int g_tile = 1;
const int g_pix_per_pix = 8; 
static SDL_Texture *texture = NULL;
static SDL_Texture *texture_01 = NULL;
Game g_game;
SDL_Window* g_window;
SDL_Renderer* g_renderer;
static map<string, SDL_Texture*> g_textures;
static map<tuple<int, int>, Object> g_objects;
static int g_map_tile[2];
static SDL_FRect g_viewport_rect;
static SDL_FRect g_ui_rect;
static Game_States g_state = BUILD;
static Mouse g_mouse;
static int id_file = 0;
static low_rez::Map g_map;


SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv);
SDL_AppResult SDL_AppIterate(void *appstate);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
void SDL_AppQuit(void *appstate, SDL_AppResult result);
void load_textures();
void destroy_textures();
void screen_to_world(float* x, float* y, SDL_FRect* viewport_rect);
void on_mouse_click(const int &x, const int &y);
void game_logic();


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

    g_ui_rect.h =
    g_ui_rect.w = 
    g_viewport_rect.w = 
    g_viewport_rect.h = TEXTURE_SIZE;
    g_map_tile[0] = TEXTURE_SIZE * 2;
    g_map_tile[1] = TEXTURE_SIZE * 2;

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_Time ticks;
    SDL_FRect dst_rect, src_rect;
    const Uint64 now = SDL_GetTicks();
    SDL_Texture *dst_texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, g_map_tile[0], g_map_tile[1]);    
    SDL_SetTextureScaleMode(dst_texture, SDL_SCALEMODE_NEAREST);

    /* we'll have some color move around over a few seconds. */
    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

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
    SDL_SetRenderDrawColor(g_renderer, 66, 66, 66, SDL_ALPHA_OPAQUE);  /* grey, full alpha */
    SDL_RenderClear(g_renderer);
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = dst_rect.h = TEXTURE_SIZE;
    SDL_RenderTexture(g_renderer, texture, NULL, &dst_rect);
    SDL_RenderTexture(g_renderer, texture_01, NULL, &dst_rect);

    for(map<tuple<int, int>, Object>::iterator objects_it = g_map.objects().begin(); objects_it != g_map.objects().end(); objects_it++)
    {
        Object obj = objects_it->second;
        SDL_FRect temp_rect = obj.rect();
        
        SDL_RenderTexture(g_renderer, obj.get_texture(), NULL, &temp_rect); 
    }

    dst_rect.x = g_mouse.x - 1;
    dst_rect.y = g_mouse.y - 1;
    dst_rect.w = dst_rect.h = g_textures["cursor"]->w;
    SDL_RenderTexture(g_renderer, g_textures["cursor"], NULL, &dst_rect);

    dst_rect.x = 0;//g_viewport_rect.x;
    dst_rect.y = 0;//g_viewport_rect.y;
    dst_rect.w = g_textures["ui_commandbar"]->w;
    dst_rect.h = g_textures["ui_commandbar"]->h;
    SDL_RenderTexture(g_renderer, g_textures["ui_commandbar"], NULL, &dst_rect); 

    // Render the render target ooooo!
    dst_rect.x = dst_rect.y = 0;
    dst_rect.w = dst_rect.h = g_viewport;
    src_rect.x = 0;
    src_rect.w = src_rect.h = TEXTURE_SIZE;
    SDL_SetRenderTarget(g_renderer, NULL);
    SDL_RenderTexture(g_renderer, dst_texture, &g_viewport_rect, &dst_rect);

    SDL_RenderPresent(g_renderer);
    SDL_DestroyTexture(dst_texture);

    game_logic();

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    float x;
    float y;
    int change_x = 0;
    int change_y = 0;
    int max_map[2] = { g_map_tile[0] / 2, g_map_tile[1] / 2 };

    if(event->type == SDL_EVENT_KEY_UP && event->key.key == SDLK_ESCAPE)
    {
        return SDL_APP_SUCCESS;
    }
    if(event->type == SDL_EVENT_KEY_UP) 
    {
        if(event->key.key == SDLK_W)
        {
            if(g_viewport_rect.y - 8 >= 0) 
            { 
                g_viewport_rect.y -= 8; 
                change_y = -8;
            }
            cout << "viewport_rect: [" << g_viewport_rect.x << ", " << g_viewport_rect.y << ", " << g_viewport_rect.w << ", " << g_viewport_rect.h << "]" << endl;

        }
        else if(event->key.key == SDLK_S)
        {
            if(g_viewport_rect.y + 8 <= max_map[1]) 
            { 
                g_viewport_rect.y += 8; 
                change_y = 8;
            }
            cout << "viewport_rect: [" << g_viewport_rect.x << ", " << g_viewport_rect.y << ", " << g_viewport_rect.w << ", " << g_viewport_rect.h << "]" << endl;

        }
        if(event->key.key == SDLK_A)
        {
            if(g_viewport_rect.x - 8 >= 0) 
            { 
                g_viewport_rect.x -= 8; 
                change_x = -8;
            }
            cout << "viewport_rect: [" << g_viewport_rect.x << ", " << g_viewport_rect.y << ", " << g_viewport_rect.w << ", " << g_viewport_rect.h << "]" << endl;

        }
        else if(event->key.key == SDLK_D)
        {
            if(g_viewport_rect.x + 8 <= max_map[0]) 
            { 
                g_viewport_rect.x += 8; 
                change_x = 8;
            }
            cout << "viewport_rect: [" << g_viewport_rect.x << ", " << g_viewport_rect.y << ", " << g_viewport_rect.w << ", " << g_viewport_rect.h << "]" << endl;
        }


        x = g_mouse.raw_x += change_x;
        y = g_mouse.raw_y += change_y;

//        cout << "before t: "<< x << "," << y << endl;
        screen_to_world(&x, &y, &g_viewport_rect);
//        cout << "after t: "<< x << "," << y << endl;

        g_mouse.x = (int)x;
        g_mouse.y = (int)y;
    }
    
    if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        x = g_mouse.raw_x = event->button.x;
        y = g_mouse.raw_y = event->button.y;
        g_mouse.held = true;
        g_mouse.event = event->type;
//        cout << "before t: "<< x << "," << y << endl;
        screen_to_world(&x, &y, &g_viewport_rect);
//        cout << "after t: "<< x << "," << y << endl;

        g_mouse.x = (int)x;
        g_mouse.y = (int)y;

        on_mouse_click(g_mouse.x, g_mouse.y);
    }
    if(event->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        x = g_mouse.raw_x = event->button.x;
        y = g_mouse.raw_y = event->button.y;
        g_mouse.held = false;
        g_mouse.event = event->type;
//        cout << "before t: "<< x << "," << y << endl;
        screen_to_world(&x, &y, &g_viewport_rect);
//        cout << "after t: "<< x << "," << y << endl;

        g_mouse.x = (int)x;
        g_mouse.y = (int)y;    
    }
    if(event->type == SDL_EVENT_MOUSE_MOTION)
    {
        x = g_mouse.raw_x = event->button.x;
        y = g_mouse.raw_y = event->button.y;
//        cout << "before t: "<< x << "," << y << endl;
        screen_to_world(&x, &y, &g_viewport_rect);
//        cout << "after t: "<< x << "," << y << endl;

        g_mouse.x = (int)x;
        g_mouse.y = (int)y;
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
    *x = (((int)((*x) / g_pix_per_pix))) + viewport_rect->x;
    *y = (((int)((*y) / g_pix_per_pix))) + viewport_rect->y;
}


void load_textures()
{
    g_textures["cursor"] = IMG_LoadTexture(g_renderer, "assets/cursor.png");
    g_textures["crate"] = IMG_LoadTexture(g_renderer, "assets/crate.png");
    g_textures["actor"] = IMG_LoadTexture(g_renderer, "assets/actor.png");
    g_textures["ui_commandbar"] = IMG_LoadTexture(g_renderer, "assets/commandbar.png");
    g_textures["wall_0000"] = IMG_LoadTexture(g_renderer, "assets/wall_0000.png");
}


void destroy_textures()
{
    map<string, SDL_Texture*>::iterator it;
    for (it = g_textures.begin(); it != g_textures.end(); ++it)
    {
        SDL_DestroyTexture(it->second);
    }
}


void on_mouse_click(const int &x, const int &y)
{
    if(g_state == BUILD && g_mouse.event == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        Object new_object;
        SDL_FRect new_rect;
        tuple<int, int> id(x, y);

        new_rect.x = x;
        new_rect.y = y;
        new_rect.w = g_textures["wall_0000"]->w;
        new_rect.h = g_textures["wall_0000"]->h;
        new_object.set_rect(new_rect);
        new_object.set_texture(g_textures["wall_0000"]);

        cout << "rect adding: " << new_rect.x << " " << new_rect.y << " " << new_rect.w << " " << new_rect.h << endl;

        if(!g_map.place(new_object))
        {
            cout << "can't place that!" << endl;
        }
    }
}


void game_logic()
{
    if(g_mouse.held)
    {
//        on_mouse_click(g_mouse.x, g_mouse.y);
    }
}

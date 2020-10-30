#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../Entity/Entity.h"
#include "../Blit/Blit.h"
#include "../Textures/Textures.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define BULLET_SPEED 32
#define BODY_WIDTH 126
#define BODY_HEIGHT 108
#define PI 3.1415926535899323846
#define LASER_LIMIT 60
#define ASTEROID_LIMIT 5

time_t t;

void addLaser(player *player, laser *laser, double angle, int offset)
{

    int radius = 42;

        laser->dx = BULLET_SPEED*(float)(cos(angle*PI/180));
        laser->dy = BULLET_SPEED*(float)(sin(angle*PI/180));
        angle += offset;
        laser->x = (int)(player->x + BODY_WIDTH/2 + radius * cos(angle*PI/180));
        laser->y = (int)(player->y + BODY_HEIGHT/2 + radius * sin(angle*PI/180));
       // laser->angle = (float)(420 + atan2(player->x, player->y) * (180/PI));
        laser->angle = player->angle;

}

int do_CollideAsteroidShip(player *player, asteroid *asteroid)
{   
    asteroid->center.x = asteroid->x; //+ (32 * asteroid->size) /2;
    asteroid->center.y = asteroid->y; //+ (32 * asteroid->size) /2;
    player->center.x = player->x +BODY_WIDTH/2;
    player->center.y = player->y +BODY_HEIGHT/2;
    
    if (player->radius + asteroid->radius>= sqrt(pow(player->center.x-asteroid->center.x, 2) + pow(player->center.y-asteroid->center.y, 2)))
    {
        
        return 1;             
    }
    else 
    {
        return 0;
    }
}

bool do_CollideLaserAsteroid(laser *laser, asteroid *asteroid)
{
    asteroid->center.x = asteroid->x + (32 * asteroid->size) /2;
    asteroid->center.y = asteroid->y + (32 * asteroid->size) /2;
    laser->center.x = laser->x +12/2;
    laser->center.y = laser->y +12/2;

    if (laser->radius + asteroid->radius >= sqrt(pow(laser->center.x-asteroid->center.x, 2) + pow(laser->center.y-asteroid->center.y, 2)))
    {
        return true;
    }
    else 
    {
        return false;
    }

}

bool laserAlive(laser *laser)
{
    if(laser->dx || laser->dy)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void resetGamestate(player *player, laser *laser, asteroid *asteroid)
{
    srand((unsigned)time(NULL));
    for (int j = 0; j < LASER_LIMIT; j++)
    {
        laser[j].x = -70;
        laser[j].y = -70; 
        laser[j].dx = 0;
        laser[j].dy = 0; 
        laser[j].angle = 0;
        laser[j].shot = 0;

        for (int i = 0; i < ASTEROID_LIMIT; i++)
        {
            asteroid[i].x = 0;
            asteroid[i].y = 0;
            asteroid[i].angle = 0;
            asteroid[i].size = (rand() % 5 +1);
            asteroid[i].delay = 120;
        }
    }

    player->x = SCREEN_WIDTH/2;
    player->y = SCREEN_HEIGHT/2;
    player->angle = 90;
    player->speed_x = 0;
    player->speed_y = 0;;
    player->shooting = 0;;
}

void asteroidRadius(asteroid *asteroid, SDL_Renderer *renderer, GameState *gameState)
{
        if (asteroid->size == 1 )
        {
            blit_angled(gameState->asteroid, asteroid->x, asteroid->y, (float)asteroid->angle, 1, renderer);
            
            asteroid->radius = (32/2) * 1;
        }
        if(asteroid->size == 2 )
        {
            blit_angled(gameState->asteroid, asteroid->x, asteroid->y, (float)asteroid->angle, 2, renderer);
           asteroid->radius = (32/2) * 2;
           
        }
        if(asteroid->size == 3 )
        {
            blit_angled(gameState->asteroid, asteroid->x, asteroid->y, (float)asteroid->angle, 3, renderer);
           asteroid->radius = (32/2) * 3;
           
        }
        if(asteroid->size == 4 )
        {
            blit_angled(gameState->asteroid, asteroid->x, asteroid->y, (float)asteroid->angle, 4, renderer);
           asteroid->radius = (32/2) * 4;
        }
        if(asteroid->size == 5 )
        {
            blit_angled(gameState->asteroid, asteroid->x, asteroid->y, (float)asteroid->angle, 5, renderer);
           asteroid->radius = (32/2) * 5;
           
        } 
}

void startExplosion (explosion *explosion, SDL_Renderer *renderer, SDL_Rect *explosion_rect)
{
   // printf("In explosie functie\n");
    SDL_Point explosion_center;

    explosion_rect->x = 0;
    explosion_rect->y = 0;
    SDL_QueryTexture(explosion->texture, NULL, NULL, &explosion_rect->w, &explosion_rect->h);
    explosion_rect->w /= 12;
    explosion_center.x = explosion_rect->w/2;
    explosion_center.y = explosion_rect->h/2;	
    // explosion[i].x = SCREEN_WIDTH/2;//(int)(asteroids[i].x - explosion_center.x/2);
    // explosion[i].y = SCREEN_HEIGHT/2;//(int)(asteroids[i].y - explosion_center.y/2);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosion->texture, SDL_BLENDMODE_ADD);
    SDL_SetTextureColorMod(explosion->texture, 255, 255, 255);//, (Uint8)(255-255/frame));
    SDL_SetTextureAlphaMod(explosion->texture, (Uint8)(255-(255/12)* explosion->frame));
    render(renderer, explosion->texture, explosion->x, explosion->y, 0, explosion_center, SDL_FLIP_NONE, explosion_rect, 1);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    explosion->frame = 0;
}
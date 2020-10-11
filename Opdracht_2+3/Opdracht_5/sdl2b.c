/*
Copyright (C) 2020
Sander Gieling
Inholland University of Applied Sciences at Alkmaar, the Netherlands
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

// Artwork bibliography
// --------------------
// Reticle taken from here:
// https://flyclipart.com/download-png#printable-crosshair-targets-clip-art-clipart-675613.png
// Blorp taken from here:
// https://opengameart.org/content/animated-top-down-survivor-player
// Desert floor tiling taken from here:
// https://www.flickr.com/photos/maleny_steve/8899498324/in/photostream/

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // for IMG_Init and IMG_LoadTexture
#include <math.h> // for atan() function
//Include mah modules:
#include "Mod/Blit/Blit.h"
#include "Mod/Entity/Entity.h"
#include "Mod/Input/Input.h"
#include "Mod/Textures/Textures.h"
#include "Mod/Update/Update.h"
//#include "modules/renderer/renderer.h"

#define FPS							60
#define SCREEN_WIDTH				1080
#define SCREEN_HEIGHT				720


void proper_shutdown(void);
void setFps(Uint32 starting_tick);

SDL_Window *window; //= NULL;
SDL_Renderer *renderer; //= NULL;

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	float prev_zombie_angle;	
	flash flash;
	bullet bullet;
	player blorp;
	zombie zombie;
	zombie.x = SCREEN_HEIGHT/3;
	zombie.y = SCREEN_WIDTH/3;
	blorp.x = SCREEN_HEIGHT / 2;
	blorp.y = SCREEN_WIDTH / 2;
	blorp.speed_x = 0.0;
	blorp.speed_y = 0.0;
	blorp.up = UP;
	blorp.down = UP;
	blorp.left = UP;
	blorp.right = UP;
	blorp.angle = 0.0;
	blorp.state = IDLE;
	blorp.shooting = 0;
	flash.r = 255;
	flash.g = 255;
	flash.b = 255;
	flash.a = 255;
	//player blorp = {(SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f, 0.0f, UP, UP, UP, UP, 0.0, NULL, NULL, IDLE};
	// New: Mouse is a type representing a struct containing x and y
	// coords of mouse pointer:
	mouse mousepointer;
	int frame = 0, zombie_frame;
	unsigned int window_flags = 0;
	unsigned int renderer_flags = SDL_RENDERER_ACCELERATED;
	Uint32 starting_tick;

	//init_renderer(renderer, window, renderer_flags, window_flags);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow("Blorp is going to F U UP!",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
		SCREEN_HEIGHT, window_flags);
	if (window == NULL)
	{
		printf("Failed to create window -- Error: %s\n",
			SDL_GetError());
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, renderer_flags);
	if (renderer == NULL)
	{
		printf("Failed to create renderer -- Error: %s\n",
			SDL_GetError());
		exit(1);
	}

	IMG_Init(IMG_INIT_PNG);
	//load all the textures
	init_all_textures(&flash, &bullet, &mousepointer, &zombie, &blorp, renderer);
	// New: Turn system mouse cursor off:
	SDL_ShowCursor(0);

	while (1)
	{
		starting_tick = SDL_GetTicks();

		SDL_SetRenderDrawColor(renderer, 120, 144, 156, 255);
		SDL_RenderClear(renderer);

		// # Sensor Reading #
		// Also takes the mouse movement into account:
		process_input(&blorp, &mousepointer);

		// # Applying Game Logic #
		// Also takes the mouse movement into account:
		update_player(&blorp, &mousepointer, &bullet, &flash);
		if (blorp.shooting)
		{
			blorp.shooting = 0;
			frame = 0;
		}
		prev_zombie_angle = zombie.angle;
		get_angle_zombie(&blorp, &zombie);
		if (prev_zombie_angle != zombie.angle)
		{
			zombie.state = WALKING_E;
		}
		if (prev_zombie_angle == zombie.angle)
		{
			zombie.state = IDLE_E;
		}
		if (zombie.state == WALKING_E)
		{
			if (prev_zombie_angle < zombie.angle)
			{
				blit_angled(zombie.txtr_body[zombie.state][zombie_frame/3], zombie.x, zombie.y, zombie.angle, 1, renderer);
			}
			if (prev_zombie_angle > zombie.angle)
			{
				blit_angled(zombie.txtr_body[zombie.state][16-(zombie_frame/3)], zombie.x, zombie.y, zombie.angle, 1, renderer);
			}
		}
		if (zombie.state == IDLE_E)
		{		
				blit_angled(zombie.txtr_body[zombie.state][zombie_frame/3], zombie.x, zombie.y, zombie.angle, 1, renderer);
		}
		if (blorp.up == DOWN || blorp.down == DOWN || blorp.left == DOWN || blorp.right == DOWN)
		{
			if (blorp.state == SHOOTING)
			{
				blorp.state = SHOOTING_WHILE_WALKING;
			}
			if (blorp.state == IDLE)
			{
				blorp.state = WALKING;
			}
		}	
		if (blorp.up == UP && blorp.down == UP && blorp.left == UP && blorp.right == UP)
		{
			if (blorp.state == SHOOTING_WHILE_WALKING /* && frame == 0*/)
			{
				blorp.state = SHOOTING;
			}
			if (blorp.state == WALKING/* && frame == 0*/)
			{
				blorp.state = IDLE;
				//zombie.state = IDLE_E;
			}
		}
		// # Actuator Output Buffering #
		// Also takes texture rotation into account:-
		if (blorp.state == WALKING || blorp.state == SHOOTING_WHILE_WALKING) 
		{
			blit_angled(blorp.txtr_feet[blorp.state][frame], blorp.x, blorp.y, blorp.angle, 1, renderer);
		} 
		if (blorp.state == 0 || blorp.state == SHOOTING) 
		{
			blit_angled(blorp.txtr_feet[0][0], blorp.x, blorp.y, blorp.angle, 1, renderer);
		}
		if (blorp.state == SHOOTING || blorp.state == SHOOTING_WHILE_WALKING)
		{
			if (frame < 3)
			{
				//printf("Gucci fam, frame = %d\n", frame);
				blit_angled(blorp.txtr_body[blorp.state][frame], blorp.x, blorp.y, blorp.angle, 1, renderer);
				flash.a = (Uint8)((255/3)*(4-frame));
				draw_flash(&flash, renderer);
			}

				if (blorp.state == SHOOTING && frame == 3)
				{
					blorp.state = IDLE;
				}
				if (blorp.state == SHOOTING_WHILE_WALKING && frame == 3)
				{
					blorp.state = WALKING;
				}
		}
		if (blorp.state == IDLE || blorp.state == WALKING)
		{	
			blit_angled(blorp.txtr_body[blorp.state][frame], blorp.x, blorp.y, blorp.angle, 1, renderer);
		}
		if (bullet.life)
		{
			blit_angled(bullet.txtr_kogel, bullet.x, bullet.y, bullet.angle, 1, renderer);
			--bullet.life;
		}
		// New: Redraw mouse pointer centered on the mouse coordinates:
		blit(mousepointer.txtr_reticle, mousepointer.x,
			mousepointer.y, 1, renderer);	
		zombie_frame++;
		++frame;
		SDL_RenderPresent(renderer);
		SDL_Delay(16);
		if (zombie_frame == 48)
		{
			zombie_frame = 0;
		}	
		if (frame == 20)
		{
			frame = 0;
		}
		setFps(starting_tick);
	}
	
	return 0;
}
void proper_shutdown(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void setFps(Uint32 starting_tick)
{
    if( (1000 / FPS) > (SDL_GetTicks() - starting_tick))
    {
        SDL_Delay(1000/FPS - (SDL_GetTicks() - starting_tick));
        printf ("FPS: %d\n", 1000/(SDL_GetTicks() - starting_tick));

    }
	else
	{
		printf ("FPS: %d\n", 1000/(SDL_GetTicks() - starting_tick));
	}
	
}
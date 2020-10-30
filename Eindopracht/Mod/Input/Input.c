#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "../Entity/Entity.h"
#include "../Sound/Sound.h"
#include "../Renderer/Renderer.h"

#define ACCELERATION 1
#define PI 3.1415926535899323846

double calc_x_Speed(double rotation)
{
   // printf("COS_x : %f ", (ACCELERATION * cos(rotation * (PI / 180))));
    return (ACCELERATION * sin(rotation * (PI / 180)));
        
}

double calc_y_Speed(double rotation)
{
    //printf("SIN_y : %f\n", (ACCELERATION *  sin(rotation * (PI / 180))));
    return (ACCELERATION * cos(rotation * (PI / 180))); 
        
}

void proper_shutdown(SDL_Window *window, SDL_Renderer *renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    Mix_Quit();
	SDL_Quit();
    TTF_Quit();

}

void move(player *Player, thruster *Thruster)
{
    if(Player->left)
    {
        Player->angle-= 4;
        Thruster->angle-= 4;

         if(Player->angle > 360)
         {
             Player->angle = SDL_fmod(Player->angle, 360);
         }

        if(Player->angle < 0)
        {
            Player->angle = Player->angle + 360;
        }

        if(Thruster->angle > 360)
         {
             Thruster->angle = SDL_fmod(Thruster->angle, 360);
         }

        if(Thruster->angle < 0)
        {
            Thruster->angle = Thruster->angle + 360;
        }
    }
    if (Player->right)
    {
        Player->angle += 4;
        Thruster->angle+= 4;

        if(Player->angle > 360)
         {
             Player->angle = SDL_fmod(Player->angle, 360);
         }

        if(Player->angle < 0)
        {
            Player->angle = Player->angle + 360;
        }

        if(Thruster->angle > 360)
         {
             Thruster->angle = SDL_fmod(Thruster->angle, 360);
         }

        if(Thruster->angle < 0)
        {
            Thruster->angle = Thruster->angle + 360;
        }
    }
    if(Player->space)
    {
        Player->speed_y = Player->speed_y + calc_y_Speed(Player->angle);
        Player->speed_x = Player->speed_x + calc_x_Speed(Player->angle);
    }
}

int process_input(SDL_Window *window, player *Player, SDL_Renderer *renderer, Mix_Chunk *soundThruster, Mix_Chunk *soundLaser)
{   
    int done = 0;
	SDL_Event event;
    int channels = Mix_Playing(-1);
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_WINDOWEVENT_CLOSE:
            {
                proper_shutdown(window, renderer);
                done = 1;
                break;
            }
			case SDL_KEYDOWN:
            {
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						proper_shutdown(window, renderer);
                        done = 1;
						break;      
                    case SDLK_RIGHT:
                        Player->right = DOWN;
                        break;
                    case SDLK_LEFT:
                        Player->left = DOWN;
                        break;
                    case SDLK_SPACE:
                        if(channels < 6)
                        {
                            //Mix_PlayChannelTimed(-1, soundThruster, 4, 100);
                           Mix_PlayChannel(1, soundThruster, 0);
                        }
                        // printf("%d Channels are playing\n", Mix_Playing(1));
                        // fflush(stdout);
                        Player->space = DOWN;
                        
                        break;
                    case SDLK_LSHIFT:
                        if(channels < 6)
                        {
                            Mix_PlayChannel(2, soundLaser, 0);
                        }

                        Player->shift = DOWN;
                        break;

					default:
						break;
                }
                break;
            
			}
            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
				{
                    case SDLK_RIGHT:
                        Player->right = UP;
                        break;
                    case SDLK_LEFT:
                        Player->left = UP;
                        break;
                    case SDLK_SPACE:
                        Player->space = UP;
                        if( Mix_PlayChannel(1, soundThruster, 0))
                        {
                            Mix_HaltChannel(1);
                        }
                        break;
                    case SDLK_LSHIFT:
                        if( Mix_PlayChannel(2, soundLaser, 0))
                        {
                            Mix_HaltChannel(2);
                        }
                        //Player->shift = UP;
                        break;

                    default:
				        break;
                }
	        	break;
		    }
            default:
				break;
	    }
    }
    return done;
}
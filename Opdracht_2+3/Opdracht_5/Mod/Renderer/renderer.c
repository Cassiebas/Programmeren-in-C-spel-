#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH				1080
#define SCREEN_HEIGHT				720
/*
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
unsigned int window_flags = 0;
unsigned int renderer_flags = SDL_RENDERER_ACCELERATED;
*/
void init_renderer(SDL_Renderer *renderer, SDL_Window *window, unsigned int renderer_flags, unsigned int window_flags)
{
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
}

#include <stdio.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH   		1024
#define SCREEN_HEIGHT  		576
#define KOE_STAART			2
#define KOE_POOT			3
#define KOE_KOP				8
#define KOE_LIJF			16
	
// Define the mighty cow as something drawable @ some x,y-coordinate and a scale with direction for rotation:
typedef struct _koe_ {
	char *name;
	int x;
	int y;
	int scale;
	int direction;

 	struct _rgb_ {
		Uint8 r;
		Uint8 g;
		Uint8 b;
	} rgb;

} koe;

void process_input(koe *koetje);
void proper_shutdown(void);
void render_cow(koe *greta_ptr, SDL_Renderer *renderer, Uint8 red, Uint8 green, Uint8 blue, int isgreta);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int rgb_counter = 0;

int main(int argc, char *argv[])
{

	(void)argc;
	(void)argv;

	// Spawn ALL THE COWS in the window:
	koe greta = {"Greta",					 (SCREEN_WIDTH / 2),    (SCREEN_HEIGHT / 2)   , 2, 0, {0, 0, 0}};
	koe milka = {"Milka",					 (SCREEN_WIDTH / 8),    (SCREEN_HEIGHT / 2)   , 3, 90, {80, 0, 128}};
	koe sundew = {"Sundew", 				 (SCREEN_WIDTH / 8) *2, (SCREEN_HEIGHT / 2)   , 1, 45, {80, 0, 128}};
	koe loud_sun_robber = {"Robber",		 (SCREEN_WIDTH / 8) *6, (SCREEN_HEIGHT / 2)   , 5, 21, {80, 0, 128}};
	koe stellar_lady = {"lady", 			 (SCREEN_WIDTH / 8) *7, (SCREEN_HEIGHT / 2)   , 4, 180, {80, 0, 128}};
	koe casper = {"Casper", 				 (SCREEN_WIDTH / 8) *5, (SCREEN_HEIGHT / 8)   , 4, 180, {80, 0, 128}};

	koe tom = {"Tom", 						 (SCREEN_WIDTH / 8) *2, (SCREEN_HEIGHT / 8) *2, 4, 0, {80, 255, 128}};
	koe miranda = {"Miranda", 				 (SCREEN_WIDTH / 8) *4, (SCREEN_HEIGHT / 8) *3, 4, 0, {80, 255, 128}};
	koe charlotte = {"Charlotte", 			 (SCREEN_WIDTH / 8) *7, (SCREEN_HEIGHT / 8) *3, 4, 0, {80, 255, 128}};
	koe martine = {"Martine", 				 (SCREEN_WIDTH / 8) *7, (SCREEN_HEIGHT / 8) *5, 4, 0, {80, 255, 128}};
	koe cris= {"Cris", 						 (SCREEN_WIDTH / 8) *6, (SCREEN_HEIGHT / 8) *6, 4, 0, {80, 255, 128}};

	koe *cows[10] = {&milka, &sundew, &loud_sun_robber, &stellar_lady, &casper, &tom, &miranda, &charlotte, &martine, &cris};

	unsigned int window_flags = 0;
	unsigned int renderer_flags = SDL_RENDERER_ACCELERATED;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow("All your grass are belong to Greta", SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
		window_flags);
	if (window == NULL)
	{
		printf("Failed to create window -- Error: %s\n",
			SDL_GetError());
		exit(1);
	}
	
	renderer = SDL_CreateRenderer(window, -1, renderer_flags);
	if (renderer == NULL) // error handling:
	{
		printf("Failed to create renderer -- Error: %s\n",
			SDL_GetError());
		exit(1);
	}
	
	while (1)
	{
		// Refresh the backbuffer to its original state:
		// RGB (39, 174, 96) should be a green grass color
		SDL_SetRenderDrawColor(renderer, 39, 174, 96, 255);
		SDL_RenderClear(renderer);
		// Process selected inputs and pay close attention to moving
		// our freshly spawned cow:
		process_input(&greta);

		//Render Greta the one and only, with her remarkable colors	(she changing colors when moving)	
		render_cow(&greta, renderer, (Uint8)  (5 * rgb_counter) % 255, (Uint8) (255 - (5 * rgb_counter) % 255), (Uint8) (10* rgb_counter) % 255, 1);


	//Render function to render all the cows atleast once, colors are null because they will be overwritten
	for(int i = 0; i < 10; i++){
		render_cow(cows[i], renderer, 0, 0, 0, 0);
	}

	// Render redrawn scene to front buffer, showing it in the 
	// actual window:
	SDL_RenderPresent(renderer);
	// Remember ~ 60 FPS of smooth Greta movements - PC Master Race!
	SDL_Delay(16);
	}

	return 0;
}

void render_cow(koe *greta_ptr, SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, int isgreta)
{
		int p = greta_ptr->scale;

		// Update Greta's location based on the input data:
		SDL_Rect lijf = {greta_ptr->x, greta_ptr->y, p*8, p*8};
		SDL_Rect kop = {greta_ptr->x + p*3-2, greta_ptr->y - 8, KOE_KOP, KOE_KOP};
		SDL_Rect staart = {greta_ptr->x + p*4-1, greta_ptr->y + p*5+8, KOE_STAART, KOE_STAART};
		SDL_Rect linkervoorpoot = {greta_ptr->x - 3, greta_ptr->y + p, KOE_POOT, KOE_POOT};
		SDL_Rect rechtervoorpoot = {greta_ptr->x + p*8, greta_ptr->y + p, KOE_POOT, KOE_POOT};
		SDL_Rect linkerachterpoot = {greta_ptr->x - 3, greta_ptr->y + p*6, KOE_POOT, KOE_POOT};
		SDL_Rect rechterachterpoot = {greta_ptr->x + p*8, greta_ptr->y + p*6, KOE_POOT, KOE_POOT}; 

		SDL_Rect bodyparts[8] = {lijf, kop, staart, linkervoorpoot, rechtervoorpoot, linkerachterpoot, rechterachterpoot};
		SDL_Rect unsignedBodyparts[8]; 

		// check for greta and her colors to make her fancy :)
		if (isgreta == 1 ){
			SDL_SetRenderDrawColor(renderer, r, g, b, 100);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, greta_ptr->rgb.r, greta_ptr->rgb.g, greta_ptr->rgb.b, 100);
		}
		
		//Function for Rotation of cows! (but it no work) :(
		for(int i = 0; i < 7; i++){
			SDL_Point point = {greta_ptr->x, greta_ptr->y};
			int save = SDL_RenderCopyEx (renderer, NULL, &bodyparts[i], &unsignedBodyparts[i], greta_ptr->direction, &point, SDL_FLIP_NONE);
			if(save == 0){
				SDL_RenderFillRect (renderer, &unsignedBodyparts[i]);
			}
			else
			{
				SDL_RenderFillRect (renderer, &bodyparts[i]);
			}

		}
		
}

void process_input(koe *koetje)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				proper_shutdown();
				exit(0);
				break;
			case SDL_KEYDOWN:
			{
				rgb_counter ++;
				switch (event.key.keysym.scancode)
				{
					case SDL_SCANCODE_ESCAPE:
						proper_shutdown();
						exit(0);
						break;
					case SDL_SCANCODE_RIGHT:
						koetje->x += 4;
						break;
					case SDL_SCANCODE_LEFT:
						koetje->x -= 4;
						break;
					case SDL_SCANCODE_UP:
						koetje->y -= 4;
						break;
					case SDL_SCANCODE_DOWN:
						koetje->y += 4;
						break;
					default:
						break;
				}
			}
				break;

			default:
				break;
		}
	}
}

void proper_shutdown(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
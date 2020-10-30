#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Entity/Entity.h"
#include "../Renderer/Renderer.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define BODY_WIDTH 126
#define BODY_HEIGHT 108
#define THRUSTER_SIZE 92

SDL_Texture *load_Texture(char *filename, SDL_Renderer *renderer)
{
    SDL_Texture *txtr;
    txtr  = IMG_LoadTexture(renderer, filename);
    
    return txtr;
}

int render(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, double angle, SDL_Point center, SDL_RendererFlip flip, SDL_Rect *src , int zoom)
{
	SDL_Rect dst;
	int res;	
	if (zoom > 0)
	{
		dst.w *= zoom;
		dst.h *= zoom;
	}
	if (zoom < 0)
	{
		dst.w /= -zoom;
		dst.h /= -zoom;	
	}
	res = SDL_RenderCopyEx(renderer, texture, src, &dst, angle, &center, flip);
	if (res != 0)
	{
		return res;
	}
	return SDL_RenderDrawPoint(renderer, x, y);
}

void doRender(SDL_Renderer *renderer, GameState *gameRender, player *player, thruster *thruster)
{
    (void)thruster;
    //Draw Render color black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);                     

    //Clear the screen to black
    SDL_RenderClear(renderer);      

    //draw background
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, gameRender->background, NULL, &backgroundRect);               
 
    //draw the ship                                   
    SDL_Rect shipRect = {player->x, player->y, BODY_WIDTH, BODY_HEIGHT};
    SDL_RenderCopyEx(renderer, gameRender->spaceship, NULL, &shipRect, player->angle, NULL, SDL_FLIP_NONE);


   const Uint8 *state = SDL_GetKeyboardState(NULL);
     if (state[SDL_SCANCODE_SPACE])
    {
        SDL_Rect src_body = {player->x , player->y, (BODY_WIDTH +80 - THRUSTER_SIZE) -17, (BODY_HEIGHT - (THRUSTER_SIZE *2)) -17};
        SDL_Point center = {src_body.w /2 , src_body.h /2};
        SDL_Rect thrustRect = {src_body.x + BODY_WIDTH /2 - THRUSTER_SIZE /2 +5, src_body.y + THRUSTER_SIZE +5, THRUSTER_SIZE, THRUSTER_SIZE};
        SDL_RenderCopyEx(renderer, gameRender->thrust[0], NULL, &thrustRect, player->angle , &center, SDL_FLIP_NONE);
    }
                                     
}

void initGamestate (GameState *gameInit, SDL_Renderer *renderer)
{
    SDL_Surface *meteorSurface = NULL;
    SDL_Surface *shipSurface = NULL;
    SDL_Surface *backgroundSurface = NULL;
    SDL_Surface *thrustSurface[2];
    SDL_Surface *laserSurface = NULL;

    meteorSurface = IMG_Load("Images/meteor.png");
    if(meteorSurface == NULL)
    {
        printf("Can't find meteor.png !\n");
        SDL_Quit();
        exit(1);
    }

    shipSurface = IMG_Load("Images/Ship.png");
    if(shipSurface == NULL)
    {
        printf("Can't find Ship.png !\n");
        SDL_Quit();
        exit(1);
    }

    backgroundSurface = IMG_Load("Images/Background1.png");
    if(backgroundSurface == NULL)
    {
        printf("Can't find Background1.png !\n");
        SDL_Quit();
        exit(1);
    }

    thrustSurface[0] = IMG_Load("Images/Flame_01.png");
    if(thrustSurface[0] == NULL)
    {
        printf("Can't find Flame_01.png !\n");
        SDL_Quit();
        exit(1);
    }

    thrustSurface[1] = IMG_Load("Images/Flame_02.png");
    if(thrustSurface[1] == NULL)
    {
        printf("Can't find Flame_02.png !\n");
        SDL_Quit();
        exit(1);
    }
    
    laserSurface = IMG_Load("Images/laser.png");
    if(laserSurface == NULL)
    {
        printf("Can't find laser.png !\n");
        SDL_Quit();
        exit(1);
    }

    //Create Texture From saaved Surface
    gameInit->asteroid = SDL_CreateTextureFromSurface(renderer, meteorSurface);
    //Free Sruface from memory
    SDL_FreeSurface(meteorSurface);
    printf("Meteor surface added\n");

    //Create Texture From saaved Surface
    gameInit->spaceship = SDL_CreateTextureFromSurface(renderer, shipSurface);
    //Free Sruface from memory
    SDL_FreeSurface(shipSurface);
    printf("Spaceship surface added\n");

    //Create Texture From saaved Surface
    gameInit->background = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    //Free Sruface from memory
    SDL_FreeSurface(backgroundSurface);
    printf("Background surface added\n");

    for(int i =0; i <= 1; i++)
    {
        //Create Texture From saaved Surface
        gameInit->thrust[i] = SDL_CreateTextureFromSurface(renderer, thrustSurface[i]);
        //Free Sruface from memory
        SDL_FreeSurface(thrustSurface[i]);
        printf("Thrust surface added\n");
    }

    //Create Texture From saaved Surface
    gameInit->laser = SDL_CreateTextureFromSurface(renderer, laserSurface);
    //Free Sruface from memory
    SDL_FreeSurface(laserSurface);
    printf("Laser surface added\n");
    
}

void removeTexture(laser *lasers, asteroid *asteroid)
{
    if(lasers != NULL)
    {
        lasers = NULL;
    }
    if(asteroid != NULL)
    {
        asteroid = NULL;
    }
}
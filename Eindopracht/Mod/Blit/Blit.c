#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../Entity/Entity.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FPS 60

void blit_angled(SDL_Texture *txtr, int x, int y, float angle, double zoom, SDL_Renderer *renderer)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(txtr, NULL, NULL, &dest.w, &dest.h);

	if (zoom > 0)
	{
		dest.w *= (int)zoom;
		dest.h *= (int)zoom;
	}
	if ( zoom < 0)
	{
		dest.w /= -(int)zoom;
		dest.h /= -(int)zoom;	
	}
	dest.x -= dest.w / 2;
	dest.y -= dest.h / 2;

	SDL_RenderCopyEx(renderer, txtr, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}

void blitLife(SDL_Renderer *renderer, int life)
{
    char tmp[12]; 
    sprintf(&tmp[0], "LIFE : %d", life);
    TTF_Font *font;  //this opens a font style and sets a size
    font = TTF_OpenFont("Roboto.ttf", 15);
    // if (!font)
    // {
    //     printf("TTF_OpenFont: %s \n", TTF_GetError());
    // }
    SDL_Color color = {211, 33, 45, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, tmp, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = SCREEN_WIDTH/2;  //controls the rect's x coordinate 
    Message_rect.y = 50; // controls the rect's y coordinte
    Message_rect.w = 95; // controls the width of the rect
    Message_rect.h = 75; // controls the height of the rect
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    TTF_CloseFont(font);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void blitScore(SDL_Renderer *renderer, int score)
{
    char tmp[15]; 
    sprintf(&tmp[0], "SCORE : %d", score);
    TTF_Font *font;  //this opens a font style and sets a size
    font = TTF_OpenFont("Roboto.ttf", 15);
    // if (!font)
    // {
    //     printf("TTF_OpenFont: %s \n", TTF_GetError());
    // }
    SDL_Color color = {132, 222, 2, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, tmp, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = SCREEN_WIDTH/2;  //controls the rect's x coordinate 
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 95; // controls the width of the rect
    Message_rect.h = 75; // controls the height of the rect
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    TTF_CloseFont(font);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void blitFps(SDL_Renderer *renderer, int fps)
{
    char tmp[7]; 
    sprintf(&tmp[0], "%d", fps);
   // printf("%s\n", tmp);
    TTF_Font *font;  //this opens a font style and sets a size
    font = TTF_OpenFont("Roboto.ttf", 12);
    if (!font)
    {
        printf("TTF_OpenFont: %s \n", TTF_GetError());
    }
    SDL_Color color = {255, 255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, tmp, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 20;  //controls the rect's x coordinate 
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 75; // controls the width of the rect
    Message_rect.h = 50; // controls the height of the rect
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    TTF_CloseFont(font);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void setFps(Uint32 starting_tick, SDL_Renderer *renderer)
{
    int fps;
    if( (1000 / FPS) > (SDL_GetTicks() - starting_tick))
    {
        SDL_Delay(1000/FPS - (SDL_GetTicks() - starting_tick));
        fps = (int)(1000/(SDL_GetTicks() - starting_tick));
        //printf ("FPS: %d\n", 1000/(SDL_GetTicks() - starting_tick));
        //printf("%d\n", fps);
        blitFps(renderer, fps);
    }
}
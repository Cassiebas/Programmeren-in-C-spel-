#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

Mix_Chunk *loadSound(const char *path)
{
    Mix_Chunk *tmpChunk = Mix_LoadWAV(path); 

    // if (tmpChunk != 0)
    // {
    //     printf("Sound Error\n");
    // }
    return tmpChunk;
}
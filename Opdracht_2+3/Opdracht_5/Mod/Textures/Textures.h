#ifndef TEXTURES
#define TEXTURES

//#include "../entities/entity.h"

void init_all_textures(flash *flash, bullet *bullet, mouse *mousepointer, zombie *zombie, player *blorp, SDL_Renderer *renderer);
void init_textures(char *path, char *file_prefix, int num_txtrs, SDL_Texture **txtr_set, SDL_Renderer *renderer);
SDL_Texture *load_texture(char *filename, SDL_Renderer *renderer);

#endif

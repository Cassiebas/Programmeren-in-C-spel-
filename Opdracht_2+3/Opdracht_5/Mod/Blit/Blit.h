#ifndef BLIT
#define BLIT

void blit(SDL_Texture *texture, int x, int y, int center, SDL_Renderer *renderer);
void blit_angled(SDL_Texture *txtr, int x, int y, float angle, int zoom, SDL_Renderer *renderer);

#endif


#ifndef TEXTURES
#define TEXTURES

SDL_Texture *load_Texture(char *filename, SDL_Renderer *renderer);
void initGamestate (GameState *gameInit, SDL_Renderer *renderer);
void doRender(SDL_Renderer *renderer, GameState *gameRender, player *player, thruster *thruster);
int render(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, double angle, SDL_Point center, SDL_RendererFlip flip, SDL_Rect *src , int zoom);
void removeTexture(laser *lasers, asteroid *asteroid);

#endif
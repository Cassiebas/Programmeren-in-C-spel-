#ifndef BLIT
#define BLIT

void blit_angled(SDL_Texture *txtr, int x, int y, float angle, double zoom, SDL_Renderer *renderer);
void blitLife(SDL_Renderer *renderer, int life);
void blitScore(SDL_Renderer *renderer, int score);
void blitFps(SDL_Renderer *renderer, int fps);
void setFps(Uint32 starting_tick, SDL_Renderer *renderer);

#endif
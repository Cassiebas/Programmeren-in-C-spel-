#ifndef UPDATE
#define UPDATE

void addLaser(player *player, laser *laser, double angle, int offset);
int do_CollideLaserAsteroid(laser *laser, asteroid *asteroid);
bool do_CollideAsteroidShip(player *player, asteroid *asteroid);
bool laserAlive(laser *laser);
void resetGamestate(player *player, laser *laser, asteroid *asteroid);
void asteroidRadius(asteroid *asteroid, SDL_Renderer *renderer, GameState *gameState);
void startExplosion(explosion *explosion, SDL_Renderer *renderer, SDL_Rect *explosion_rect);

#endif
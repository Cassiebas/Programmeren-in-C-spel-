#ifndef INPUT
#define INPUT

void move(player *Player, thruster *Thruster);
int process_input(SDL_Window *window, player *Player, SDL_Renderer *renderer, Mix_Chunk *soundThruster, Mix_Chunk *soundLaser);
double calc_x_Speed(double rotation);
double calc_y_Speed(double rotation);
void proper_shutdown(SDL_Window *window, SDL_Renderer *renderer);

#endif
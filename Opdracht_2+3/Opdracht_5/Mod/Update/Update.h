#ifndef UPDATE
#define UPDATE

void draw_flash(flash *flash, SDL_Renderer *renderer);
void process_bullet(player *player, bullet *bullet, float angle, mouse *mouse, flash *flash);
void update_player(player *tha_playa, mouse *tha_mouse, bullet *bullet, flash *flash);
float get_angle(int x1, int y1, int x2, int y2);
void get_angle_zombie(player *player, zombie *zombie);

#endif

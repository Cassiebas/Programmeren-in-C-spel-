#include <math.h>
#include <SDL2/SDL.h>
#include "../Entity/Entity.h"
#include "../Blit/Blit.h"

#define PI							3.14159265358979323846
// Move this much pixels every frame a move is detected:
#define PLAYER_MAX_SPEED			6.0f
// AFTER a movement-key is released, reduce the movement speed for
// every consecutive frame by (1.0 - this amount):
#define PLAYER_DECELERATION			0.9f
#define BULLET_LIFESPAN				300
#define MUZZLE_VEL				20

float get_angle(int x1, int y1, int x2, int y2)
{
	float radius_schouder = 310.0;
	return (float)(360 + atan2(y2 - y1, x2 - x1) * (180.0 / PI) - (float)asin((radius_schouder / sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)))*PI/180) * PI*180);
}

void process_bullet(player *player, bullet *bullet, float angle, mouse *mouse, flash *flash)
{
	int radius = 140;
	if (player->shooting)
	{
		bullet->life = BULLET_LIFESPAN;
		bullet->speed_x = MUZZLE_VEL*(float)(cos(angle*PI/180));
		bullet->speed_y = MUZZLE_VEL*(float)(sin(angle*PI/180));
		angle += 23;
		bullet->x = (int)(player->x + radius * cos(angle*PI/180));
		bullet->y = (int)(player->y + radius * sin(angle*PI/180));
		bullet->angle = (float)(360 + atan2(mouse->y - bullet->y, mouse->x - bullet->x) * (180/PI)); //blorp is magic, blorp can shoot tha bullets backwards
		if (player->state == IDLE)
		{
			player->state = SHOOTING;
		}
		if (player->state == WALKING)
		{
			player->state = SHOOTING_WHILE_WALKING;
		}
		printf("graden: %.2f \n", bullet->angle);
		flash->x = bullet->x;
		flash->y = bullet->y;
		flash->angle = bullet->angle;
		flash->a = 255;	
	}
	if (bullet->life)
	{
		bullet->x += (int)bullet->speed_x;
		bullet->y += (int)bullet->speed_y;	
	}
	if (bullet->life == 0)
	{
		player->state = IDLE;
	}
		//printf("bullet bevindt zich op \n x: %d \n y: %d \n De snelheid is: \n x: %.2f \n y: %.2f \n", bullet->x, bullet->y, bullet->speed_x, bullet->speed_y);
}


void update_player(player *tha_playa, mouse *tha_mouse, bullet *bullet, flash *flash)
{
	if (tha_playa->up)
	{
		tha_playa->y -= (int)PLAYER_MAX_SPEED;
		if (bullet->life >= (BULLET_LIFESPAN - 3))
		{
			flash->y -= (int)PLAYER_MAX_SPEED;	
		}
		//bullet->y += (int)PLAYER_MAX_SPEED;
	}
	if (tha_playa->down)
	{
		tha_playa->y += (int)PLAYER_MAX_SPEED;
		if (bullet->life >= (BULLET_LIFESPAN - 3))
		{
			flash->y += (int)PLAYER_MAX_SPEED;
		}
		//bullet->y -= (int)PLAYER_MAX_SPEED;
	}
	if (tha_playa->left)
	{
		tha_playa->x -= (int)PLAYER_MAX_SPEED;
		if (bullet->life >= (BULLET_LIFESPAN - 3))
		{
			flash->x -= (int)PLAYER_MAX_SPEED;
		}
		//bullet->x += (int)PLAYER_MAX_SPEED;
	}
	if (tha_playa->right)
	{
		tha_playa->x += (int)PLAYER_MAX_SPEED;
		if (bullet->life >= (BULLET_LIFESPAN - 3))
		{
			flash->x += (int)PLAYER_MAX_SPEED;
		}
		//bullet->x -= (int)PLAYER_MAX_SPEED;
	}
	tha_playa->angle = get_angle(tha_playa->x, tha_playa->y,
		tha_mouse->x, tha_mouse->y);
	process_bullet(tha_playa, bullet, tha_playa->angle, tha_mouse, flash);
}


void draw_flash(flash *flash, SDL_Renderer *renderer)
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
	SDL_SetTextureBlendMode(flash->txtr_flash, SDL_BLENDMODE_ADD);
	SDL_SetTextureColorMod(flash->txtr_flash, flash->r, flash->g, flash->b);
	SDL_SetTextureAlphaMod(flash->txtr_flash, flash->a);
	blit_angled(flash->txtr_flash, flash->x, flash->y, flash->angle, -5, renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void get_angle_zombie(player *player, zombie *zombie)
{
	zombie->angle = (float)(360 + atan2(player->y - zombie->y, player->x - zombie->x) * (180.0 / PI));
}


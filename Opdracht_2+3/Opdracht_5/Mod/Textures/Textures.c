#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Entity/Entity.h"

//static SDL_Window *window = NULL;
//static SDL_Renderer *renderer = NULL;


SDL_Texture *load_texture(char *filename, SDL_Renderer *renderer)
{
	//IMG_Init(filename);
	SDL_Texture *txtr;
	txtr = IMG_LoadTexture(renderer, filename);
	return txtr;
}

void init_textures(char *path, char *file_prefix, int num_txtrs, SDL_Texture **txtr_set, SDL_Renderer *renderer)
{
	char filename[128];
	for (int i = 0; i < num_txtrs; i++)
	{
		sprintf(filename, "%s%s%d.png", path, file_prefix, i);
		printf("full file location: %s \n", filename);
		txtr_set[i] = load_texture(filename, renderer);
	}
}


void init_all_textures(flash *flash, bullet *bullet, mouse *mousepointer, zombie *zombie, player *blorp, SDL_Renderer *renderer)
{
	//load all the textures
	char path_feet[41], path_body[41], prefix_body[24], prefix_feet[24]; //31+10 & survivor-shoot_handgun_\0 = 24 chars
	for (zombiestate state = 0; state < MAX_STATE_E; state++)
	{
		if (state == IDLE_E)
		{
			strcpy(path_body, "gfx/tds_zombie/idle/");
	    		strcpy(prefix_body, "skeleton-idle_");
	    		//printf("%s \n %s \n", path_body, prefix_body);
	    		init_textures(path_body, prefix_body, 17, zombie->txtr_body[state], renderer);
		}
		if (state == WALKING_E)
		{
			strcpy(path_body, "gfx/tds_zombie/walking/");
	    		strcpy(prefix_body, "skeleton-move_");
	    		//printf("%s \n %s \n", path_body, prefix_body);
	    		init_textures(path_body, prefix_body, 17, zombie->txtr_body[state], renderer);

		}
		if (state == ATTACKING_E)
		{
			strcpy(path_body, "gfx/tds_zombie/attack/");
	    		strcpy(prefix_body, "skeleton-attack_");
	    		//printf("%s \n %s \n", path_body, prefix_body);
	    		init_textures(path_body, prefix_body, 9, zombie->txtr_body[state], renderer);

		}
	}

	for (playerstate state = 0; state < MAX_STATE; state++)
	{
		if (state == 0)
		{

			strcpy(path_body, "gfx/idlebody/");
			strcpy(path_feet, "gfx/idlefeet/");
			strcpy(prefix_body, "survivor-idle_handgun_");
			strcpy(prefix_feet, "survivor-idle_");
			//printf("%s \n %s \n", path_feet, path_body);
			init_textures(path_body, prefix_body, 20, blorp->txtr_body[state], renderer);
			init_textures(path_feet, prefix_feet, 1, blorp->txtr_feet[state], renderer);
		}
		if (state == 1)
		{
			strcpy(path_body, "gfx/movebody/");
			strcpy(path_feet, "gfx/walkfeet/");
			strcpy(prefix_body, "survivor-move_handgun_");
			strcpy(prefix_feet, "survivor-walk_");
			//printf("%s \n %s \n", path_feet, path_body);
			init_textures(path_body, prefix_body, 20, blorp->txtr_body[state], renderer);
			init_textures(path_feet, prefix_feet, 20, blorp->txtr_feet[state], renderer);

		}
		if (state == 2)
		{
			strcpy(path_body, "gfx/shoot/");
			strcpy(path_feet, "gfx/idlefeet/");
			strcpy(prefix_body, "survivor-shoot_handgun_");
			strcpy(prefix_feet, "survivor-idle_");
			//printf("%s \n %s \n", path_feet, path_body);
			init_textures(path_body, prefix_body, 3, blorp->txtr_body[state], renderer);
			init_textures(path_feet, prefix_feet, 20, blorp->txtr_feet[state], renderer);

		}
		if (state == 3)
		{
			strcpy(path_body, "gfx/shoot/");
			strcpy(path_feet, "gfx/walkfeet/");
			strcpy(prefix_body, "survivor-shoot_handgun_");
			strcpy(prefix_feet, "survivor-walk_");
			//printf("%s \n %s \n", path_feet, path_body);
			init_textures(path_body, prefix_body, 3, blorp->txtr_body[state], renderer);
			init_textures(path_feet, prefix_feet, 20, blorp->txtr_feet[state], renderer);

		}


	}

	//blorp.txtr_player = load_texture("gfx/blorp.png");
	// New: Load mousepointer texture:
	mousepointer->txtr_reticle = load_texture("gfx/reticle.png", renderer);
	bullet->txtr_kogel = load_texture("gfx/bullet20x5.png", renderer);
	flash->txtr_flash = load_texture("gfx/muzzle_flashs/m_2.png", renderer);
}


#ifndef ENTITY
#define ENTITY

typedef struct _mouse_ {
	int x;
	int y;
	SDL_Texture *txtr_reticle;
} mouse;

typedef enum _playerstate_ {
	IDLE = 0,
	WALKING = 1,
	SHOOTING = 2,
	SHOOTING_WHILE_WALKING = 3,
	MAX_STATE = 4
}playerstate;

typedef enum _zombiestate_ {
	IDLE_E = 0,
	WALKING_E = 1,
	ATTACKING_E = 2,
	MAX_STATE_E = 3
}zombiestate;

// Added: speed in both directions and rotation angle:
typedef struct _player_ {
	int x;
	int y;
	float speed_x;
	float speed_y;
	int up;
	int down;
	int left;
	int right;
	float angle;
	int shooting;
	playerstate state;
	SDL_Texture *txtr_body[MAX_STATE][200];
	SDL_Texture *txtr_feet[MAX_STATE][200]; //eerst werkte dit met 20, toen opeens segmentation fault
} player;

typedef struct _zombie_ {
	int x;
	int y;
	float angle;
	SDL_Texture *txtr_body[MAX_STATE_E][100];
	zombiestate state;

}zombie;

typedef struct _bullet_ {
		int x;
		int y;
		float speed_x;
		float speed_y;
		float angle;
		int life;
		SDL_Texture *txtr_kogel;	
}bullet;

typedef struct _flash_ {
		int x;
		int y;
		float angle;
		SDL_Texture *txtr_flash;
		Uint8 r;
		Uint8 g;
		Uint8 b;
		Uint8 a;
}flash;

typedef enum _keystate_ {
	UP = 0,
	DOWN = 1
} keystate;

#endif


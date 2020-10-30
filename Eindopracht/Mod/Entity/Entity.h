#ifndef ENTITY
#define ENTITY

typedef enum _keystate_{
    UP = 0,
    DOWN = 1
} keystate ;

typedef struct _player_{
    int x;
    int y;
    //short life;
    double angle;
    double speed_x, speed_y;
    int shooting;
    SDL_Point center;
    float radius;

   // hitbox player_box;
    keystate left;
    keystate right;
    keystate space;
    keystate shift;

} player; 

typedef struct _thrust_{
    int x, y;
    double angle;
    // double speed_x, speed_y;
} thruster;

typedef struct _asteroid_ {
    int x, y;
    int dx, dy;
    double angle;
    int size;
    int delay;
    float radius;
    SDL_Point center;
    //int asteroid_angle;
    //hitbox asteroid_box;

} asteroid;

typedef struct _laser_{
    int x, y;
    float dx, dy;
    double angle;
    int shot;
    float radius;
    SDL_Point center;
    int delay;
    //float radius;
   // SDL_Point *center;
    //hitbox laser_box;
} laser;

typedef struct _explosion_{
    int x;
    int y;
    int frame;
    SDL_Texture *texture;

} explosion;

typedef struct _state_ {
    //array
    //int asteroid_array[ASTEROID_LIMIT];
    //Images
    SDL_Texture *asteroid;
    SDL_Texture *spaceship;
    SDL_Texture *background;
    SDL_Texture *thrust[2];
    SDL_Texture *laser;
  //  SDL_Texture *explosion;
    //laser *lasers[LASER_LIMIT];

} GameState;

#endif
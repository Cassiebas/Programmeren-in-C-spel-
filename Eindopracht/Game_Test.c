#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TRUE 1 
#define FPS  60
#define ASTEROID_LIMIT 5
#define ACCELERATION 1
#define BULLET_SPEED 32
#define PI 3.1415926535899323846
#define SPEED 5.5
#define LASER_LIMIT 60
#define BODY_WIDTH 126
#define BODY_HEIGHT 108
#define THRUSTER_SIZE 92
#define OFFSET_Y 5
#define TRUE 1

time_t t;

typedef struct _hitbox_{
    int x, y;

} hitbox;

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

//laser *lasers[LASER_LIMIT] = { NULL };

unsigned int renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

//void renderThrust(SDL_Renderer *renderer);
void addLaser(player *player, laser *laser, double angle, int offset);
void removeTexture(laser *lasers, asteroid *asteroid);
void blit_angled(SDL_Texture *txtr, int x, int y, float angle, double zoom, SDL_Renderer *renderer);
void proper_shutdown(SDL_Window *window, SDL_Renderer *renderer);
//void process_bullet(player *player, GameState *gameState, float angle);
int process_input(SDL_Window *window, player *Player, SDL_Renderer *renderer, Mix_Chunk *soundThruster, Mix_Chunk *soundLaser);
void doRender(SDL_Renderer *renderer, GameState *gameRender, player *player, thruster *thruster);
void initGamestate (GameState *gameInit, SDL_Renderer *renderer);
//void makeRandomAsteroid(asteroid *makeAsteroid);
double calc_x_Speed(double rotation);
double calc_y_Speed(double rotation);
void setFps(Uint32 starting_tick, SDL_Renderer *renderer);
void moveLaser(player *Player);
void move(player *Player, thruster *Thruster);
void blitFps(SDL_Renderer *renderer, int fps);
bool laserAlive(laser *laser);
bool do_CollideLaserAsteroid(laser *laser, asteroid *asteroid);
SDL_Texture *load_Texture(char *filename, SDL_Renderer *renderer);
int render(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, double angle, SDL_Point center, SDL_RendererFlip flip, SDL_Rect *src , int zoom);
void blitScore(SDL_Renderer *renderer, int score);
bool do_CollideAsteroidShip(player *player, asteroid *asteroid);
void resetGamestate(player *player, laser *laser, asteroid *asteroid);
void blitLife(SDL_Renderer *renderer, int life);
Mix_Chunk *loadSound(const char *path);

int main(int argc, char *argv[])
{
    (void)argc;
	(void)argv;
    int i = 0, last_i;
    int offset = 28, done = 0, score = 0, scoreBlit = 0;
    int life = 3, scoreLife = life;
    SDL_Point explosion_center;
    SDL_Rect explosion_rect;
    Uint32 starting_tick;                           //Creat Frame/tick
    SDL_Window *window = NULL;                      //Create a window
    SDL_Renderer *renderer = NULL;                  //Creat a renderer
    int channels = Mix_Playing(-1);                 
    int frame = 0;                           
    srand((unsigned)time(NULL));

    GameState gameState;
    //lasers[LASER_LIMIT];
    laser lasers[LASER_LIMIT];
    asteroid asteroids[ASTEROID_LIMIT];
    explosion explosion[ASTEROID_LIMIT+1];
    player Player;

    Player.x = SCREEN_WIDTH / 2;
    Player.y = SCREEN_HEIGHT / 2;
    Player.angle = 90;
    Player.shooting = 0;
    Player.speed_x = 0;
    Player.speed_y = 0;
    Player.center.x = BODY_WIDTH / 2;
    Player.center.y = BODY_HEIGHT / 2;
    Player.radius = BODY_HEIGHT / 2;
    Player.left = UP;
    Player.right = UP;
    Player.shift = UP;
    Player.space = UP;

    thruster Thruster = {(SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0};

    //init lasers
    for (int i =0; i < LASER_LIMIT; i++)
    {
        lasers[i].x = -70;
        lasers[i].y = -70; 
        lasers[i].dx = 0;
        lasers[i].dy = 0; 
        lasers[i].angle = 0;
        lasers[i].shot = 0;
        lasers[i].radius = 12 /2;
        lasers[i].center.x = 12/2;
        lasers[i].center.y = 12/2;
        lasers[i].delay = i;
    }

    for (int i = 0; i < ASTEROID_LIMIT; i++)
    {
        asteroids[i].x = 0;
        asteroids[i].y = 0;
        asteroids[i].dx = (int)((rand () % 4 +1) * pow(-1, rand() % 2  +1));
        asteroids[i].dy = (int)((rand () % 4 +1) * pow(-1, rand() % 2  +1));
        asteroids[i].angle = 0;
        asteroids[i].size = (rand() % 5 +1);
        asteroids[i].delay = 0;
        asteroids[i].radius = 32 /2;
        asteroids[i].center.x = 32/2;
        asteroids[i].center.y = 32/2;
    }

    for(int i = 0; i< ASTEROID_LIMIT+1; i++)
    {
        explosion[i].x = -70;
        explosion[i].y = -70;
        explosion[i].frame = -1;
        explosion[i].texture = load_Texture("Images/Explosion.png", renderer);
    }

    //Check if can reach SDL_Library 
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)                                  
    {
        printf("Couldn't initialize SDL, %s\n :( ", SDL_GetError());
        exit(1);
    }
    TTF_Init();

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096)< 0)
    {
        printf("Sound Error: %s \n", Mix_GetError());
    }

    //create window :)
    window = SDL_CreateWindow("Pew Pew space cookies",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT, 0);
    
    if (window == NULL)
    {
        printf("Couldn't create window");
        exit(1);
    }

    //Set renderer to VSYNC frefreshrate.
    renderer = SDL_CreateRenderer(window, -1, renderer_flags);      
    if (renderer == NULL) 
	{
		//printf("Failed to create renderer -- Error: %s\n", SDL_GetError(0));
		exit(1);
	}

    Mix_Music *bgm = Mix_LoadMUS("Sounds/BackgroundMusic.flac");
    Mix_Chunk *soundExplosion = loadSound("Sounds/Explosion.wav");
    Mix_Chunk *soundThruster = loadSound("Sounds/Engine.mp3");
    Mix_Chunk *soundLaser = loadSound("Sounds/Laser.wav");

    initGamestate(&gameState, renderer);
    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(bgm, -1);
    }

    while(!done)
    {   
        //Get frame tick
        starting_tick = SDL_GetTicks();
        
        Player.x += (int)(Player.speed_x / SPEED);
        Player.y -= (int)(Player.speed_y / SPEED);

        if(Player.y > SCREEN_HEIGHT)
        {
            Player.y = Player.y % SCREEN_HEIGHT;
        }
        else if (Player.y < 0)
        {
            Player.y = Player.y + SCREEN_HEIGHT;
        }

        if(Player.x > SCREEN_WIDTH)
        {
            Player.x = Player.x % SCREEN_WIDTH;
        }
        else if (Player.x < 0)
        {
            Player.x = Player.x + SCREEN_WIDTH;
        }
        
        doRender(renderer, &gameState, &Player, &Thruster);

        if(process_input(window, &Player, renderer, soundThruster, soundLaser) == 1)
        {
            done = 1;
        }
        move(&Player, &Thruster);

        //spawning space cookies
        for(i =0; i < ASTEROID_LIMIT; i++)
        {   
            if (explosion[i].frame >= 0)
            {
                // printf("Collision! BOOOOM\n");
                explosion[i].frame++;
                if (explosion[i].frame == 12)
                {
                    explosion[i].frame = -1;
                    explosion[i].x = -200;
                    explosion[i].y = 100;
                    //exploded = false;
                }
                if (explosion[i].frame%2)
                {
                explosion_rect.x += explosion_rect.w;
                }
                explosion_center.x = explosion_rect.x + explosion_rect.w/2;
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
                SDL_SetTextureBlendMode(explosion[i].texture, SDL_BLENDMODE_ADD);
                SDL_SetTextureColorMod(explosion[i].texture, 255, 255, 255);//, (Uint8)(255-255/frame));
                SDL_SetTextureAlphaMod(explosion[i].texture, (Uint8)(255-(255/26)*frame));
                render(renderer, explosion[i].texture, explosion[i].x, explosion[i].y, 0, explosion_center, SDL_FLIP_NONE, &explosion_rect, 1);
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            }

            if(asteroids[i].delay > 0)
            {
                asteroids[i].delay --;
            }
            int Asteroid_Border = (rand() % 4 + 1);

            //if Asteroid is at border of screen blit him
            if((asteroids[i].x >= 0 || asteroids[i].x <= SCREEN_WIDTH) && (asteroids[i].y >= 0 || asteroids[i].y <= SCREEN_HEIGHT)
                &&  asteroids[i].delay == 0) 
            {

                if (i % 2)
                {
                    asteroids[i].angle = frame;
                }

                else
                {
                    asteroids[i].angle = 360 - frame;
                }
                
                if (asteroids[i].x == 0&& asteroids[i].y ==0 )
                {
                    if(Asteroid_Border == 1)
                    {
                        asteroids[i].x = rand() % SCREEN_WIDTH;
                        asteroids[i].y = 0;
                        if(asteroids[i].dx < 0 )
                        {
                            asteroids[i].dx *= -1;
                            if (asteroids[i].dy < 0 ){asteroids[i].dy *= -1;}
                        }
                    }
                    if(Asteroid_Border == 2)
                    {
                        asteroids[i].x = 0;
                        asteroids[i].y = rand() % SCREEN_HEIGHT;
                        if(asteroids[i].dy < 0 )
                        {
                            asteroids[i].dy *= -1;
                            if (asteroids[i].dx < 0 ){asteroids[i].dx *= -1;}
                        }
                    }
                    if(Asteroid_Border == 3)
                    {
                        asteroids[i].x = SCREEN_WIDTH;
                        asteroids[i].y = rand () % SCREEN_HEIGHT;
                        if(asteroids[i].dx > 0 ){asteroids[i].dx *= -1;}
                        
                        
                    }
                    if(Asteroid_Border == 4)
                    {
                        asteroids[i].x = rand () % SCREEN_WIDTH;
                        asteroids[i].y = SCREEN_HEIGHT;

                        if(asteroids[i].dy > 0){asteroids[i].dy *= -1;}
                    }               
                }

                else
                {
                    asteroids[i].x += (int)asteroids[i].dx;
                    asteroids[i].y += (int)asteroids[i].dy;
                }

            blit_angled(gameState.asteroid, asteroids[i].x, asteroids[i].y, (float)asteroids[i].angle, asteroids[i].size, renderer);   

            }
            //if Asteroid is out of screen remove him
            if(asteroids[i].x < 0 || asteroids[i].x > SCREEN_WIDTH || asteroids[i].y < 0 || asteroids[i].y > SCREEN_HEIGHT)
                {
                    //printf("3 ");
                    asteroids[i].x = 0;
                    asteroids[i].y = 0;
                    // asteroids[i].dx = 0;
                    // asteroids[i].dy = 0;
                    asteroids[i].angle = 0;
                    asteroids[i].size = (rand() % 5 +1);
                    asteroids[i].delay = 120;
                }

            if(do_CollideAsteroidShip(&Player, &asteroids[i]))
            {  

                printf("Collision Cookie with Ship!\n");
                asteroids[i].x = -100;
                asteroids[i].y = -100;
                // asteroids[i].dx = 0;
                // asteroids[i].dy = 0;
                asteroids[i].angle = 0;
                asteroids[i].size = (rand() % 5 +1);
                asteroids[i].delay = 120;

                score --;
                life --;
                scoreLife = life;
            }
        }
        
        //spawning lasers
        for (int j =0; j < LASER_LIMIT; j++)
        {   
        
            //printf("Velocity X%.2f  ,  Y%.2f\n", lasers[i].dx, lasers[i].dy);
            //fflush(stdout);
            if (j == 0)
            {
                last_i = LASER_LIMIT - 1;
            }
            else
            {
                last_i = j -1;
            }
            
            if((j+1) % 2)
            {
                offset *= -1;
            }

            if (lasers[last_i].delay > 0)
            {
                lasers[last_i].delay --;
            }
   
            if(Player.shift == DOWN && (lasers[j].dx == 0 && lasers[j].dy == 0) && (laserAlive(&lasers[last_i]) == false) && (lasers[j].shot == 0) && lasers[last_i].delay == 0)
            {
                //prjntf("1\n");
                addLaser(&Player, &lasers[j], Player.angle - 90, offset);
                //Player.shift = UP;
                //Player.shooting = 0;
                lasers[j].shot = 1; 
                lasers[j].delay = LASER_LIMIT;

            }
        
            //jf laser alive bljt hjm
            if(lasers[j].x > 0 && lasers[j].x < SCREEN_WIDTH && lasers[j].y > 0 && lasers[j].y < SCREEN_HEIGHT
            && (lasers[j].dx != 0 || lasers[j].dy != 0 ) && (lasers[j].shot == 1))
            {

                lasers[j].x += (int)lasers[j].dx;
                lasers[j].y += (int)lasers[j].dy;
                blit_angled(gameState.laser, lasers[j].x, lasers[j].y, (float)lasers[j].angle, 1, renderer);
            }
            
            //jf Laser dead restore jt's values to null
            if((lasers[j].x <= 0 || lasers[j].x >= SCREEN_WIDTH || lasers[j].y <= 0 || lasers[j].y >= SCREEN_HEIGHT) && (lasers[j].dx != 0 || lasers[j].dy != 0))
            {
                //prjntf("3 ");
                lasers[j].x = -70;
                lasers[j].y = -70; 
                lasers[j].dx = 0;
                lasers[j].dy = 0; 
                lasers[j].angle = 0;
                lasers[j].shot = 0;
            }

        }
        //Collision detection and explosion
        for (int j = 0; j < LASER_LIMIT; j++)
        {
            for (int i = 0; i < ASTEROID_LIMIT; i++)
            {   
                
                if(do_CollideLaserAsteroid(&lasers[j], &asteroids[i]) != 0)
                {
                    if(channels < 6)
                    {
                        Mix_PlayChannel(3, soundExplosion, 0);
                    }
                    explosion_rect.x = 0;
                    explosion_rect.y = 0;
                    SDL_QueryTexture(explosion[i].texture, NULL, NULL, &explosion_rect.w, &explosion_rect.h);
                    explosion_rect.w /= 12;
                    explosion_center.x = explosion_rect.w/2;
                    explosion_center.y = explosion_rect.h/2;	
                    explosion[i].x = SCREEN_WIDTH/2;//(int)(asteroids[i].x - explosion_center.x/2);
                    explosion[i].y = SCREEN_HEIGHT/2;//(int)(asteroids[i].y - explosion_center.y/2);
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
                    SDL_SetTextureBlendMode(explosion[i].texture, SDL_BLENDMODE_ADD);
                    SDL_SetTextureColorMod(explosion[i].texture, 255, 255, 255);//, (Uint8)(255-255/frame));
                    SDL_SetTextureAlphaMod(explosion[i].texture, (Uint8)(255-(255/12)* explosion[i].frame));
                    render(renderer, explosion[i].texture, explosion[i].x, explosion[i].y, 0, explosion_center, SDL_FLIP_NONE, &explosion_rect, 1);
                    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
                    lasers[j].x = (j+1)*-200;
                    lasers[j].y = (j+1)*-200;
                    explosion[j].frame = 0;

                    //explode
                    lasers[j].x = -70;
                    lasers[j].y = -70; 
                    lasers[j].dx = 0;
                    lasers[j].dy = 0; 
                    lasers[j].angle = 0;
                    lasers[j].shot = 0;

                    asteroids[i].x = -200;
                    asteroids[i].y = -200;
                    // asteroids[i].dx = 0;
                    // asteroids[i].dy = 0;
                    asteroids[i].angle = 0;
                    asteroids[i].size = (rand() % 5 +1);
                    asteroids[i].delay = 120;

                    score++;
                    scoreBlit = score;
                }
                
                if (scoreLife == 0)
                {
                    printf("Reset! \n");
                    scoreLife = 3;
                    life = 3;
                    scoreBlit = 0;
                    score = 3;
                    resetGamestate(&Player, &lasers[j], &asteroids[i]);
                }
            }
        }

        frame ++;
        if(frame == 360)
        {
            frame = 0;
        }

        blitLife(renderer, scoreLife);
        blitScore(renderer, scoreBlit);
        setFps(starting_tick, renderer);
        SDL_RenderPresent(renderer); 
        
    }

    Mix_FreeChunk(soundLaser);
    Mix_FreeChunk(soundThruster);
    Mix_FreeMusic(bgm);

    bgm = NULL;
    soundThruster = NULL;
    soundLaser = NULL;
    //Destroys texture of:                                        
    for(int i = 0; i < ASTEROID_LIMIT; i++)
    { 
        printf("Safely removing textures..\n");
        for (int j = 0; j < LASER_LIMIT; j++)
        {
            removeTexture(&lasers[j], &asteroids[i]);
        }
    }
    
    proper_shutdown(window, renderer);

    return 0;
}

void doRender(SDL_Renderer *renderer, GameState *gameRender, player *player, thruster *thruster)
{
    (void)thruster;
    //Draw Render color black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);                     

    //Clear the screen to black
    SDL_RenderClear(renderer);      

    //draw background
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, gameRender->background, NULL, &backgroundRect);               
 
    //draw the ship                                   
    SDL_Rect shipRect = {player->x, player->y, BODY_WIDTH, BODY_HEIGHT};
    SDL_RenderCopyEx(renderer, gameRender->spaceship, NULL, &shipRect, player->angle, NULL, SDL_FLIP_NONE);


   const Uint8 *state = SDL_GetKeyboardState(NULL);
     if (state[SDL_SCANCODE_SPACE])
    {
        SDL_Rect src_body = {player->x , player->y, (BODY_WIDTH +80 - THRUSTER_SIZE) -17, (BODY_HEIGHT - (THRUSTER_SIZE *2)) -17};
        SDL_Point center = {src_body.w /2 , src_body.h /2};
        SDL_Rect thrustRect = {src_body.x + BODY_WIDTH /2 - THRUSTER_SIZE /2 +5, src_body.y + THRUSTER_SIZE +5, THRUSTER_SIZE, THRUSTER_SIZE};
        SDL_RenderCopyEx(renderer, gameRender->thrust[0], NULL, &thrustRect, player->angle , &center, SDL_FLIP_NONE);
    }
                                     
}

int process_input(SDL_Window *window, player *Player, SDL_Renderer *renderer, Mix_Chunk *soundThruster, Mix_Chunk *soundLaser)
{   
    int done = 0;
	SDL_Event event;
    int channels = Mix_Playing(-1);
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_WINDOWEVENT_CLOSE:
            {
                proper_shutdown(window, renderer);
                done = 1;
                break;
            }
			case SDL_KEYDOWN:
            {
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						proper_shutdown(window, renderer);
                        done = 1;
						break;      
                    case SDLK_RIGHT:
                        Player->right = DOWN;
                        break;
                    case SDLK_LEFT:
                        Player->left = DOWN;
                        break;
                    case SDLK_SPACE:
                        if(channels < 6)
                        {
                            //Mix_PlayChannelTimed(-1, soundThruster, 4, 100);
                           Mix_PlayChannel(1, soundThruster, 0);
                        }
                        // printf("%d Channels are playing\n", Mix_Playing(1));
                        // fflush(stdout);
                        Player->space = DOWN;
                        
                        break;
                    case SDLK_LSHIFT:
                        if(channels < 6)
                        {
                            //Mix_PlayChannelTimed(2, soundLaser, -1, 100);
                            while(Player->shift == DOWN && Player->shift != UP)
                            {Mix_PlayChannel(2, soundLaser, 1);}
                        }
                        Player->shift = DOWN;
                        break;

					default:
						break;
                }
                break;
            
			}
            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym)
				{
                    case SDLK_RIGHT:
                        Player->right = UP;
                        break;
                    case SDLK_LEFT:
                        Player->left = UP;
                        break;
                    case SDLK_SPACE:
                        Player->space = UP;
                        if( Mix_PlayChannel(1, soundThruster, 0))
                        {
                            Mix_HaltChannel(1);
                        }
                        break;
                    case SDLK_LSHIFT:
                        if( Mix_PlayChannel(2, soundLaser, 0))
                        {
                            Mix_HaltChannel(2);
                        }
                        Player->shift = UP;
                        break;

                    default:
				        break;
                }
	        	break;
		    }
            default:
				break;
	    }
    }
    return done;
}

double calc_x_Speed(double rotation)
{
   // printf("COS_x : %f ", (ACCELERATION * cos(rotation * (PI / 180))));
    return (ACCELERATION * sin(rotation * (PI / 180)));
        
}

double calc_y_Speed(double rotation)
{
    //printf("SIN_y : %f\n", (ACCELERATION *  sin(rotation * (PI / 180))));
    return (ACCELERATION * cos(rotation * (PI / 180))); 
        
}

void move(player *Player, thruster *Thruster)
{
    if(Player->left)
    {
        Player->angle-= 4;
        Thruster->angle-= 4;

         if(Player->angle > 360)
         {
             Player->angle = SDL_fmod(Player->angle, 360);
         }

        if(Player->angle < 0)
        {
            Player->angle = Player->angle + 360;
        }

        if(Thruster->angle > 360)
         {
             Thruster->angle = SDL_fmod(Thruster->angle, 360);
         }

        if(Thruster->angle < 0)
        {
            Thruster->angle = Thruster->angle + 360;
        }
    }
    if (Player->right)
    {
        Player->angle += 4;
        Thruster->angle+= 4;

        if(Player->angle > 360)
         {
             Player->angle = SDL_fmod(Player->angle, 360);
         }

        if(Player->angle < 0)
        {
            Player->angle = Player->angle + 360;
        }

        if(Thruster->angle > 360)
         {
             Thruster->angle = SDL_fmod(Thruster->angle, 360);
         }

        if(Thruster->angle < 0)
        {
            Thruster->angle = Thruster->angle + 360;
        }
    }
    if(Player->space)
    {
        Player->speed_y = Player->speed_y + calc_y_Speed(Player->angle);
        Player->speed_x = Player->speed_x + calc_x_Speed(Player->angle);
    }
}

void moveLaser(player *Player)
{
    if (Player->shift)
        {
            Player->shooting = 1;
            // process_bullet(&Player, &gameState, Player->angle);
            //addLaser(Player, laser, Player->angle);
            printf("Shooting: %d\n", Player->shooting);
            
        }
}

void addLaser(player *player, laser *laser, double angle, int offset)
{

    int radius = 42;

        laser->dx = BULLET_SPEED*(float)(cos(angle*PI/180));
        laser->dy = BULLET_SPEED*(float)(sin(angle*PI/180));
        angle += offset;
        laser->x = (int)(player->x + BODY_WIDTH/2 + radius * cos(angle*PI/180));
        laser->y = (int)(player->y + BODY_HEIGHT/2 + radius * sin(angle*PI/180));
       // laser->angle = (float)(420 + atan2(player->x, player->y) * (180/PI));
        laser->angle = player->angle;

}

void removeTexture(laser *lasers, asteroid *asteroid)
{
    if(lasers != NULL)
    {
        lasers = NULL;
    }
    if(asteroid != NULL)
    {
        asteroid = NULL;
    }
}

void initGamestate (GameState *gameInit, SDL_Renderer *renderer)
{
    SDL_Surface *meteorSurface = NULL;
    SDL_Surface *shipSurface = NULL;
    SDL_Surface *backgroundSurface = NULL;
    SDL_Surface *thrustSurface[2];
    SDL_Surface *laserSurface = NULL;

    meteorSurface = IMG_Load("Images/meteor.png");
    if(meteorSurface == NULL)
    {
        printf("Can't find meteor.png !\n");
        SDL_Quit();
        exit(1);
    }

    shipSurface = IMG_Load("Images/Ship.png");
    if(shipSurface == NULL)
    {
        printf("Can't find Ship.png !\n");
        SDL_Quit();
        exit(1);
    }

    backgroundSurface = IMG_Load("Images/Background1.png");
    if(backgroundSurface == NULL)
    {
        printf("Can't find Background1.png !\n");
        SDL_Quit();
        exit(1);
    }

    thrustSurface[0] = IMG_Load("Images/Flame_01.png");
    if(thrustSurface[0] == NULL)
    {
        printf("Can't find Flame_01.png !\n");
        SDL_Quit();
        exit(1);
    }

    thrustSurface[1] = IMG_Load("Images/Flame_02.png");
    if(thrustSurface[1] == NULL)
    {
        printf("Can't find Flame_02.png !\n");
        SDL_Quit();
        exit(1);
    }
    
    laserSurface = IMG_Load("Images/laser.png");
    if(laserSurface == NULL)
    {
        printf("Can't find laser.png !\n");
        SDL_Quit();
        exit(1);
    }

    //Create Texture From saaved Surface
    gameInit->asteroid = SDL_CreateTextureFromSurface(renderer, meteorSurface);
    //Free Sruface from memory
    SDL_FreeSurface(meteorSurface);
    printf("Meteor surface added\n");

    //Create Texture From saaved Surface
    gameInit->spaceship = SDL_CreateTextureFromSurface(renderer, shipSurface);
    //Free Sruface from memory
    SDL_FreeSurface(shipSurface);
    printf("Spaceship surface added\n");

    //Create Texture From saaved Surface
    gameInit->background = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    //Free Sruface from memory
    SDL_FreeSurface(backgroundSurface);
    printf("Background surface added\n");

    for(int i =0; i <= 1; i++)
    {
        //Create Texture From saaved Surface
        gameInit->thrust[i] = SDL_CreateTextureFromSurface(renderer, thrustSurface[i]);
        //Free Sruface from memory
        SDL_FreeSurface(thrustSurface[i]);
        printf("Thrust surface added\n");
    }

    //Create Texture From saaved Surface
    gameInit->laser = SDL_CreateTextureFromSurface(renderer, laserSurface);
    //Free Sruface from memory
    SDL_FreeSurface(laserSurface);
    printf("Laser surface added\n");
    
}

void proper_shutdown(SDL_Window *window, SDL_Renderer *renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    Mix_Quit();
	SDL_Quit();
    TTF_Quit();

}

void setFps(Uint32 starting_tick, SDL_Renderer *renderer)
{
    int fps;
    if( (1000 / FPS) > (SDL_GetTicks() - starting_tick))
    {
        SDL_Delay(1000/FPS - (SDL_GetTicks() - starting_tick));
        fps = (int)(1000/(SDL_GetTicks() - starting_tick));
        //printf ("FPS: %d\n", 1000/(SDL_GetTicks() - starting_tick));
        //printf("%d\n", fps);
        blitFps(renderer, fps);
    }
}

void blitFps(SDL_Renderer *renderer, int fps)
{
    char tmp[7]; 
    sprintf(&tmp[0], "%d", fps);
   // printf("%s\n", tmp);
    TTF_Font *font;  //this opens a font style and sets a size
    font = TTF_OpenFont("Roboto.ttf", 12);
    if (!font)
    {
        printf("TTF_OpenFont: %s \n", TTF_GetError());
    }
    SDL_Color color = {255, 255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, tmp, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = 20;  //controls the rect's x coordinate 
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 75; // controls the width of the rect
    Message_rect.h = 50; // controls the height of the rect
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    TTF_CloseFont(font);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void blitScore(SDL_Renderer *renderer, int score)
{
    char tmp[12]; 
    sprintf(&tmp[0], "SCORE : %d", score);
    TTF_Font *font;  //this opens a font style and sets a size
    font = TTF_OpenFont("Roboto.ttf", 15);
    // if (!font)
    // {
    //     printf("TTF_OpenFont: %s \n", TTF_GetError());
    // }
    SDL_Color color = {132, 222, 2, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, tmp, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = SCREEN_WIDTH/2;  //controls the rect's x coordinate 
    Message_rect.y = 0; // controls the rect's y coordinte
    Message_rect.w = 95; // controls the width of the rect
    Message_rect.h = 75; // controls the height of the rect
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    TTF_CloseFont(font);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void blitLife(SDL_Renderer *renderer, int life)
{
    char tmp[12]; 
    sprintf(&tmp[0], "LIFE : %d", life);
    TTF_Font *font;  //this opens a font style and sets a size
    font = TTF_OpenFont("Roboto.ttf", 15);
    // if (!font)
    // {
    //     printf("TTF_OpenFont: %s \n", TTF_GetError());
    // }
    SDL_Color color = {211, 33, 45, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, tmp, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture
    SDL_Rect Message_rect; //create a rect
    Message_rect.x = SCREEN_WIDTH/2;  //controls the rect's x coordinate 
    Message_rect.y = 50; // controls the rect's y coordinte
    Message_rect.w = 95; // controls the width of the rect
    Message_rect.h = 75; // controls the height of the rect
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
    TTF_CloseFont(font);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void blit_angled(SDL_Texture *txtr, int x, int y, float angle, double zoom, SDL_Renderer *renderer)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(txtr, NULL, NULL, &dest.w, &dest.h);
	// Textures that are rotated MUST ALWAYS be rendered with their
	// center at (x, y) to have a symmetrical center of rotation:
	if (zoom > 0)
	{
		dest.w *= (int)zoom;
		dest.h *= (int)zoom;
	}
	if (zoom < 0)
	{
		dest.w /= (int)-zoom;
		dest.h /= (int)-zoom;	
	}
	dest.x -= dest.w / 2;
	dest.y -= dest.h / 2;
	// Look up what this function does. What do these rectangles
	// mean? Why is the source rectangle NULL? What are acceptable
	// values for the `angle' parameter?
	SDL_RenderCopyEx(renderer, txtr, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}

bool laserAlive(laser *laser)
{
    if(laser->dx || laser->dy)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool do_CollideLaserAsteroid(laser *laser, asteroid *asteroid)
{

    asteroid->center.x = asteroid->x + (32 * asteroid->size) /2;
    asteroid->center.y = asteroid->y + (32 * asteroid->size) /2;
    laser->center.x = laser->x +12/2;
    laser->center.y = laser->y +12/2;

    if (laser->radius + (asteroid->radius * (float)asteroid->size) >= sqrt(pow(laser->center.x-asteroid->center.x, 2) + pow(laser->center.y-asteroid->center.y, 2)))
    {

        return true;
    }
    else 
    {
        return false;
    }

}

bool do_CollideAsteroidShip(player *player, asteroid *asteroid)
{
    asteroid->center.x = asteroid->x + (32 * asteroid->size) /2;
    asteroid->center.y = asteroid->y + (32 * asteroid->size) /2;
    player->center.x = player->x +BODY_WIDTH/2;
    player->center.y = player->y +BODY_HEIGHT/2;
    
    if (player->radius + (asteroid->radius * (float)asteroid->size) >= sqrt(pow(player->center.x-asteroid->center.x, 2) + pow(player->center.y-asteroid->center.y, 2)))
    {

        return true;             
    }
    else 
    {
        return false;
    }
}

SDL_Texture *load_Texture(char *filename, SDL_Renderer *renderer)
{
    SDL_Texture *txtr;
    txtr  = IMG_LoadTexture(renderer, filename);
    
    return txtr;
}

int render(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, double angle, SDL_Point center, SDL_RendererFlip flip, SDL_Rect *src , int zoom)
{
	SDL_Rect dst;
	int res;	
	if (zoom > 0)
	{
		dst.w *= zoom;
		dst.h *= zoom;
	}
	if (zoom < 0)
	{
		dst.w /= -zoom;
		dst.h /= -zoom;	
	}
	res = SDL_RenderCopyEx(renderer, texture, src, &dst, angle, &center, flip);
	if (res != 0)
	{
		return res;
	}
	return SDL_RenderDrawPoint(renderer, x, y);
}

void resetGamestate(player *player, laser *laser, asteroid *asteroid)
{
    for (int j = 0; j < LASER_LIMIT; j++)
    {
        laser[j].x = -70;
        laser[j].y = -70; 
        laser[j].dx = 0;
        laser[j].dy = 0; 
        laser[j].angle = 0;
        laser[j].shot = 0;

        for (int i = 0; i < ASTEROID_LIMIT; i++)
        {
            asteroid[i].x = 0;
            asteroid[i].y = 0;
            asteroid[i].angle = 0;
            asteroid[i].size = (rand() % 5 +1);
            asteroid[i].delay = 120;
        }
    }

    player->x = SCREEN_WIDTH/2;
    player->y = SCREEN_HEIGHT/2;
    player->angle = 90;
    player->speed_x = 0;
    player->speed_y = 0;;
    player->shooting = 0;;
}

Mix_Chunk *loadSound(const char *path)
{
    Mix_Chunk *tmpChunk = Mix_LoadWAV(path); 

    if (tmpChunk != 0)
    {
        printf("Sound Error\n");
    }
    return tmpChunk;
}
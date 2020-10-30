#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Mod/Renderer/Renderer.h"
#include "Mod/Entity/Entity.h"
#include "Mod/Blit/Blit.h"
#include "Mod/Input/Input.h"
#include "Mod/Sound/Sound.h"
#include "Mod/Textures/Textures.h"
#include "Mod/Update/Update.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TRUE 1 
#define ASTEROID_LIMIT 5
#define ACCELERATION 1
#define BULLET_SPEED 32
#define SPEED 5.5
#define LASER_LIMIT 60
#define BODY_WIDTH 126
#define BODY_HEIGHT 108

time_t t;

unsigned int renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
unsigned int window_flags = 0;

int main(int argc, char *argv[])
{
    (void)argc;
	(void)argv;
    srand((unsigned)time(NULL));
    int i = 0, last_i;
    int offset = 28, done = 0, score = 0, scoreBlit = score;
    int life = 3, scoreLife = life;
    int Asteroid_Border;
    SDL_Rect explosion_rect;
    Uint32 starting_tick;                           //Creat Frame/tick
    SDL_Window *window = NULL;                      //Create a window
    SDL_Renderer *renderer = NULL;                  //Creat a renderer
    int channels = Mix_Playing(-1);                 
    int frame = 0;                           

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
    //init asteroids
    for (int i = 0; i < ASTEROID_LIMIT; i++)
    {
        asteroids[i].x = 0;
        asteroids[i].y = 0;
        asteroids[i].dx = (int)((rand () % 4 +1) * pow(-1, rand() % 2  +1));
        asteroids[i].dy = (int)((rand () % 4 +1) * pow(-1, rand() % 2  +1));
        asteroids[i].angle = 0;
        asteroids[i].size = (rand() % 5 +1);
        asteroids[i].delay = 0;
        asteroids[i].radius = 32/2;
        asteroids[i].center.x = 32/2;
        asteroids[i].center.y = 32/2;
        asteroids[i].radius *= (float)asteroids[i].size;
    }
    //init explosions
    for(int i = 0; i< ASTEROID_LIMIT; i++)
    {
        explosion[i].x = -70;
        explosion[i].y = -70;
        explosion[i].frame = -1;
        explosion[i].texture = load_Texture("Images/Explosion.png", renderer);
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow("Space Coookies Pew pew!",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
		SCREEN_HEIGHT, window_flags);
	if (window == NULL)
	{
		printf("Failed to create window -- Error: %s\n",
			SDL_GetError());
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, renderer_flags);
	if (renderer == NULL)
	{
		printf("Failed to create renderer -- Error: %s\n",
			SDL_GetError());
		exit(1);
	}

    TTF_Init();
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096)< 0)
    {
        printf("Sound Error: %s \n", Mix_GetError());
    }

    Mix_Music *bgm = Mix_LoadMUS("Sounds/BackgroundMusic.flac");
    Mix_Chunk *soundExplosion = loadSound("Sounds/Explosion.wav");
    Mix_Chunk *soundThruster = loadSound("Sounds/Engine.mp3");
    Mix_Chunk *soundLaser = loadSound("Sounds/Laser.mp3");

    initGamestate(&gameState, renderer);
    if(!Mix_PlayingMusic())
    {
        Mix_PlayMusic(bgm, -1);
    }

    while(!done)
    {   
        //Get frame tick
        starting_tick = SDL_GetTicks();
        
        //player acceleration
        Player.x += (int)(Player.speed_x / SPEED);
        Player.y -= (int)(Player.speed_y / SPEED);

        //Making the Playfield wraparound
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
        
        //Render the nonrecursive elements
        doRender(renderer, &gameState, &Player, &Thruster);

        //Read player inputs
        if(process_input(window, &Player, renderer, soundThruster, soundLaser) == 1)
        {
            done = 1;
        }
        //Move player via read inputs
        move(&Player, &Thruster);

        //spawning space cookies
        for(i =0; i < ASTEROID_LIMIT; i++)
        {   
            if (explosion[i].frame >= 0)
            {
                // printf("Collision! BOOOOM\n");
                explosion[i].frame++;
                if (explosion[i].frame == 24)
                {
                    explosion[i].frame = -1;
                    explosion[i].x = -200;
                    explosion[i].y = 100;
                }
                if (explosion[i].frame%2)
                {
                explosion_rect.x += explosion_rect.w;
                }
                startExplosion(explosion, renderer, &explosion_rect);
            }

            if(asteroids[i].delay > 0)
            {
                asteroids[i].delay --;
            }

            Asteroid_Border = (rand() % 4 + 1);

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
                //blit via callback function
                asteroidRadius(&asteroids[i], renderer, &gameState);
            }
            //if Asteroid is out of screen remove him
            if(asteroids[i].x < 0 || asteroids[i].x > SCREEN_WIDTH || asteroids[i].y < 0 || asteroids[i].y > SCREEN_HEIGHT)
                {
                    asteroids[i].x = 0;
                    asteroids[i].y = 0;
                    asteroids[i].angle = 0;
                    asteroids[i].size = (rand() % 5 +1);
                    //asteroidRadius(&asteroids[i], renderer, &gameState);
                    asteroids[i].delay = 120;
                }
            //Collision ship and Asteroid
            if(do_CollideAsteroidShip(&Player, &asteroids[i]))
            {  
                printf("Collision Cookie with Ship! %f \n", asteroids[i].radius);
                asteroids[i].x = -100;
                asteroids[i].y = -100;
                asteroids[i].angle = 0;
                asteroids[i].delay = 120;
                score --;
                life --;
                scoreLife = life;
            }
        }
        
        //spawning lasers
        for (int j =0; j < LASER_LIMIT; j++)
        {   
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
                addLaser(&Player, &lasers[j], Player.angle - 90, offset);
                Player.shift = UP;
                //Player.shooting = 0;
                lasers[j].shot = 1; 
                lasers[j].delay = LASER_LIMIT;

            }
        
            //jf laser alive blit hjm
            if(lasers[j].x > 0 && lasers[j].x < SCREEN_WIDTH && lasers[j].y > 0 && lasers[j].y < SCREEN_HEIGHT
            && (lasers[j].dx != 0 || lasers[j].dy != 0 ) && (lasers[j].shot == 1))
            {

                lasers[j].x += (int)lasers[j].dx;
                lasers[j].y += (int)lasers[j].dy;
                blit_angled(gameState.laser, lasers[j].x, lasers[j].y, (float)lasers[j].angle, 1, renderer);
            }
            
            //jf Laser dead restore it's values to null
            if((lasers[j].x <= 0 || lasers[j].x >= SCREEN_WIDTH || lasers[j].y <= 0 || lasers[j].y >= SCREEN_HEIGHT) && (lasers[j].dx != 0 || lasers[j].dy != 0))
            {
                lasers[j].x = -70;
                lasers[j].y = -70; 
                lasers[j].dx = 0;
                lasers[j].dy = 0; 
                lasers[j].angle = 0;
                lasers[j].shot = 0;
            }

        }
        
        //Collision detection and explosion
        for (int i = 0; i < ASTEROID_LIMIT; i++)
        {   
            for (int j = 0; j < LASER_LIMIT; j++)
            {
                if((do_CollideLaserAsteroid(&lasers[j], &asteroids[i]) == 1) && (asteroids[i].size == 5))
                {
                    score --;
                    scoreBlit = score; 
                    //printf("Collision Laser + COOKIE\n");
                    fflush(stdout);
                    if(channels < 6)
                    {
                        Mix_PlayChannel(3, soundExplosion, 0);
                    }

                    startExplosion(explosion, renderer, &explosion_rect);

                    //explode
                    lasers[j].x = -70;
                    lasers[j].y = -70; 
                    lasers[j].dx = 0;
                    lasers[j].dy = 0; 
                    lasers[j].angle = 0;
                    lasers[j].shot = 0;

                    asteroids[i].x = -200;
                    asteroids[i].y = -200;
                    asteroids[i].angle = 0;
                    asteroids[i].delay = 120;

                    if(j == 59 )
                    {
                        score++;
                    }
                }

                if(do_CollideLaserAsteroid(&lasers[j], &asteroids[i]) == 1)
                {  
                    
                    score ++;
                    scoreBlit = score; 
                    //printf("Collision Laser + COOKIE\n");
                    fflush(stdout);
                    if(channels < 6)
                    {
                        Mix_PlayChannel(3, soundExplosion, 0);
                    }

                    startExplosion(explosion, renderer, &explosion_rect);

                    //explode
                    lasers[j].x = -70;
                    lasers[j].y = -70; 
                    lasers[j].dx = 0;
                    lasers[j].dy = 0; 
                    lasers[j].angle = 0;
                    lasers[j].shot = 0;

                    asteroids[i].x = -200;
                    asteroids[i].y = -200;
                    asteroids[i].angle = 0;
                    asteroids[i].delay = 120;
                }
                
                if (scoreLife <= 0)
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
#include <stdio.h> // for printf() calls
#include <SDL2/SDL.h> // SDL_* functions, enum values and structs

void process_input(void);
void proper_shutdown(void);

// Define screen width parameters for a 16:9 ratio window.
// For example, you could use 1024x576, 1280x720 or 1920x1080.
// If you have a 16:10 screen, you could for example use 1024x640, etc.
#define SCREEN_WIDTH   1024
#define SCREEN_HEIGHT  576

// A graphical application consists of an SDL_Renderer
// and an SDL_Window. We'll manipulate both through
// their pointers. The actual `objects' are built 
// in memory using functions:
//
// SDL_Window is constructed by calling `SDL_CreateWindow()'
// Its declaration is ONLY available in the SDL library source code, 
// not in development libs, because its internals are platform-specific.
// Since its internal structure is useless to developers, it 
// is hidden.
//
// SDL_Renderer is constructed by calling `SDL_CreateRenderer()'
// More info: https://wiki.libsdl.org/SDL_Renderer
//
// These structures are created globally, because when passing them
// to functions we'll either need pointer-to-pointer or a function
// pointer that is unable to take arguments. We'll perfect this 
// later on, because you should avoid globals whenever you are able...
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(int argc, char *argv[])
{
	// We're compiling this in a very strict way, which means the 
	// compiler is going to whine about us not using argc and argv.
	// We might be using these later on, so let me show you an 
	// industry-accepted trick of `using' these variables without 
	// actually using them:
	(void)argc;
	(void)argv;

	// Window flags are enumerated in /usr/include/SDL2/SDL_video.h
	// Enum starts @ 1, so zero can be used as an indicator for
	// no flags set...
	// See: https://wiki.libsdl.org/SDL_WindowFlags
	// and: https://wiki.libsdl.org/CategoryVideo
	unsigned int window_flags = 0;
	// The renderer can take advantage of hardware acceleration if
	// it is present on the system and made available to the OS
	// through drivers.
	// See https://wiki.libsdl.org/SDL_RendererFlags
	unsigned int renderer_flags = SDL_RENDERER_ACCELERATED;
	
	// SDL_Init() takes flags (OR-ed together) to initialize different
	// subsystems, like SDL_INIT_VIDEO, _AUDIO, _JOYSTICK, _EVENTS, etc.
	// See https://wiki.libsdl.org/SDL_Init
	// For now let's just initialize the video subsystem:
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	// As for the second and third arguments of the SDL_CreateWindow()
	// function: SDL_WINDOWPOS_UNDEFINED lets the OS determine where to
	// place the window on screen. YMMV. Arg 4 and 5 control window 
	// size, arg 6 the window_flags we set earlier (i.e. environment 
	// settings). See https://wiki.libsdl.org/SDL_CreateWindow 
	// for possible alternatives.
	window = SDL_CreateWindow("Test 1", SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
		window_flags);
	if (window == NULL) // error handling:
	{
		printf("Failed to create window -- Error: %s\n",
			SDL_GetError());
		// Let the OS know shit went sideways when closing this program:
		exit(1);
	}
	
	// Analogous to the SDL_CreateWindow() function, arg 1 is the 
	// window to create the renderer in, arg 2 selects the rendering
	// driver (where `-1' selects the first one to accept the
	// requested flags) and arg 3 sets the renderer environment
	// through flags.
	// See https://wiki.libsdl.org/SDL_CreateRenderer
	renderer = SDL_CreateRenderer(window, -1, renderer_flags);
	if (renderer == NULL) // error handling:
	{
		printf("Failed to create renderer -- Error: %s\n",
			SDL_GetError());
		exit(1);
	}
	
	// Before entering the `game loop', set a color for the background:
	// https://wiki.libsdl.org/SDL_SetRenderDrawColor
	// If you want translucency, set the last argument to a value
	// lower than 255 (which represents a fully opaque color):
	SDL_SetRenderDrawColor(renderer, 120, 144, 156, 255);
	// Now clear the current rendering target with the drawing color,
	// i.e. since the renderer takes up the entire space of the 
	// window, set the entire renderer to a single color.
	// This will NOT be presented on the window, YET.
	// This `clearing of the renderer' is done on the video BACKBUFFER.
	// See the example @ https://wiki.libsdl.org/SDL_RenderClear
	SDL_RenderClear(renderer);

	while (1)
	{
		// Handle keyboard strokes and window-events:
		process_input();
		// Update the screen with any rendering performed since the 
		// previous call to SDL_RenderPresent(). In our case, we're not
		// drawing anything new while the application is running, so
		// this is here waiting for you to turn this application into
		// a proper game.
		SDL_RenderPresent(renderer);
		// Wait a specified number of milliseconds before processing
		// input again. This wait time will help determine the game's
		// framerate and input lag. Getting this wrong will render your
		// game unplayable. On the other hand: PC MASTER RACE FTW, so
		// at least 60 FPS of course! 1 / 60 ~ 0.016667 ms, so 16 ms
		// should give a solid 60 FPS when we're not drawing anything:
		SDL_Delay(16);
	}

	return 0;
}

void process_input(void)
{
	// SDL_Event will remain a bit of a mystery for now, since by 
	// nature, C is NOT a language that has event-enabled facilities 
	// hooked into the OS. Event handling can however be done in C, 
	// although the details are somewhat complicated. For now, let's 
	// assume this can actually work and concentrate on this thing 
	// called an `SDL_Event':

	// SDL_Event is a union (LOOK IT UP IN YOUR BOOK) that can `present' 
	// its data to the application through different `looking glasses'. 
	// Each of these looking glasses is, in reality, a struct. 
	// Some of these structs include (but are not limited to) 
	// `SDL_KeyboardEvent', `SDL_MouseWheelEvent' and `SDL_QuitEvent'.
	
	// The event sets its own type. When a type like
	// `SDL_KEYDOWN' or `SDL_KEYUP' is detected, we INFER that we're
	// looking at an `SDL_KeyboardEvent'. When a type `SDL_QUIT' is 
	// detected, we know we're dealing with an `SDL_QuitEvent'. This
	// mapping from event.type to union.member can be found here:
	// https://wiki.libsdl.org/SDL_Event#Remarks
	// (look for the table titled `Relationships between event types 
	// and union members').
	// All event types are listed here:
	// https://wiki.libsdl.org/SDL_EventType
	
	// So, when we want to handle the `close button' of the window, we
	// could use SDL_QUIT 
	// See: https://wiki.libsdl.org/SDL_EventType#SDL_QUIT
	// When we want to also close on pressing the ESC key, we'll be 
	// listening to the SDL_KEYDOWN event.

	// Events are put in a QUEUE (a buffer of sorts) in the order of
	// occurence. When you are good and ready, you can read any events
	// waiting in this queue by using the `SDL_PollEvent()'-function.
	// Calling this function places the next event from the queue in
	// the event variable (by passing it using `&event').
	// You may then proceed to read its type and determine a suitable
	// course of action based on this information.

	// In our case, we'll be reading the queue until it's empty on 
	// a fixed moment during our game loop --
	// see: https://wiki.libsdl.org/SDL_PollEvent

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				proper_shutdown();
				exit(0);
				break;
			case SDL_KEYDOWN:
				// https://wiki.libsdl.org/CategoryKeyboard
				// https://wiki.libsdl.org/SDL_Scancode
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					proper_shutdown();
					exit(0);
				}
				break;
			default:
				break;
		}
	}
}

void proper_shutdown(void)
{
	// Make sure to look up these functions @
	// https://wiki.libsdl.org/CategoryAPI
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

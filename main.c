// pong
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

#define PADDLE_H 50
#define PADDLE_W 5

#define BALL_SIZE 5
#define BALL_HALF (BALL_SIZE/2)

#define PLAYER_1_WIN 0
#define PLAYER_2_WIN 1

typedef struct {
    int xvel;
    int yvel;
    
    SDL_Rect geometry;
} game_object_t;

void game_object_update(SDL_Renderer* renderer, game_object_t* object) {
    SDL_RenderFillRect(renderer, &object->geometry);

    object->geometry.x += object->xvel;
    object->geometry.y += object->yvel;
}

void ball_move(game_object_t* ball, game_object_t* a, game_object_t* b) {
    if (ball->geometry.x > WINDOW_WIDTH-BALL_SIZE) ball->xvel = -1;
    if (ball->geometry.x < 0) ball->xvel = 1;

    if (ball->geometry.y > WINDOW_HEIGHT-BALL_SIZE) ball->yvel = -1;
    if (ball->geometry.y < 0) ball->yvel = 1;

    if(ball->geometry.x < a->geometry.x    + a->geometry.w &&
       ball->geometry.x + ball->geometry.w > a->geometry.x &&
       ball->geometry.y < a->geometry.y    + a->geometry.h &&
       ball->geometry.y + ball->geometry.h > a->geometry.y) {
	ball->xvel = 1;
    }

    if(ball->geometry.x < b->geometry.x    + b->geometry.w &&
       ball->geometry.x + ball->geometry.w > b->geometry.x &&
       ball->geometry.y < b->geometry.y    + b->geometry.h &&
       ball->geometry.y + ball->geometry.h > b->geometry.y) {
	ball->xvel = -1;
    }
}

void player_move(game_object_t* player, const unsigned char* keys) {
    if (keys[SDL_SCANCODE_DOWN]) {
	player->yvel = 1;
    } else if (keys[SDL_SCANCODE_UP]) {
	player->yvel = -1;
    } else {
	player->yvel = 0;
    }

    if (player->geometry.y > WINDOW_HEIGHT - player->geometry.h) {
	player->geometry.y = WINDOW_HEIGHT - player->geometry.h;
    }

    if (player->geometry.y < 0) {
	player->geometry.y = 0;
    }
}

int main(void) {
    SDL_Window*   window   = NULL;
    SDL_Renderer* renderer = NULL;
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
	fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
	return EXIT_FAILURE;
    }
    
    window = SDL_CreateWindow
	("pong", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    if (window == NULL) {
	fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
	return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer
	(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
	fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
	return EXIT_FAILURE;
    }

    TTF_Font* Sans = TTF_OpenFont("fonts/OpenSans-Bold.ttf", 24);

    SDL_Color White = { 255, 255, 255, 255 };

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "put your text here", White);

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect = {
        .x = 0,
	.y = 0,
	.w = 100,
	.h = 100
    };

    game_object_t ball = {
       .xvel = -1,
       .yvel = 1,
       .geometry = {
	   .x = WINDOW_WIDTH/2-BALL_HALF,
	   .y = WINDOW_HEIGHT/2-BALL_HALF,
	   .w = BALL_SIZE,
	   .h = BALL_SIZE
       }
    };

    game_object_t player = {
       .xvel = 0,
       .yvel = 0,
       .geometry = {
           .x = 10,
	   .y = WINDOW_HEIGHT/2,
	   .w = PADDLE_W,
	   .h = PADDLE_H
       }
    };

    game_object_t computer = {
       .xvel = 0,
       .yvel = 0,
       .geometry = {
	   .x = WINDOW_WIDTH-PADDLE_W-10,
	   .y = WINDOW_HEIGHT/2,
	   .w = PADDLE_W,
	   .h = PADDLE_H
       }
    };

    const unsigned char* keys = SDL_GetKeyboardState(NULL);
   
    bool isRunning = true;
    while (isRunning == true) {
	SDL_Delay(5);

	SDL_Event e;
	if (SDL_PollEvent(&e)) {
	    switch (e.type) {
	    case SDL_QUIT:
		isRunning = false;
		break;
	    }
	}

	// set background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// render here
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	ball_move(&ball, &player, &computer);
        game_object_update(renderer, &ball);
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	game_object_update(renderer, &computer);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	player_move(&player, keys);
        game_object_update(renderer, &player);

	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
   
    SDL_Quit();
    
    return 0;
}

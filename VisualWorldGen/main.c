#include <SDL.h>
#include <stdio.h>
#include <time.h>

//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

void initWorld(int world[][10], int rowSize, int colSize)
{
	int i, j;

	srand((unsigned)time(NULL));

	for (i = 0; i < rowSize; i++)
		for (j = 0; j < colSize; j++)
			world[i][j] = rand() % 6;
}

SDL_Renderer *initRenderer(SDL_Window *window, SDL_Renderer *renderer)
{
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! Error: %s\n", SDL_GetError());
		return false;
	}

	return renderer;
}

SDL_Window *initSDL(SDL_Window *window)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		return false;
	}
	
	window = SDL_CreateWindow("Visual World Generator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	return window;
}

void setBlockColor(SDL_Renderer* renderer, int num)
{
	switch (num)
	{
	case 0:	// green
		SDL_SetRenderDrawColor(renderer, 0x00, 0x7F, 0x00, 0xFF);
		break;
	case 1:	// blue
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
		break;
	case 2:	// grey
		SDL_SetRenderDrawColor(renderer, 0xC0, 0xC0, 0xC0, 0xFF);
		break;
	case 3:	// brown
		SDL_SetRenderDrawColor(renderer, 0xB8, 0x73, 0x33, 0xFF);
		break;
	case 4:	// dark green
		SDL_SetRenderDrawColor(renderer, 0x20, 0xB2, 0xAA, 0x2F);
		break;
	case 5:
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0x2F);
		break;
	default:
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		break;
	}
}

void renderWorldWithSpaces(SDL_Renderer *renderer, int world[][10], int rowSize, int colSize)
{
	int space;
	int i, j;
	int x, y, width, height;
	int startX, startY;

	space = 10;
	startX = startY = 0;
	if (SCREEN_WIDTH >= SCREEN_HEIGHT)
	{
		startX = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
		width = SCREEN_HEIGHT / colSize - space*2;
		height = SCREEN_HEIGHT / rowSize - space*2;
	}
	else if (SCREEN_WIDTH < SCREEN_HEIGHT)
	{
		startY = (SCREEN_HEIGHT - SCREEN_WIDTH) / 2;
		width = SCREEN_WIDTH / colSize - space*2;
		height = SCREEN_WIDTH / rowSize - space*2;
	}

	for (i = 0; i < rowSize; i++)
		for (j = 0; j < colSize; j++)
		{
			setBlockColor(renderer, world[i][j]);
			SDL_Rect rect = {space + (width + space*2)*i + startX, space + (height + space*2)*j + startY, width, height };
			SDL_RenderFillRect(renderer, &rect);
		}
}

void renderWorld(SDL_Renderer *renderer, int world[][10], int rowSize, int colSize)
{
	int i, j;
	int x, y, width, height;
	int startX, startY;

	startX = startY = 0;
	if (SCREEN_WIDTH >= SCREEN_HEIGHT)
	{
		startX = (SCREEN_WIDTH - SCREEN_HEIGHT) / 2;
		width = SCREEN_HEIGHT / colSize;
		height = SCREEN_HEIGHT / rowSize;
	}
	else if (SCREEN_WIDTH < SCREEN_HEIGHT)
	{
		startY = (SCREEN_HEIGHT - SCREEN_WIDTH) / 2;
		width = SCREEN_WIDTH / colSize;
		height = SCREEN_WIDTH / rowSize;
	}

	for (i = 0; i < rowSize; i++)
		for (j = 0; j < colSize; j++)
		{
			setBlockColor(renderer, world[i][j]);
			SDL_Rect rect = { width*i + startX, height*j + startY, width, height };
			SDL_RenderFillRect(renderer, &rect);
		}
}

int main(int argc, char* args[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Event e;

	int world[10][10];

	bool quit = false;

	initWorld(world, 10, 10);

	if ((window = initSDL(window)) && (renderer = initRenderer(window, renderer)))
	{
		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
					quit = true;
			}

			// Clear screen
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);

			// Render world
			renderWorld(renderer, world, 10, 10);

			// Update screen
			SDL_RenderPresent(renderer);
		}
	}

	// Free memory
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;
	
	SDL_Quit();

	return 0;
}
#include <SDL.h>
#include <vector>


enum class EDirection
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
};

void CheckIfPassTheWall(std::vector<SDL_Rect>& Body)
{
	if (Body.at(0).x >= 500)
	{
		Body.at(0).x = 0;
	}
	if (Body.at(0).x < 0)
	{
		Body.at(0).x = 500;
	}
	if (Body.at(0).y >= 500)
	{
		Body.at(0).y = 0;
	}
	if (Body.at(0).y < 0)
	{
		Body.at(0).y = 500;
	}
}

bool CheckIfBitsItself(const std::vector<SDL_Rect>& Body)
{
	for (size_t i = 1; i < Body.size() - 1; i++)
	{
		if (Body.at(0).x == Body.at(i).x && Body.at(0).y == Body.at(i).y)
			return true;
	}
	return false;
}

bool SnakeEatApple(SDL_Rect Apple, const std::vector<SDL_Rect> &Body)
{
	return Apple.x == Body.at(0).x && Apple.y == Body.at(0).y;
}

int main(int argc, char* argv[])
{
	// Setup Window
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* Window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, SDL_WINDOW_SHOWN);
	SDL_Renderer* Renderer = SDL_CreateRenderer(Window, -1, 0);
	SDL_Event Event;

	int MovementSpeed = 10;

	EDirection SnakeDirection = EDirection::RIGHT;

	std::vector<SDL_Rect> Body;
	SDL_Rect Head{ 250, 250, 10, 10 };
	Body.push_back(Head);

	SDL_Rect Apple{ rand() % 50 * 10, rand() % 50 * 10, 10, 10 };

	bool running = true;
	while (running)
	{
		// move previus node to the next node position
		for (size_t i = Body.size() - 1; i > 0; i--)
		{
			Body.at(i).x = Body.at(i - 1).x;
			Body.at(i).y = Body.at(i - 1).y;
		}
		while (SDL_PollEvent(&Event))
		{
			if (Event.type == SDL_QUIT)
				running = false;
			if (Event.type == SDL_KEYDOWN)
				if (Event.key.keysym.sym == SDLK_UP && SnakeDirection != EDirection::DOWN)
					SnakeDirection = EDirection::UP;
			if (Event.key.keysym.sym == SDLK_RIGHT && SnakeDirection != EDirection::LEFT)
				SnakeDirection = EDirection::RIGHT;
			if (Event.key.keysym.sym == SDLK_DOWN && SnakeDirection != EDirection::UP)
				SnakeDirection = EDirection::DOWN;
			if (Event.key.keysym.sym == SDLK_LEFT && SnakeDirection != EDirection::RIGHT)
				SnakeDirection = EDirection::LEFT;
		}
		
		// input Movement
		switch (SnakeDirection)
		{
		case EDirection::UP:
			Body.at(0).y -= MovementSpeed;

			break;
		case EDirection::RIGHT:
			Body.at(0).x += MovementSpeed;

			break;
		case EDirection::DOWN:
			Body.at(0).y += MovementSpeed;

			break;
		case EDirection::LEFT:
			Body.at(0).x -= MovementSpeed;

			break;

		default:
			break;
		}

		CheckIfPassTheWall(Body);

		// check if snake eats the apple and make the snake wrog
		if (SnakeEatApple(Apple, Body))
		{
			Apple.x = rand() % 50 * 10;
			Apple.y = rand() % 50 * 10;

			int NewX = Body.at(Body.size() - 1).x;
			int NewY = Body.at(Body.size() - 1).y;

			SDL_Rect NewRect{ NewX, NewY, 10, 10 };
			Body.push_back(NewRect);
		}


		// Clear window
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
		SDL_RenderClear(Renderer);

		running = !CheckIfBitsItself(Body);

		// Draw body
		for (size_t i = 0; i < Body.size(); i++)
		{
			SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(Renderer, &Body.at(i));
		}


		// Draw Apple
		SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(Renderer, &Apple);

		SDL_RenderPresent(Renderer);
		SDL_Delay(60);
	}

	return 0;
}
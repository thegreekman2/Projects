/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

For a C++ project simply rename the file to .cpp and re-run the build script 

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event 
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial 
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you 
--  wrote the original software. If you use this software in a product, an acknowledgment 
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"
#include <iostream>
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};
int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
	for(unsigned int i = 0; i < deque.size(); i++)
	{
		if(Vector2Equals(deque[i], element))
		{
			return true;
		}
	}
	return false;
}

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if(currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

class Food
{

public:
	Vector2 position;
	Texture2D texture;

	Food(deque<Vector2> snakeBody)
	{
		Image image = LoadImage("Graphics/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw(){
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
	}

	Vector2 GenerateRandomCell()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{x, y};
	}

	Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
	{
		Vector2 position = GenerateRandomCell();

		while(ElementInDeque(position, snakeBody))
		{
			position = GenerateRandomCell();
		}

		return position;
	}

};

class Snake
{
	public:
	deque<Vector2> body = {Vector2{9, 9}, Vector2{8, 9}, Vector2{7, 9},
							Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9},
							Vector2{3, 9}, Vector2{2, 9}, Vector2{1, 9}};
	Vector2 direction = {1, 0};
	bool addSegment = false;

	void Draw()
	{
		for(int i = 0; i < body.size(); i++)
		{
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{offset + x * cellSize,
			 offset + y * cellSize, (float)cellSize, (float)cellSize};
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);

		}
	}

	void Update()
	{
		body.push_front(Vector2Add(body[0], direction));
		if(addSegment == true)
		{
			addSegment = false;
			
		}
		else{
			body.pop_back();
		}

	}

	void Reset()
	{
		body = {Vector2{9, 9}, Vector2{8, 9}, Vector2{7, 9},
				Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9},
				Vector2{3, 9}, Vector2{2, 9}, Vector2{1, 9}};
		Vector2 direction = {1, 0};
	}

};

class Game
{
	public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;
	int score = 0;
	Sound eatSound;
	Sound wallSound;

	Game()
	{
		InitAudioDevice();
		eatSound = LoadSound("Sounds/eat.mp3");
		wallSound = LoadSound("Sounds/wall.mp3");

	}

	~Game()
	{
		UnloadSound(eatSound);
		UnloadSound(wallSound);
		CloseAudioDevice();
	}

	void Draw()
	{
		food.Draw();
		snake.Draw();
	}

	void Update()
	{
		if(running)
		{
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdges();
			CheckCollisionWithTail();
		}
	}

	void CheckCollisionWithFood()
	{
		if(Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.GenerateRandomPos(snake.body);
			snake.addSegment = true;
			score ++;
			PlaySound(eatSound);
		}
	}

	void CheckCollisionWithEdges()
	{
		if(snake.body[0].x == -1 || snake.body[0].x == cellCount)
		{
			GameOver();
		}
		if(snake.body[0].y == -1 || snake.body[0].y == cellCount)
		{
			GameOver();
		}
	}

	void GameOver()
	{
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		running = false;
		score = 0;
		PlaySound(wallSound);
	}

	void CheckCollisionWithTail()
	{
		deque<Vector2> headlessBody = snake.body;
		headlessBody.pop_front();
		if(ElementInDeque(snake.body[0], headlessBody))
		{
			GameOver();
		}
	}
};

int main ()
{
	cout << "Starting the game" << "\n";

	InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake)");
	SetTargetFPS(60);
	
	Game game = Game();

	while (WindowShouldClose() == false){
		BeginDrawing();

		if(eventTriggered(0.2))
		{
			game.Update();
		}

		if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
		{
			game.snake.direction = {0, -1};
			game.running = true;
		}
		if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
		{
			game.snake.direction = {0, 1};
			game.running = true;
		}
		if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
		{
			game.snake.direction = {1, 0};
			game.running = true;
		}
		if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
		{
			game.snake.direction = {-1, 0};
			game.running = true;
		}

		ClearBackground(green);
		DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, 
			(float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5,
		 	darkGreen);
			DrawText("Retrooo Snakeee", offset - 5, 20, 40, darkGreen);
			DrawText(TextFormat("%i", game.score * 1000), offset - 5 , offset + cellSize * cellCount + 10, 40, darkGreen);
		game.Draw();


		EndDrawing();
	}

	return 0;
}

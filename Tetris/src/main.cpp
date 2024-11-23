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

#include <raylib.h>
#include "game.h"
#include "constants.h"
#include "colors.h"
#include <iostream>

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

double lastUpdateTime = 0;


bool EventTriggered(double interval)
{
	double currentTime = GetTime();
	if(currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}


int main ()
{
	Color darkBlue = {44, 44, 127, 255};
	InitWindow(gridWidth + windowOffset, gridHeight + gridOffset, "Raylib Tetris");
	SetTargetFPS(60);

	Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);




	Game game = Game();


	while(WindowShouldClose() == false)
	{
		UpdateMusicStream(game.bgMusic);
		game.HandleInput();
		if(EventTriggered(.35))
		{
			game.MoveBlockDown();
		}		
		BeginDrawing();
		ClearBackground(darkBlue);
		
		
		if(!game.gameIsOver)
		{
			DrawTextEx(font, "Next", {370, 125}, 38, 2, WHITE);
			DrawRectangleRounded({320, 165, 170, 180}, 0.3, 6, lightBlue);

			DrawTextEx(font, "NxtNxt", {345, 350}, 38, 2, WHITE);
			DrawRectangleRounded({320, 395, 170, 180}, 0.3, 6, lightBlue);
		}
		else
		{
			DrawTextEx(font, "GAME OVER", {320, 450}, 38, 2, WHITE);
			DrawTextEx(font, "Press to restart", {330, 500}, 18, 1, WHITE);
		}

		DrawTextEx(font, "SKORE", {360, 15}, 38, 2, WHITE);
		DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);
		char scoreText[10];
		sprintf(scoreText, "%d", game.score);
		Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
		DrawTextEx(font, scoreText, {320 + (170 - textSize.x)/2, 65}, 38, 2, WHITE);
		game.Draw();
		EndDrawing();
	}

	CloseWindow();
}

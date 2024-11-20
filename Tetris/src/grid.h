#pragma once
#include <vector>
#include <raylib.h>

class Grid
{
public:
    Grid();
    void Initialize();
    int grid[20][10];
    void Print();
    void Draw();

private:
    std::vector<Color> GetCellColors();
    int numRows;
    int numCols;
    int cellSize;
    std::vector<Color> colors;


};


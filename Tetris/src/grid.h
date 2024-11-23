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
    bool IsCellOutside(int row, int column);
    bool IsCellEmpty(int row, int col);
    int ClearFullRows();

    

private:
    bool isRowFull(int row);
    void clearRow(int row);
    void MoveRowDown(int row, int numRows);
    int numRows;
    int numCols;
    int cellSize;
    std::vector<Color> colors;


};




#include "block.h"
#include <iostream>
#include "constants.h"

Block::Block()
{
    cellSize = 30;
    rotationState = 0;
    colors = GetCellColors();
    row_offset = 0; 
    column_offset = 0;
}

void Block::Draw(int offsetX, int offsetY)
{
    std::vector<Position> tiles = GetCellPositions();
    for(Position item: tiles)
    {
        DrawRectangle(item.column * cellSize + offsetX, item.row * cellSize + offsetY, cellSize - 1, cellSize -1, colors[id]);
    }
    
}

void Block::Move(int rows, int columns)
{
    row_offset += rows;
    column_offset += columns;
}

std::vector<Position> Block::GetCellPositions()
{
    std::vector<Position> tiles = cells[rotationState];
    std::vector<Position> movedTiles;
    for(Position item: tiles)
    {
        Position newPos = Position(item.row + row_offset, item.column + column_offset);
        movedTiles.push_back(newPos);
    }
    return movedTiles;
}

void Block::Rotate()
{
    rotationState ++;
    if(rotationState > (int)cells.size() - 1)
    {
        rotationState = 0;
    }
}

void Block::undoRotation()
{
    rotationState --;
    if (rotationState == -1)
    {
        rotationState = cells.size() - 1;
    }
}

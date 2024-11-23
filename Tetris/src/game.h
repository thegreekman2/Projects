#pragma once
#include "blocks.cpp"
#include "grid.h"


class Game 
{

public:
    Game();
    ~Game();
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    bool gameIsOver;
    int score;
    Music bgMusic;


private:
    Block GetRandomBlock();
    std::vector<Block> resetBlocks();
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Block nextnextBlock;
    Grid grid;
    void MoveBlockLeft();
    void MoveBlockRight();
    bool IsBlockOutside();
    void fwdRotateBlock();
    void bckwdRotateBlock();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UpdateScore(int linesCleared, int moveDownPoints);
    Sound rotateSound;
    Sound lowClearSound;
    Sound highClearSound;


};
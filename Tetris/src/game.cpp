#include "game.h"
#include <random>



Game::Game()
{
    grid = Grid();
    blocks = resetBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    nextnextBlock = GetRandomBlock();
    gameIsOver = false;
    score = 0;
    InitAudioDevice();
    bgMusic = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(bgMusic);
    rotateSound = LoadSound("Sounds/rotate.mp3");
    lowClearSound = LoadSound("Sounds/clear.mp3");
    highClearSound = LoadSound("Sounds/score.mp3");

}

Game::~Game()
{
    UnloadMusicStream(bgMusic);
    UnloadSound(rotateSound);
    UnloadSound(lowClearSound);
    UnloadSound(highClearSound);
    CloseAudioDevice();
    
}

Block Game::GetRandomBlock()
{
    if(blocks.empty())
    {
        blocks = resetBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;

}

std::vector<Block> Game::resetBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(),
              SBlock(), TBlock(), ZBlock()};
}

void Game::Draw()
{
    grid.Draw();
    currentBlock.Draw(11, 11);
    if(!gameIsOver)
    {
        switch(nextBlock.id)
        {
            case 3:
                nextBlock.Draw(255,240);
                break;
            case 4:
                nextBlock.Draw(255,230);
                break;
            default:
                nextBlock.Draw(270, 220);
                break;
        }
        switch(nextnextBlock.id)
        {
            case 3:
                nextnextBlock.Draw(255,470);
                break;
            case 4:
                nextnextBlock.Draw(255,460);
                break;
            default:
                nextnextBlock.Draw(270, 450);
                break;
        }
    }
}

void Game::HandleInput()
{
    int keyPressed = GetKeyPressed();
    if(gameIsOver && keyPressed != 0)
    {
        gameIsOver = false;
        Reset();
    }
    switch(keyPressed)
    {
        case KEY_LEFT:
            MoveBlockLeft();
            break;
        case KEY_RIGHT:
            MoveBlockRight();
            break;
        case KEY_DOWN:
            MoveBlockDown();
            UpdateScore(0,1);
            break;
        case KEY_R:
            fwdRotateBlock();
            break;
        case KEY_E:
            bckwdRotateBlock();
            break;
    }
}

void Game::MoveBlockLeft()
{
    if(!gameIsOver)
    {
        currentBlock.Move(0, -1);
        if(IsBlockOutside() || BlockFits() == false ){
            currentBlock.Move(0,1);
        }
    }
}

void Game::MoveBlockRight()
{
    if(!gameIsOver)
    {
        currentBlock.Move(0, 1);
        if(IsBlockOutside() || BlockFits() == false ){
            currentBlock.Move(0,-1);
        }
    }
}

void Game::MoveBlockDown()
{
    if(!gameIsOver)
    {
        currentBlock.Move(1, 0);
        if(IsBlockOutside() || BlockFits() == false){
            currentBlock.Move(-1,0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        if(grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::fwdRotateBlock()
{
    if(!gameIsOver)
    {
        currentBlock.Rotate();
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.undoRotation();
        }
        else
        {
            PlaySound(rotateSound);
        }
    }
}

void Game::bckwdRotateBlock()
{
    if(!gameIsOver)
    {
        currentBlock.undoRotation();
        if(IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Rotate();
        }
        else
        {
            PlaySound(rotateSound);
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if(BlockFits() == false)
    {
        gameIsOver = true;
    }
    nextBlock = nextnextBlock;
    nextnextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if(rowsCleared > 1){
        PlaySound(highClearSound);
        UpdateScore(rowsCleared, 0);
    }
    else if (rowsCleared > 0)
    {
        PlaySound(lowClearSound);
        UpdateScore(rowsCleared, 0);
    }
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for(Position item: tiles)
    {
        if(grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = resetBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    nextnextBlock = GetRandomBlock();
    score = 0;
}

void Game::UpdateScore(int linesCleared, int moveDownPoints)
{
    switch(linesCleared)
    {
        case 1:
            score += 100;
            break;
        case 2:
            score += 300;
            break;
        case 3:
            score += 500;
            break;
        case 4:
            score += 1000;
            break;
        default:
            break;
    }
    score += moveDownPoints;
}

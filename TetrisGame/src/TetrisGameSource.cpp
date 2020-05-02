// (C) Stipl3x 2020

/*
 * Tetris Game Source file. This file provides the game logic
 * and the main UI. It uses the ConsoleEngine provided by
 *
 * GNU GPLv3
 * (C) Stipl3x 2020. This is just an example of application.
 *
 * Enjoy!:D
 */

#include "graphics/graphics.hpp"
#include <time.h>

// Console properties
constexpr int WIDTH = 12;
constexpr int HEIGHT = 22;
constexpr int W_PADDING = 2;
constexpr int H_PADDING = 1;
constexpr char BORDER_FONT = '#';

// Game objects
constexpr int SHAPE_WIDTH = 4;
constexpr int SHAPE_HEIGHT = 4;

// Start position for a shape
int currentXPostion = WIDTH / 2 - SHAPE_WIDTH / 2;
int currentYPosition = 1;

char shapeAsset[7][16] = {  // L shape
                           {' ', 'X', 'X', ' ',
                            ' ', 'X', ' ', ' ',
                            ' ', 'X', ' ', ' ',
                            ' ', ' ', ' ', ' '},

                            // J shape
                           {' ', 'X', 'X', ' ',
                            ' ', ' ', 'X', ' ',
                            ' ', ' ', 'X', ' ',
                            ' ', ' ', ' ', ' '},

                            // I shape
                           {' ', 'X', ' ', ' ',
                            ' ', 'X', ' ', ' ',
                            ' ', 'X', ' ', ' ',
                            ' ', 'X', ' ', ' '},

                            // Block shape
                           {' ', 'X', 'X', ' ',
                            ' ', 'X', 'X', ' ',
                            ' ', ' ', ' ', ' ',
                            ' ', ' ', ' ', ' '},

                            // T shape
                           {' ', 'X', ' ', ' ',
                            ' ', 'X', 'X', ' ',
                            ' ', 'X', ' ', ' ',
                            ' ', ' ', ' ', ' '},

                            // 4 shape
                           {' ', 'X', ' ', ' ',
                            ' ', 'X', 'X', ' ',
                            ' ', ' ', 'X', ' ',
                            ' ', ' ', ' ', ' '},

                            // IDK shape :D
                           {' ', ' ', 'X', ' ',
                            ' ', 'X', 'X', ' ',
                            ' ', 'X', ' ', ' ',
                            ' ', ' ', ' ', ' '} };

// Game instance
SCE::graphics::ConsoleEngine tetrisBoard;

// Game loop functions
bool WantsToStartNewGame();
void StartNewGame();
void RunGame();
void EndGame();

void ProcessInput(char* t_currentShape);
void RotateShape(char* t_currentShape);
void CopyShape(char* t_currentShape, char* t_shapeToBeCopied);
bool CompareShapes(char* t_currentShape, char* t_shapeToBeCompared);
void CheckInitSpaceFor(char* t_currentShape);

int main()
{
    tetrisBoard.setCursorVisibility(false);

    // Game loop start
    while (WantsToStartNewGame())
    {
        StartNewGame();

        RunGame();

        EndGame();
    }
    // Game loop end

    SCE_CONSOLE_INPUT.get();
}

bool WantsToStartNewGame()
{
    tetrisBoard.clearConsoleScreen();

    SCE_CONSOLE_OUTPUT << "Your last score was: " << tetrisBoard.getMyScore() << SCE_CONSOLE_NEW_LINE;
    SCE_CONSOLE_OUTPUT << "Press Enter key to start a new game...";
    
    // Wait for the user to press Enter
    while (!tetrisBoard.isThisKeyPressed(VK_RETURN));

    tetrisBoard.clearConsoleScreen();

    return true;
}

void StartNewGame()
{
    tetrisBoard.reset();
    tetrisBoard.createGameScreen(WIDTH, HEIGHT, W_PADDING, H_PADDING, BORDER_FONT);

    return;
}

void RunGame()
{
    constexpr int NUMBER_OF_TICKS = 15;

    bool b_isRunning = true;
    bool b_isTimeToUpdate = false;

    int currentTick = 0;

    // Generate a random seed every new game
    srand(time(0));

    int randomShapeNumber = rand() % 7;
    int randomRotator = rand() % 4;
    char currentShape[16];
    char futureShape[16];

    // Create the first random shape to use and apply a random rotation
    CopyShape(currentShape, shapeAsset[randomShapeNumber]);
    for (int rotateIndex = 0; rotateIndex < randomRotator; rotateIndex++)
    {
        RotateShape(currentShape);
    }
    
    // Create the next random shape to use and apply a random rotation
    randomShapeNumber = rand() % 7;
    CopyShape(futureShape, shapeAsset[randomShapeNumber]);
    for (int rotateIndex = 0; rotateIndex < randomRotator; rotateIndex++)
    {
        RotateShape(futureShape);
    }

    // Make sure the shape is on the first line
    CheckInitSpaceFor(currentShape);

    tetrisBoard.renderGameScreen();
    tetrisBoard.renderGameObject(currentShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion, currentYPosition);
    tetrisBoard.displayFutureGameObject(futureShape, SHAPE_WIDTH, SHAPE_HEIGHT);

    while (b_isRunning)
    {
        // Aprox time for a single tick
        Sleep(50);
        currentTick++;

        if (currentTick >= NUMBER_OF_TICKS)
        {
            b_isTimeToUpdate = true;
        }

        ProcessInput(currentShape);

        if (b_isTimeToUpdate)
        {
            b_isTimeToUpdate = false;
            currentTick = 0;

            // Game can continue
            if (!tetrisBoard.isGoingToCollide(currentShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion, currentYPosition + 1))
            {
                currentYPosition++;
            }
            // Update game and add a new piece
            else
            {
                for (int heightIndex = 0; heightIndex < 4; heightIndex++)
                {
                    for (int widthIndex = 0; widthIndex < 4; widthIndex++)
                    {
                        tetrisBoard.changeAtPosition(currentXPostion + widthIndex, currentYPosition + heightIndex,
                            currentShape[heightIndex * 4 + widthIndex]);
                    }
                }

                tetrisBoard.checkForLines();

                // Restart the position
                currentXPostion = WIDTH / 2 - SHAPE_WIDTH / 2;
                currentYPosition = 1;

                // Update the current shape with the next shape
                CopyShape(currentShape, futureShape);

                // Create a new next shape and apply a random rotation
                randomShapeNumber = rand() % 7;
                CopyShape(futureShape, shapeAsset[randomShapeNumber]);
                randomRotator = rand() % 4;
                for (int rotateIndex = 0; rotateIndex < randomRotator; rotateIndex++)
                {
                    RotateShape(futureShape);
                }

                // Make sure it is on first line
                CheckInitSpaceFor(currentShape);
            }

            tetrisBoard.renderGameScreen();
            tetrisBoard.renderGameObject(currentShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion, currentYPosition);
            tetrisBoard.displayFutureGameObject(futureShape, SHAPE_WIDTH, SHAPE_HEIGHT);
        }

        // If a new piece was generated and it collides with the board, then end game
        if (tetrisBoard.isGoingToCollide(currentShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion, currentYPosition))
        {
            b_isRunning = false;
        }
    }
    
    return;
}

void EndGame()
{
    tetrisBoard.clearConsoleScreen();
    SCE_CONSOLE_OUTPUT << "GAME OVER!!! THANK YOU FOR PLAYING!!!";
    Sleep(1000);

    return;
}

void ProcessInput(char* t_currentShape)
{
    constexpr int ROTATE_DELAY = 5;

    bool b_newMove = false;
    static int rotateCounter = ROTATE_DELAY;

    // Increase the counter
    rotateCounter++;

    // Left move
    if (tetrisBoard.isThisKeyPressed('A') || tetrisBoard.isThisKeyPressed(VK_LEFT))
    {
        if (!tetrisBoard.isGoingToCollide(t_currentShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion - 1, currentYPosition))
        {
            currentXPostion--;
            b_newMove = true;
        }
    }

    // Right move
    else if (tetrisBoard.isThisKeyPressed('D') || tetrisBoard.isThisKeyPressed(VK_RIGHT))
    {
        if(!tetrisBoard.isGoingToCollide(t_currentShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion + 1, currentYPosition))
        {
            currentXPostion++;
            b_newMove = true;
        }
    }

    // Down move
    else if (tetrisBoard.isThisKeyPressed('S') || tetrisBoard.isThisKeyPressed(VK_DOWN))
    {
        if (!tetrisBoard.isGoingToCollide(t_currentShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion, currentYPosition + 1))
        {
            currentYPosition++;
            b_newMove = true;
        }
    }

    else if (tetrisBoard.isThisKeyPressed(VK_SPACE))
    {
        if (rotateCounter >= ROTATE_DELAY)
        {
            rotateCounter = 0;
            RotateShape(t_currentShape);
            b_newMove = true;
        }
    }

    if (b_newMove)
    {
        tetrisBoard.renderGameScreen();
        tetrisBoard.renderGameObject(t_currentShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion, currentYPosition);
    }

    return;
}

void RotateShape(char* t_currentShape)
{
    char auxShape[16];

    // All cases, but the box
    if (!CompareShapes(t_currentShape, shapeAsset[3]))
    {
        // Create a copy
        CopyShape(auxShape, t_currentShape);

        // Rotate the copy
        for (int heightIndex = 0; heightIndex < SHAPE_HEIGHT; heightIndex++)
        {
            for (int widthIndex = 0; widthIndex < SHAPE_WIDTH; widthIndex++)
            {
                auxShape[heightIndex * SHAPE_HEIGHT + widthIndex] =
                    t_currentShape[SHAPE_WIDTH * SHAPE_HEIGHT - (widthIndex + 1) * 4 + heightIndex];
            }
        }

        // If it is ok to rotate, then update the current shape
        if (!tetrisBoard.isGoingToCollide(auxShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion, currentYPosition))
        {
            CopyShape(t_currentShape, auxShape);
        }
    }

    return;
}

void CopyShape(char* t_currentShape, char* t_shapeToBeCopied)
{
    for (int index = 0; index < SHAPE_WIDTH * SHAPE_HEIGHT; index++)
    {
        t_currentShape[index] = t_shapeToBeCopied[index];
    }
}

bool CompareShapes(char* t_currentShape, char* t_shapeToBeCompared)
{
    for (int index = 0; index < SHAPE_WIDTH * SHAPE_HEIGHT; index++)
    {
        if (t_currentShape[index] != t_shapeToBeCompared[index])
        {
            return false;
        }
    }

    return true;
}

void CheckInitSpaceFor(char* t_currentShape)
{
    while (!tetrisBoard.isGoingToCollide(t_currentShape, SHAPE_WIDTH, SHAPE_HEIGHT, currentXPostion, currentYPosition - 1))
    {
        currentYPosition--;
    }

    return;
}

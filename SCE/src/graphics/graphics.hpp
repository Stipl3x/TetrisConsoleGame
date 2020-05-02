#pragma once

// (C) Stipl3x 2020

/*
 * The ConsoleEngine class provides engine functionality for any
 * console based game. The class allocates memory for a pointer as game buffer
 * with the origin on top-left in a XY coordinates system displayed on the console.
 * Width represents the number of characters on the horizontaly side and
 * Height  represents the number of characters verticaly,
 * both starting at 0 at the most top-left position.
 * The index for the pointer in the XY system is calculated as:
 * Y_position * Width + X_position, meaning you skip Y lines and add X columns.
 *
 * GNU GPLv3
 * (C) Stipl3x 2020
 *
 */

#define SCE_CONSOLE_OUTPUT std::cout
#define SCE_CONSOLE_INPUT std::cin
#define SCE_CONSOLE_NEW_LINE std::endl

#include <iostream>
#include <Windows.h>

namespace SCE { namespace graphics {

    class ConsoleEngine
    {
    public:
        ConsoleEngine(); // Constructor
        ~ConsoleEngine(); // Destructor

        //*****Public Variables*****


        //*****Public Methods*****
        // Getters
        int getMyScore() const;

        // Essential game functions
        void reset();
        void createGameScreen(int t_width, int t_height, int t_widthPadding, int t_heightPadding, char t_borderFont);
        void renderGameScreen();
        void renderGameObject(char* t_currentGameObject, int t_width, int t_height, int t_currentXPosition, int t_currentYPosition);
        void displayFutureGameObject(char* t_futureGameObject, int t_width, int t_height);

        // Game checkers
        bool isBorder(int t_widthIndex, int t_heightIndex) const;
        bool isGoingToCollide(char* t_currentGameObject, int t_width, int t_height, int t_currentXPosition, int t_currentYPosition) const;

        // Console components - Windows
        void setCursorVisibility(bool t_visibiltyFlag);
        void moveCursorTo(int t_widthIndex, int t_heightIndex);
        void clearConsoleScreen();
        bool isThisKeyPressed(int t_keyToCheck) const;

        // Game instance modifiers
        void changeAtPosition(int t_widthIndex, int t_heightIndex, char t_newFont);
        void checkForLines();
        void updateGameBoard(int t_lineNumber);



        //*****Only hidden class stuff*****
    private:
        //*****Private Variables*****
        // Game properties
        int m_width;
        int m_height;
        char m_borderFont;
        char m_emptyFont;

        int m_firstColumn;
        int m_lastColumn;
        int m_firstLine;
        int m_lastLine;

        int m_widthPadding;
        int m_heightPadding;

        // Game instance
        char* m_gameInstance;

        // Game components
        int m_score;

        //*****Private Methods*****
        void this_displayScore();
        void this_displayLogo();
    };

} }

// (C) Stipl3x 2020

#include "graphics.hpp"

namespace SCE { namespace graphics {

    ConsoleEngine::ConsoleEngine() { reset(); }
    ConsoleEngine::~ConsoleEngine() { }

    //********************************************************************************

    int ConsoleEngine::getMyScore() const { return m_score; }

    //********************************************************************************

    void ConsoleEngine::reset()
    {
        // Reset all the private variables
        m_width = 0;
        m_height = 0;
        m_borderFont = ' ';
        m_emptyFont = ' ';

        m_firstColumn = 0;
        m_lastColumn = 0;
        m_firstLine = 0;
        m_lastLine = 0;

        m_widthPadding = 0;
        m_heightPadding = 0;

        // If MyGameInstance was used before, clear it's memory
        if (m_gameInstance != nullptr)
        {
            delete[] m_gameInstance;
        }
        m_gameInstance = nullptr;

        m_score = 0;

        moveCursorTo(0, 0);
    }

    void ConsoleEngine::createGameScreen(int t_width, int t_height, int t_widthPadding, int t_heightPadding, char t_borderFont)
    {
        // Update the variables
        m_width = t_width;
        m_height = t_height;
        m_borderFont = t_borderFont;

        // Update the indexes for borders
        m_firstColumn = 0;
        m_lastColumn = m_width - 1;
        m_firstLine = 0;
        m_lastLine = m_height - 1;

        // Update paddings used to move the game position on console
        m_widthPadding = t_widthPadding;
        m_heightPadding = t_heightPadding;

        // Should be treated as an array
        m_gameInstance = new char[m_width * m_height];
        if (m_gameInstance == nullptr)
        {
            SCE_CONSOLE_OUTPUT << "Error initializing GameTable memory!\n";
            exit(0);
        }

        // Initialize the game instance with border from the beginning
        for (int heightIndex = 0; heightIndex < m_height; heightIndex++)
        {
            for (int widthIndex = 0; widthIndex < m_width; widthIndex++)
            {
                if (isBorder(widthIndex, heightIndex))
                {
                    m_gameInstance[heightIndex * m_width + widthIndex] = m_borderFont;
                }
                else
                {
                    m_gameInstance[heightIndex * m_width + widthIndex] = m_emptyFont;
                }
            }
        }

        return;
    }

    void ConsoleEngine::renderGameScreen()
    {
        // Print the playground
        for (int heightIndex = 0; heightIndex < m_height; heightIndex++)
        {
            for (int widthIndex = 0; widthIndex < m_width; widthIndex++)
            {
                // Add paddings to the indexes to move the game position on console
                moveCursorTo(widthIndex + m_widthPadding, heightIndex + m_heightPadding);
                SCE_CONSOLE_OUTPUT << m_gameInstance[heightIndex * m_width + widthIndex];
            }
        }

        // Display score
        this_displayScore();

        // Display the name of the engine creator
        this_displayLogo();

        return;
    }

    void ConsoleEngine::renderGameObject(char* t_currentGameObject, int t_width, int t_height, int t_currentXPosition, int t_currentYPosition)
    {
        for (int heightIndex = 0; heightIndex < t_height; heightIndex++)
        {
            for (int widthIndex = 0; widthIndex < t_width; widthIndex++)
            {
                // Render only blocks that have font on them
                if (t_currentGameObject[heightIndex * t_height + widthIndex] != m_emptyFont)
                {
                    moveCursorTo(m_widthPadding + t_currentXPosition + widthIndex,
                        m_heightPadding + t_currentYPosition + heightIndex);
                    SCE_CONSOLE_OUTPUT << t_currentGameObject[heightIndex * t_width + widthIndex];
                }
            }
        }

        return;
    }

    // Made it because it renders outside the game board
    void ConsoleEngine::displayFutureGameObject(char* t_futureGameObject, int t_width, int t_height)
    {
        moveCursorTo(2 * m_widthPadding + m_width, m_heightPadding + 5);
        SCE_CONSOLE_OUTPUT << "Next piece:";

        for (int heightIndex = 0; heightIndex < t_height; heightIndex++)
        {
            for (int widthIndex = 0; widthIndex < t_width; widthIndex++)
            {
                moveCursorTo(2 * m_widthPadding + m_width + widthIndex + 3, m_heightPadding + 5 + 2 + heightIndex);
                SCE_CONSOLE_OUTPUT << t_futureGameObject[heightIndex * t_width + widthIndex];
            }
        }

        return;
    }

    //********************************************************************************

    bool ConsoleEngine::isBorder(int t_widthIndex, int t_heightIndex) const
    {
        if (t_widthIndex == m_firstColumn || t_widthIndex == m_lastColumn ||
            t_heightIndex == m_firstLine || t_heightIndex == m_lastLine)
        {
            return true; // It is a border
        }

        return false;
    }

    bool ConsoleEngine::isGoingToCollide(char* t_currentGameObject, int t_width, int t_height, int t_currentXPosition, int t_currentYPosition) const
    {
        for (int heightIndex = 0; heightIndex < t_height; heightIndex++)
        {
            for (int widthIndex = 0; widthIndex < t_width; widthIndex++)
            {
                // Check to see if the space at this position is occupied,
                if (m_gameInstance[(t_currentYPosition + heightIndex) * m_width + (t_currentXPosition + widthIndex)] != m_emptyFont)
                {
                    // and also that the game object is occupied at this position
                    if (t_currentGameObject[heightIndex * t_width + widthIndex] != m_emptyFont)
                    {
                        return true; // Is going to collide
                    }
                }
            }
        }

        return false;
    }

    //********************************************************************************

    void ConsoleEngine::setCursorVisibility(bool t_visibiltyFlag)
    {
        HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        cursorInfo.dwSize = 100;
        cursorInfo.bVisible = t_visibiltyFlag;
        SetConsoleCursorInfo(handleOut, &cursorInfo);

        return;
    }

    void ConsoleEngine::moveCursorTo(int t_widthIndex, int t_heightIndex)
    {
        HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord = { t_widthIndex, t_heightIndex };
        SetConsoleCursorPosition(handleOut, coord);

        return;
    }

    void ConsoleEngine::clearConsoleScreen()
    {
        system("cls");
        moveCursorTo(0, 0);

        return;
    }

    // This returns instant response when called
    bool ConsoleEngine::isThisKeyPressed(int t_keyToCheck) const
    {
        if (GetAsyncKeyState(t_keyToCheck) & 0x8000)
        {
            return true; // The key was pressed
        }

        return false;
    }

    //********************************************************************************

    void ConsoleEngine::changeAtPosition(int t_widthIndex, int t_heightIndex, char t_newFont)
    {
        // Change only if the "pixel" is not used
        if (m_gameInstance[t_heightIndex * m_width + t_widthIndex] == m_emptyFont)
        {
            moveCursorTo(t_widthIndex, t_heightIndex);
            m_gameInstance[t_heightIndex * m_width + t_widthIndex] = t_newFont;
        }

        return;
    }

    void ConsoleEngine::checkForLines()
    {
        bool b_isLine = true;

        // Only inside borders
        for (int heigthIndex = 1; heigthIndex < m_height - 1; heigthIndex++)
        {
            b_isLine = true;

            // Go through every line
            for (int widthIndex = 1; widthIndex < m_width - 1; widthIndex++)
            {
                if (m_gameInstance[heigthIndex * m_width + widthIndex] == m_emptyFont)
                    b_isLine = false;
            }

            if (b_isLine)
                updateGameBoard(heigthIndex);
        }

        return;
    }

    void ConsoleEngine::updateGameBoard(int t_lineNumber)
    {
        // Empty the line with a little animation
        for (int widthIndex = 1; widthIndex < m_lastColumn; widthIndex++)
        {
            moveCursorTo(m_widthPadding + widthIndex, m_heightPadding + t_lineNumber);
            SCE_CONSOLE_OUTPUT << m_emptyFont;
            Sleep(50);
        }

        // Move the upper pieces down a level for the highest line
        for (int heightIndex = t_lineNumber; heightIndex > 0; heightIndex--)
        {
            for (int widthIndex = 1; widthIndex < m_lastColumn; widthIndex++)
            {
                // Take care of the border
                if (isBorder(widthIndex, heightIndex - 1))
                {
                    m_gameInstance[heightIndex * m_width + widthIndex] = m_emptyFont;
                }
                else
                {
                    m_gameInstance[heightIndex * m_width + widthIndex] = m_gameInstance[(heightIndex - 1) * m_width + widthIndex];
                }
            }
        }

        // Update score
        m_score += 100;

        renderGameScreen();

        return;
    }

    //********************************************************************************
    //                                Private methods
    //********************************************************************************

    void ConsoleEngine::this_displayScore()
    {
        moveCursorTo(2 * m_widthPadding + m_width, m_heightPadding);
        SCE_CONSOLE_OUTPUT << "SCORE: " << getMyScore();

        return;
    }

    void ConsoleEngine::this_displayLogo()
    {
        moveCursorTo(2 * m_widthPadding + m_width, m_heightPadding + m_lastLine);
        SCE_CONSOLE_OUTPUT << "Made by Stipl3x";

        return;
    }

} }

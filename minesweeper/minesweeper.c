#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>

#include "Input.h"
#include "GameGrid.h"

// DEFINITIONS //
#define SIZE 20
#define ARRAY_SIZE SIZE*SIZE

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define COLOR_BLACK  "\033[0;30m"
#define COLOR_WHITE "\033[0;37m"

static const char charset[] = ". 123456789*F";
static const char* colours[] = {
    COLOR_WHITE,
    ANSI_COLOR_RESET,
    ANSI_COLOR_BLUE,
    ANSI_COLOR_GREEN,
    ANSI_COLOR_RED,
    ANSI_COLOR_MAGENTA,
    ANSI_COLOR_YELLOW,
    ANSI_COLOR_CYAN,
    COLOR_BLACK,
    COLOR_WHITE,
    ANSI_COLOR_RESET,
    COLOR_WHITE,
    ANSI_COLOR_RED
};


Input playerInput();

// MAIN //

/// <summary>
/// Program's entry point
/// </summary>
/// <returns>int</returns>
int main()
{
    srand(time(NULL));

    int playing = 1;
    int won = 0;
    GameGrid game;
    initGrid(&game);

    int bombCount = countBombs(&game);
    
    while (playing) 
    {
        if (system("CLS")) system("clear");
        displayGameGrid(&game);
        printf("\nBOMBS: %d\nTO DIG: %d", bombCount, countFound(&game));
        Input input = playerInput();
        
        if (input.flag) {
            placeFlag(&game, input.x, input.y);
        }
        else {
            if (game.displayGrid[input.x + input.y * 20] != charset[12])
                 playing = !digAt(&game, input.x, input.y);
        }

        if (bombCount == countFound(&game))
        {
            won = 1;
            playing = 0;
        }
    }

    if (system("CLS")) system("clear");
    completeGrid(&game);
    displayGameGrid(&game);

    if (won) 
    {
        printf("\nYOU WON!");
    }
    else 
    {
        printf("\nYou Lost Lol.... Loser..... Lol?...........");
    }
}

// FUNCTIONS //

// Input

/// <summary>
/// Manual flush!
/// </summary>
void Flush()
{
    while (getchar() != '\n');
}

/// <summary>
/// gets input from user
/// </summary>

Input playerInput() {
    int playerInputX = -1;
    int playerInputY = -1;
    Input Return;
    Return.flag = 0;

    printf("\n\nFormat: 'X Y', add an 'f' at the start if you want to place a flag (Example: 'f 10 10', '5 8')");
    char* valid = "Where do you want to play?";
    while (!(playerInputX >= 0 && playerInputX < SIZE && playerInputY >= 0 && playerInputY < SIZE))
    {
        printf("\n%s> ", valid);

        char str[20];
        fgets(str, 20, stdin);
        
        Return.flag = tolower(str[0]) == 'f';
        str[0] = Return.flag ? ' ' : str[0];
        (void)sscanf_s(str, "%d%d", &playerInputX, &playerInputY);
        valid = "Select a VALID place to play";
    }

    Return.x = playerInputX;
    Return.y = playerInputY;

    return Return;
}

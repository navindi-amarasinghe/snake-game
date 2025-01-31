#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"
#include "random.h"
#include "terminal.h"
#include "newSleep.h"

int main(int argc, char *argv[]) {
    int rows, cols;
    char **map;
    int playerRow, playerCol;
    int snakeRow, snakeCol;
    char userInput;
    int gameOver;
    float delay= 0.5;

    if (argc != 3) {
        printf("Usage: %s <map_row> <map_col>\n", argv[0]);
        return 1;
    }

    rows = atoi(argv[1]);
    cols = atoi(argv[2]);

    if (rows < 5 || cols < 5) {
        printf("Map size must be at least 5x5\n");
        return 1;
    }

    initializeMap(rows, cols, &map);

    srand(time(NULL));

    /* Initialize player position randomly */
    playerRow = 1;
    playerCol = 1;  

    /* Initialize snake position randomly */
    do {
        snakeRow = rows - 2;
        snakeCol = cols - 2;
    } while (snakeRow == playerRow && snakeCol == playerCol);

    map[playerRow][playerCol] = PLAYER;
    map[snakeRow][snakeCol] = SNAKE;
    
    generateFood(rows, cols, map);
      
    gameOver = 0;
    while (!gameOver) {
        clearScreen();
        printMap(rows, cols, map);
        printInstructions(); 
        newSleep(delay);

        disableBuffer();
        userInput = getUserInput(); /* Get user input */
        enableBuffer();

        switch (userInput) {
            case 'w':
            case 'a':
            case 's':
            case 'd':
                /* Move player */
                movePlayer(userInput, &playerRow, &playerCol, rows, cols, map, &gameOver);
                
                /* Check for collision */
                if (gameOver == 2) {
                    printf("You lose!\n");
                    gameOver = true;
                    break;
                }
                
                /* Move snake */
                moveSnake(rows, cols, playerRow, playerCol, &snakeRow, &snakeCol, map, &gameOver);
                
                /* Check whether the food has been eaten by the player */
                if (gameOver == 1) {
                    printf("You Won!\n");
                    gameOver = true;
                    break;
                }

                break;
            case 'q':
                printf("Quitting the game.\n");
                gameOver = true;
                break;
            default:
          /* Print warning for invalid key */
                printf("Invalid key\n");
                break;
      }
    }

    /* Free memory */
    freeMap(rows, map);

    return 0;
}


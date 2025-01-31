#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <termios.h>
#include "game.h"
#include "random.h"

/* Function to clear the terminal screen */
void clearScreen() {
    printf("\033[2J\033[H");
}

/* Function to initialize the map with borders */
void initializeMap(int rows, int cols, char ***map) {
    int i, j;
    *map = (char **)malloc(rows * sizeof(char *));

    for (i = 0; i < rows; i++) {
        (*map)[i] = (char *)malloc(cols * sizeof(char));

        for (j = 0; j < cols; j++) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                (*map)[i][j] = BORDER;
            } else {
                (*map)[i][j] = EMPTY; /* Initialize all non-border spaces as empty */
            }
        }
    }
}

/* Function to print the map */
void printMap(int rows, int cols, char **map) {
    int i, j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n"); /* Move to the next line after printing each row */
    }
}

/* Function to print the instructions for user input */
void printInstructions() {
    printf("Press 'w' key to move up\n");
    printf("Press 's' key to move down\n");
    printf("Press 'a' key to move left\n");
    printf("Press 'd' key to move right\n");
    printf("Press 'q' to quit\n");
}

/* Function to get user input */
char getUserInput() {
    char userInput;
    scanf(" %c", &userInput);
    return userInput;
}

/* Function to generate random food position */
void generateFood(int rows, int cols, char **map) {
    int foodRow, foodCol;

    /* Initialize random number generator */
    initRandom();
    do {
        foodRow = randomUCP(1, rows - 2);
        foodCol = randomUCP(1, cols - 2);
    } while (map[foodRow][foodCol] != EMPTY);

    map[foodRow][foodCol] = FOOD;
}


/* Function to move the player */
void movePlayer(char direction, int *playerRow, int *playerCol, int rows, int cols, char **map, int *gameOver) {
    int prevPlayerRow = *playerRow;
    int prevPlayerCol = *playerCol;

    switch (direction) {
        case 'w':
            if (*playerRow > 0) {
                (*playerRow)--;
            }
            break;
        case 's':
            if (*playerRow < (rows - 1)) {
                (*playerRow)++;
            }
            break;
        case 'a':
            if (*playerCol > 0) {
                (*playerCol)--;
            }
            break;
        case 'd':
            if (*playerCol < (cols - 1)) {
                (*playerCol)++;
            }
            break;
        case 'q':
            *gameOver = 3;
            break;
        default:
            printf("Invalid key\n");
            break;
    }


    if (map[*playerRow][*playerCol] == FOOD) {
    map[*playerRow][*playerCol] = EMPTY;  
    *gameOver = 1;
    }

    map[prevPlayerRow][prevPlayerCol] = EMPTY;
    map[*playerRow][*playerCol] = PLAYER;
}

/* Function to move the snake */
void moveSnake(int rows, int cols, int playerRow, int playerCol, int *snakeRow, int *snakeCol, char **map, int *gameOver) {
    int randMove = randomUCP(0, 7);
    int newRow = *snakeRow;
    int newCol = *snakeCol;

    /* Calculate distance between snake and player */ 
    int rowDistance = abs(*snakeRow - playerRow);
    int colDistance = abs(*snakeCol - playerCol);

    /* Direct movement towards player if one grid away */
    if ((rowDistance == 1 && colDistance == 0) || (rowDistance == 0 && colDistance == 1)) {
        if (*snakeRow < playerRow) newRow = *snakeRow + 1;
        else if (*snakeRow > playerRow) newRow = *snakeRow - 1;
        if (*snakeCol < playerCol) newCol = *snakeCol + 1;
        else if (*snakeCol > playerCol) newCol = *snakeCol - 1;
    } else {
        /* Randomized movement if not one grid away from player */
        switch (randMove) {
            case 0:
                newRow = (*snakeRow > 1) ? (*snakeRow - 1) : (*snakeRow); /* Up */
                break;
            case 1:
                newRow = (*snakeRow < (rows - 2)) ? (*snakeRow + 1) : (*snakeRow); /* Down */
                break;
            case 2:
                newCol = (*snakeCol > 1) ? (*snakeCol - 1) : (*snakeCol); /* Left */
                break;
            case 3:
                newCol = (*snakeCol < (cols - 2)) ? (*snakeCol + 1) : (*snakeCol); /* Right */
                break;
            case 4:
                newRow = (*snakeRow > 1) ? (*snakeRow - 1) : (*snakeRow); /* Up & Left */
                newCol = (*snakeCol > 1) ? (*snakeCol - 1) : (*snakeCol); 
                break;
            case 5:
                newRow = (*snakeRow > 1) ? (*snakeRow - 1) : (*snakeRow); /* Up & Right */
                newCol = (*snakeCol < (cols - 2)) ? (*snakeCol + 1) : (*snakeCol); 
                break;
            case 6:
                newRow = (*snakeRow < (rows - 2)) ? (*snakeRow + 1) : (*snakeRow); /* Down & Left */
                newCol = (*snakeCol > 1) ? (*snakeCol - 1) : (*snakeCol); 
                break;
            case 7:
                newRow = (*snakeRow < (rows - 2)) ? (*snakeRow + 1) : (*snakeRow); /* Down & Right */
                newCol = (*snakeCol < (cols - 2)) ? (*snakeCol + 1) : (*snakeCol); 
                break;
        }
    }

    /* Check if the new position is valid (inside the borders) */
    if (newRow >= 1 && newRow < (rows - 1) && newCol >= 1 && newCol < (cols - 1)) {
        /* Move the snake to the new position */
        map[*snakeRow][*snakeCol] = EMPTY; 
        *snakeRow = newRow;
        *snakeCol = newCol;
        
        /* Check for collision with food */
        if (map[*snakeRow][*snakeCol] == FOOD) {
            generateFood(rows, cols, map);  
        } else {
            map[*snakeRow][*snakeCol] = SNAKE;
        }
    }

    /* Check for collision with player */
    if (*snakeRow == playerRow && *snakeCol == playerCol) {
        map[playerRow][playerCol] = EMPTY;
        printf("You lose!\n");
        *gameOver = 2;
    }
    
    /* Check if the player is surrounded by the snake */
    if (map[playerRow + 1][playerCol] != EMPTY &&
        map[playerRow - 1][playerCol] != EMPTY &&
        map[playerRow][playerCol + 1] != EMPTY &&
        map[playerRow][playerCol - 1] != EMPTY) {
        printf("You are surrounded by the snake!\n");
        *gameOver = 2;
    }
}


/* Function to free memory allocated for the map */
void freeMap(int rows, char **map) {
    int i;
    for (i = 0; i < rows; i++) {
        free(map[i]);
    }
    free(map);
}



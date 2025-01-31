#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

/* Constants for characters on the map */
#define BORDER '*'
#define PLAYER 'P'
#define FOOD '@'
#define SNAKE '~'
#define EMPTY ' '

/* Function prototypes */
void clearScreen();
void initializeMap(int rows, int cols, char ***map);
void printMap(int rows, int cols, char **map);
void generateFood(int rows, int cols, char **map);
void movePlayer(char direction, int *playerRow, int *playerCol, int rows, int cols, char **map, int *gameOver);
void moveSnake(int rows, int cols, int playerRow, int playerCol, int *snakeRow, int *snakeCol, char **map, int *gameOver);
bool checkCollision(int playerRow, int playerCol, int snakeRow, int snakeCol, int foodRow, int foodCol);
void freeMap(int rows, char **map);
void printInstructions();
char getUserInput();

#endif /* GAME_H */


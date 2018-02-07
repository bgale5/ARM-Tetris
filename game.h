#ifndef GAME_H
#define GAME_H
#include "display.h"

#define COL_MAX 10
#define ROW_MAX 20
#define COL_MIN 1
#define ROW_MIN 1
#define TETROMINO_TYPES 7
#define ORIENTATIONS 3
#define TETROMINO_CELLS 4
#define PIXEL_SCALE_FACTOR 6

typedef enum {
	I = 0, J, L, O, S, T, Z
} SHAPE;

typedef struct {
	char column;
	char row;
} gameLoc;

typedef struct {
	SHAPE type;
	char orientation;
	gameLoc origin;
} tet;

// Global, current and next tetrominos at all times
extern tet currentTet;
extern tet nextTet;
extern unsigned char score;
extern unsigned char speed; // Ticks per second

void newTet(gameLoc origin, char orientation, SHAPE type, tet *t);
void drawGameBorder(void);
void gameLoop(void);
void drawSquare(gameLoc *loc, char colour);
void drawTet(tet *tet, char colour);
void rotateTet(tet *t);
void mvLeft(tet *t);
void mvRight(tet *t);
void mvDown(tet *t);
char downCollision(tet *t);
char leftCollision(tet *t);
char rightCollision(tet *t);
char rotCollision(tet *t);
char clearGoal(void);
void compress(char lines, char start);
char isGameover(void);
void gameOver(void);
#endif

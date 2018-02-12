#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define		SIZE		10
#define     GET_UP      "\033[A"
#define     GET_DOWN    "\033[B"
#define     GET_LEFT    "\033[D"
#define     GET_RIGHT   "\033[C"

#define		UP		1
#define		DOWN	2
#define		LEFT	3
#define		RIGHT	4 

int const ROW = 4;
int const COL = 4;
int game[4][4] = {0};

// status of the game
int const GAME_OVER = 1;
int const GAME_WIN = 2;
int const GAME_CONTINUE = 3;

typedef int gamenum;
gamenum game_2 = 2;
gamenum game_4 = 4;
gamenum game_8 = 8;
gamenum game_16 = 16;
gamenum	game_32 = 32;
gamenum	game_64 = 64;
gamenum	game_128 = 128;
gamenum	game_256 = 256;
gamenum	game_512 = 512;
gamenum	game_1024 = 1024;
gamenum	game_2048 = 2048;

/*
 * print the array
 * */
void mprint() {
	for (int i = 0; i < ROW; ++i) {
		printf("---------------------------------\n");
		for (int j = 0; j < COL; ++j) {
			if (game[i][j] == 0) {
				 printf("| \t");
			} else {
				printf("| %d\t", game[i][j]);
			}
		}
		printf("|\n");
	}
	printf("---------------------------------\n");
}

int createNumber() {
	int x = -1;
	int y = -1;
	int times = 0;
	int maxTimes = ROW * COL;
	// 2/3 to create 2, 1/3 to create 4
	int whitch = rand() % 3;
	do {
		x = rand() % ROW;
		y = rand() % COL;
		++times;
	} while (game[x][y] != 0 && times <= maxTimes);

	// full
	if (times >= maxTimes) {
		return 0;
	} else {
		gamenum num;
		if (whitch == 0) {
			num = game_4;
		} else if (whitch) {
			num = game_2;
		}
		game[x][y] = num;
	}

	return 1;
}

/*
 * process the direction
 * */
void process(int direction) {
	switch (direction) {
		case UP:
			// top not move
			for(int row = 1; row < ROW; ++row) {
				for(int crow = row; crow >= 1; --crow) {
					for(int col = 0; col < COL; ++col) {
						// last is space
						if(game[crow-1][col] == 0) {
							game[crow-1][col] = game[crow][col];
							game[crow][col] = 0;
						} else {
							// merge
							if(game[crow-1][col] == game[crow][col]) {
								game[crow - 1][col] *= 2;
								game[crow][col] = 0;
							}
						}
					}
				}
			}
			break;
		case DOWN:
			for(int row = ROW - 2; row >= 0; --row) {
				for(int crow = row; crow < ROW - 1; ++crow) {
					for(int col = 0; col < COL; ++col) {
						if(game[crow + 1][col] == 0) {
							game[crow + 1][col] = game[crow][col];
							game[crow][col] = 0;
						} else {
							if(game[crow + 1][col] == game[crow][col]) {
								game[crow + 1][col] *= 2;
								game[crow][col] = 0;
							}
						}
					}
				}
			}
			break;
		case LEFT:
			for (int col = 1; col < COL; ++col) {
				for (int ccol = col; ccol >= 1; --ccol) {
					for (int row = 0; row < ROW; ++row) {
						if (game[row][ccol-1] == 0) {
							game[row][ccol - 1] = game[row][ccol];
							game[row][ccol] = 0;
						} else {
							if(game[row][ccol - 1] == game[row][ccol]) {
								game[row][ccol - 1] *= 2;
								game[row][ccol] = 0;
							}
						}
					}
				}
			}
			break;
		case RIGHT:
			for (int col = COL - 2; col >= 0; --col) {
				for (int ccol = col; ccol <= COL - 2; ++ccol) {
					for (int row = 0; row < ROW; ++row) {
						if (game[row][ccol + 1] == 0) {
							game[row][ccol + 1] = game[row][ccol];
							game[row][ccol] = 0;
						} else {
							if (game[row][ccol + 1] == game[row][ccol]) {
								game[row][ccol + 1] *= 2;
								game[row][ccol] = 0;
							}
						}
					}
				}
			}
			break;
	}

}

/*
 * get the direction from input
 * */
int input() {
	int direction = 0;
	char *get = (char *) malloc(SIZE);
	for ( ; ; ) {
		fgets(get, SIZE, stdin);
		get[strlen(get) - 1] = '\0';

		if(!strcmp(get, GET_UP)) {
			direction = UP;
			break;
		} else if (!strcmp(get, GET_DOWN)) {
			direction = DOWN;
			break;
		} else if (!strcmp(get, GET_LEFT)) {
			direction = LEFT;
			break;
		} else if (!strcmp(get, GET_RIGHT)) {
			direction = RIGHT;
			break;
		}
	}

	return direction;
}

/*
 * judge the status of the game
 * */
int judgeStatus() {
	// win the game
	for(int i = 0; i < ROW; ++i) {
		for(int j = 0; j < COL; ++j) {
			if(game[i][j] == 2048) {
				return GAME_WIN;
				break;
			}
		}
	}

	// check row
	for(int i = 0 ; i < ROW; ++i) {
		for(int j = 0; j < COL - 1; ++j) {
			if(!game[i][j] || (game[i][j] == game[i][j+1])) {
				return GAME_CONTINUE;
				break;
			}
		}
	}

	// check col
	for(int j = 0; j< COL; ++j) {
		for(int i = 0; i < ROW -1; ++i) {
			if(!game[i][j] || (game[i][j] == game[i+1][j])) {
				return GAME_CONTINUE;
				break;
			}
		}
	}

	return GAME_OVER;
}

int main() {
	// set a random seed
	srand((unsigned int)time(NULL));
	createNumber(); 
	createNumber(); 
	mprint();
	int direction = 0;
	int status = -1; 
	while(1) {
		direction = input();
		status = judgeStatus();
		if(direction && status == GAME_CONTINUE) {
			process(direction); 
			createNumber();
			mprint();
			sleep(1);
		} else if(status == GAME_WIN) {
			mprint();
			printf("\n ^_^    You Win    ^_^ \n");
			break;
		} else if (status == GAME_OVER) {
			mprint();
			printf("\n -_-    You lose    -_- \n");
			break;
		}
	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ESCAPE_CODE "\033[H\033[J"

#define ROWS 35
#define COLS 130
#define SPEED 10 // fraction of a second for one tick
#define SYM_DEAD "-"
#define SYM_ALIVE "#"

typedef enum {
    DEAD,
    ALIVE,
} State;

typedef struct {
   State currentState;
   State nextState; 
} Cell;

void init_grid(Cell grid[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            grid[y][x].currentState = DEAD;
            grid[y][x].nextState = DEAD;
        }
    }
}

void print_grid(Cell grid[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (grid[y][x].currentState == DEAD) {
                printf(SYM_DEAD);
            } else {
                printf(SYM_ALIVE);
            }
        }
        printf("\n");
    }
}

void update_grid(Cell grid[ROWS][COLS], int *alive_counter) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            int neighbor_counter = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dy == 0 && dx == 0) continue;
                    int row = (y + dy + ROWS) % ROWS;
                    int col = (x + dx + COLS) % COLS;
                    if (grid[row][col].currentState == ALIVE) {
                        neighbor_counter++;
                    }
                }
            }

            // Game rules
            if (grid[y][x].currentState == ALIVE) {
                if (neighbor_counter < 2) {
                    grid[y][x].nextState = DEAD;
                } else if (neighbor_counter < 4) {
                    grid[y][x].nextState = ALIVE;
                } else {
                    grid[y][x].nextState = DEAD;
                }
            } else {
                if (neighbor_counter == 3) {
                    grid[y][x].nextState = ALIVE;
                } else {
                    grid[y][x].nextState = DEAD;
                }
            }
        }
    }
    int local_alive_counter = 0;
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            grid[y][x].currentState = grid[y][x].nextState;
            if (grid[y][x].currentState == ALIVE) local_alive_counter++;
        }
    }
    *alive_counter = local_alive_counter;
}

void draw_acorn(Cell grid[ROWS][COLS]);
void draw_glider(Cell grid[ROWS][COLS]); 
void draw_r_pentomino(Cell grid[ROWS][COLS]);

// entry point
int main() {
    Cell grid[ROWS][COLS] = {0};
    int generation_counter = 1;
    init_grid(grid);

    draw_acorn(grid);

    int alive_counter = -1;
    while (alive_counter != 0) {
        printf("Generation: %d\n", generation_counter);
        print_grid(grid);

        usleep(1000000 / SPEED);
        printf(ESCAPE_CODE);

        update_grid(grid, &alive_counter);
        generation_counter++;
    }

    return 0;
}

void draw_acorn(Cell grid[ROWS][COLS]) {
    int c_row = ROWS / 2;
    int c_col = COLS / 2;

    grid[c_row][c_col].currentState = ALIVE;
    grid[c_row + 1][c_col + 1].currentState = ALIVE;
    grid[c_row + 1][c_col + 2].currentState = ALIVE;
    grid[c_row + 1][c_col + 3].currentState = ALIVE;
    grid[c_row - 1][c_col - 2].currentState = ALIVE;
    grid[c_row + 1][c_col - 2].currentState = ALIVE;
    grid[c_row + 1][c_col - 3].currentState = ALIVE;
}

void draw_glider(Cell grid[ROWS][COLS]) {
    grid[1][1].currentState = ALIVE;
    grid[2][2].currentState = ALIVE;
    grid[3][0].currentState = ALIVE;
    grid[3][1].currentState = ALIVE;
    grid[3][2].currentState = ALIVE;
}

void draw_r_pentomino(Cell grid[ROWS][COLS]) {
    int c_row = ROWS / 2;
    int c_col = COLS / 2;
    grid[c_row][c_col].currentState = ALIVE;
    grid[c_row - 1][c_col].currentState = ALIVE;
    grid[c_row + 1][c_col].currentState = ALIVE;
    grid[c_row][c_col - 1].currentState = ALIVE;
    grid[c_row + 1][c_col + 1].currentState = ALIVE;

}

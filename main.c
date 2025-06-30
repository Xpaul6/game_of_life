#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ESCAPE_CODE "\033[H\033[J"

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

typedef struct {
    Cell** grid;
    int rows;
    int cols;
    int speed;
} Game;

void init_grid(Game* game) {
    for (int i = 0; i < game->rows; i++) {
        game->grid[i] = malloc(game->cols * sizeof(Cell));
    }
    for (int y = 0; y < game->rows; y++) {
        for (int x = 0; x < game->cols; x++) {
            game->grid[y][x].currentState = DEAD;
            game->grid[y][x].nextState = DEAD;
        }
    }
}

void print_grid(Game* game) {
    char* buffer = malloc(game->rows * (game->cols + 1) + 1);

    char* row_ptr = buffer;
    for (int y = 0; y < game->rows; y++) {
        for (int x = 0; x < game->cols; x++) {
            if (game->grid[y][x].currentState == DEAD) {
                *row_ptr++ = SYM_DEAD[0];
            } else {
                *row_ptr++ = SYM_ALIVE[0];
            }
        }
        *row_ptr++ = '\n';
    }

    printf("%s", buffer);
    free(buffer);
}

void update_grid(Game* game, int* alive_counter) {
    for (int y = 0; y < game->rows; y++) {
        for (int x = 0; x < game->cols; x++) {
            int neighbor_counter = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dy == 0 && dx == 0) continue;
                    int row = (y + dy + game->rows) % game->rows;
                    int col = (x + dx + game->cols) % game->cols;
                    if (game->grid[row][col].currentState == ALIVE) {
                        neighbor_counter++;
                    }
                }
            }

            // Game rules
            if (game->grid[y][x].currentState == ALIVE) {
                if (neighbor_counter < 2) {
                    game->grid[y][x].nextState = DEAD;
                } else if (neighbor_counter < 4) {
                    game->grid[y][x].nextState = ALIVE;
                } else {
                    game->grid[y][x].nextState = DEAD;
                }
            } else {
                if (neighbor_counter == 3) {
                    game->grid[y][x].nextState = ALIVE;
                } else {
                    game->grid[y][x].nextState = DEAD;
                }
            }
        }
    }
    int local_alive_counter = 0;
    for (int y = 0; y < game->rows; y++) {
        for (int x = 0; x < game->cols; x++) {
            game->grid[y][x].currentState = game->grid[y][x].nextState;
            if (game->grid[y][x].currentState == ALIVE) local_alive_counter++;
        }
    }
    *alive_counter = local_alive_counter;
}

void draw_acorn(Game* game);
void draw_glider(Game* game); 
void draw_r_pentomino(Game* game);
void draw_diehart(Game* game);

// entry point
int main(int argc, char** argv) {
    Game game = {0};

    game.rows = 35;
    game.cols = 130;
    game.speed = 10;

    if (argc == 5) {
        game.cols = strtol(argv[1], NULL, 10);
        game.rows = strtol(argv[2], NULL, 10);
        game.speed = strtol(argv[3], NULL, 10);
        if (game.cols < 10 || game.rows < 10 || game.speed < 1) {
            printf("Minimal grid size is 10x10, minimal speed is 1 tick/s\nExiting...\n");
            return -1;
        }
    }

    game.grid = malloc(game.rows * sizeof(Cell*));

    init_grid(&game);

    if (!strcmp(argv[4], "glider")) {
        draw_glider(&game);
    } else if (!strcmp(argv[4], "diehart")) {
        draw_diehart(&game);
    } else if (!strcmp(argv[4], "r-pentomino")) {
        draw_r_pentomino(&game);
    } else {
        draw_acorn(&game);
    }

    int generation_counter = 1;
    int alive_counter = -1;
    
    printf(ESCAPE_CODE);

    while (alive_counter != 0) {
        printf("Generation: %d\n", generation_counter);
        print_grid(&game);

        usleep(1000000 / game.speed);
        printf(ESCAPE_CODE);

        update_grid(&game, &alive_counter);
        generation_counter++;
    }

    return 0;
}

void draw_acorn(Game* game) {
    int c_row = game->rows / 2;
    int c_col = game->cols / 2;

    game->grid[c_row][c_col].currentState = ALIVE;
    game->grid[c_row + 1][c_col + 1].currentState = ALIVE;
    game->grid[c_row + 1][c_col + 2].currentState = ALIVE;
    game->grid[c_row + 1][c_col + 3].currentState = ALIVE;
    game->grid[c_row - 1][c_col - 2].currentState = ALIVE;
    game->grid[c_row + 1][c_col - 2].currentState = ALIVE;
    game->grid[c_row + 1][c_col - 3].currentState = ALIVE;
}

void draw_glider(Game* game) {
    game->grid[1][1].currentState = ALIVE;
    game->grid[2][2].currentState = ALIVE;
    game->grid[3][0].currentState = ALIVE;
    game->grid[3][1].currentState = ALIVE;
    game->grid[3][2].currentState = ALIVE;
}

void draw_r_pentomino(Game* game) {
    int c_row = game->rows / 2;
    int c_col = game->cols / 2;
    game->grid[c_row][c_col].currentState = ALIVE;
    game->grid[c_row - 1][c_col].currentState = ALIVE;
    game->grid[c_row + 1][c_col].currentState = ALIVE;
    game->grid[c_row][c_col - 1].currentState = ALIVE;
    game->grid[c_row + 1][c_col + 1].currentState = ALIVE;

}

void draw_diehart(Game* game) {
    int c_row = game->rows / 2;
    int c_col = game->cols / 2;

    game->grid[c_row][c_col - 3].currentState = ALIVE;
    game->grid[c_row][c_col - 2].currentState = ALIVE;
    game->grid[c_row + 1][c_col - 3].currentState = ALIVE;
    game->grid[c_row - 1][c_col + 3].currentState = ALIVE;
    game->grid[c_row + 1][c_col + 2].currentState = ALIVE;
    game->grid[c_row + 1][c_col + 3].currentState = ALIVE;
    game->grid[c_row + 1][c_col + 4].currentState = ALIVE;
}

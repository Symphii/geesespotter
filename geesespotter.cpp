#include "geesespotter_lib.h"
#include "geesespotter.h"
#include <iostream>

int coord_index( std::size_t x_dim, std::size_t x_loc, std::size_t y_loc );

char * create_board(std::size_t x_dim, std::size_t y_dim) {
    int array_size{};
    array_size = (x_dim * y_dim);
    char * a_board{ new char[array_size]{} };
    return a_board; 
}

void clean_board(char * board) {
    delete[] board;
    board = nullptr;
    return;
}

void print_board(char * board, std::size_t x_dim, std::size_t y_dim) {
    int x{0};
    int value{0};
    for (int i{0}; i < y_dim; i++) {
        for (int j{0}; j < x_dim; j++) {
            if ((board[x] & marked_mask()) == marked_mask()) {
                std::cout << "M";
                x++;
            }
            else if ((board[x] & hidden_mask()) == hidden_mask()) {
                std::cout << "*";
                x++;
            }
            else {
                value = (board[x] & value_mask());
                std::cout << value;
                x++;
            }
        }
        std::cout << std::endl;
    }
    return;
}

void hide_board(char * board, std::size_t x_dim, std::size_t y_dim) {
    for (int i{0}; i < (x_dim * y_dim); i++) {
        board[i] = (board[i] | hidden_mask());
    }

}

int mark(char * board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc) {
    int index{coord_index(x_dim, x_loc, y_loc)};
    if ((board[index] & hidden_mask()) == 0x00) {
        return 2;
    }
    else {
        board[index] = (board[index] ^ marked_mask());
        return 0;
    }
}

void compute_neighbours(char * board, std::size_t x_dim, std::size_t y_dim) {
    int index{0};
    int value{0};
    for (int i{0}; i < y_dim; i++) {
        for (int j{0}; j < x_dim; j++) {
            value = 0;
            if ((board[index] & value_mask()) != 0x09) {
                if ((((j-1) >= 0) && ((i-1) >= 0)) && ((board[coord_index(x_dim, j-1, i-1)] & value_mask()) == 0x09)) {
                    value += 1;
                }
                if (((j-1) >= 0) && ((board[coord_index(x_dim, j-1, i)] & value_mask()) == 0x09)) {
                    value += 1;
                }
                if ((((j-1) >= 0) && ((i+1) < y_dim)) && ((board[coord_index(x_dim, j-1, i+1)] & value_mask()) == 0x09)) {
                    value += 1;
                }
                if (((i-1) >= 0) && ((board[coord_index(x_dim, j, i-1)] & value_mask()) == 0x09)) {
                    value += 1;
                }
                if (((i+1) < y_dim) && ((board[coord_index(x_dim, j, i+1)] & value_mask()) == 0x09)) {
                    value += 1;
                }
                if ((((j+1) < x_dim) && ((i-1) >= 0)) && ((board[coord_index(x_dim, j+1, i-1)] & value_mask()) == 0x09)) {
                    value += 1;
                }
                if (((j+1) < x_dim) && ((board[coord_index(x_dim, j+1, i)] & value_mask()) == 0x09)) {
                    value += 1;
                }
                if ((((j+1) < x_dim) && ((i+1) < y_dim)) && ((board[coord_index(x_dim, j+1, i+1)] & value_mask()) == 0x09)) {
                    value += 1;
                }
                if (value == 1) {
                    board[index] = board[index] | 0x01;
                }
                if (value == 2) {
                    board[index] = board[index] | 0x02;
                }
                if (value == 3) {
                    board[index] = board[index] | 0x03;
                }
                if (value == 4) {
                    board[index] = board[index] | 0x04;
                }
                if (value == 5) {
                    board[index] = board[index] | 0x05;
                }
                if (value == 6) {
                    board[index] = board[index] | 0x06;
                }
                if (value == 7) {
                    board[index] = board[index] | 0x07;
                }
                if (value == 8) {
                    board[index] = board[index] | 0x08;
                }
            }    
            index++;
        }
    }
}

bool is_game_won(char * board, std::size_t x_dim, std::size_t y_dim) {
    int hidden{0};
    int geese{0};
    for (int i{0}; i < (x_dim * y_dim); i++) {
        if ((board[i] & hidden_mask()) == hidden_mask()) {
            hidden++;
        }
        if ((board[i] & value_mask()) == 0x09) {
            geese++;
        }
    }
    if (geese == hidden) {
        return true;
    }
    else {
        return false;
    }
}

int reveal(char * board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc) {
    int index{0}; 
    std::size_t j{x_loc};
    std::size_t i{y_loc};
    index = coord_index(x_dim, x_loc, y_loc);
    if ((board[index] & marked_mask()) == marked_mask()) {
        return 1;
    }
    if ((board[index] & value_mask()) == 0x09) {
        board[index] = (board[index] & 0xdf);
        return 9;
    }
    if ((board[index] & value_mask()) == 0x00) {
            if ((j != 0) && (i != 0)) {
                board[coord_index(x_dim, j-1, i-1)] = (board[coord_index(x_dim, j-1, i-1)] & 0xdf);
            }
            if (j != 0) {
                board[coord_index(x_dim, j-1, i)] = (board[coord_index(x_dim, j-1, i)] & 0xdf);
            }
            if ((j != 0) && ((i+1) < y_dim)) {
                board[coord_index(x_dim, j-1, i+1)] = (board[coord_index(x_dim, j-1, i+1)] & 0xdf);
            }
            if (i != 0) {
                board[coord_index(x_dim, j, i-1)] = (board[coord_index(x_dim, j, i-1)] & 0xdf);
            }
            if ((i+1) < y_dim) {
                board[coord_index(x_dim, j, i+1)] = (board[coord_index(x_dim, j, i+1)] & 0xdf);
            }
            if (((j+1) < x_dim) && (i != 0)) {
                board[coord_index(x_dim, j+1, i-1)] = (board[coord_index(x_dim, j+1, i-1)] & 0xdf);
            }
            if ((j+1) < x_dim) {
                board[coord_index(x_dim, j+1, i)] = (board[coord_index(x_dim, j+1, i)] & 0xdf);
            }
            if (((j+1) < x_dim) && ((i+1) < y_dim)) {
                board[coord_index(x_dim, j+1, i+1)] = (board[coord_index(x_dim, j+1, i+1)] & 0xdf);
            }
    }
    if ((board[index] & hidden_mask()) == 0x00) {
        return 2;
    }
    else {
        board[index] = (board[index] & 0xdf);
        return 0;
    }
}

int coord_index( std::size_t x_dim, std::size_t x_loc, std::size_t y_loc ) {
    int index{0};
    index = ((x_dim * y_loc) + x_loc);
    return index;
}
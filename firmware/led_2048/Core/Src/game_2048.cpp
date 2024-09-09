/*
 * 2048.cpp
 *
 *  Created on: Aug 31, 2024
 *      Author: 1miln
 */

#include "game_2048.h"
//#include "main.h"



    game_2048:: game_2048(uint8_t rows, uint8_t cols) {
        _row = rows;
        _col = cols;
        _score = 0;

       _grid = new int[_row * _col];

    }


    game_2048:: ~game_2048() {
        delete[] _grid;
        _grid = nullptr;
    }


    bool game_2048:: vacancy() {
        for (int k = 0; k < _row*_col; ++k) {
            if (_grid[k] == 0) return true;
        }

        return false;
    }


    // updates the board in accordance to the user input
    bool game_2048:: move(char input) {

        int *temp_line;
        int *temp_grid = new int[_row * _col];

        // creates copy of _grid
        for (int i = 0; i < _row * _col; ++i)
            temp_grid[i] = _grid[i];

        if (input == 'w') { // UP
            temp_line = new int[_row];

            for (int c = 0; c < _col; ++c) {
                for (int r = 0; r < _row; ++r)
                    temp_line[r] = _grid[c + _col * r];

                _score += shift_line(temp_line, _row);

                for (int r = 0; r < _row; ++r)
                    _grid[c + _col * r] = temp_line[r];
            }

        } else if (input == 'd') { // RIGHT
            temp_line = new int[_col];

            for (int r = 0; r < _row; ++r) {

                for (int c = _col - 1; c >= 0; --c)
                    temp_line[_col - 1 - c] = _grid[_col * r + c];

                _score += shift_line(temp_line, _col);

                for (int c = _col - 1; c >= 0; --c)
                    _grid[_col * r + c] = temp_line[_col - 1 - c];
            }

        } else if (input == 's') { // DOWN
            temp_line = new int[_row];

            for (int c = 0; c < _col; ++c) {
                for (int r = _row - 1; r >= 0; --r)
                    temp_line[_row - 1 - r] = _grid[c + _col * r];

                _score += shift_line(temp_line, _row);

                for (int r = _row - 1; r >= 0; --r)
                    _grid[c + _col * r] = temp_line[_row - 1 - r];
            }

        } else if (input == 'a') { // LEFT
            temp_line = new int[_col];

            for (int r = 0; r < _row; ++r) {

                for (int c = 0; c < _col; ++c)
                    temp_line[c] = _grid[_col * r + c];

                _score += shift_line(temp_line, _col);

                for (int c = 0; c < _col; ++c)
                    _grid[_col * r + c] = temp_line[c];
            }

        }
        else if (input == 'r') { // TODO delete this
            delete[] temp_grid;
            clear_grid();
            _score = 0;
            return true;
        }
        else if (input == 8) { // backspace causes automatic loss TODO delete this

            for (int k = 0; k < _row*_col; k++) {
                _grid[k] = (intptr_t)_grid + 1 + k;
            }



            // cout << "\nOops, you broke it!";
            delete[] temp_grid;
            return true;
        }
        else if (input == 27) { // esc key was pressed
            delete[] temp_grid;
            std:: exit(1);
            return false;
        }
        else {
            delete[] temp_grid;
            return false;
        }

        bool not_identical = false;

        // checks if grid is identical before and after.
        for (int k = 0; k < _row*_col; ++k) {
            if (_grid[k] != temp_grid[k]) {
                not_identical = true;
                break;
            }
        }

        delete[] temp_line;
        delete[] temp_grid;
        return not_identical;
    }


    // helper function that deals with shifting and merging cells in a line (i.e. row/col) after the user makes a move
    // This function will be called regardless if the given line has no valid shifts or is completely empty
    int game_2048:: shift_line(int* line, int size) { // TODO should this be static???
        std:: stack<int> s;
        int num_empty = 0, score = 0;

        for (int k = 0; k < size; ++k) {

            if (line[k] != 0) {

                if (!s.empty() && s.top() == line[k]) {  //TODO will this if statement terminate before it checks for an undefined equality?
                    num_empty++;
                    s.top() *= 2;
                    score += s.top();

                    // This while loop ensures the next cell is properly pushed to the stack,
                    // as to avoid a "double merge"; for instance
                    // [0, 2, 2, 4] ---> [4, 4, 0, 0], NOT [0, 2, 2, 4] ---> [4, 4, 0, 0] --> [8, 0, 0, 0]
                    while (k < size-1) {
                        k++;

                        if (line[k] != 0) {
                            s.push(line[k]);
                            break;
                        }

                        num_empty++;
                    }
                }
                else { // merge does not occur
                    s.push(line[k]);
                }

            }
            else {
                num_empty++;
            }
        }

        if (num_empty == size)
            return 0;


        // pops the stack back into the array in reverse order
        for (int j = size-1; j >= 0; --j) {

            if (num_empty > 0) {
                line[j] = 0;
                --num_empty;
            }
            else {
                line[j] = s.top();
                s.pop();
            }
        }

        return score;
    }


    // Checks if there are any valid moves
    bool game_2048:: has_moves() {

        if (vacancy())
            return true;

        for (int r = 0; r < _row; ++r) {
            for (int c = 0; c < _col; ++c) {

                // checks for equality to element on its right
               if (c < _col-1 && _grid[_col*r + c] == _grid[_col*r + c + 1])
                   return true;

               // checks for equality to element below
               if (r < _row-1 && _grid[_col*r + c] == _grid[_col*r + c + _col])
                   return true;
            }
        }

        return false;
    }


    // Adds a random "2" or "4" to a random empty grid space
    // TODO change populate algorithm for larger boards
    void game_2048:: populate(unsigned int seed) {

        int* empty = new int[_row*_col];
        int num_empty = 0;

        for (int k = 0; k < _row*_col; ++k) {
            if(_grid[k] == 0) {
                empty[num_empty] = k;
                num_empty++;
            }
        }

        srand(seed);
        int element = (rand() % static_cast<int>(num_empty));
        int val = (rand() % static_cast<int>(4));


        if (val == 2) {
            _grid[empty[element]] = 4;
        }
        else {
            _grid[empty[element]] = 2;
        }

        delete[] empty;
    }


    void game_2048:: clear_grid() {

        for (int k = 0; k < _row*_col; ++k) {
            _grid[k] = 0;
        }
    }




    // "Game loop" of the 2048 clone
    bool game_2048:: play() {

        clear_grid();

        while(1) { // TODO add condition for player to end game

            bool lost = false;
            _score = 0;

            while(!lost) {

                if (vacancy())
                    populate((int) std::clock()); // TODO replace this with HAL_GetTick?


                // TODO make input() function base off of button press
                while(!move('a')) {
                    if (!has_moves()) {
                        lost = true;
                        break;
                    }
                }
            }

            // TODO update score

            if (0) // TODO add condition for changing the settings of the game
                return true;

            clear_grid();
        }

        return false;
    }



int main_2048() {






	return EXIT_SUCCESS;
}



/*
int main() {

    const int MAX_ROWS = 10, MAX_COL = 21;
    bool reset = false;

    do {

        int rows = 4, cols = 4;

        cout << "Please enter the number of rows: ";
        cin >> rows;
        if (!cin || rows <= 1) rows = 4;

        cout << "Please enter the number of columns: ";
        cin >> cols;
        if (!cin || cols <= 1) cols = 4;

        cin.clear();

        ShowConsoleCursor(false);
        Grid game = Grid(rows, cols);
        reset = game.play();
        game.~Grid();
    }
    while(reset);

    // TODO: allow for creating games of custom size
    // Todo: Add scoreboard.txt using fstream
    // TODO: Pressing escape exits and saves! Grid is saved into txt file. When pressing r or losing, txt file is cleared.


    /* Test line stack
    int score = 0;
    const int SIZE = 4;


    // <------ elements are pushed this way
    //int test[SIZE] = {0, 2, 0, 2,  8, 0, 8, 0, 8, 0, 8, 0, 4, 0, 0, 4, 0};
    int test[SIZE] = {0, 2048, 0, 2};

    score = Grid::line_stack(test, SIZE);

    cout << "score: " << score << "\n";
    for (int k = 0; k < SIZE; k++) {
        cout << test[k] << " ";
    }



    cin >> score;
    *//*/

    return EXIT_SUCCESS;
}
*/


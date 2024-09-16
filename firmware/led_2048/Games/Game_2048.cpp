/*
 * 2048.cpp
 *
 *  Created on: Aug 31, 2024
 *      Author: 1miln
 */


#include "Game_2048.h"

#include <stack>
#include <cstdlib>
#include <cmath>



Game_2048:: Game_2048(uint8_t rows, uint8_t cols) {
	_row = rows;
	_col = cols;
	_score = 0;

	_grid = new int[_row * _col];
}


Game_2048:: ~Game_2048() {
	delete[] _grid;
	_grid = nullptr;
}


bool Game_2048:: vacancy() {
	for (int k = 0; k < _row*_col; ++k) {
		if (_grid[k] == 0) return true;
	}

	return false;
}


// updates the board in accordance to the user input
bool Game_2048:: move(uint8_t input, uint8_t** rows) {

	int* temp_line;
	int* temp_grid = new int[_row * _col];

	// creates copy of _grid
	for (int i = 0; i < _row * _col; ++i)
		temp_grid[i] = _grid[i];

	if ((input&0x01) == 0) { // UP
		temp_line = new int[_row];

		for (int c = 0; c < _col; ++c) {
			for (int r = 0; r < _row; ++r)
				temp_line[r] = _grid[c + _col * r];

			_score += shift_line(temp_line, _row);
			display(rows); // prevents flashing

			for (int r = 0; r < _row; ++r)
				_grid[c + _col * r] = temp_line[r];
		}

	} else if ((input&0x02) == 0) { // RIGHT
		temp_line = new int[_col];

		for (int r = 0; r < _row; ++r) {

			for (int c = _col - 1; c >= 0; --c)
				temp_line[_col - 1 - c] = _grid[_col * r + c];

			_score += shift_line(temp_line, _col);
			display(rows);

			for (int c = _col - 1; c >= 0; --c)
				_grid[_col * r + c] = temp_line[_col - 1 - c];

		}

	} else if ((input&0x04) == 0) { // DOWN
		temp_line = new int[_row];

		for (int c = 0; c < _col; ++c) {
			for (int r = _row - 1; r >= 0; --r)
				temp_line[_row - 1 - r] = _grid[c + _col * r];

			_score += shift_line(temp_line, _row);
			display(rows);

			for (int r = _row - 1; r >= 0; --r)
				_grid[c + _col * r] = temp_line[_row - 1 - r];

		}

	} else if ((input&0x08) == 0) { // LEFT
		temp_line = new int[_col];

		for (int r = 0; r < _row; ++r) {

			for (int c = 0; c < _col; ++c)
				temp_line[c] = _grid[_col * r + c];

			_score += shift_line(temp_line, _col);
			display(rows);

			for (int c = 0; c < _col; ++c)
				_grid[_col * r + c] = temp_line[c];

		}

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
int Game_2048:: shift_line(int* line, int size) { // TODO should this be static???
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
bool Game_2048:: has_moves() {

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
void Game_2048:: populate(uint32_t seed) {

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


void Game_2048:: clear_grid() {

	for (int k = 0; k < _row*_col; ++k) {
		_grid[k] = 0;
	}
}


// Returns the colour depending on the value of _grid
uint8_t Game_2048:: get_colour(int val) {

	if (val == 0)
		return 0;

	const uint8_t COLOUR_LIST[15] = {0,
			0b00000111, // 2 --> white-1
			0b00101110, // 4 --> pink-2
			0b00000001, // 8 --> red-2
			0b00110001, // 16 --> red-4
			0b00011000, // 32 --> orange-2
			0b00100010, // 64 --> yellow-3
			0b00111001, // 128 --> lime-4
			0b01000011, // 256 --> green-5
			0b01011100, // 512 --> turqouise-6
			0b01110100, // 1024 --> cyan-7
			0b10000101, // 2048 --> blue-8
			0b01110110, // 4096 --> magenta-7
			0b11000110, // 8192 --> magenta-13
			0b11110110 // 16384 --> magenta-16
	};


	uint8_t index = (uint8_t) std::log2(val); // TODO this is likely bad


	return COLOUR_LIST[index];

}


void Game_2048:: update_grid(uint8_t* led_grid) {

	uint8_t i = 0;

	for (uint8_t k = 0; k < 25; k++) {

		if (k/5 < _col || k%5 < _row) {
			led_grid[k] = get_colour(_grid[i]);
			i++;
		}
		else
			led_grid[k] = 0;
	}

}

/*
// "Game loop" of the 2048 clone
uint8_t Game_2048:: play(uint8_t input) {


	if (move(input,rows)) { // change has been made to the grid

		if (vacancy())
			populate(HAL_GetTick());

		if (has_moves())
			return 1; // there are still valid moves

		return 2; // game lost

	}


	return 0; // no change
}
*/


int Game_2048:: main_2048(uint8_t** rows, uint8_t* grid) {

	clear_grid();
	solid_colour_grid(grid, 0);
	bool lost = false;

	uint32_t status_diff_millis = HAL_GetTick();

	uint8_t button_states = 0xFF, prev_button_states = 0xFF;
	uint32_t button_time_diffs[8];
	for (int btn = 0; btn < 8; btn++)
		 button_time_diffs[btn] = HAL_GetTick();

	populate(HAL_GetTick());

	while(!lost) {

		if (prev_button_states != button_states) {

			// prevents bottom row from momentarily flashing due to polling
			disable_mux5();

			if (move(button_states, rows)) { // if change has been made to the grid

				if (vacancy())
					populate(HAL_GetTick());

				if (!has_moves())
					lost=true; // no valid moves


				update_grid(grid);
				set_grid(rows, grid, 6);

			}
		}



		prev_button_states = button_states;
		button_states = update_button_states(button_states, button_time_diffs);
		display(rows);

		if (HAL_GetTick() - status_diff_millis > 1000) {
		  	status_diff_millis = HAL_GetTick();
		  	LED_heartbeat();
		}

	}

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


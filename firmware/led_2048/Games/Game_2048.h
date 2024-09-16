/*
 * game_2048.h
 *
 *  Created on: Sep 7, 2024
 *      Author: 1miln
 */

#ifndef INC_GAME_2048_H_
#define INC_GAME_2048_H_

#include "stm32f3xx_hal.h"
#include "main.h"


class Game_2048 {

	private:
		uint8_t _row;
		uint8_t _col;
		uint16_t  _score; // TODO when display the score, divide it by four!
		int* _grid;

		bool vacancy();
		bool move(uint8_t input, uint8_t** rows);
		int shift_line(int* line, int size);
		bool has_moves();
		uint8_t play(uint8_t input);
		uint8_t get_colour(int val);
		void populate(uint32_t seed);




	public:
		Game_2048(uint8_t rows, uint8_t cols);
		~Game_2048();
		void clear_grid();
		void update_grid(uint8_t* led_grid);
		int main_2048(uint8_t** rows, uint8_t* grid);
};




#endif /* INC_GAME_2048_H_ */

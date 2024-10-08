/*
 * rbg_led.c
 *
 *  Created on: Jul 18, 2024
 *      Author: 1miln
 */

#include "7_seg_display_x4.h"
#include "stm32f4xx_hal.h"

// TODO: add solution for values larger than 64 bits.
void shift_reg(long shift_set, uint8_t len, uint16_t pins[3]) {

	// TODO make struct for pins that include pin num, port, and maybe tpye into one package
	uint16_t data = pins[0], latch = pins[1], clk = pins[2];
	long msb = powl(2, len-1);

	// reverse shift
	for (int k = 0; k < len; ++k) {

		HAL_GPIO_WritePin(GPIOC, clk, 0);
		HAL_GPIO_WritePin(GPIOC, data, ((shift_set << k) & msb) == 0? 0 : 1);
		HAL_GPIO_WritePin(GPIOC, clk, 1);

	}

	HAL_GPIO_WritePin(GPIOC, latch, 1);
	HAL_GPIO_WritePin(GPIOC, latch, 0);
}

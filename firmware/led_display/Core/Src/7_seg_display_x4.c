/*
 * 7_seg_display_x4.c
 *
 *  Created on: Jul 3, 2024
 *      Author: 1miln
 */

#include "7_seg_display_x4.h"
#include "stm32f4xx_hal.h"

// Todo: For "M" and "W", their seven segment display representation uses 2 characters, so "Mm" and "Ww" instead of their respective
// characters when spelling a word
// e.g. Winner --> Wwinner, Cow --> CoWw, Miles --> Mmiles,

const uint8_t CHAR_TO_SEVEN_SEG[128] = {
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0x7E, // -
		0xFF,
		0xFF,
		0x01, // 0
		0x4F, // 1
		0x12, // 2
		0x06, // 3
		0x4C, // 4
		0x24, // 5
		0x20, // 6
		0x0F, // 7
		0x00, // 8
		0x04, // 9
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0x08, // A
		0x60, // b
		0x31, // C
		0x42, // d
		0x30, // E
		0x38, // F
		0x21, // G
		0x48, // H
		0x6F, // i
		0x43, // J
		0xFF,
		0x71, // L
		0x19, // Mm (edge case)
		0x6A, // n
		0x01, // 0
		0x18, // P
		0x0C, // q
		0x7A, // r
		0x24, // 5
		0x70, // t
		0x41, // U
		0xFF,
		0x61, // Ww (edge case)
		0xFF,
		0x44, // y
		0xFF,
		0x31, // [
		0xFF,
		0x07, // ]
		0xFF,
		0x77, // _
		0xFF,
		0x08, // A
		0x60, // b
		0x72, // c
		0x42, // d
		0x30, // E
		0x38, // F
		0x21, // G
		0x68, // h
		0x6F, // i
		0x43, // J
		0xFF,
		0x4F, // 1
		0x0D, // Mm (edge case)
		0x6A, // n
		0x62, // o
		0x18, // P
		0x0C, // q
		0x7A, // r
		0x24, // 5
		0x70, // t
		0x63, // u
		0xFF,
		0x43, // Ww (edge case)
		0xFF,
		0x44, // y
		0xFF,
		0xFF,
		0xFF,
		0xFF,
		0x7E, // -
		0xFF
};

const uint8_t INT_TO_SEVEN_SEG[10] = {
		0x01, // 0
		0x4F, // 1
		0x12, // 2
		0x06, // 3
		0x4C, // 4
		0x24, // 5
		0x20, // 6
		0x0F, // 7
		0x00, // 8
		0x04, // 9
};

void multi_seven_seg_set(uint8_t config, uint16_t seg_pins[4]) {
	(config&0x01) == 0x01? HAL_GPIO_WritePin(GPIOB, seg_pins[0], 0) : HAL_GPIO_WritePin(GPIOB, seg_pins[0], 1);
	(config&0x02) == 0x02? HAL_GPIO_WritePin(GPIOB, seg_pins[1], 0) : HAL_GPIO_WritePin(GPIOB, seg_pins[1], 1);
	(config&0x04) == 0x04? HAL_GPIO_WritePin(GPIOB, seg_pins[2], 0) : HAL_GPIO_WritePin(GPIOB, seg_pins[2], 1);
	(config&0x08) == 0x08? HAL_GPIO_WritePin(GPIOB, seg_pins[3], 0) : HAL_GPIO_WritePin(GPIOB, seg_pins[3], 1);
}

void seven_seg_shift_reg(uint8_t shift_set, uint16_t ser_pin, uint16_t clk_pin, uint16_t latch_pin) {

	// FIXME: remove this when you find a use for the most significant bit
	// ensures the unused bit is always low
	HAL_GPIO_WritePin(GPIOC, clk_pin, 0);
	HAL_GPIO_WritePin(GPIOC, ser_pin, 0);
	HAL_GPIO_WritePin(GPIOC, clk_pin, 1);

	// reverse shift
	for (int k = 0; k < 7; ++k) {

		HAL_GPIO_WritePin(GPIOC, clk_pin, 0);
		HAL_GPIO_WritePin(GPIOC, ser_pin, (shift_set>>k)&0x01);
		HAL_GPIO_WritePin(GPIOC, clk_pin, 1);

	}

	HAL_GPIO_WritePin(GPIOC, latch_pin, 1);
	HAL_GPIO_WritePin(GPIOC, latch_pin, 0);

}


void display_word_single(char word[], int delay, _Bool flash, uint16_t ser_pin, uint16_t clk_pin, uint16_t latch_pin) {

	uint16_t length = strlen(word);

	for (int k = 1; k < length; ++k) {

		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[k]], ser_pin, clk_pin, latch_pin);
		HAL_Delay(delay);

		if (flash) {
			seven_seg_shift_reg(0xFF, ser_pin, clk_pin, latch_pin);
			HAL_Delay(delay/4);
		}

	}
}


void display_num_multiple(uint16_t num, int speed, uint16_t ser_pin, uint16_t clk_pin, uint16_t latch_pin, uint16_t seg_pins[4]) {


	uint8_t delay = 1;
	num %= 10000; // truncates extra digits since it can only display four segments

	uint8_t thousands, hundreds, tens, ones;

	thousands = num/1000;
	num %= 1000;

	hundreds = num/100;
	num %= 100;

	tens = num/10;
	ones = num%10;


	seven_seg_shift_reg(INT_TO_SEVEN_SEG[thousands], ser_pin, clk_pin, latch_pin);
	multi_seven_seg_set(1, seg_pins);
	HAL_Delay(delay);

	seven_seg_shift_reg(INT_TO_SEVEN_SEG[hundreds], ser_pin, clk_pin, latch_pin);
	multi_seven_seg_set(2, seg_pins);
	HAL_Delay(delay);

	seven_seg_shift_reg(INT_TO_SEVEN_SEG[tens], ser_pin, clk_pin, latch_pin);
	multi_seven_seg_set(4, seg_pins);
	HAL_Delay(delay);

	seven_seg_shift_reg(INT_TO_SEVEN_SEG[ones], ser_pin, clk_pin, latch_pin);
	multi_seven_seg_set(8, seg_pins);
	HAL_Delay(delay);


	// turns pins off
	multi_seven_seg_set(0, seg_pins);
}



void display_word_multiple(char word[], int speed, uint16_t ser_pin, uint16_t clk_pin, uint16_t latch_pin, uint16_t seg_pins[4]) {

	// fixme find a solution for words that are less than four characters

	uint8_t delay = 1;
	uint16_t length = strlen(word);

	if (length == 1) {
		multi_seven_seg_set(1, seg_pins);
		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[0]], ser_pin, clk_pin, latch_pin);
		HAL_Delay(delay);

	}
	else if (length == 2) {
		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[0]], ser_pin, clk_pin, latch_pin);
		multi_seven_seg_set(1, seg_pins);
		HAL_Delay(delay);

		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[1]], ser_pin, clk_pin, latch_pin);
		multi_seven_seg_set(2, seg_pins);
		HAL_Delay(delay);

	}
	else if (length == 3) {
		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[0]], ser_pin, clk_pin, latch_pin);
		multi_seven_seg_set(1, seg_pins);
		HAL_Delay(delay);

		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[1]], ser_pin, clk_pin, latch_pin);
		multi_seven_seg_set(2, seg_pins);
		HAL_Delay(delay);

		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[2]], ser_pin, clk_pin, latch_pin);
		multi_seven_seg_set(4, seg_pins);
		HAL_Delay(delay);


	}
	else if (length == 4) {

		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[0]], ser_pin, clk_pin, latch_pin);
		multi_seven_seg_set(1, seg_pins);
		HAL_Delay(delay);

		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[1]], ser_pin, clk_pin, latch_pin);
		multi_seven_seg_set(2, seg_pins);
		HAL_Delay(delay);

		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[2]], ser_pin, clk_pin, latch_pin);
		multi_seven_seg_set(4, seg_pins);
		HAL_Delay(delay);

		seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[3]], ser_pin, clk_pin, latch_pin);
		multi_seven_seg_set(8, seg_pins);
		HAL_Delay(delay);

	}
	else {

		for (int offset = 0; offset <= length-4; ++offset) {

			for (int count = 0; count < speed; ++count) {
					seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[offset]], ser_pin, clk_pin, latch_pin);
					multi_seven_seg_set(1, seg_pins);
					HAL_Delay(delay);

					seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[offset+1]], ser_pin, clk_pin, latch_pin);
					multi_seven_seg_set(2, seg_pins);
					HAL_Delay(delay);

					seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[offset+2]], ser_pin, clk_pin, latch_pin);
					multi_seven_seg_set(4, seg_pins);
					HAL_Delay(delay);

					seven_seg_shift_reg(CHAR_TO_SEVEN_SEG[word[offset+3]], ser_pin, clk_pin, latch_pin);
					multi_seven_seg_set(8, seg_pins);
					HAL_Delay(delay);
				}
		}
	}

	// turns pins off
	multi_seven_seg_set(0, seg_pins);
}

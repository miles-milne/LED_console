/*
 * 7_seg_display_x4.h
 *
 *  Created on: Jul 3, 2024
 *      Author: 1miln
 */

#ifndef INC_7_SEG_DISPLAY_X4_H_
#define INC_7_SEG_DISPLAY_X4_H_

#include "7_seg_display_x4.h"
#include "stm32f3xx_hal.h"

void seven_seg_shift_reg(uint8_t shift_set, uint16_t ser_pin, uint16_t clk_pin, uint16_t latch_pin);
void display_word_single(char word[], int delay, _Bool flash, uint16_t ser_pin, uint16_t clk_pin, uint16_t latch_pin);
void display_word_multiple(char word[], int speed, uint16_t ser_pin, uint16_t clk_pin, uint16_t latch_pin, uint16_t seg_pins[4]);
void display_num_multiple(uint16_t num, int speed, uint16_t ser_pin, uint16_t clk_pin, uint16_t latch_pin, uint16_t seg_pins[4]);


// 0x00 for all off, 0x01 for seg 1, 0x02 for seg 2, 0x04 for seg 3, 0x08 for seg 4
void multi_seven_seg_set(uint8_t config, uint16_t seg_pins[4]);



#endif /* INC_7_SEG_DISPLAY_X4_H_ */

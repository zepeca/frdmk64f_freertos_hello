#ifndef DRAW_CMD_H
#define DRAW_CMD_H

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdint.h> // Include this for uint8_t

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#define MAX7219_SEG_NUM 1 // The number matrices.
#define MAX7219_BUFFER_SIZE MAX7219_SEG_NUM * 8 // The size of the buffer

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void draw_cmd(uint8_t cmd);
extern void delay_ms(uint16_t miliseconds);
extern void SPI_init(void);
extern void max7219_init(uint8_t num, uint8_t *buffer, uint8_t buffer_size);
extern void cleanMatrix(void);

#endif /*DRAW_CMD_H*/

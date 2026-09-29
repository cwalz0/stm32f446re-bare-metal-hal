#include "SSD1306.h"
#include "gpio.h"
#include "spi.h"
#include "systick.h"
#include "font_5x7_bitmap.h"
#include <stdint.h>

static const uint8_t SSD1306_Init_Sequence[] = {
	0xAE, 		// Display (off)
	0x20, 0x00, 	// Memory Addressing Mode (horizontal)
	0xA6, 		// Normal display (not inverted)
	0xA8, 0x3f, 	// Multiplex Ratio (64 lines)
	0xD3, 0x00, 	// Set Display Offset (0)
	0x40, 		// Set Display Start line to 0
	0x8D, 0x14, 	// Set Charge Pump (enable)
	0xA1, 		// Set Segment Re-map (flip horizontally)
	0xC8, 		// Set COM Output Scan Direction (flip vertically)
	0xDA, 0x12,	// Set COM Pins Hardware Config ()
	0x81, 0x7F,	// Set Contrast Control to midpoint (0-255)
	0xD9, 0xF1,	// Set Pre-charge Period
	0xDB, 0x40, 	// Set VCOMH Deselect Level
	0xA4, 		// Entire Display ON resume to RAM content display
	0xAF 		// Display (On)
};

static uint8_t display_buffer[1024];

void SSD1306_Init(void){
	// Initialize SPI2
	SPI2_Init();

	// configure OLED Pins
	GPIO_InitTypeDef OLED_Ctrl = {0};
	OLED_Ctrl.Speed = 3;
	OLED_Ctrl.Mode = GPIO_MODE_OUTPUT;

	// CS ~ Chip Select
	OLED_Ctrl.Pin = 14;
	GPIO_Init(GPIOB, &OLED_Ctrl);

	// DC ~ Data/Command selector
	OLED_Ctrl.Pin = 1;
	GPIO_Init(GPIOB, &OLED_Ctrl);

	// RES ~ Hardware Reset
	OLED_Ctrl.Pin = 2;
	GPIO_Init(GPIOB, &OLED_Ctrl);

	// Pull CS HIGH to release SPI bus
	GPIO_WritePin(GPIOB, 14, GPIO_PIN_SET);

	// Pull RES HIGH to prevent hardware reset state
	GPIO_WritePin(GPIOB, 2, GPIO_PIN_SET);

	Delay_ms(10); // wait for power to stablize
	
	// pull to LOW to trigger RAM wipe
	GPIO_WritePin(GPIOB, 2, GPIO_PIN_RESET);

	Delay_ms(10); // wait for reset
	
	GPIO_WritePin(GPIOB, 2, GPIO_PIN_SET);

	Delay_ms(100); // wait for power to stablize
	
	for (int i = 0; i < sizeof(SSD1306_Init_Sequence); i++){
		SSD1306_WriteCommand(SSD1306_Init_Sequence[i])	;
	} 
	Delay_ms(100); // wait 100ms
}

void SSD1306_WriteCommand(uint8_t command){
	GPIO_WritePin(GPIOB, 14, GPIO_PIN_RESET); // pull CS low to wake screen
	GPIO_WritePin(GPIOB, 1, GPIO_PIN_RESET); // pull DC low to signal incoming command
	
	SPI2_WriteByte(command);

	GPIO_WritePin(GPIOB, 14, GPIO_PIN_SET); // pull CS high to release SPI bus
}

void SSD1306_ClearBuffer(void){
	for (int i = 0; i < sizeof(display_buffer); i++){
		display_buffer[i] = 0x00;
	}
}

void SSD1306_UpdateDisplay(void){
    // 1. Reset the RAM pointer to the top-left corner (Columns 0 to 127)
    SSD1306_WriteCommand(0x21); 
    SSD1306_WriteCommand(0x00); // Start Column: 0
    SSD1306_WriteCommand(0x7F); // End Column: 127

    // 2. Reset the Page pointer to cover all vertical rows (Pages 0 to 7)
    SSD1306_WriteCommand(0x22); 
    SSD1306_WriteCommand(0x00); // Start Page: 0
    SSD1306_WriteCommand(0x07); // End Page: 7

    // 3. Stream the 1024-byte frame buffer
    GPIO_WritePin(GPIOB, 14, GPIO_PIN_RESET); // CS low
    GPIO_WritePin(GPIOB, 1, GPIO_PIN_SET);    // DC high (Data Mode)
    
    for (int i = 0; i < sizeof(display_buffer); i++){
        SPI2_WriteByte(display_buffer[i]);
    }
    
    GPIO_WritePin(GPIOB, 14, GPIO_PIN_SET); // CS high
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color){
	if (x >= 128 || y >= 64) { return; } // prevent out of bounds pixels
	uint16_t buffer_index = x + ((y/8) * 128);
	uint8_t bit_position = y % 8;

	if(color == 0){
		display_buffer[buffer_index] &= ~(1U << bit_position);
	} else if (color == 1) {
	display_buffer[buffer_index] |= (1U << bit_position);
	}
}

void SSD1306_DrawChar(unsigned char c, uint32_t x, uint8_t y, uint8_t color){
	for (uint8_t col = 0; col < 5; col++){
		uint8_t column_data = font5x7bitmap[(c * 5) + col];

		for (uint8_t row = 0; row < 8; row++){ // 5x7 (8 for padding)
			if (column_data & (1U << row)){
				SSD1306_DrawPixel(x + col, y + row, color);
			}
		}
	}
}

void SSD1306_DrawString(char *str, uint8_t x, uint8_t y, uint8_t color){
	uint8_t current_x = x;
	while (*str){
		SSD1306_DrawChar(*str, current_x, y, color);

		// move 5 pixels to right + 1 pixel for padding
		current_x += 6;

		str++;

		if (current_x >= 128){
			break; // prevent line wrapping / over mapping memory
		}
	}
}

void SSD1306_DrawBitmap(const uint8_t* bitmap){
	for (uint16_t i = 0; i < 1024; i++){
		display_buffer[i] = bitmap[i];
	}
}

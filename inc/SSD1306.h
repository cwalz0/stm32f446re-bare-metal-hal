#ifndef SSD1306_H
#define SSD1306_H
#include <stdint.h>

void SSD1306_Init(void);
void SSD1306_WriteCommand(uint8_t command);
void SSD1306_ClearBuffer(void);
void SSD1306_UpdateDisplay(void);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void SSD1306_DrawChar(unsigned char c, uint32_t x, uint8_t y, uint8_t color);
void SSD1306_DrawString(char* str, uint8_t x, uint8_t y, uint8_t color);
void SSD1306_DrawBitmap(const uint8_t* bitmap);

#endif

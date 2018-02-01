#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
	for(int r=0; r<height; r++)
	{
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)&videoBuffer[OFFSET(row+r, col, 240)];
		REG_DMA3CNT = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
	}
}


void delay(int n)
{
	volatile int size = 0;
	for(int i=0; i<n*4000; i++)
	{
		size = size + 1;
	}
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 161);
}

void fillScreen(volatile u16 color)
{
	REG_DMA3SAD = (u32)&color;
	REG_DMA3DAD = (u32)videoBuffer;
	REG_DMA3CNT = (160*240) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;

}

void drawChar(int row, int col, char ch, unsigned short color)
{
	for(int r=0; r< 8; r++)
	{
		for(int c=0; c<6; c++)
		{
			if (fontdata_6x8[OFFSET(r, c, 6) + ch*48] == 1)
			{
				setPixel(row+r, col+c, color);
			}
		}
	}
}

void drawString(int row, int col, char str[], unsigned short color)
{
	while(*str)
	{
		drawChar(row, col, *str++, color);
		col += 6;
		
	}
}

void drawImage(int row, int col, int width, int height, const u16* image) {
    for(int i = 0; i < height; i++) {
        DMA[3].src = &image[i*width];
        DMA[3].dst = &videoBuffer[(row+i)*240+col];
        DMA[3].cnt = width | DMA_ON;
    }
}

